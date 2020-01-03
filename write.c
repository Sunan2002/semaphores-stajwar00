/*
#include "semaphore.h"

int main(){


    int semd = shmget(KEY_SHM, SEG_SIZE, 0600);

    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = -1;
    printf("trying to get in\n");
    semop(semd, &sb, 1);

    write_line();

    sb.sem_op = 1;
    semop(semd, &sb, 1);
    
    return 0;

}
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <errno.h>

#define KEY_SEM 24602
#define KEY_SHM 24601
#define SEG_SIZE 200

int main() {
  int semd = semget(KEY_SEM, 1, 0);
  char * file_name = "story";
  struct sembuf sb;

  sb.sem_num = 0;
  sb.sem_op = -1;
  printf("trying to get in\n");

  semop(semd, &sb, 1);
  int shmd;
  shmd = shmget(KEY_SHM, SEG_SIZE, 0600);
  if(shmd == -1) {
    printf("error %d: %s\n", errno, strerror(errno));
    return 1;
  }
  char * data;
  data = shmat(shmd, 0, 0);
  printf("Last addition: %s\n", data);
  printf("Your addition: ");
  fgets(data, SEG_SIZE, stdin);
  *strchr(data, '\n') = 0;
  int fd = open(file_name, O_WRONLY | O_APPEND);
  write(fd, data, SEG_SIZE);
  
  close(fd);
  shmdt(data);
  sb.sem_op = 1;
  semop(semd, &sb, 1); 
  return 0;
}
