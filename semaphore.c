#include "semaphore.h"

#define KEY 24061

int create_semaphore(){
    int semd;
    int r;
    int v;
    semd = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0644);
    if (semd == -1) {
        printf("error %d: %s", errno, strerror(errno));
        semd = semget(KEY_SEM, 1, 0);
        v = semctl(semd, 0, GETVAL, 0);
        printf("\nsemctl returned: %d", v);
        printf("\nsemaphore created");
    }
    else{
        union semun us;
        us.val = 3;
        r = semctl(semd, 0, SETVAL, us);
        printf("\nsemctl returned: %d",r);
        printf("\nsemaphore created");
    }
    return 0;
}

int remove_semaphore(int semd){
    semctl(semd, IPC_RMID, 0);
    printf("\nRemoved semaphore.");
    return 0;
}

int shared_memory(){
    int shmd;
    shmd = shmget(KEY, sizeof(int), IPC_CREAT | 0640);
    printf("\nshmd: %d", shmd);
    printf("\nshared memory created");
    return 0;
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
        fd = open(file_name, O_TRUNC);
    }
    close(fd);
    printf("file created\n");
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

int read_file(char * file_name){
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

int write_line(){
    int shmd;
    char * data;

    shmd = shmget(KEY_SHM, SEG_SIZE, 0600);
    if (shmd < 0){
        printf("Error %d: %s", errno, strerror(errno));
        return 1;
    }

    data = shmat(shmd, 0, 0);
    printf("\nLast addition: %s\n", data);
    printf("\nYour new addition: ");
    fgets(data, SEG_SIZE, stdin);
    *strchr(data, '\n') = 0;
    int fd = open(FILE_NAME, O_WRONLY | O_APPEND);
    write(fd, data, SEG_SIZE);
    close(fd);
    shmdt(data);
    return 0;
}
