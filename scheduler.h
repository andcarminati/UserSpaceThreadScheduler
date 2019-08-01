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
#include "queue.h"


#ifndef SCHEDULER_H
#define SCHEDULER_H 
#define N_PRIO 32



typedef struct priorities {
	int count;
	int map;
	QUEUE queues[N_PRIO];
	
} PRIORITIES;

typedef struct run_queue {
	int thread_count;
	int active_count;
	int expired_count;
	PRIORITIES *active;
	PRIORITIES *expired;
	QUEUE dead;
	
} RQ;

RQ *rq;
THREAD *current;

static int priority_quantum[] = {140, 130, 120, 110, 100, 90, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35,
			  30, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6};

void init_sched(void);
void schedule(void);
void set_quantum(THREAD*);
void sched_run_queue(RQ*);
void insert_thread(THREAD*);
int select_next_prio(PRIORITIES*);
THREAD* remover_ativa(int);
void inserir_expirada(THREAD*);

#endif /*SCHEDULER_H*/
