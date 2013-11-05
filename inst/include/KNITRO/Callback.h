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
        
        int eval(const int evalRequestCode,
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
                  void * userParams
        ){
            // construct parameters for making the R call
            NumericVector x_( x, x+n) ;
            RObject lambda_ ; if( lambda ) lambda_ = wrap(lambda, lambda+m+n) ;
            NumericVector obj_( obj, obj+1) ;
            NumericVector c_( c, c+m) ;
            NumericVector objGrad_( objGrad, objGrad+m) ;
            NumericVector jac_( jac, jac+nnzJ) ;
            RObject hessian_; if(hessian) hessian_ = wrap( hessian, hessian+nnzH) ;
            RObject hessVector_; if(hessVector) hessVector_ = wrap( hessVector, hessVector+n) ;
            // SEXP userParams_xp = PROTECT( R_MakeExternalPtr( userParams , R_NilValue, R_NilValue ) ) ;
            
            // callback to R
            try{
                List res = fun(evalRequestCode, n, m, nnzJ, nnzH, 
                    x_, lambda_, 
                    obj_, c_, objGrad_, 
                    jac_, hessian_, hessVector_, 
                    R_NilValue // userParams_xp
                    );  
                CharacterVector names = res.names() ;
                if( in( CharacterVector::create("obj"), names ) ){
                    *obj = as<double>( res["obj"] ) ; 
                }
                if( in( CharacterVector::create("c"), names ) ){
                    NumericVector out_c = res["c"] ;
                    std::copy( out_c.begin(), out_c.end(), c ) ;
                }
                
                // UNPROTECT(1) ; // userParams_xp
                return 0 ;
            } catch(...){
                Rprintf( "error occcured\n") ; 
                return -1 ;
            }
        }
        
    private:
        Function fun ;
    } ;
    
    
}
RCPP_EXPOSED_CLASS_NODECL(knitro::Callback)
#endif
