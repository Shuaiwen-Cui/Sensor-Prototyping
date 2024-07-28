/**
 * @file wt.c
 * @brief This script is to perform wavelet transform on the generated signal.
 * @version 1.0
 * @date 2024-07-22
 * @copyright Copyright (c) 2024
 */

/**
 * @name INCLUDES
 *
 */
#include "wt.h"
/**
 * @name MACROS
 *
 */

/**
 * @name VARIABLES
 *
 */
// db10 wavelet filter coefficients in float32_t

const float32_t db10_lo[FILTER_SIZE] = {
    0.0033357253f, -0.0125807520f, -0.0062414902f, 0.0775714938f,
    -0.0322448696f, -0.2422948871f, 0.1384281459f, 0.7243085284f,
    0.6038292698f, 0.1601023979f, -0.1801274485f, -0.3163387738f,
    0.1092361373f, 0.1143656546f, -0.0277599556f, -0.0138106793f,
    0.0025745177f, 0.0011175188f, -0.0004662169f, -0.0000709833f};

const float32_t db10_hi[FILTER_SIZE] = {
    -0.0000709833f, -0.0004662169f, 0.0011175188f, 0.0025745177f,
    -0.0138106793f, -0.0277599556f, 0.1143656546f, 0.1092361373f,
    -0.3163387738f, -0.1801274485f, 0.1601023979f, 0.6038292698f,
    -0.7243085284f, 0.1384281459f, -0.2422948871f, -0.0322448696f,
    0.0775714938f, -0.0062414902f, -0.0125807520f, 0.0033357253f};

const float32_t db10_lo_r[FILTER_SIZE] = {
    -0.0000709833f, 0.0004662169f, 0.0011175188f, -0.0025745177f,
    -0.0138106793f, 0.0277599556f, 0.1143656546f, -0.1092361373f,
    -0.3163387738f, 0.1801274485f, 0.1601023979f, -0.6038292698f,
    0.7243085284f, -0.1384281459f, -0.2422948871f, 0.0322448696f,
    0.0775714938f, 0.0062414902f, -0.0125807520f, -0.0033357253f};

const float32_t db10_hi_r[FILTER_SIZE] = {
    0.0033357253f, 0.0125807520f, -0.0062414902f, -0.0775714938f,
    0.0322448696f, 0.2422948871f, -0.1384281459f, -0.7243085284f,
    0.6038292698f, -0.1601023979f, -0.1801274485f, 0.3163387738f,
    0.1092361373f, -0.1143656546f, -0.0277599556f, 0.0138106793f,
    0.0025745177f, -0.0011175188f, -0.0004662169f, 0.0000709833f};

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
void dwt(const float32_t *signal, float32_t *output, int length, int levels)
{
    float32_t temp[length];
    memcpy(temp, signal, length * sizeof(float32_t));

    int step, i, j;
    for (step = 0; step < levels; step++)
    {
        int half = length >> (step + 1);
        for (i = 0; i < half; i++)
        {
            output[i] = 0;
            output[half + i] = 0;
            for (j = 0; j < FILTER_SIZE; j++)
            {
                int k = (2 * i + j) % length;
                output[i] += temp[k] * db10_lo[j];
                output[half + i] += temp[k] * db10_hi[j];
            }
        }
        memcpy(temp, output, length * sizeof(float32_t));
    }
}

/**
 * @name idwt - Inverse Discrete Wavelet Transform
 * @brief This function performs the Inverse Discrete Wavelet Transform on the input coefficients.
 * @param coeffs - The input coefficients pointer
 * @param output - The output signal pointer
 * @param length - The length of the input coefficients
 * @param levels - The number of decomposition levels
 * @return void
 */
void idwt(const float32_t *coeffs, float32_t *output, int length, int levels)
{
    float32_t temp[length];
    memcpy(temp, coeffs, length * sizeof(float32_t));

    int step, i, j;
    for (step = levels - 1; step >= 0; step--)
    {
        int half = length >> (step + 1);
        memset(output, 0, length * sizeof(float32_t)); // Clear output buffer
        for (i = 0; i < half; i++)
        {
            for (j = 0; j < FILTER_SIZE; j++)
            {
                int k = (2 * i + j) % length;
                output[k] += temp[i] * db10_lo_r[j] + temp[half + i] * db10_hi_r[j];
            }
        }
        memcpy(temp, output, length * sizeof(float32_t));
    }
}
