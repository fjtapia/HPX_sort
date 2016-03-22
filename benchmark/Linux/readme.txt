Benchmark
================

For to compile the two program , need to include the next directories
hpx/include and hpx/benchmark/include

Compile the file_generator.cpp program
execute the next command line
./file_generator input.bin 100000000

This create a input.bin file with random information, used in the benchmark.
This file have a size of 800 MB

Compile the program benchmark.cpp and run without parameters in the same 
directory than the  input.bin file

If you want to save the information generated execute
./benchmark >output.txt

and you will have the resuts in that file
