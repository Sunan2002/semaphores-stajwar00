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
#define KEY_SHM 24601
#define SEG_SIZE 200

int create_semaphore(){
    int semd;
    int r;
    int v;
    semd = semget(KEY_SEM, 1, IPC_CREAT | IPC_EXCL | 0644);
    if (semd == -1) {
        printf("error %d: %s", errno, strerror(errno));
        semd = semget(KEY_SEM, 1, 0);
        v = semctl(semd, 0, GETVAL, 0);
        printf("\nSemaphore created");
    }
    else{
        union semun us;
        us.val = 3;
        r = semctl(semd, 0, SETVAL, us);
        printf("\nSemaphore created");
    }
    return semd;
}

int remove_semaphore(int semd){
    semctl(semd, IPC_RMID, 0);
    printf("\nRemoved semaphore.");
    return 0;
}

int shared_memory(){
    int shmd;
    shmd = shmget(KEY_SHM, SEG_SIZE, IPC_CREAT | 0640);
    printf("\nshared memory created");
    return shmd;
}

int remove_memory(int shmd){
    shmctl (shmd, IPC_RMID, 0);
    printf("\nMemory has been removed");
    return 0;
}

int create_file(char * file_name){
    int fd;
    fd = open(file_name, O_CREAT | O_EXCL | O_TRUNC, 0644);
    if (fd == -1){
        fd = open(file_name, O_TRUNC, 0644);
    }
    printf("\nFile created\n");
    return fd;
}

int remove_file(char * file_name){
    if(remove(file_name) == 0) {
        printf("\nRemoved File");
    }
    else {
        printf("\nFile could not be removed");
    }
    return 0;
}

/* int read_file(char * file_name){
    int fd;
    fd = open(file_name, O_RDONLY);
    if (fd < 0){
        printf("Error %d: %s", errno, strerror(errno));
        return 1;
    }
    printf("The story so far:\n");
    char buff[SEG_SIZE];
    while (read(fd, buff, SEG_SIZE) > 0){
        printf("%s\n", buff);
    }
    close(fd);
    return 0;
}
*/



int main(int argc, char * argv[]){

    int semd;
    int shmd;
    int fd;
    char * file_name = "story";
    
    if (argc > 1){

        if(strcmp(argv[1], "-c") == 0){
            semd = create_semaphore();
            shmd = shared_memory();
            fd = create_file(file_name);
        }

        else if(strcmp(argv[1], "-r") == 0){

            int semd = semget(KEY_SEM, 1, 0);
            struct sembuf sb;
            sb.sem_num = 0;
            sb.sem_op = -1;
            printf("\ntrying to get in");
            semop(semd, &sb, 1);

            //read_file(FILE_NAME);
            fd = open(file_name, O_RDONLY);
            if (fd < 0){
                printf("Error %d: %s", errno, strerror(errno));
                return 1;
            }
            printf("The story so far:\n");
            char buff[SEG_SIZE];
            while (read(fd, buff, SEG_SIZE) > 0){
                printf("%s\n", buff);
            }
            printf("\n\n");

            semd = semget(KEY_SEM, 1, 0);
            shmd = shmget(KEY_SHM, SEG_SIZE, 0600);
            remove_memory(shmd);
            remove_file(file_name);
            remove_semaphore(semd);

        }

        else if(strcmp(argv[1], "-v") == 0){
            fd = open(file_name, O_RDONLY);
            if (fd < 0){
                printf("Error %d: %s", errno, strerror(errno));
                return 1;
            }
            printf("The story so far:\n");
            char buff[SEG_SIZE];
            while (read(fd, buff, SEG_SIZE) > 0){
                printf("%s\n", buff);
            }
            close(fd);
        }
    }
    else{
        printf("\nPlease use either -c, -r, -v\n");
    }
    return 0;
}
