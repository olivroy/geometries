#ifndef R_SFHEADERS_LIST_COLLAPSE_H
#define R_SFHEADERS_LIST_COLLAPSE_H

#include <Rcpp.h>
#include "geometries/utils/lists/utils.hpp"

namespace geometries {
namespace utils {

  /*
   * collapses a list of list of vectors
   *
   * list(
   *   list(
   *     x = c(),
   *     y = c(),
   *     z = c()
   *   ),
   *   list(
   *     x = c(),
   *     y = c(),
   *     z = c()
   *   )
   * )
   *
   * and adds an 'id' column
   */
  inline Rcpp::List collapse_list(
      Rcpp::List& lst,
      R_xlen_t& total_rows
  ) {

    R_xlen_t lst_size = lst.size();
    // each list must have the same number of columns.
    if( lst_size == 0 ) {
      return lst; // #nocov
    }

    R_xlen_t i;
    R_xlen_t j;

    Rcpp::List first_list = lst[ 0 ];
    R_xlen_t n_vectors = first_list.length() + 1; // vector for each matrix column, and an id column

    Rcpp::List lst_res( n_vectors );

    for( i = 0; i < n_vectors; ++i ) {
      lst_res[ i ] = Rcpp::NumericVector( total_rows, Rcpp::NumericVector::get_na() );
    }

    R_xlen_t row_counter = 0;
    R_xlen_t vector_size = 0;

    for( i = 0; i < lst_size; ++i ) {
      Rcpp::List inner_list = lst[ i ];
      R_xlen_t n_col = inner_list.size();

      if( n_vectors - 1 != n_col ) {
        Rcpp::stop("sfheaders - unknown issue - please report this, along with an example, at www.github.com/dcooley/sfheaders/issues"); // #nocov
      }

      for( j = 0; j < n_col; ++j ) {

        SEXP s = inner_list[ j ];
        Rcpp::NumericVector new_vector = Rcpp::as< Rcpp::NumericVector >( s );
        vector_size = new_vector.length();

        Rcpp::NumericVector current_vector = lst_res[ j + 1 ];
        lst_res[ j + 1 ] = geometries::utils::fill_vector( current_vector, new_vector, row_counter );;
      }

      // id column
      double id = i + 1;
      SEXP s2 = lst_res[ 0 ];
      Rcpp::NumericVector current_id_vector = Rcpp::as< Rcpp::NumericVector >( s2 );
      Rcpp::NumericVector new_id_vector = Rcpp::rep( id, vector_size );

      lst_res[ 0 ] = geometries::utils::fill_vector( current_id_vector, new_id_vector, row_counter );

      row_counter = row_counter + vector_size;
    }
    return lst_res;
  }

  // collapse_list - user-supplied 'id', incremented for each list element.
  inline Rcpp::List collapse_list( Rcpp::List& lst, R_xlen_t& total_rows, double& id ) {

    R_xlen_t lst_size = lst.size();
    // each list must have the same number of columns.
    if( lst_size == 0 ) {
      return lst; // #nocov
    }

    R_xlen_t i;
    R_xlen_t j;

    Rcpp::List first_list = lst[ 0 ];
    R_xlen_t n_vectors = first_list.length() + 1; // vector for each matrix column, and an id column

    Rcpp::List lst_res( n_vectors );

    for( i = 0; i < n_vectors; ++i ) {
      lst_res[ i ] = Rcpp::NumericVector( total_rows, Rcpp::NumericVector::get_na() );
    }

    R_xlen_t row_counter = 0;
    R_xlen_t vector_size = 0;

    for( i = 0; i < lst_size; ++i ) {
      Rcpp::List inner_list = lst[ i ];
      R_xlen_t n_col = inner_list.size();

      if( n_vectors - 1 != n_col ) {
        Rcpp::stop("sfheaders - unknown issue - please report this, along with an example, at www.github.com/dcooley/sfheaders/issues"); // #nocov
      }

      for( j = 0; j < n_col; ++j ) {

        SEXP s = inner_list[ j ];
        Rcpp::NumericVector new_vector = Rcpp::as< Rcpp::NumericVector >( s );
        vector_size = new_vector.length();

        Rcpp::NumericVector current_vector = lst_res[ j + 1 ];
        lst_res[ j + 1 ] = geometries::utils::fill_vector( current_vector, new_vector, row_counter );;
      }

      // id column
      //double id = i + 1;
      SEXP s2 = lst_res[ 0 ];
      Rcpp::NumericVector current_id_vector = Rcpp::as< Rcpp::NumericVector >( s2 );
      Rcpp::NumericVector new_id_vector = Rcpp::rep( id, vector_size );

      lst_res[ 0 ] = geometries::utils::fill_vector( current_id_vector, new_id_vector, row_counter );

      row_counter = row_counter + vector_size;
      ++id;
    }
    return lst_res;
  }

} // utils
} // geometries

#endif
