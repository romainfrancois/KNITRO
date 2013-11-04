#include <rknitro.h>

using namespace knitro ;
using namespace Rcpp ;

namespace knitro{
    
    // disambiguating
    int Context_set_param( Context* context, SEXP id, SEXP value ){
        return context->set_param( id, value ) ;    
    }
    
}

RCPP_MODULE(knitro){
    
    class_<Bounds>( "Bounds" )
        .constructor<SEXP,SEXP>()
    ;
    class_<SparseMatrix>( "SparseMatrix" )
        .constructor<>()
        .constructor<int,IntegerVector, IntegerVector>()
    ;
    
    class_<Context>( "Context" )
        .constructor()
        .method( "set_param", &Context_set_param )
        .method( "reset_params_to_defaults", &Context::reset_params_to_defaults )
        .method( "save_param_file", &Context::save_param_file )
        .method( "load_param_file", &Context::load_param_file )
    ;

}
