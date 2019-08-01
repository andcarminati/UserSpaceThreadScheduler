#include "queue.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv){
	QUEUE *q;
	
	q = alloc_queue();

	int i1 = 1, i2 = 2, i3 = 3, i4 = 4;
	
	enqueue(q, &i1);
	enqueue(q, &i2);
	enqueue(q, &i3);
	enqueue(q, &i4);
	
	int *i;
	int tam;

	tam = q->size;
	i = dequeue(q);

	printf("n = %d, %d \n",tam, *i);

	tam = q->size;
	i = dequeue(q);

	printf("n = %d, %d \n",tam, *i);

	tam = q->size;
	i = dequeue(q);

	printf("n = %d, %d \n",tam, *i);
	

	tam = q->size;
	i = dequeue(q);

	printf("n = %d, %d \n",tam, *i);

}

