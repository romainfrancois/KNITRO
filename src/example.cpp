#include <RcppKnitro.h>

using namespace Rcpp ;
using namespace knitro ;


/* callback function that evaluates the objective
   and constraints */
int  callback (const int evalRequestCode,
    const int n,
    const int m,
    const int nnzJ,
    const int nnzH,
    const double * const x,
    const double * const lambda,
          double * const obj,
          double * const c,
          double * const objGrad,
          double * const jac,
          double * const hessian,
          double * const hessVector,
          void * userParams) {
    
    if (evalRequestCode == KTR_RC_EVALFC) {
            /* objective function */
            *obj    = 1000 - x[0]*x[0] - 2*x[1]*x[1] -
                    x[2]*x[2] - x[0]*x[1] - x[0]*x[2];
    
            /* constraints */
            c[0]    = 8*x[0] + 14*x[1] + 7*x[2] - 56;
            c[1]    = x[0]*x[0] + x[1]*x[1] + x[2]*x[2] -25;
    
            return(0);
    }
    else {
            printf ("Wrong evalRequestCode in callback function.\n");
            return(-1);
    }
} 

// [[Rcpp::export]]
int test_knitro(){
    int  nStatus;

    /* variables that are passed to KNITRO */
    knitro::Context kc ;
    
    int n, m, nnzJ, nnzH, objGoal, objType;
    int *cType;
    int *jacIndexVars, *jacIndexCons;
    double obj, *x, *lambda;
    double *xLoBnds, *xUpBnds, *xInitial, *cLoBnds, *cUpBnds;
    int i, j, k; // convenience variables
    
    /*problem size and mem allocation */
    n = 3;
    m = 2;
    nnzJ = n*m;
    nnzH = 0;
    x = (double *) malloc (n * sizeof(double));
    lambda = (double *) malloc ((m+n) * sizeof(double));
    
    xLoBnds      = (double *) malloc (n * sizeof(double));
    xUpBnds      = (double *) malloc (n * sizeof(double));
    xInitial     = (double *) malloc (n * sizeof(double));
    cType        = (int    *) malloc (m * sizeof(int));
    cLoBnds      = (double *) malloc (m * sizeof(double));
    cUpBnds      = (double *) malloc (m * sizeof(double));
    jacIndexVars = (int    *) malloc (nnzJ * sizeof(int));
    jacIndexCons = (int    *) malloc (nnzJ * sizeof(int));
    
    /* objective type */
    objType = KTR_OBJTYPE_GENERAL;
    objGoal = KTR_OBJGOAL_MINIMIZE;
    
    /* bounds and constraints type */
    for (i = 0; i < n; i++) {
            xLoBnds[i] = 0.0;
            xUpBnds[i] = KTR_INFBOUND;
    }
    for (j = 0; j < m; j++) {
            cType[j] = KTR_CONTYPE_GENERAL;
            cLoBnds[j] = 0.0;
            cUpBnds[j] = (j == 0 ? 0.0 : KTR_INFBOUND);
    }
    
    /* initial point */
    for (i = 0; i < n; i++)
            xInitial[i] = 2.0;
    
    /* sparsity pattern (here, of a full matrix) */
    k = 0;
    for (i = 0; i < n; i++)
            for (j = 0; j < m; j++) {
                    jacIndexCons[k] = j;
                    jacIndexVars[k] = i;
                    k++;
            }

    /* set options: automatic gradient and hessian matrix */
    kc.set_param( "gradopt", KTR_GRADOPT_FORWARD) ;
    kc.set_param( "hessopt", KTR_HESSOPT_BFGS) ;
    kc.set_param( "outlev" , 1) ;
    
    /* register the callback function */
    if (KTR_set_func_callback (kc, &callback) != 0)
            exit( -1 );
    
    /* pass the problem definition to KNITRO */
    nStatus = KTR_init_problem (kc, n, objGoal, objType,
                    xLoBnds, xUpBnds,
                    m, cType, cLoBnds, cUpBnds,
                    nnzJ, jacIndexVars, jacIndexCons,
                    nnzH, NULL, NULL, xInitial, NULL);
    
    /* free memory (KNITRO maintains its own copy) */
    free (xLoBnds);
    free (xUpBnds);
    free (xInitial);
    free (cType);
    free (cLoBnds);
    free (cUpBnds);
    free (jacIndexVars);
    free (jacIndexCons);
    
    /* solver call */
    nStatus = KTR_solve (kc, x, lambda, 0, &obj,
            NULL, NULL, NULL, NULL, NULL, NULL);
    
    if (nStatus != 0)
            printf ("\nKNITRO failed to solve the problem, final status = %d\n",
                    nStatus);
    else
            printf ("\nKNITRO successful, objective is = %e\n", obj);
    
    /* delete the KNITRO instance and primal/dual solution */
    free (x);
    free (lambda);
    
    return( 0 );   
}
