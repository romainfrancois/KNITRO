require(methods)
require(KNITRO)

# creating a new knitro context
kc <- new( Context )

# setting knitro parameters
kc$set_param( "gradopt", KTR_GRADOPT_FORWARD) ;
kc$set_param( "hessopt", KTR_HESSOPT_BFGS) ;
kc$set_param( "outlev" , 1L ) ;

# the callback
# it must take this signature and 
# - return a list of modified objects on success
# - call stop on failure
fun <- function(evalRequestCode, n, m, nnzJ, nnzH,x,lambda,obj,c,objGrad,jac,hessian,hessVector){
  if (evalRequestCode != KTR_RC_EVALFC)
    stop("Wrong evalRequestCode in callback function.");
  
  # objective function
  obj <- 1000 - x[1]*x[1] - 2*x[2]*x[2] - x[3]*x[3] - x[1]*x[2] - x[1]*x[3]
  
  # constraints
  c <- c(
    8*x[1] + 14*x[2] + 7*x[3] - 56, 
    x[1]*x[1] + x[2]*x[2] + x[3]*x[3] - 25
  )
  
  list( obj = obj, c = c )
}
cb <- new( Callback, fun )
kc$set_func_callback(cb)

# problem size
n <- 3

# variable bounds
xBounds <- new( Bounds, rep(0,n), rep(KTR_INFBOUND, n) ) 
xInitial <- rep( 2.0, n )
lambdaInitial <- numeric() 

# constraints
m <- 2
cBounds <- new( Bounds, rep(0,m), c(0, KTR_INFBOUND ) )
cType <- rep(KTR_CONTYPE_GENERAL, m)

objType <- KTR_OBJTYPE_GENERAL
objGoal <- KTR_OBJGOAL_MINIMIZE

# construct sparsity of jacobian (here a full n*m) matrix
mat <- matrix(0, nrow = m, ncol = n)
jacIndexVars <- as.vector(col(mat)) - 1
jacIndexCons <- as.vector(row(mat)) - 1
nnzJ <- n*m
jac <- new( SparseMatrix, nnzJ, jacIndexVars, jacIndexCons )

# empty sparisity pattern for the hessian
hess <- new( SparseMatrix )

# initialize the problem
kc$init_problem(
  n, objGoal, objType, 
  xBounds, m, cType, cBounds, 
  jac, hess, 
  xInitial, lambdaInitial
)

# solving
kc$solve( rep(0.0, n), rep(0.0, n+m), 0, 0 )

