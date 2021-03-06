#!/usr/bin/env bash

set -euo pipefail

# Arguments:    benchmark                                   clang flags     iter    benchmark flags
./benchmark.py "histogram/histogram-seq.c"                  "-O3"           "100"   "histogram/inputs/small.bmp"
./benchmark.py "histogram/histogram-seq.c"                  "-O3"           "50"    "histogram/inputs/med.bmp"
./benchmark.py "histogram/histogram-seq.c"                  "-O3"           "25"    "histogram/inputs/large.bmp"
./benchmark.py "kmeans/kmeans-seq.c"                        "-O3 -mno-sse"  "100"
./benchmark.py "linear_regression/linear_regression-seq.c"  "-O3"           "250"   "linear_regression/inputs/key_file_50MB.txt"
./benchmark.py "linear_regression/linear_regression-seq.c"  "-O3"           "100"   "linear_regression/inputs/key_file_100MB.txt"
./benchmark.py "linear_regression/linear_regression-seq.c"  "-O3"           "50"    "linear_regression/inputs/key_file_500MB.txt"
./benchmark.py "matrix_multiply/matrix_multiply-seq.c"      "-O3"           "100"   "200" "matrix_multiply/inputs/matrix_a" "matrix_multiply/inputs/matrix_b"
./benchmark.py "pca/pca-seq.c"                              "-O3 -mno-sse"  "1000"  "-r 100 -c 100 -s 1000"
./benchmark.py "pca/pca-seq.c"                              "-O3 -mno-sse"  "500"   "-r 200 -c 200 -s 1000"
./benchmark.py "pca/pca-seq.c"                              "-O3 -mno-sse"  "250"   "-r 1000 -c 1000 -s 1000"
./benchmark.py "string_match/string_match-seq.c"            "-O3 -mno-sse"  "250"   "string_match/inputs/key_file_50MB.txt"
./benchmark.py "string_match/string_match-seq.c"            "-O3 -mno-sse"  "100"   "string_match/inputs/key_file_100MB.txt"
./benchmark.py "string_match/string_match-seq.c"            "-O3 -mno-sse"  "25"    "string_match/inputs/key_file_500MB.txt"
./benchmark.py "word_count/word_count-seq.c"                "-O3"           "250"   "word_count/inputs/word_10MB.txt"

# Arguments:    benchmark                                       clang flags     iter    benchmark flags
./benchmark.py "histogram/histogram-pthread.c"                  "-O3"           "100"   "histogram/inputs/small.bmp"
./benchmark.py "histogram/histogram-pthread.c"                  "-O3"           "50"    "histogram/inputs/med.bmp"
./benchmark.py "histogram/histogram-pthread.c"                  "-O3"           "25"    "histogram/inputs/large.bmp"
./benchmark.py "kmeans/kmeans-pthread.c"                        "-O3 -mno-sse"  "100"
# linear_regression: missing support for movups
./benchmark.py "linear_regression/linear_regression-pthread.c"  "-O3"           "250"   "linear_regression/inputs/key_file_50MB.txt"
./benchmark.py "linear_regression/linear_regression-pthread.c"  "-O3"           "100"   "linear_regression/inputs/key_file_100MB.txt"
./benchmark.py "linear_regression/linear_regression-pthread.c"  "-O3"           "50"    "linear_regression/inputs/key_file_500MB.txt"
# matrix_multiply:  Use still stuck around after Def is destroyed
./benchmark.py "matrix_multiply/matrix_multiply-pthread.c"      "-O3"           "150"   "200" "matrix_multiply/inputs/matrix_a" "matrix_multiply/inputs/matrix_b"
# pca: segfaults
#./benchmark.py "pca/pca-pthread.c"                              "-O3 -mno-sse"  "1000"  "-r 100 -c 100 -s 1000"
#./benchmark.py "pca/pca-pthread.c"                              "-O3 -mno-sse"  "500"   "-r 200 -c 200 -s 1000"
#./benchmark.py "pca/pca-pthread.c"                              "-O3 -mno-sse"  "250"   "-r 1000 -c 1000 -s 1000"
./benchmark.py "string_match/string_match-pthread.c"            "-O3"           "250"   "string_match/inputs/key_file_50MB.txt"
./benchmark.py "string_match/string_match-pthread.c"            "-O3"           "100"   "string_match/inputs/key_file_100MB.txt"
./benchmark.py "string_match/string_match-pthread.c"            "-O3"           "25"    "string_match/inputs/key_file_500MB.txt"
./benchmark.py "word_count/word_count-pthread.c"                "-O3"           "250"   "word_count/inputs/word_10MB.txt"
