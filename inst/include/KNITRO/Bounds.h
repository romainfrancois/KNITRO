#ifndef RKNITRO_Bounds_H
#define RKNITRO_Bounds_H

namespace knitro {
    
    class Bounds {
    public:
        Bounds( SEXP low_, SEXP up_ ), low_null(true), up_null(true){
            if( !Rf_isNull(low_ ) ){
                low_null = false ;
                low = low_ ;
            }
            if( !Rf_isNull(up_ ) ){
                up_null = false ;
                up = up_ ;
            }
        }
        
    private:
        NumericVector low; 
        NumericVector up ;
        bool low_null ;
        bool up_null ;
    } ;
    
    
}
RCPP_EXPOSED_CLASS_NODECL(knitro::Bounds)
#endif
