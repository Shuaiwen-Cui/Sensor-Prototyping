/**
 * @file app_control.c
 * @brief This script is to control the application related codes.
 * @version 1.0
 * @date 2024-07-22
 * @copyright Copyright (c) 2024
 */

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

FATFS fs;    /* Work area (file system object) for logical drive */
FIL fil;     /* File object */
FRESULT res; /* Result code */
UINT br;     /* Bytes read */
UINT bw;     /* Bytes written */

char lab_data_dir[] = "/SDD-LAB-DATA"; // Dataset directory, only the lab dataset is used in this demo
char sim_data_dir[] = "/SDD-SIM-DATA"; // Dataset directory
char lab_wt_record_dir[] = "/SDD-LAB-WT"; // Wavelet transform record directory
char lab_ica_record_dir[] = "/SDD-LAB-ICA"; // Independent component analysis record directory

char prefix[] = "TEST";               // Prefix for the lab dataset files
char group_lable[] = {'A', 'B', 'C'}; // Group lables for the lab dataset files

char file_name[128];
char file_dir[128];
char file_path[128];

// float32_t lab_data_buffer[LAB_SIGNAL_LEN];

/**
 * @name FUNCTIONS
 * @brief Sudden damage detection function: I - Data process, II - Edge computation: WT + ICA + NN
 *
 */
int Sudden_Damage_Detection(void)
{
    // [PRE: VARIABLES]

    // loop variables
    int i, j, k;

    // data buffer
    char read_buffer[2048];  // each line cannot exceed 128 characters
    char write_buffer[2048]; // each line cannot exceed 128 characters
    float32_t **RESP_BUF;    // = (float32_t *)memory_alloc(LAB_NUM_FLOORS * LAB_SIGNAL_LEN * sizeof(float32_t));
    int num_group = sizeof(group_lable) / sizeof(group_lable[0]);

    // load data
    int selected_group = 0; // selected group for the lab dataset files

    // sudden damage detection variables
    int total_moves;                      // Total moves for the sudden damage detection
    int idx_move;                         // Index of the move for the sudden damage detection
    float32_t window_signal[WINDOW_SIZE]; // Window signal for the sudden damage detection

    int num_c_start;
    int num_c_end;

    float32_t **RAW_SIGNAL;            // Raw signal for the sudden damage detection
    float32_t **WT_COEFFICIENTS;       // Wavelet transform coefficients for the sudden damage detection, row for each floor, column for each coefficient,
    float32_t **WT_SIGNAL;             // Wavelet transform signal for the sudden damage detection
    float32_t *ICA_SIGNAL_TEMP;        // Temporary data - ICA signal buffer for general use
    float32_t *ICA_SIGNAL_CENTRALIZED; // Temporary data - centralized signal from the WT_SIGNAL
    float32_t *ICA_SIGNAL_WHITENED;    // Temporary data - whitened signal from the ICA_SIGNAL_CENTRALIZATION
    float32_t *W;                      // Temporary data - demixing matrix in the fastica algorithm iteration
    float32_t *S;                      // Temporary data - extracted source signal from the ICA_SIGNAL_WHITENED
    float32_t **ICA_SIGNAL;            // Independent component analysis signal for the sudden damage detection

    printf("============================================================================\n\r");
    printf("[APPLICATION- Sudden Damage Detection] Started!                             \n\r");

    // [I - DATA PROCESS]
    printf("[APPLICATION- Sudden Damage Detection] <I DATA PROCESS> START! \n\r");

    // 1.1 check the file in the given folder
    printf("[APPLICATION- Sudden Damage Detection] <I DATA PROCESS> Scanning the lab dataset directory.\n\r");
    FatFs_ScanDir(lab_data_dir);
    printf("Path %s scanned.\n\r", lab_data_dir);

    // printf("[APPLICATION- Sudden Damage Detection] <I DATA PROCESS> Scanning the simulated dataset directory.\n\r");
    // FatFs_ScanDir(sim_data_dir);
    // printf("Path %s scanned.\n\r", sim_data_dir);

    // 1.2 prepare the data buffer - double pointer
    printf("[APPLICATION- Sudden Damage Detection] <I DATA PROCESS> Preparing data buffers (load file data into buffer for downstream processing).\n\r");

    RESP_BUF = (float32_t **)memory_alloc(LAB_NUM_FLOORS * sizeof(float32_t *));
    if (RESP_BUF == NULL)
    {
        printf("Memory allocation failed.\n\r");
        return -1;
    }

    for (i = 0; i < LAB_NUM_FLOORS; i++)
    {
        RESP_BUF[i] = (float32_t *)memory_alloc(LAB_SIGNAL_LEN * sizeof(float32_t));
        if (RESP_BUF[i] == NULL)
        {
            printf("Memory allocation failed.\n\r");
            return -1;
        }
    }

    // set the buffer to zero
    for (i = 0; i < LAB_NUM_FLOORS; i++)
    {
        for (j = 0; j < LAB_SIGNAL_LEN; j++)
        {
            RESP_BUF[i][j] = 0.0;
        }
    }

    // 1.3 read the data from the file into the buffer
    printf("[APPLICATION- Sudden Damage Detection] <I DATA PROCESS> Reading data into buffers.\n\r");
    for (i = 0; i < LAB_NUM_FLOORS; i++)
    {
        // set the file name
        sprintf(file_name, "%s_%c_%d.csv", prefix, group_lable[selected_group], i);
        sprintf(file_path, "%s/%s", lab_data_dir, file_name);

        printf("processing file: %s\n", file_path);

        // open the file
        res = f_open(&fil, file_path, FA_READ);
        if (res != FR_OK)
        {
            printf("Open failed: %d\n", res);
            return -1;
        }

        // read the file, assume only one colum of data in each file
        j = 0;
        while (f_gets(read_buffer, sizeof(read_buffer), &fil))
        {
            if (j < LAB_SIGNAL_LEN)
            {
                RESP_BUF[i][j++] = atof(read_buffer);
            }
            else
            {
                printf("Buffer overflow, too many data entries.\n");
                break;
            }
        }

        // close the file
        res = f_close(&fil);
        if (res != FR_OK)
        {
            printf("Close failed: %d\n", res);
            return -1;
        }
    }

    // // print the last 5 rows and 5 columns of the data in the buffer
    // printf("The last 5 rows and 5 columns of the data in the buffer:\n");
    // for (i = LAB_SIGNAL_LEN - 5; i < LAB_SIGNAL_LEN; i++)
    // {
    //     for (j = LAB_NUM_FLOORS - 5; j < LAB_NUM_FLOORS; j++)
    //     {
    //         printf("%16.8f ", RESP_BUF[j][i]);
    //     }
    //     printf("\n");
    // }

    printf("[APPLICATION- Sudden Damage Detection] <I DATA PROCESS> END! \n\r");

    // [II - EDGE COMPUTATION: WT + ICA + NN]

    // pre
    total_moves = (LAB_SIGNAL_LEN - WINDOW_SIZE) / WINDOW_GAP + 1;
    idx_move = 0;

    printf("total moves: %d\n", total_moves);

    printf("[APPLICATION- Sudden Damage Detection] <II EDGE COMPUTATION> Started.\n\r");

    for (idx_move = 0; idx_move < total_moves; idx_move++)
    {
        // allocate memory for the raw signal, length is WINDOW_SIZE
        RAW_SIGNAL = (float32_t **)memory_alloc(LAB_NUM_FLOORS * sizeof(float32_t *));
        for (i = 0; i < LAB_NUM_FLOORS; i++)
        {
            RAW_SIGNAL[i] = (float32_t *)memory_alloc(WINDOW_SIZE * sizeof(float32_t));
        }
        printf("processing move: %d out of %d\n", idx_move + 1, total_moves);

        // 2.1 wavelet transform
        // 2.1.1 prepare the raw signal
        for (i = 0; i < LAB_NUM_FLOORS; i++)
        {
            for (j = 0; j < WINDOW_SIZE; j++)
            {
                RAW_SIGNAL[i][j] = RESP_BUF[i][idx_move * WINDOW_GAP + j]; // row for each floor, column for each time step
            }
        }

        // 2.1.2 wavelet transform - DWT
        // allocate memory for wavelet coefficients
        WT_COEFFICIENTS = (float32_t **)memory_alloc(LAB_NUM_FLOORS * sizeof(float32_t *));
        for (i = 0; i < LAB_NUM_FLOORS; i++)
        {
            WT_COEFFICIENTS[i] = (float32_t *)memory_alloc(WINDOW_SIZE * sizeof(float32_t));
        }

        for (i = 0; i < LAB_NUM_FLOORS; i++)
        {
            dwt(RAW_SIGNAL[i], WT_COEFFICIENTS[i], WINDOW_SIZE, WT_DEC_LEVEL);
        }

        // free memory for raw signal
        for (i = 0; i < LAB_NUM_FLOORS; i++)
        {
            memory_free(RAW_SIGNAL[i]);
        }
        memory_free(RAW_SIGNAL);

        // 2.1.3 coefficients process, only use level 4 detail coefficients, set the remainder to zero - [cA4, cD4, cD3, cD2, cD1]
        num_c_start = WINDOW_SIZE / (1 << WT_DEC_LEVEL);
        num_c_end = 2 * num_c_start;
        for (i = 0; i < LAB_NUM_FLOORS; i++)
        {
            for (j = 0; j < WINDOW_SIZE; j++)
            {
                if (j < num_c_start || j >= num_c_end)
                {
                    WT_COEFFICIENTS[i][j] = 0.0;
                }
            }
        }

        // 2.1.4 wavelet transform - IDWT
        // allocate memory for wavelet signal
        WT_SIGNAL = (float32_t **)memory_alloc(LAB_NUM_FLOORS * sizeof(float32_t *));
        for (i = 0; i < LAB_NUM_FLOORS; i++)
        {
            WT_SIGNAL[i] = (float32_t *)memory_alloc(WINDOW_SIZE * sizeof(float32_t));
        }

        for (i = 0; i < LAB_NUM_FLOORS; i++)
        {
            idwt(WT_COEFFICIENTS[i], WT_SIGNAL[i], WINDOW_SIZE, WT_DEC_LEVEL);
        }

        // free memory for wavelet coefficients
        for (i = 0; i < LAB_NUM_FLOORS; i++)
        {
            memory_free(WT_COEFFICIENTS[i]);
        }
        memory_free(WT_COEFFICIENTS);

        // 2.1.3 write the wavelet output to a file
        sprintf(file_name, "WT_move_%03d.csv", idx_move);
        sprintf(file_dir, "%s", lab_wt_record_dir);
        sprintf(file_path, "%s/%s", file_dir, file_name);

        // mount the file system
        res = f_mount(&fs, "", 0);
        if (res != FR_OK)
        {
            printf("Mount failed: %d\n", res);
            return -1;
        }
        // else
        // {
        //     printf("Mount successful.\n");
        // }

        // check whether the directory exists, if not, create it
        res = f_mkdir(file_dir);
        // if(res != FR_OK)
        // {
        //     printf("Directory exists.\n");
        // }
        // else
        // {
        //     printf("Directory created.\n");
        // }

        // open the file
        res = f_open(&fil, file_path, FA_CREATE_ALWAYS | FA_WRITE);
        if (res != FR_OK)
        {
            printf("Open failed: %d\n", res);
            return -1;
        }
        // else
        // {
        //     printf("File opened.\n");
        // }

        // write the data to the file, each column is a floor, each row is a time step, each value is separated by a comma
        for (i = 0; i < WINDOW_SIZE; i++)
        {
            for (j = 0; j < LAB_NUM_FLOORS; j++)
            {
                sprintf(write_buffer, "%16.8f", WT_SIGNAL[j][i]);
                res = f_write(&fil, write_buffer, strlen(write_buffer), &bw);
                if (res != FR_OK)
                {
                    printf("Write failed: %d\n", res);
                    return -1;
                }

                if (j < LAB_NUM_FLOORS - 1)
                {
                    res = f_write(&fil, ",", 1, &bw);
                    if (res != FR_OK)
                    {
                        printf("Write failed: %d\n", res);
                        return -1;
                    }
                }
            }
            res = f_write(&fil, "\n", 1, &bw);
            if (res != FR_OK)
            {
                printf("Write failed: %d\n", res);
                return -1;
                printf("Write successful.\n");
            }
        }
        // printf("File written.\n");

        // close the file
        res = f_close(&fil);
        if (res != FR_OK)
        {
            printf("Close failed: %d\n", res);
            return -1;
        }
        // else
        // {
        //     printf("File created.\n");
        // }

        // unmount the file system
        res = f_mount(NULL, "", 0);
        if (res != FR_OK)
        {
            printf("Unmount failed: %d\n", res);
            return -1;
        }
        // else
        // {
        //     printf("Unmount successful.\n");
        // }

        // 2.2 independent component analysis

        // // allocate memory for the ICA signal ICA_SIGNAL_TEMP
        // ICA_SIGNAL_TEMP = (float32_t *)memory_alloc(WINDOW_SIZE * sizeof(float32_t));

        // // copy the data from WT_SIGNAL to ICA_SIGNAL_TEMP
        // for (i = 0; i < LAB_NUM_FLOORS; i++)
        // {
        //     for (j = 0; j < WINDOW_SIZE; j++)
        //     {
        //         ICA_SIGNAL_TEMP[j] = WT_SIGNAL[i][j];
        //     }
        // }

        // free memory for wavelet signal
        for (i = 0; i < LAB_NUM_FLOORS; i++)
        {
            memory_free(WT_SIGNAL[i]);
        }
        memory_free(WT_SIGNAL);

        // printf("centralization done.\n");

        // // 2.2.1 centralize the signal
        // ICA_SIGNAL_CENTRALIZED = (float32_t *)memory_alloc(WINDOW_SIZE * sizeof(float32_t));
        // center_data(ICA_SIGNAL_TEMP, ICA_SIGNAL_CENTRALIZED, LAB_NUM_FLOORS, WINDOW_SIZE);

        // // free memory for ICA_SIGNAL_TEMP
        // memory_free(ICA_SIGNAL_TEMP);

        // // 2.2.2 whiten the signal
        // ICA_SIGNAL_WHITENED = (float32_t *)memory_alloc(WINDOW_SIZE * sizeof(float32_t));
        // whiten_data(ICA_SIGNAL_CENTRALIZED, ICA_SIGNAL_WHITENED, LAB_NUM_FLOORS, WINDOW_SIZE);

        // // free memory for ICA_SIGNAL_CENTRALIZED
        // memory_free(ICA_SIGNAL_CENTRALIZED);

        // // 2.2.3 independent component analysis

        // // allocate memory for the demixing matrix W
        // W = (float32_t *)memory_alloc(LAB_NUM_FLOORS * LAB_NUM_FLOORS * sizeof(float32_t));
        // FastICA(ICA_SIGNAL_WHITENED, W, LAB_NUM_FLOORS, WINDOW_SIZE, NUM_SOURCE, 200, 0.0001);

        // // allocate memory for the extracted source signal S
        // S = (float32_t *)memory_alloc(LAB_NUM_FLOORS * WINDOW_SIZE * sizeof(float32_t));

        // // extract the source signal
        // extract_sources(W, ICA_SIGNAL_WHITENED, S, LAB_NUM_FLOORS, WINDOW_SIZE, NUM_SOURCE);

        // // free memory for ICA_SIGNAL_WHITENED
        // memory_free(ICA_SIGNAL_WHITENED);

        // // free memory for W
        // memory_free(W);

        // // allocate memory for the ICA signal ICA_SIGNAL
        // ICA_SIGNAL = (float32_t **)memory_alloc(NUM_SOURCE * sizeof(float32_t *));
        // for (i = 0; i < NUM_SOURCE; i++)
        // {
        //     ICA_SIGNAL[i] = (float32_t *)memory_alloc(WINDOW_SIZE * sizeof(float32_t));
        // }

        // // copy the data from S to ICA_SIGNAL
        // for (i = 0; i < NUM_SOURCE; i++)
        // {
        //     for (j = 0; j < WINDOW_SIZE; j++)
        //     {
        //         ICA_SIGNAL[i][j] = S[i * WINDOW_SIZE + j];
        //     }
        // }

        // // free memory for S
        // memory_free(S);

        // // 2.2.4 write the ICA output to a file
        // sprintf(file_name, "ICA_move_%03d.csv", idx_move);
        // sprintf(file_dir, "%s", lab_ica_record_dir);
        // sprintf(file_path, "%s/%s", file_dir, file_name);

        // // mount the file system
        // res = f_mount(&fs, "", 0);
        // if (res != FR_OK)
        // {
        //     printf("Mount failed: %d\n", res);
        //     return -1;
        // }

        // // check whether the directory exists, if not, create it
        // res = f_mkdir(file_dir);

        // // open the file
        // res = f_open(&fil, file_path, FA_CREATE_ALWAYS | FA_WRITE);
        // if (res != FR_OK)
        // {
        //     printf("Open failed: %d\n", res);
        //     return -1;
        // }

        // // write the data to the file, each column is a floor, each row is a time step, each value is separated by a comma
        // for (i = 0; i < WINDOW_SIZE; i++)
        // {
        //     for (j = 0; j < NUM_SOURCE; j++)
        //     {
        //         sprintf(write_buffer, "%16.8f", ICA_SIGNAL[j][i]);
        //         res = f_write(&fil, write_buffer, strlen(write_buffer), &bw);
        //         if (res != FR_OK)
        //         {
        //             printf("Write failed: %d\n", res);
        //             return -1;
        //         }

        //         if (j < NUM_SOURCE - 1)
        //         {
        //             res = f_write(&fil, ",", 1, &bw);
        //             if (res != FR_OK)
        //             {
        //                 printf("Write failed: %d\n", res);
        //                 return -1;
        //             }
        //         }
        //     }
        //     res = f_write(&fil, "\n", 1, &bw);
        //     if (res != FR_OK)
        //     {
        //         printf("Write failed: %d\n", res);
        //         return -1;
        //     }
        // }

        // // close the file
        // res = f_close(&fil);
        // if (res != FR_OK)
        // {
        //     printf("Close failed: %d\n", res);
        //     return -1;
        // }

        // // unmount the file system
        // res = f_mount(NULL, "", 0);

        // 2.3 neural network / statistical analysis for spike detection, and then record the results to a detection file

        // xxx


















        // // free memory for ICA_SIGNAL
        // for (i = 0; i < NUM_SOURCE; i++)
        // {
        //     memory_free(ICA_SIGNAL[i]);
        // }
        // memory_free(ICA_SIGNAL);
    }

    for (i = 0; i < LAB_NUM_FLOORS; i++)
    {
        memory_free(RESP_BUF[i]);
    }
    memory_free(RESP_BUF);

    printf("[APPLICATION- Sudden Damage Detection] <II EDGE COMPUTATION> Ended (WT only).\n\r");

    printf("[APPLICATION- Sudden Damage Detection] Finished!                            \n\r");
    printf("============================================================================\n\r");
    return 0;
}
