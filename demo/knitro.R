require(methods)
require(KNITRO)

kc <- new( Context )
kc$set_param( "gradopt", KTR_GRADOPT_FORWARD) ;
kc$set_param( "hessopt", KTR_HESSOPT_BFGS) ;
kc$set_param( "outlev" , 1L ) ;

fun <- function(evalRequestCode, n, m, nnzJ, nnzH,x,lambda,obj,c,objGrad,jac,hessian,hessVector,userParams){
  message( sprintf( "evalRequestCode : %s", evalRequestCode) )
  if (evalRequestCode == KTR_RC_EVALFC) {
    # objective function
    obj[1] = 1000 - x[1]*x[1] - 2*x[2]*x[2] - x[3]*x[3] - x[1]*x[2] - x[1]*x[3];
  
    # constraints
    c[1]    = 8*x[1] + 14*x[2] + 7*x[3] - 56;
    c[2]    = x[1]*x[1] + x[2]*x[2] + x[3]*x[3] - 25;
  
    return(0);
  } else {
    message("Wrong evalRequestCode in callback function.\n");
    return(-1);
  }
  
}
cb <- new( Callback, fun )
kc$set_func_callback(cb)

n <- 3
xBounds <- new( Bounds, rep(0,n), rep(KTR_INFBOUND, n) ) 
xInitial <- rep( 2.0, n )
lambdaInitial <- numeric() 

m <- 2
cBounds <- new( Bounds, rep(0,m), c(0, KTR_INFBOUND ) )
cType <- rep(KTR_CONTYPE_GENERAL, m)

objType <- KTR_OBJTYPE_GENERAL
objGoal <- KTR_OBJGOAL_MINIMIZE

mat <- matrix(0, nrow = m, ncol = n)
jacIndexVars <- as.vector(col(mat)) - 1
jacIndexCons <- as.vector(row(mat)) - 1
nnzJ <- n*m
jac <- new( SparseMatrix, nnzJ, jacIndexVars, jacIndexCons )
hess <- new( SparseMatrix )

kc$init_problem(
  n, objGoal, objType, 
  xBounds, m, cType, cBounds, 
  jac, hess, 
  xInitial, lambdaInitial
)
x <- numeric(n)
lambda <- numeric(n+m)
kc$solve( rep(0.0, n), rep(0.0, n+m), 0, 0 )

