#!/bin/bash
rm result1.txt result2.txt
g++ test.cpp -o test.out
./test.out  modellist.txt  testing_data1.txt  result1.txt
./test.out modellist.txt   testing_data2.txt result2.txt
