#ifndef R_GEOMETRIES_GEOMETRIES_COORDINATES_H
#define R_GEOMETRIES_GEOMETRIES_COORDINATES_H

#include "geometries/coordinates/dimensions.hpp"
#include "geometries/utils/lists/collapse.hpp"
#include "geometries/utils/lists/fill.hpp"

namespace geometries {
namespace coordinates {

  /*
   * Each of these functions turn individual geometries
   * into list structures
   * list(
   *   x = c()
   *   , y = c()
   *   , z = c()
   *   , .... etc
   * )
   *
   *
   */

  // each of these functions turn the geometries into a list
  // of coordinates and ids

  // forward declaration ---------
  inline Rcpp::List coordinates(
      SEXP& geometry,
      R_xlen_t& geometry_rows
    );

  inline void coordinates(
      SEXP& geometry,
      Rcpp::List& res,
      R_xlen_t& start_row_idx,
      R_xlen_t& start_col_idx,
      double& id
    );

  // -------------------------------
  inline void coordinates(
      Rcpp::Vector< REALSXP >& geometry,
      Rcpp::List& res,
      R_xlen_t& start_row_idx,
      R_xlen_t& start_column_idx,
      double& id
  ) {
    // TODO:
    // do I need to call 'fill_vector'? or can I optimise this
    // by just inserting it at teh relevant position?
    R_xlen_t n_col = geometry.length();
    R_xlen_t n_row = 1;
    R_xlen_t i;
    // Rcpp::Vector< RTYPE > idv = Rcpp::rep( id, n_row );
    // Rcpp::Vector< RTYPE > current_vector;
    // Rcpp::Vector< RTYPE > new_vector;
    // for( i = 0; i < n_col; ++i ) {
    //   current_vector = res[ start_column_idx + i + 1 ];
    //   new_vector = geometry[i];
    //   res[ start_column_idx + i + 1 ] = geometries::utils::fill_vector( current_vector, new_vector, start_row_idx );
    // }
    // current_vector = res[ start_column_idx ];
    // res[ start_column_idx ] = geometries::utils::fill_vector( current_vector, idv, start_row_idx );
  }

  inline void coordinates(
    Rcpp::Vector< REALSXP >& geometry,
    Rcpp::List& res,
    R_xlen_t& start_row_idx,
    R_xlen_t& start_column_idx
  ) {
    //typedef typename Rcpp::traits::storage_type< RTYPE >::type T;
    double id = 1;
    coordinates( geometry, res, start_row_idx, start_column_idx, id );
  }

  // vectors - no need for an id column
  inline Rcpp::List coordinates(
    Rcpp::Vector< REALSXP >& geometry,
    R_xlen_t& geometry_rows,
    double& id
  ) {
    R_xlen_t n_col = geometry.length();
    R_xlen_t n_row = 1;
    R_xlen_t i;
    Rcpp::List res( n_col + 1 );
    Rcpp::Vector< REALSXP > idv = Rcpp::rep( id, n_row );
    res[0] = idv;
    for( i = 0; i < n_col; ++i ) {
      res[i + 1] = geometry[i];
    }
    geometry_rows = 1;
    return res;
  }

  inline Rcpp::List coordinates(
    Rcpp::Vector< REALSXP >& geometry,
    R_xlen_t& geometry_rows
  ) {
    //typedef typename Rcpp::traits::storage_type< RTYPE >::type T;
    double id = 1;
    return coordinates( geometry, geometry_rows, id );
  }

  inline void coordinates(
    Rcpp::Matrix< REALSXP >& geometry,
    Rcpp::List& res,
    R_xlen_t& start_row_idx,
    R_xlen_t& coord_col_idx,
    double& id
  ) {
    R_xlen_t n_col = geometry.ncol(); // +1 for the id column
    R_xlen_t n_row = geometry.nrow();
    R_xlen_t i;
    Rcpp::Vector< REALSXP > idv = Rcpp::rep( id, n_row );
    Rcpp::Vector< REALSXP > current_vector;
    Rcpp::Vector< REALSXP > new_vector;

    // this loop does the coordinates
    for( i = 0; i < n_col; ++i ) {
      current_vector = res[ i + coord_col_idx ];
      new_vector = geometry( Rcpp::_, i );
      res[ i + coord_col_idx ] = geometries::utils::fill_vector( current_vector, new_vector, start_row_idx );
    }
    start_row_idx += n_row;
  }

  inline void coordinates(
    Rcpp::Matrix< REALSXP >& geometry,
    Rcpp::List& res,
    R_xlen_t& start_row_idx, // where to start filling the result vector
    R_xlen_t& coord_col_idx // the first column of res to be filled.
  ) {
    //typedef typename Rcpp::traits::storage_type< RTYPE >::type T;
    double id = 1;
    coordinates( geometry, res, start_row_idx, coord_col_idx, id );
  }

  inline Rcpp::List coordinates(
    Rcpp::Matrix< REALSXP >& geometry,
    R_xlen_t& geometry_rows,
    double& id
  ) {
    R_xlen_t n_col = geometry.ncol();
    R_xlen_t n_row = geometry.nrow();
    R_xlen_t i;
    Rcpp::List res( n_col + 1 );
    Rcpp::Vector< REALSXP > idv = Rcpp::rep( id, n_row );
    res[0] = idv;
    for( i = 0; i < n_col; ++i ) {
      res[ i + 1 ] = geometry( Rcpp::_, i );
    }
    geometry_rows = geometry.nrow();
    return res;
  }

  inline Rcpp::List coordinates(
    Rcpp::Matrix< REALSXP >& geometry,
    R_xlen_t& geometry_rows
  ) {
    //typedef typename Rcpp::traits::storage_type< RTYPE >::type T;
    double id = 1;
    return coordinates( geometry, geometry_rows, id );
  }

  inline void coordinates(
    Rcpp::List& geometry,
    Rcpp::List& res,
    R_xlen_t& start_row_idx,
    R_xlen_t& coord_col_idx,
    double& id
  ) {
    R_xlen_t i;
    R_xlen_t n = geometry.length();
    for( i = 0; i < n; ++i ) {
      SEXP inner_geometry = geometry[ i ];
      Rcpp::List inner_dimension = geometries::coordinates::geometry_dimensions( inner_geometry );
      Rcpp::IntegerMatrix dim = inner_dimension["dimensions"];
      Rcpp::Rcout << "inner_dimension: " << dim << std::endl;

      R_xlen_t total_coordinates = dim( dim.nrow() -1 , 1 );
      total_coordinates += 1;
      R_xlen_t nest = inner_dimension["max_nest"];
      Rcpp::Rcout << "total_coordinates: " << total_coordinates << std::endl;
      Rcpp::Rcout << "id: " << id << std::endl;
      Rcpp::Rcout << "nest: " << nest << std::endl;
      Rcpp::Rcout << "id column : " << coord_col_idx - 1 - nest << std::endl;
      Rcpp::Rcout << "start_row_idx " << start_row_idx << std::endl;
      Rcpp::Rcout << "coord_col_idx " << coord_col_idx << std::endl;
      R_xlen_t id_col = coord_col_idx - 2 - nest;


      Rcpp::Vector< REALSXP > idv = Rcpp::rep( id, total_coordinates );
      Rcpp::Rcout << "idv: " << idv << std::endl;
      Rcpp::Vector< REALSXP > current_vector = res[ id_col ];
      Rcpp::Rcout << "current_vector: " << current_vector << std::endl;
      res[ id_col ] = geometries::utils::fill_vector( current_vector, idv, start_row_idx );

      // the 'coordinates()' function updates 'start_row_idx' by-refernece, so it has to come
      // after the 'id's, where it is used.
      coordinates( inner_geometry, res, start_row_idx, coord_col_idx, id );
      id++;
    }
    // every iteration the id column needs to be filled.
    // the 'matrix' function will handle the inner-most ID.
    // but we need a column for each item at the same list depth
    // will it work where everytime this loop finishes we can
    // fill a vector of IDs?
    // we need to know the total number of rows of coordinates which are in all
    // the list elements
    // and how deep we are
    // do I need to re-calculate the dimension of each individual geometry?
    // everytime we find an inner-geometry, they will all have the same dim & nest
    //

  }

  inline void coordinates(
    Rcpp::List& geometry,
    Rcpp::List& res,
    R_xlen_t& start_row_idx,
    R_xlen_t& coord_col_idx
  ) {
    // typedef typename Rcpp::traits::storage_type< RTYPE >::type T;
    double id = 1;
    coordinates( geometry, res, start_row_idx, coord_col_idx, id );
  }

  // a single geometry which is in a list should
  // all be nested at the same level
  // and have the same diemensions
  inline Rcpp::List coordinates(
    Rcpp::List& geometry,
    R_xlen_t& geometry_rows
  ) {
    R_xlen_t i;
    R_xlen_t n = geometry.length();
    Rcpp::List res( n );
    for(i = 0; i < n; ++i ) {
      R_xlen_t total_rows = 0;
      SEXP inner_geometry = geometry[ i ];
      res[ i ] = coordinates( inner_geometry, total_rows );
      geometry_rows += total_rows;
    }
    // geometry_rows is required because 'collapse_list' needs to know how long
    // each vector needs to be
    return geometries::utils::collapse_list< REALSXP >( res, geometry_rows );
  }

  // fills the list with vectors
  inline void coordinates(
    SEXP& geometry,
    Rcpp::List& res,
    R_xlen_t& start_row_idx,
    R_xlen_t& coord_col_idx,
    double& id

  ) {
    switch( TYPEOF( geometry ) ) {
    case INTSXP: {
    //   if( Rf_isMatrix( geometry ) ) {
    //   Rcpp::IntegerMatrix im = Rcpp::as< Rcpp::IntegerMatrix >( geometry );
    //   return coordinates( im, res, start_row_idx, coord_col_idx, id );
    // } else {
    //   Rcpp::IntegerVector iv = Rcpp::as< Rcpp::IntegerVector >( geometry );
    //   return coordinates( iv, res, start_row_idx, coord_col_idx, id );
    // }
    }
    case REALSXP: {
      if( Rf_isMatrix( geometry ) ) {
      Rcpp::NumericMatrix nm = Rcpp::as< Rcpp::NumericMatrix >( geometry );
      return coordinates( nm, res, start_row_idx, coord_col_idx, id );
    } else {
      Rcpp::NumericVector nv = Rcpp::as< Rcpp::NumericVector >( geometry );
      return coordinates( nv, res, start_row_idx, coord_col_idx, id );
    }
    }
    case VECSXP: {
      if( Rf_isNewList( geometry ) ) {
        Rcpp::List lst = Rcpp::as< Rcpp::List >( geometry );
        return coordinates( lst, res, start_row_idx, coord_col_idx, id );
      }
    }
    default: {
      Rcpp::stop("geometries - can't access coordinates for this object");
    }
    }
  }

  // returns a list
  inline Rcpp::List coordinates(
    SEXP& geometry,
    R_xlen_t& geometry_rows
  ) {
    switch( TYPEOF( geometry ) ) {
    case INTSXP: {
      // if( Rf_isMatrix( geometry ) ) {
      //   Rcpp::IntegerMatrix im = Rcpp::as< Rcpp::IntegerMatrix >( geometry );
      //   return coordinates( im, geometry_rows );
      // } else {
      //   Rcpp::IntegerVector iv = Rcpp::as< Rcpp::IntegerVector >( geometry );
      //   return coordinates( iv, geometry_rows );
      // }
    }
    case REALSXP: {
      if( Rf_isMatrix( geometry ) ) {
        Rcpp::NumericMatrix nm = Rcpp::as< Rcpp::NumericMatrix >( geometry );
        return coordinates( nm, geometry_rows );
      } else {
        Rcpp::NumericVector nv = Rcpp::as< Rcpp::NumericVector >( geometry );
        return coordinates( nv, geometry_rows );
      }
    }
    case VECSXP: {
      if( Rf_isNewList( geometry ) ) {
        Rcpp::List lst = Rcpp::as< Rcpp::List >( geometry );
        return coordinates( lst, geometry_rows );
      }
    }
    default: {
      Rcpp::stop("geometries - can't access coordinates for this object");
    }
    }
    return Rcpp::List::create(); // #nocov never reaches
  }
} // coordinates
} // geometries

#endif
