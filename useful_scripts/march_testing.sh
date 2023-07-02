#!/bin/bash

# A script to run all march tests on Ramulator

# Uncomment following if you want to simulate everything on all systems
# declare -a algorithms=("mats" "matsp" "matspp" "marcha" "marchb" "marchc" "marchy" "marchx" "marching10")
# declare -a configs=("ALDRAM" "DDR3" "DDR4" "DSARP" "GDDR5" "HBM" "LPDDR3" "LPDDR4" "PCM" "SALP" "STTMRAM" 
#                     "TLDRAM" "WideIO" "WideIO2")

declare -a algorithms=("mats" "matspp")
declare -a configs=("DDR3" "DDR4")


for config in "${configs[@]}";
do
        for algorithm in "${algorithms[@]}";
        do
                echo "Running $algorithm test on a $config system"
                ./ramulator configs/$config-config.cfg --mode=dram --stats march_results/${config}_${algorithm}_1mb.stats traces/$algorithm.trace
        done
done

echo "March tests completed"

