#include <Rcpp.h>

#include "geometries/shapes/shapes.hpp"

#include "geometries/coordinates/coordinates_impl.hpp"


// POINTS -----------
// [[Rcpp::export]]
SEXP rcpp_get_vec(
    SEXP x,
    SEXP cols
) {
  return geometries::shapes::to_vec( x, cols );
}

// LINE ----------
// [[Rcpp::export]]
SEXP rcpp_to_mat(
    SEXP x,
    SEXP cols
) {
  return geometries::shapes::to_mat( x, cols );
}

// LINES -----------
// [[Rcpp::export]]
SEXP rcpp_get_list_mat(
    SEXP x,
    SEXP cols,
    SEXP id
) {
  return geometries::shapes::to_listMat( x, cols, id );
}


/*
 * Returns the coordinates of a single geometry
 */
// [[Rcpp::export]]
SEXP rcpp_coordinates(
    SEXP& geometries
  ) {
  return geometries::coordinates::coordinates_impl( geometries );
}


// [[Rcpp::export]]
SEXP rcpp_from_listMat( Rcpp::List lst ) {
  R_xlen_t geometry_rows = 0;
  return geometries::shapes::from_listMat< REALSXP >( lst, geometry_rows );
}

