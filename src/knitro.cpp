#include <rknitro.h>

using namespace knitro ;
using namespace Rcpp ;

namespace knitro{
    
    // disambiguating
    int Context_set_param( Context* context, SEXP id, SEXP value ){
        return context->set_param( id, value ) ;    
    }
    
    static Callback* func_callback ;
    
    int  rknitro_callback (const int evalRequestCode,
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
              void * userParams) 
    {
        if(!func_callback) stop( "no callback has been registered" ) ;
        return func_callback->eval( evalRequestCode, n, m, nnzJ, nnzH, x, lambda, obj, c, objGrad, jac, hessian, hessVector, userParams ) ;
    }
    
    int rknitro_set_func_callback(KTR_context_ptr ptr, const Callback& cb){
        *func_callback = cb ;
        return KTR_set_func_callback( ptr, rknitro_callback ) ;
    }
    
}

RCPP_MODULE(knitro){
    
    class_<Bounds>( "Bounds" )
        .constructor<SEXP,SEXP>()
    ;
    class_<SparseMatrix>( "SparseMatrix" )
        .constructor()
        .constructor<int,IntegerVector, IntegerVector>()
    ;
    
    class_<Callback>( "Callback" )
        .constructor<Function>()
    ;
    
    
    class_<Context>( "Context" )
        .constructor()
        .method( "set_param", &Context_set_param )
        .method( "reset_params_to_defaults", &Context::reset_params_to_defaults )
        .method( "save_param_file", &Context::save_param_file )
        .method( "load_param_file", &Context::load_param_file )
        .method( "init_problem", &Context::init_problem )
        .method( "set_func_callback", &Context::set_func_callback )
    ;

}
