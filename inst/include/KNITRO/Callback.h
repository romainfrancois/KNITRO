#ifndef RKNITRO_Callback_H
#define RKNITRO_Callback_H

namespace knitro {
    
    #define ASSERT_ARG(__ARG__) if( TAG(p) != Rf_install(__ARG__) ) stop( "expecting formal argument '"  __ARG__ "'" ) ; p = CDR(p); arg = CAR(p)
    
    class Callback {
    public:
        Callback(Function fun_): fun(fun_) {
            Pairlist formals = clone( FORMALS(fun) ) ;
            if( formals.size() != 13) stop("expected a function with 13 arguments") ;
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
                  double * const hessVector
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
            
            // callback to R
            try{
                List res = fun(evalRequestCode, n, m, nnzJ, nnzH, 
                    x_, lambda_, 
                    obj_, c_, objGrad_, 
                    jac_, hessian_, hessVector_
                    );  
                CharacterVector names = res.names() ;
                CharacterVector variables = CharacterVector::create( 
                    "obj", "c", "objGrad", "jac", "hessian", "hessVector"
                    ) ;
                LogicalVector test = in( variables, names) ;
                
                if( test[0] ){ // obj
                    *obj = as<double>( res["obj"] ) ; 
                }
                if( test[1] ){ // c
                    NumericVector out_c = res["c"] ;
                    std::copy( out_c.begin(), out_c.end(), c ) ;
                }
                if( test[2] ){ // objGrad
                    NumericVector out_objGrad = res["objGrad"] ;
                    std::copy( out_objGrad.begin(), out_objGrad.end(), objGrad ) ;
                }
                if( test[3] ){ // jac
                    NumericVector out_jac = res["jac"] ;
                    std::copy( out_jac.begin(), out_jac.end(), jac ) ;
                }
                if( test[4] ){ // hessian
                    NumericVector out_hessian = res["hessian"] ;
                    std::copy( out_hessian.begin(), out_hessian.end(), hessian ) ;
                }
                if( test[5] ){ // hessVector
                    NumericVector out_hessVector = res["hessVector"] ;
                    std::copy( out_hessVector.begin(), out_hessVector.end(), hessVector ) ;
                }
                
                return 0 ;
            } catch(eval_error& ex){
                Rprintf( "error occured during callback evaluation: %s\n", ex.what() ) ; 
                return -1 ;
            } catch(...){
                Rprintf( "error during callback evaluation") ;
                return -1 ;
            }
        }
        
    private:
        Function fun ;
    } ;
    
    
}
RCPP_EXPOSED_CLASS_NODECL(knitro::Callback)
#endif
