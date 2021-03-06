#!/bin/bash


make
g++ simu.cpp -o simu.out 
for i in {155..165}
do                                                                                                
./train  $i  model_init.txt  seq_model_01.txt  model_01.txt
./train  $i  model_init.txt  seq_model_02.txt  model_02.txt
./train  $i  model_init.txt  seq_model_03.txt  model_03.txt                                      
./train  $i  model_init.txt  seq_model_04.txt  model_04.txt
./train  $i  model_init.txt  seq_model_05.txt  model_05.txt

rm result1.txt
rm result2.txt
./test modellist.txt testing_data1.txt result1.txt
./test modellist.txt testing_data2.txt result2.txt 
./simu.out
done
