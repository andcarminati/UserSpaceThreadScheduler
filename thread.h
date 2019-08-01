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


#include <sys/ucontext.h>
#include "timer.h"

#ifndef THREAD_H
#define THREAD_H  

#define THREAD_RUNNING 1
#define THREAD_ZOMBIE 2
#define THREAD_SLEEP 3

typedef struct thread{
	int id;
	int nova;
	int state;
	int quantum;
	int prio;
	int exit_status;
	ucontext_t *context;
} THREAD;

int init(void);
int create(void (* entry)(int), int arg, int prio);
int join(int tid, int * status);
void exit_thread(int status);
void yield(void);
int getid(void);

typedef struct thread_node {
	THREAD *data;
	struct thread_node *next;
} THREAD_NODE;

typedef struct thread_set {
	THREAD_NODE *head;
	int size;
} THREAD_SET;

void insert(THREAD_SET*, THREAD*);
THREAD* search_and_remove(THREAD_SET*, int tid);
THREAD_SET* alloc_set(void);



#endif
