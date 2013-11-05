KNITRO
==========

This is a proof of concept interface between R and knitro. The goal for the proof of
concept is to enable users to express the code from 
[this page](http://www.artelys.com/knitro_doc/2_userGuide/gettingStarted/startCallableLibrary.html)
entirely in R. 

[`demo/knitro.R`](https://github.com/romainfrancois/KNITRO/blob/master/demo/knitro.R) contains a demo. 

Installation
==========

This package does not contain knitro, so you have to install your own copy and 
deal with licences as you would normally do. 

Currently, the package uses an environment variable `KNITRO` to locate 
a directory where it will find knitro's `include` and `lib` directory. Make sure
this variable is set and points to the correct directory. 

For example, I have this in my `~/.bash_profile`: 

```
export KNITRO=/Users/romain/knitro
DYLD_LIBRARY_PATH=$KNITRO/lib
export DYLD_LIBRARY_PATH
```

Callbacks
==========

The `KNITRO` R package currently supports R functions as callbacks. For example
the function below can be used as a callback. 

```
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
```

The internal code of the package checks that the function supplied as a 
callback has precisely these arguments, in the same order. 

If changes are made to the arguments, they must be returned as a list. In this 
example, changes are made to `obj` and `c`. 


