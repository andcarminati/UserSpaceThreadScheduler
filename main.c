#include "thread.h"
#include "scheduler.h"
#include <stdio.h>

int foo(int i){
	sleep(10);
	while(1){
		printf("Thread  A\n");
	}
}

int foo2(int i){
	while(1){
		printf("\t\t\t\t\t\t\t\tThread  C\n");
	}
}

/*
 *
 * Prioridades vão de 0 a 31
 * 
 */
int main(){
	init();
	create((void *) &foo, 1, 31);
	create((void *) &foo2, 1, 31);
	while(1){
		printf("\t\t\t\tThread  B\n");
	}
}

