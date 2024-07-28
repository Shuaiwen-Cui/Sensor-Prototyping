/**
 * @file wt.h
 * @brief This script is to perform wavelet transform on the generated signal.
 * @version 1.0
 * @date 2024-07-22
 * @copyright Copyright (c) 2024
 */

#ifndef WT_H
#define WT_H

/**
 * @name INCLUDES
 *
 */
#include "app_control.h"

/**
 * @name MACROS
 *
 */
#define FILTER_SIZE 20

/**
 * @name VARIABLES
 *
 */

/**
 * @name FUNCTIONS
 *
 */

/**
 * @name dwt - Discrete Wavelet Transform
 * @brief This function performs the Discrete Wavelet Transform on the input signal.
 * @param signal - The input signal pointer
 * @param output - The output signal pointer
 * @param length - The length of the input signal
 * @param levels - The number of decomposition levels
 * @return void
 */
void dwt(const float32_t *signal, float32_t *output, int length, int levels);

/**
 * @name idwt - Inverse Discrete Wavelet Transform
 * @brief This function performs the Inverse Discrete Wavelet Transform on the input coefficients.
 * @param coeffs - The input coefficients pointer
 * @param output - The output signal pointer
 * @param length - The length of the input coefficients
 * @param levels - The number of decomposition levels
 * @return void
 * 
 */
void idwt(const float32_t *coeffs, float32_t *output, int length, int levels);


























#endif /* WT_H */