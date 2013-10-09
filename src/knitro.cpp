#include <RcppKnitro.h>

using namespace knitro ;
using namespace Rcpp ;

namespace knitro{
    
    // disambiguating
    int Context_set_param( Context* context, SEXP id, SEXP value ){
        return context->set_param( id, value ) ;    
    }
}

RCPP_MODULE(knitro){
    
    class_<Context>( "Context" )
        .constructor()
        .method( "set_param", &Context_set_param ) 
    ;

}
