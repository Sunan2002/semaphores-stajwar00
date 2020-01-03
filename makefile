all: control.o write.o control.o semaphore.o
	gcc -o control control.o semaphore.o
	gcc -o write write.o semaphore.o

control.o: control.c semaphore.h semaphore.o
	gcc -c control.c

write.o: write.c semaphore.h semaphore.o
	gcc -c write.c

semaphore.o: semaphore.c semaphore.h
	gcc -c semaphore.c

clean:
	rm *.o
	rm control
	rm write
	rm story
	rm telephone