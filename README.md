# CorenessMaximization

This is the source code of our paper "Coreness Maximization: An Edge Insertion Method to Enhance Network Stability".

## Experiment Settings

 All algorithms are implemented in C++ and compiled by g++ compiler at -O3 optimization level, and they are conducted on a machine with Inter(R) Xeon(R) CPU E5-2667 v4@3.20GHz processor and 128GB memory, with Ubuntu installed.

## Compile

```shell
cd CoreMaximization
cmake .
make
```

## Run

```shell
./run.sh
```

The shell script automatically runs five core algorithms, VS, GS, VGB, VS-pre and VGB-pre. The corresponding output will automatically generate an output folder and remain in the corresponding file. The initial default b setting is 50.
