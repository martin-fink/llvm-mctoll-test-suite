/* Copyright (c) 2007-2009, Stanford University
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Stanford University nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY STANFORD UNIVERSITY ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL STANFORD UNIVERSITY BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define CHECK_ERROR(a)                                                         \
  if (a) {                                                                     \
    perror("Error at line\n\t" #a "\nSystem Msg");                             \
    exit(1);                                                                   \
  }

typedef struct {
  int *matrix_A;
  int *matrix_B;
  int *matrix_out;
  int matrix_len;
} mm_data_t;

#define BLOCK_LEN 100

/** matrix_mult()
 *  Blocked Matrix Multiply Function
 */
void matrix_mult(mm_data_t *data_in) {
  int i, j, k, a, b, c, end_i, end_j, end_k;

  for (i = 0; i < (data_in->matrix_len) * data_in->matrix_len; i++) {
    if (i % data_in->matrix_len == 0)
      printf("\n");
    printf("%d  ", data_in->matrix_A[i]);
  }
  printf("\n\n");

  for (i = 0; i < (data_in->matrix_len) * data_in->matrix_len; i++) {
    if (i % data_in->matrix_len == 0)
      printf("\n");
    printf("%d  ", data_in->matrix_B[i]);
  }
  printf("\n\n");

  for (i = 0; i < data_in->matrix_len; i += BLOCK_LEN)
    for (j = 0; j < data_in->matrix_len; j += BLOCK_LEN)
      for (k = 0; k < data_in->matrix_len; k += BLOCK_LEN) {
        end_i = i + BLOCK_LEN;
        end_j = j + BLOCK_LEN;
        end_k = k + BLOCK_LEN;
        for (a = i; a < end_i && a < data_in->matrix_len; a++)
          for (b = j; b < end_j && b < data_in->matrix_len; b++)
            for (c = k; c < end_k && c < data_in->matrix_len; c++) {
              data_in->matrix_out[(data_in->matrix_len) * a + b] +=
                  (data_in->matrix_A[(data_in->matrix_len) * a + c] *
                   data_in->matrix_B[(data_in->matrix_len) * c + b]);
            }
      }

  for (i = 0; i < data_in->matrix_len; i++) {
    for (j = 0; j < data_in->matrix_len; j++) {
      printf("%d  ", data_in->matrix_out[(data_in->matrix_len) * i + j]);
    }

    printf("\n");
  }
}

int main(int argc, char *argv[]) {
  int i, j;
  int fd_A, fd_B, create_files;
  char *fdata_A, *fdata_B;
  int matrix_len, file_size;
  struct stat finfo_A, finfo_B;
  char *fname_A, *fname_B, *fname_out;

  srand((unsigned) time(NULL));

  // Make sure a filename is specified
  if (argc < 4) {
    printf("USAGE: %s matrix_size matrix_a_file matrix_b_file [create_files]\n", argv[0]);
    exit(1);
  }

  create_files = argc > 4;

  fname_A = argv[2];
  fname_B = argv[3];
  CHECK_ERROR((matrix_len = atoi(argv[1])) < 0);
  file_size = ((matrix_len * matrix_len)) * sizeof(int);

  printf("MatrixMult: Side of the matrix is %d \n", matrix_len);
  printf("MatrixMult: Running...\n");

  /* If the matrix files do not exist, create them */
  if (create_files) {
    printf("Creating files\n");

    int value = 0;
    CHECK_ERROR((fd_A = open(fname_A, O_CREAT | O_RDWR, S_IRWXU)) < 0);
    ftruncate(fd_A, sizeof(int));
    CHECK_ERROR((fd_B = open(fname_B, O_CREAT | O_RDWR, S_IRWXU)) < 0);
    ftruncate(fd_B, sizeof(int));

    for (i = 0; i < matrix_len; i++) {
      for (j = 0; j < matrix_len; j++) {
        value = (rand()) % 11;
        write(fd_A, &value, sizeof(int));
        //printf("%d  ",value);
      }
      //printf("\n");
    }
    //printf("\n");

    for (i = 0; i < matrix_len; i++) {
      for (j = 0; j < matrix_len; j++) {
        value = (rand()) % 11;
        write(fd_B, &value, sizeof(int));
        //printf("%d  ",value);
      }
      //printf("\n");
    }

    CHECK_ERROR(close(fd_A) < 0);
    CHECK_ERROR(close(fd_B) < 0);
  }

  // Read in the file
  CHECK_ERROR((fd_A = open(fname_A, O_RDONLY)) < 0);
  // Get the file info (for file length)
  CHECK_ERROR(fstat(fd_A, &finfo_A) < 0);
  // Memory map the file
  CHECK_ERROR((fdata_A = mmap(0, file_size + 1, PROT_READ | PROT_WRITE,
                              MAP_PRIVATE, fd_A, 0)) == NULL);

  // Read in the file
  CHECK_ERROR((fd_B = open(fname_B, O_RDONLY)) < 0);
  // Get the file info (for file length)
  CHECK_ERROR(fstat(fd_B, &finfo_B) < 0);
  // Memory map the file
  CHECK_ERROR((fdata_B = mmap(0, file_size + 1, PROT_READ | PROT_WRITE,
                              MAP_PRIVATE, fd_B, 0)) == NULL);

  // Setup splitter args
  mm_data_t mm_data;
  mm_data.matrix_len = matrix_len;
  mm_data.matrix_A = ((int *) fdata_A);
  mm_data.matrix_B = ((int *) fdata_B);
  mm_data.matrix_out = calloc(sizeof(int), matrix_len * matrix_len);

  printf("MatrixMult: Calling Serial Matrix Multiplication\n");

  // gettimeofday(&starttime,0);

  matrix_mult(&mm_data);

  // printf("MatrixMult: Multiply Completed time = %ld\n", (endtime.tv_sec -
  // starttime.tv_sec));

  CHECK_ERROR(munmap(fdata_A, file_size + 1) < 0);
  CHECK_ERROR(close(fd_A) < 0);

  CHECK_ERROR(munmap(fdata_B, file_size + 1) < 0);
  CHECK_ERROR(close(fd_B) < 0);

  return 0;
}
