/*
Simple userspace thread scheduler
Copyright (C) 2016  Andreu Carminati
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

void enqueue(QUEUE *q, void *data){
	NODE *n = NULL;
	NODE *aux = NULL;
	
	n = (NODE*) malloc(sizeof(NODE));
	n->data = data;
	n->next = NULL;
	
	if(!q->head){	
		q->head = n;
	} else{
		aux = q->head;
		while(aux->next != NULL){
			aux = aux->next;
		}
		aux->next = n;
		
	}
	q->size++;
} 

void* dequeue(QUEUE *q){
	NODE *n;
	void *data;
	
	if(!q->head){
		data = NULL;
	} else {
		n = q->head;
		data = n->data;
		q->head = n->next;
		free(n);
		q->size--;
	}

	return data;
}

void* alloc_queue(void){
	QUEUE *q;

	q = (QUEUE*) malloc(sizeof(QUEUE));
	q->head = NULL;
	q->size = 0;

	return q;
}

