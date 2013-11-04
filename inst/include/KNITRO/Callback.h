#ifndef RKNITRO_Callback_H
#define RKNITRO_Callback_H

namespace knitro {
    
    #define ASSERT_ARG(__ARG__) if( TAG(p) != Rf_install(__ARG__) ) stop( "expecting formal argument '"  __ARG__ "'" ) ; p = CDR(p); arg = CAR(p)
    
    class Callback {
    public:
        Callback(Function fun_): fun(fun_) {
            Pairlist formals = clone( FORMALS(fun) ) ;
            if( formals.size() != 14) stop("expected a function with 14 arguments") ;
            SEXP p = formals ; SEXP arg = CAR(p) ;
            
            ASSERT_ARG( "evalRequestCode" ) ;
            ASSERT_ARG( "n" ) ;
            ASSERT_ARG( "m" ) ;
            ASSERT_ARG( "nnzJ" ) ;
            ASSERT_ARG( "nnzH" ) ;
            ASSERT_ARG( "x" ) ;
            ASSERT_ARG( "lambda" ) ;
            ASSERT_ARG( "obj" ) ;
            ASSERT_ARG( "c" ) ;
            ASSERT_ARG( "objGrad" ) ;
            ASSERT_ARG( "jac" ) ;
            ASSERT_ARG( "hessian" ) ;
            ASSERT_ARG( "hessVector" ) ;
            ASSERT_ARG( "userParams" ) ;
        }
        
        
    private:
        Function fun ;
    } ;
    
    
}
RCPP_EXPOSED_CLASS_NODECL(knitro::Callback)
#endif
