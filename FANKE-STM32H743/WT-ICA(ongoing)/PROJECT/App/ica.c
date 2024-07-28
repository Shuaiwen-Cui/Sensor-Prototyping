/**
 * @file ica.c
 * @brief This script is to perform independent component analysis on the generated signal.
 * @version 1.0
 * @date 2024-07-22
 * @copyright Copyright (c) 2024
 */

/**
 * @name INCLUDES
 *
 */
#include "ica.h"

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
// void center_data(float32_t *data, float32_t *centered_data, uint32_t rows, uint32_t cols)
// {
//     for (uint32_t i = 0; i < rows; i++)
//     {
//         float32_t mean;
//         arm_mean_f32(&data[i * cols], cols, &mean);
//         for (uint32_t j = 0; j < cols; j++)
//         {
//             centered_data[i * cols + j] = data[i * cols + j] - mean;
//         }
//     }
// }

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
// void compute_covariance_matrix(float32_t *data, float32_t *cov_matrix, uint32_t rows, uint32_t cols)
// {
//     arm_matrix_instance_f32 data_matrix;
//     arm_matrix_instance_f32 transposed_data_matrix;
//     arm_matrix_instance_f32 result_matrix;

//     float32_t transposed_data[cols * rows];
//     arm_mat_init_f32(&data_matrix, rows, cols, data);
//     arm_mat_init_f32(&transposed_data_matrix, cols, rows, transposed_data);
//     arm_mat_trans_f32(&data_matrix, &transposed_data_matrix);

//     float32_t temp_cov_matrix[cols * cols];
//     arm_mat_init_f32(&result_matrix, cols, cols, temp_cov_matrix);
//     arm_mat_mult_f32(&transposed_data_matrix, &data_matrix, &result_matrix);

//     for (uint32_t i = 0; i < cols * cols; i++)
//     {
//         cov_matrix[i] = temp_cov_matrix[i] / (float32_t)(rows - 1);
//     }
// }

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
// void jacobi_eigenvalue_decomposition(float32_t *matrix, float32_t *eigenvalues, float32_t *eigenvectors, uint32_t size)
// {
//     // Initialize eigenvectors as the identity matrix
//     for (uint32_t i = 0; i < size; i++)
//     {
//         for (uint32_t j = 0; j < size; j++)
//         {
//             eigenvectors[i * size + j] = (i == j) ? 1.0f : 0.0f;
//         }
//     }

//     float32_t *A = matrix;
//     float32_t *V = eigenvectors;
//     uint32_t iterations = 0;
//     const uint32_t max_iterations = 100;
//     const float32_t tolerance = 1e-10;

//     while (iterations < max_iterations)
//     {
//         // Find the largest off-diagonal element in A
//         float32_t max_val = 0.0f;
//         uint32_t p = 0;
//         uint32_t q = 1;
//         for (uint32_t i = 0; i < size; i++)
//         {
//             for (uint32_t j = i + 1; j < size; j++)
//             {
//                 if (fabsf(A[i * size + j]) > max_val)
//                 {
//                     max_val = fabsf(A[i * size + j]);
//                     p = i;
//                     q = j;
//                 }
//             }
//         }

//         if (max_val < tolerance)
//         {
//             break;
//         }

//         // Calculate the rotation angle
//         float32_t theta = 0.5f * atanf((2.0f * A[p * size + q]) / (A[q * size + q] - A[p * size + p]));
//         float32_t cos_theta = cosf(theta);
//         float32_t sin_theta = sinf(theta);

//         // Perform the rotation
//         for (uint32_t i = 0; i < size; i++)
//         {
//             if (i != p && i != q)
//             {
//                 float32_t A_ip = A[i * size + p];
//                 float32_t A_iq = A[i * size + q];
//                 A[i * size + p] = A_ip * cos_theta - A_iq * sin_theta;
//                 A[p * size + i] = A[i * size + p];
//                 A[i * size + q] = A_iq * cos_theta + A_ip * sin_theta;
//                 A[q * size + i] = A[i * size + q];
//             }
//         }

//         float32_t A_pp = A[p * size + p];
//         float32_t A_qq = A[q * size + q];
//         float32_t A_pq = A[p * size + q];

//         A[p * size + p] = A_pp * cos_theta * cos_theta + A_qq * sin_theta * sin_theta - 2.0f * A_pq * cos_theta * sin_theta;
//         A[q * size + q] = A_qq * cos_theta * cos_theta + A_pp * sin_theta * sin_theta + 2.0f * A_pq * cos_theta * sin_theta;
//         A[p * size + q] = 0.0f;
//         A[q * size + p] = 0.0f;

//         // Update the eigenvectors
//         for (uint32_t i = 0; i < size; i++)
//         {
//             float32_t V_ip = V[i * size + p];
//             float32_t V_iq = V[i * size + q];
//             V[i * size + p] = V_ip * cos_theta - V_iq * sin_theta;
//             V[i * size + q] = V_iq * cos_theta + V_ip * sin_theta;
//         }

//         iterations++;
//     }

//     // Copy the diagonal elements to eigenvalues
//     for (uint32_t i = 0; i < size; i++)
//     {
//         eigenvalues[i] = A[i * size + i];
//     }
// }

// /**
//  * @name compute_whitening_matrix_jacobi
//  * @brief This function is to compute the whitening matrix using Jacobi method.
//  * @param cov_matrix: input covariance matrix
//  * @param whitening_matrix: output whitening matrix
//  * @param size: size of the matrix
//  * @return void
//  *
//  */
// void compute_whitening_matrix_jacobi(float32_t *cov_matrix, float32_t *whitening_matrix, uint32_t size)
// {
//     float32_t eigenvalues[size];
//     float32_t eigenvectors[size * size];
//     float32_t temp_matrix[size * size];
//     float32_t diag_matrix[size * size];

//     // Perform eigenvalue decomposition using Jacobi method
//     jacobi_eigenvalue_decomposition(cov_matrix, eigenvalues, eigenvectors, size);

//     // Construct the diagonal matrix of eigenvalues
//     for (uint32_t i = 0; i < size; i++)
//     {
//         diag_matrix[i * size + i] = 1.0f / sqrtf(eigenvalues[i]);
//         for (uint32_t j = 0; j < size; j++)
//         {
//             if (i != j)
//             {
//                 diag_matrix[i * size + j] = 0.0f;
//             }
//         }
//     }

//     // Compute the whitening matrix
//     arm_matrix_instance_f32 eigenvectors_matrix;
//     arm_matrix_instance_f32 diag_matrix_instance;
//     arm_matrix_instance_f32 temp_matrix_instance;

//     arm_mat_init_f32(&eigenvectors_matrix, size, size, eigenvectors);
//     arm_mat_init_f32(&diag_matrix_instance, size, size, diag_matrix);
//     arm_mat_init_f32(&temp_matrix_instance, size, size, temp_matrix);

//     arm_mat_mult_f32(&eigenvectors_matrix, &diag_matrix_instance, &temp_matrix_instance);
//     arm_mat_trans_f32(&eigenvectors_matrix, &diag_matrix_instance);
//     arm_mat_mult_f32(&temp_matrix_instance, &diag_matrix_instance, whitening_matrix);
// }

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
// void whiten_data(float32_t *data, float32_t *whitened_data, uint32_t rows, uint32_t cols)
// {
//     float32_t cov_matrix[cols * cols];
//     float32_t whitening_matrix[cols * cols];

//     // Compute the covariance matrix
//     compute_covariance_matrix(data, cov_matrix, rows, cols);

//     // Compute the whitening matrix using Jacobi method
//     compute_whitening_matrix_jacobi(cov_matrix, whitening_matrix, cols);

//     // Apply the whitening matrix
//     arm_matrix_instance_f32 whitening_matrix_instance;
//     arm_matrix_instance_f32 data_matrix;
//     arm_matrix_instance_f32 whitened_data_matrix;

//     arm_mat_init_f32(&whitening_matrix_instance, cols, cols, whitening_matrix);
//     arm_mat_init_f32(&data_matrix, cols, rows, data);
//     arm_mat_init_f32(&whitened_data_matrix, cols, rows, whitened_data);

//     arm_mat_mult_f32(&whitening_matrix_instance, &data_matrix, &whitened_data_matrix);
// }

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
// void gx(float32_t *x, float32_t *gx_res, uint32_t size)
// {
//     for (uint32_t i = 0; i < size; i++)
//     {
//         gx_res[i] = tanhf(x[i]);
//     }
// }

// /**
//  * @name div_gx
//  * @brief This function is to compute the g'(x) function.
//  * @param x: input data
//  * @param div_gx_res: output g'(x) data
//  * @param size: size of the data
//  * @return void
//  *
//  */
// void div_gx(float32_t *x, float32_t *div_gx_res, uint32_t size)
// {
//     for (uint32_t i = 0; i < size; i++)
//     {
//         div_gx_res[i] = 1.0f - tanhf(x[i]) * tanhf(x[i]);
//     }
// }

// /**
//  * @name decorrelation_data
//  * @brief This function is to decorrelate the data.
//  * @param W: input data
//  * @param size: size of the data
//  * @return void
//  *
//  */
// void decorrelation_data(float32_t *W, uint32_t rows, uint32_t num_sources)
// {
//     arm_matrix_instance_f32 W_matrix;
//     arm_matrix_instance_f32 W_transposed_matrix;
//     arm_matrix_instance_f32 product_matrix;
//     arm_matrix_instance_f32 eigen_vectors;
//     arm_matrix_instance_f32 diag_matrix;
//     arm_matrix_instance_f32 temp_matrix;

//     float32_t W_transposed[rows * num_sources];
//     float32_t product[num_sources * num_sources];
//     float32_t eigen_values[num_sources];
//     float32_t eigen_vectors_data[num_sources * num_sources];
//     float32_t diag_data[num_sources * num_sources];
//     float32_t temp_data[num_sources * num_sources];

//     arm_mat_init_f32(&W_matrix, num_sources, rows, W);
//     arm_mat_init_f32(&W_transposed_matrix, rows, num_sources, W_transposed);
//     arm_mat_trans_f32(&W_matrix, &W_transposed_matrix);

//     arm_mat_init_f32(&product_matrix, num_sources, num_sources, product);
//     arm_mat_mult_f32(&W_matrix, &W_transposed_matrix, &product_matrix);

//     // Perform eigenvalue decomposition using Jacobi method
//     jacobi_eigenvalue_decomposition(product, eigen_values, eigen_vectors_data, num_sources);

//     // Construct the diagonal matrix of eigenvalues
//     for (uint32_t i = 0; i < num_sources; i++)
//     {
//         diag_data[i * num_sources + i] = 1.0f / sqrtf(eigen_values[i]);
//         for (uint32_t j = 0; j < num_sources; j++)
//         {
//             if (i != j)
//             {
//                 diag_data[i * num_sources + j] = 0.0f;
//             }
//         }
//     }

//     // Compute the decorrelation matrix
//     arm_mat_init_f32(&eigen_vectors, num_sources, num_sources, eigen_vectors_data);
//     arm_mat_init_f32(&diag_matrix, num_sources, num_sources, diag_data);
//     arm_mat_init_f32(&temp_matrix, num_sources, num_sources, temp_data);

//     arm_mat_mult_f32(&eigen_vectors, &diag_matrix, &temp_matrix);
//     arm_mat_trans_f32(&eigen_vectors, &diag_matrix);
//     arm_mat_mult_f32(&temp_matrix, &diag_matrix, W);
// }

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
// void FastICA(float32_t *Z, float32_t *W, uint32_t rows, uint32_t cols, uint32_t num_sources, uint32_t maxIter, float32_t tol)
// {
//     float32_t S[num_sources * cols];
//     float32_t gs[num_sources * cols];
//     float32_t div_gs[num_sources * cols];
//     float32_t W_new[num_sources * rows];
//     float32_t diff;

//     // Initialize W with random values
//     for (uint32_t i = 0; i < num_sources; i++)
//     {
//         for (uint32_t j = 0; j < rows; j++)
//         {
//             W[i * rows + j] = (float32_t)rand() / RAND_MAX;
//         }
//     }

//     // Decorrelate W
//     decorrelation_data(W, rows, num_sources);

//     for (uint32_t iter = 0; iter < maxIter; iter++)
//     {
//         // Compute S = WZ
//         arm_matrix_instance_f32 W_matrix;
//         arm_matrix_instance_f32 Z_matrix;
//         arm_matrix_instance_f32 S_matrix;

//         arm_mat_init_f32(&W_matrix, num_sources, rows, W);
//         arm_mat_init_f32(&Z_matrix, rows, cols, Z);
//         arm_mat_init_f32(&S_matrix, num_sources, cols, S);

//         arm_mat_mult_f32(&W_matrix, &Z_matrix, &S_matrix);

//         // Compute gs and div_gs
//         gx(S, gs, num_sources * cols);
//         div_gx(S, div_gs, num_sources * cols);

//         // Update W
//         for (uint32_t i = 0; i < num_sources; i++)
//         {
//             for (uint32_t j = 0; j < rows; j++)
//             {
//                 float32_t mean_div_gs;
//                 arm_mean_f32(&div_gs[i * cols], cols, &mean_div_gs);
//                 W_new[i * rows + j] = 0.0f;
//                 for (uint32_t k = 0; k < cols; k++)
//                 {
//                     W_new[i * rows + j] += gs[i * cols + k] * Z[j * cols + k];
//                 }
//                 W_new[i * rows + j] /= (float32_t)cols;
//                 W_new[i * rows + j] -= mean_div_gs * W[i * rows + j];
//             }
//         }

//         // Decorrelate W_new
//         decorrelation_data(W_new, rows, num_sources);

//         // Compute difference between W_new and W
//         diff = 0.0f;
//         for (uint32_t i = 0; i < num_sources * rows; i++)
//         {
//             diff += fabsf(W_new[i] - W[i]);
//         }

//         // Update W
//         for (uint32_t i = 0; i < num_sources * rows; i++)
//         {
//             W[i] = W_new[i];
//         }

//         // Check convergence
//         if (diff < tol)
//         {
//             break;
//         }
//     }
// }

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
// void extract_sources(float32_t *W, float32_t *X, float32_t *S, uint32_t rows, uint32_t cols, uint32_t num_sources)
// {
//     arm_matrix_instance_f32 W_matrix;
//     arm_matrix_instance_f32 X_matrix;
//     arm_matrix_instance_f32 S_matrix;

//     arm_mat_init_f32(&W_matrix, num_sources, rows, W);
//     arm_mat_init_f32(&X_matrix, rows, cols, X);
//     arm_mat_init_f32(&S_matrix, num_sources, cols, S);

//     arm_mat_mult_f32(&W_matrix, &X_matrix, &S_matrix);
// }

// Example

// int main() {
//     // Example data: each row corresponds to one sensor's data
//     float32_t data[] = {
//         1.0f, 2.0f, 3.0f,
//         4.0f, 5.0f, 6.0f,
//         7.0f, 8.0f, 9.0f
//     };
//     uint32_t rows = 3;
//     uint32_t cols = 3;
//     uint32_t num_sources = 2; // Number of source signals
//     float32_t centered_data[rows * cols];
//     float32_t whitened_data[rows * cols];
//     float32_t W[num_sources * rows];
//     float32_t S[num_sources * cols];

//     // Center the data
//     center_data(data, centered_data, rows, cols);

//     // Whiten the data
//     whiten_data(centered_data, whitened_data, rows, cols);

//     // Perform ICA
//     FastICA(whitened_data, W, rows, cols, num_sources, 200, 0.00001);

//     // Extract source signals
//     extract_sources(W, whitened_data, S, rows, cols, num_sources);

//     // Print the results
//     printf("Whitened Data:\n");
//     for (uint32_t i = 0; i < rows; i++) {
//         for (uint32_t j = 0; j < cols; j++) {
//             printf("%f ", whitened_data[i * cols + j]);
//         }
//         printf("\n");
//     }

//     printf("Estimated W:\n");
//     for (uint32_t i = 0; i < num_sources; i++) {
//         for (uint32_t j = 0; j < rows; j++) {
//             printf("%f ", W[i * rows + j]);
//         }
//         printf("\n");
//     }

//     printf("Source Signals:\n");
//     for (uint32uint32_t i = 0; i < num_sources; i++) {
//         for (uint32_t j = 0; j < cols; j++) {
//             printf("%f ", S[i * cols + j]);
//         }
//         printf("\n");
//     }

//     return 0;
// }
