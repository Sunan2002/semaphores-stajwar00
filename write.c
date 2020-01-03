#include "semaphore.h"

int main(){

    int semd = shmget(KEY_SHM, SEG_SIZE, 0600);

    struct sembuf buf;
    buf.sem_num = 0;
    buf.sem_op = -1;
    printf("trying to get in\n");
    semop(semd, &buf, 1);

    write_line();

    buf.sem_op = 1;
    semop(semd, &buf, 1);
    
    return 0;
}