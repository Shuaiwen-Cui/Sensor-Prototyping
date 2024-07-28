/**
 * @file ica.h
 * @brief This script is to perform independent component analysis on the generated signal.
 * @version 1.0
 * @date 2024-07-22
 * @copyright Copyright (c) 2024
 */

#ifndef ICA_H
#define ICA_H

/**
 * @name INCLUDES
 * 
 */
#include "app_control.h"


/**
 * @name MACROS
 * 
 */


/**
 * @name VARIABLES
 * 
 */


/**
 * @name FUNCTIONS
 * 
 */

// /* --------------------------------------------------------------------------
// DATA CENTRALIZATION
// -------------------------------------------------------------------------- */

// /**
//  * @name center_data
//  * @brief This function is to centralize the data.
//  * @param data: input data
//  * @param centered_data: output centralized data
//  * @param rows: number of rows
//  * @param cols: number of columns
//  * @return void
//  *
//  */
// void center_data(float32_t *data, float32_t *centered_data, uint32_t rows, uint32_t cols);

// /* --------------------------------------------------------------------------
// DATA WHITENING
// -------------------------------------------------------------------------- */

// /**
//  * @name compute_covariance_matrix
//  * @brief This function is to compute the covariance matrix.
//  * @param data: input data
//  * @param cov_matrix: output covariance matrix
//  * @param rows: number of rows
//  * @param cols: number of columns
//  * @return void
//  *
//  */
// void compute_covariance_matrix(float32_t *data, float32_t *cov_matrix, uint32_t rows, uint32_t cols);

// /**
//  * @name jacobi_eigenvalue_decomposition
//  * @brief This function is to perform eigenvalue decomposition using Jacobi method.
//  * @param matrix: input matrix
//  * @param eigenvalues: output eigenvalues
//  * @param eigenvectors: output eigenvectors
//  * @param size: size of the matrix
//  * @return void
//  *
//  */
// void jacobi_eigenvalue_decomposition(float32_t *matrix, float32_t *eigenvalues, float32_t *eigenvectors, uint32_t size);

// /**
//  * @name compute_whitening_matrix_jacobi
//  * @brief This function is to compute the whitening matrix using Jacobi method.
//  * @param cov_matrix: input covariance matrix
//  * @param whitening_matrix: output whitening matrix
//  * @param size: size of the matrix
//  * @return void
//  *
//  */
// void compute_whitening_matrix_jacobi(float32_t *cov_matrix, float32_t *whitening_matrix, uint32_t size);

// /**
//  * @name whiten_data
//  * @brief This function is to whiten the data.
//  * @param data: input data
//  * @param whitened_data: output whitened data
//  * @param rows: number of rows
//  * @param cols: number of columns
//  * @return void
//  *
//  */
// void whiten_data(float32_t *data, float32_t *whitened_data, uint32_t rows, uint32_t cols);

// /* --------------------------------------------------------------------------
// FAST ICA
// -------------------------------------------------------------------------- */

// /**
//  * @name gx
//  * @brief This function is to compute the g(x) function.
//  * @param x: input data
//  * @param gx_res: output g(x) data
//  * @param size: size of the data
//  * @return void
//  *
//  */
// void gx(float32_t *x, float32_t *gx_res, uint32_t size);
// /**
//  * @name div_gx
//  * @brief This function is to compute the g'(x) function.
//  * @param x: input data
//  * @param div_gx_res: output g'(x) data
//  * @param size: size of the data
//  * @return void
//  *
//  */
// void div_gx(float32_t *x, float32_t *div_gx_res, uint32_t size);

// /**
//  * @name decorrelation_data
//  * @brief This function is to decorrelate the data.
//  * @param W: input data
//  * @param size: size of the data
//  * @return void
//  *
//  */
// void decorrelation_data(float32_t *W, uint32_t rows, uint32_t num_sources);
// /**
//  * @name FastICA
//  * @brief This function is to perform FastICA.
//  * @param Z: input data
//  * @param W: output data
//  * @param rows: number of rows
//  * @param cols: number of columns
//  * @param num_sources: number of sources
//  * @param maxIter: maximum number of iterations
//  * @param tol: tolerance
//  * @return void
//  *
//  */
// void FastICA(float32_t *Z, float32_t *W, uint32_t rows, uint32_t cols, uint32_t num_sources, uint32_t maxIter, float32_t tol);

// /**
//  * @name extract_sources
//  * @brief This function is to extract the sources.
//  * @param W: input data
//  * @param X: input data
//  * @param S: output data
//  * @param rows: number of rows
//  * @param cols: number of columns
//  * @return void
//  *
//  */
// void extract_sources(float32_t *W, float32_t *X, float32_t *S, uint32_t rows, uint32_t cols, uint32_t num_sources);


#endif  /* ICA_H */