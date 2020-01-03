  
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/shm.h>
#include <fcntl.h>

#define KEY_SEM 24602
#define KEY_SHM 24603
#define SEG_SIZE 200
#define FILE_NAME "story"

int create_semaphore();
int remove_semaphore(int semd);
int shared_memory();
int remove_memory(int shmd);
int create_file(char * file_name);
int remove_file(char * file_name);
int read_file(char * file_name);
int write_line();