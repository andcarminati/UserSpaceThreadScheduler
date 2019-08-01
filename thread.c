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

#include "thread.h"
#include "timer.h"
#include "scheduler.h"
#include <sys/ucontext.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int thread_count = 0;

int __run_thread(void (* entry)(), int arg){
	
	entry(arg);
	while(1);
	exit_thread(0);
}

int init(void){
	THREAD *th;

	init_sched();
	th = malloc(sizeof(THREAD));
	th->context = malloc(sizeof(ucontext_t));
	th->state = THREAD_RUNNING;
	th->quantum = 10;
	th->prio = 0x1F;
	th->nova = 0;
	th->id = thread_count++;
	current = th;
	start_timer();
	
	rq->thread_count++;
	rq->active_count++;
}
char stack[SIGSTKSZ];
int create(void (* entry)(), int arg, int prio){
	THREAD *th;

	th = malloc(sizeof(THREAD));
	th->context = malloc(sizeof(ucontext_t));
	th->id = thread_count++;
    th->nova = 1;
	th->context->uc_link = NULL;
	th->context->uc_stack.ss_sp = malloc(16384);
	th->context->uc_stack.ss_size = 16384;
	th->exit_status = 0;
	getcontext(th->context);
	makecontext(th->context, __run_thread, 2, entry, arg);

	th->prio = prio;

	set_quantum(th);
    insert_thread(th);
		
	return 0;
}

int join(int tid, int * status){

}

void exit_thread(int status){
	current->exit_status = 0;
	current->state = THREAD_ZOMBIE;
	schedule();
}

void yield(void){
	current->quantum = 0;
	schedule();
}
int getid(void){
	return current->id;
}

void insert(THREAD_SET* set, THREAD* thread){
	
}
	
THREAD* search_and_remove(THREAD_SET* set, int tid){
	return NULL;
}
	
THREAD_SET* alloc_set(void){
	THREAD_SET *q;

	q = (THREAD_SET*) malloc(sizeof(THREAD_SET));
	q->head = NULL;
	q->size = 0;

	return q;
}
 
