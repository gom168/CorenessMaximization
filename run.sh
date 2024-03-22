#!/bin/bash
dataset="twitter_copen"
pre_list=(0 1)  # 1 pre 0 without pre
mode_list=(0 1 2)  # 0 vertex 1 group 2 vertex+group
b_list=(10)
verify=0
output_dir="./output/$dataset/"
data_dir="./dataset/${dataset}.txt"
if [ ! -d "$output_dir" ]; then
    mkdir -p "$output_dir"
fi

for b in ${b_list[@]}
do
    for mode in ${mode_list[@]}
    do
        for pre in ${pre_list[@]}
        do
            if [[ $pre -eq 1 && $mode -eq 1 ]]; then
                    continue
            fi
            if [[ $pre -eq 0 && $mode -eq 0 ]]; then
                model_name="VS"
            fi
            if [[ $pre -eq 0 && $mode -eq 1 ]]; then
                model_name="GS"
            fi
            if [[ $pre -eq 0 && $mode -eq 2 ]]; then
                model_name="VGB"
            fi
            if [[ $pre -eq 1 && $mode -eq 0 ]]; then
                model_name="VS_pre"
            fi
            if [[ $pre -eq 1 && $mode -eq 2 ]]; then
                model_name="VGB_pre"
            fi
            insert_dir="./output/$dataset/insert_b=${b}_pre=${pre}_mode=${mode}_.txt"
            (/usr/bin/time -v nohup ./CoreMaximization $data_dir $b $insert_dir $verify $pre $mode &) >&  ./output/$dataset/${dataset}_exp_b=${b}_model=${model_name}.txt
        done
    done
done



