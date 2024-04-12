# Sensor-Prototyping
files for sensor prototyping

## GET A CLONE OF THIS REPO IN YOUR LOCAL MACHINE

enter the folder you want to clone the repo to, and run the following command in the terminal:

```bash
git clone https://github.com/Shuaiwen-Cui/Sensor-Prototyping.git
```

## HOW TO USE THIS REPO

Use KEIL v5 to open the project file - "/BASELINE-NO-RTOS/MDK-ARM/STM32H723.uprojx", then click "Build" to compile the project, and click "Download" to download the program to the STM32H723ZGT6 board. In this repo, the dev kit is FK723M1-ZGT6 from FANKE TECHNOLOGY.

## UPLOAD TO YOUR OWN GIT REPO

Two scripts were included in this repo to facilitate the uploading of the project to your own git repo. 

Before use them, kindly replace the git repo URL in the scripts with your own git repo URL. Details can be found in the scripts themselves.

(1) upload.sh - for Mac and Linux users
    in this repo directory, run the following command in the terminal:

    bash upload.sh

(2) win-upload.bat - for Windows users
    in this repo directory, run the following command in the terminal:

    ./win-upload.bat
