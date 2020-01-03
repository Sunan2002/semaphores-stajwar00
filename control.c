#include "semaphore.h"

int main(int argc, char * argv[]){
    
    if (argc > 1){

        if(strcmp(argv[1], "-c") == 0){
            create_semaphore();
            shared_memory();
            create_file(FILE_NAME);
        }

        else if(strcmp(argv[1], "-r") == 0){

            int semd = semget(KEY_SEM, 1, 0);
            struct sembuf buf;
            buf.sem_num = 0;
            buf.sem_op = -1;
            semop(semd, &buf, 1);

            read_file(FILE_NAME);
            int shmd = shmget(KEY_SHM, SEG_SIZE, 0600);
            remove_memory(shmd);
            remove_file(FILE_NAME);
            remove_semaphore(semd);

        }

        else if(strcmp(argv[1], "-v") == 0){
            read_file(FILE_NAME);
        }
    }
    else{
        printf("\nPlease use either -c, -r, -v\n");
    }
    return 0;
}