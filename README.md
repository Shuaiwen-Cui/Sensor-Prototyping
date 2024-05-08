# Sensor-Prototyping
files for sensor prototyping

## GET A CLONE OF THIS REPO IN YOUR LOCAL MACHINE

enter the folder you want to clone the repo to, and run the following command in the terminal:

```bash
git clone https://github.com/Shuaiwen-Cui/Sensor-Prototyping.git
```

## BRANCHES

- **BASELINE-NO-RTOS**: the basic version, all three examples available with this version
- **BASELINE-NO-RTOS-NEW-NNLIB**: compared to the basic version, the NN lib is updated, but NN examples are not compatible, so only the first two examples are available in this version
- **BASELINE-NO-RTOS-SUDDEN-DAMAGE-DETECTION**: under construction, not available yet.

note: for a stable version, use the **BASELINE-NO-RTOS** branch.

## HOW TO USE THIS REPO

Use KEIL v5 to open the project file - "/BASELINE-NO-RTOS/MDK-ARM/STM32H723.uprojx", then click "Build" to compile the project, and click "Download" to download the program to the STM32H723ZGT6 board. In this repo, the dev kit is FK723M1-ZGT6 from FANKE TECHNOLOGY.

Current project has some examples in the main.c file. 

- Example 1 - FILE IO on SD card using FATFS
- Example 2 - CMSIS-DSP FFT
- Example 3 - CMSIS-NN Neural Network Inference for image classification

You can comment out the examples you don't want to run in the main.c file.

To develop your own application, you can use modulized codes and incorporate pertinent files into the project. Rember to add the related including paths in settings, in KEIL, under 'options' -> 'C/C++' -> 'Include Paths'.

## FEATURES

This repo is to facilitate IoT-sensor prototyping. Some basic functions and modules were incorporated.

- **LED**: to show program running status
- **UART**: to communicate with the host PC
- **SDMMC**: to read/write data from/to the SD card
- **FATFS**: file system for file management on the SD card
- **CMSIS-DSP**: to perform digital signal processing
- **CMSIS-NN**: to perform neural network inference onboard

NOTE: the current version does not use RTOS. 

## UPLOAD TO YOUR OWN GIT REPO

Two scripts were included in this repo to facilitate the uploading of the project to your own git repo. 

Before use them, kindly replace the git repo URL in the scripts with your own git repo URL. Details can be found in the scripts themselves.

(1) upload.sh - for Mac and Linux users
    in this repo directory, run the following command in the terminal:

    bash upload.sh

(2) win-upload.bat - for Windows users
    in this repo directory, run the following command in the terminal:

    ./win-upload.bat
