CC=gcc

programa: main.o timer.o scheduler.o thread.o queue.o
	$(CC) -o minhasthreads main.o timer.o scheduler.o thread.o queue.o -g

clean:
	rm -rf *.o 
