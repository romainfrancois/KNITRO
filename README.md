RcppKnitro
==========

```
require(RcppKnitro)

kc <- new( Context )
kc$set_param( "gradopt", KTR_GRADOPT_FORWARD) ;
kc$set_param( "hessopt", KTR_HESSOPT_BFGS) ;
kc$set_param( "outlev" , 1L ) ;
    
```

