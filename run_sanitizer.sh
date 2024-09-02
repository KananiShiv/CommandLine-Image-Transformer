#!/bin/bash
mkdir -p ./tests/actual_outputs
./build/hw2_main_sanitizer -c 125,130,150,40 -p 85,130 -i ./tests/images/stony.sbu -o ./tests/actual_outputs/result.ppm -r "Go STONY BROOK,./tests/fonts/font1.txt,2,50,5"
./build/hw2_main_sanitizer -c 125,130,150,40 -i ./tests/images/stony.sbu -p 85,130 -o ./tests/actual_outputs/result.ppm -r "Go STONY BROOK,./tests/fonts/font4.txt,2,100,10"
./build/hw2_main_sanitizer -i ./tests/images/desert.sbu -p 90,60 -c 90,10,50,100 -o ./tests/actual_outputs/result.ppm
./build/hw2_main_sanitizer -o ./tests/actual_outputs/result.ppm -c 170,100,90,180 -p 0,0 -i ./tests/images/stony.ppm
./build/hw2_main_sanitizer -o ./tests/actual_outputs/result.ppm -c 36,60,50,50 -p 50,80 -i ./tests/images/seawolf.ppm
./build/hw2_main_sanitizer -i ./tests/images/stony.ppm -p 22,32 -o ./tests/actual_outputs/result.ppm -c 20,30,120,140
./build/hw2_main_sanitizer -i ./tests/images/seawolf.ppm -o ./tests/actual_outputs/result.ppm
./build/hw2_main_sanitizer -o ./tests/actual_outputs/result.ppm -i ./tests/images/desert.sbu
./build/hw2_main_sanitizer -i ./tests/images/desert.sbu -r "seawolves,./tests/fonts/font1.txt,1,100,150" -o ./tests/actual_outputs/result.ppm
./build/hw2_main_sanitizer -i ./tests/images/desert.sbu -o ./tests/actual_outputs/result.ppm -r "new YORK state,./tests/fonts/font3.txt,1,10,220"