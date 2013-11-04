#ifndef RKNITRO_SparseMatrix_H
#define RKNITRO_SparseMatrix_H

namespace knitro {

    class SparseMatrix {
    public:
        SparseMatrix() : n(0){}
        
        SparseMatrix( int n_, IntegerVector rows_, IntegerVector cols_ ) : n(n_), rows(rows_), cols(cols_) {
            if( n > 0 ){
                if( rows_.size() != n ) stop( "wrong size for variable index of non zero" ) ;
                if( cols_.size() != n ) stop( "wrong size for variable index of non zero" ) ;
                rows = rows_ ;
                cols = cols_ ;
            }
        }
        
        inline int size() const{ 
            return n ; 
        }
        
        inline int* get_rows() const {
            if( n ) return rows.begin() ;
            return NULL ;
        }
        
        inline int* get_cols() const {
            if( n ) return cols.begin() ;
            return NULL ;
        }
        
    private:
        int n ;
        IntegerVector rows ;
        IntegerVector cols ;
    } ;
}
RCPP_EXPOSED_CLASS_NODECL(knitro::SparseMatrix)

#endif                         
