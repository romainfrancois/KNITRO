#include <rknitro.h>

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
    double obj ;
    
    /*problem size and mem allocation */
    n = 3;
    m = 2;
    nnzJ = n*m;
    nnzH = 0;
    NumericVector x(n) ;
    NumericVector lambda(m+n) ;
    
    NumericVector xLoBnds(n) ;
    NumericVector xUpBnds(n, KTR_INFBOUND) ;
    NumericVector xInitial(n, 2.0);
    IntegerVector cType(m, KTR_CONTYPE_GENERAL);
    NumericVector cLoBnds(m);
    NumericVector cUpBnds(m, KTR_INFBOUND);
    cUpBnds[0] = 0.0 ; 
    IntegerVector jacIndexVars(nnzJ);
    IntegerVector jacIndexCons(nnzJ);
    
    /* objective type */
    objType = KTR_OBJTYPE_GENERAL;
    objGoal = KTR_OBJGOAL_MINIMIZE;
    
    /* sparsity pattern (here, of a full matrix) */
    int k = 0;
    for (int i = 0; i < n; i++){
            for (int j = 0; j < m; j++) {
                    jacIndexCons[k] = j;
                    jacIndexVars[k] = i;
                    k++;
            }
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
    
    /* solver call */
    nStatus = KTR_solve (kc, x, lambda, 0, &obj,
            NULL, NULL, NULL, NULL, NULL, NULL);
    
    if (nStatus != 0)
            Rprintf ("\nKNITRO failed to solve the problem, final status = %d\n",
                    nStatus);
    else
            Rprintf ("\nKNITRO successful, objective is = %e\n", obj);
    
    return( 0 );   
}
