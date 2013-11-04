#ifndef RKNITRO_SparseMatrix_H
#define RKNITRO_SparseMatrix_H

namespace knitro {

    class SparseMatrix {
    public:
        SparseMatrix() : n(0){}
        
        SparseMatrix( int n_, IntegerVector vars_, IntegerVector cols_ ) : n(n_), vars(vars_), cols(cols_) {
            if( n > 0 ){
                if( vars_.size() != n ) stop( "wrong size for variable index of non zero" ) ;
                if( cols_.size() != n ) stop( "wrong size for variable index of non zero" ) ;
                vars = vars_ ;
                cols = cols_ ;
            }
        }
        
    private:
        int n ;
        IntegerVector vars ;
        IntegerVector cols ;
    } ;
}
RCPP_EXPOSED_CLASS_NODECL(knitro::SparseMatrix)

#endif                         
