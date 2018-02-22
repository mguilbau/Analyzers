#!/bin/bash

#SC
./bsub.sh 7 250 350 1 250 2 2 "anaV2" "job7_c2n_sub1_140218"
./bsub.sh 0 120 150 1 120 2 2 "anaV2" "job0_c2n_1_sub1_140218"
./bsub.sh 0 150 185 1 150 2 2 "anaV2" "job0_c2n_2_sub1_140218"
./bsub.sh 16 185 250 1 185 2 2 "anaV2" "job16_c2n_sub1_140218"

./bsub.sh 7 250 350 2 250 2 2 "anaV2" "job7_c2n_sub2_140218"
./bsub.sh 0 120 150 2 120 2 2 "anaV2" "job0_c2n_1_sub2_140218"
./bsub.sh 0 150 185 2 150 2 2 "anaV2" "job0_c2n_2_sub2_140218"
./bsub.sh 16 185 250 2 185 2 2 "anaV2" "job16_c2n_sub2_140218"

./bsub.sh 7 250 350 3 250 2 2 "anaV2"  "job7_c2n_sub3_140218" 
./bsub.sh 0 120 150 3 120 2 2 "anaV2"  "job0_c2n_1_sub3_140218" 
./bsub.sh 0 150 185 3 150 2 2 "anaV2"  "job0_c2n_2_sub3_140218" 
./bsub.sh 16 185 250 3 185 2 2 "anaV2" "job16_c2n_sub3_140218" 

./bsub.sh 7 250 350 4 250 2 2 "anaV2" "job7_c2n_sub4_140218" 
./bsub.sh 0 120 150 4 120 2 2 "anaV2" "job0_c2n_1_sub4_140218" 
./bsub.sh 0 150 185 4 150 2 2 "anaV2" "job0_c2n_2_sub4_140218" 
./bsub.sh 16 185 250 4 185 2 2 "anaV2" "job16_c2n_sub4_140218" 
