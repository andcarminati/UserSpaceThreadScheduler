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
#include "scheduler.h"
#include "thread.h"
#include "timer.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/ucontext.h>

#define DEBUG 1

int enter_sched = 1;
sigset_t intmask;

void disable_interrupts(){
	if (sigprocmask(SIG_BLOCK, &intmask, NULL) == -1){
	    exit(-1);
    }		
}

void enable_interrupts(){
	if (sigprocmask(SIG_UNBLOCK, &intmask, NULL) == -1){
	    exit(-1);	 
	} 	
}		

void init_sched(){

	rq = (RQ*) malloc(sizeof(RQ));
	rq->thread_count = 0;
	rq->active = (PRIORITIES*) malloc(sizeof(PRIORITIES));
	rq->expired = (PRIORITIES*) malloc(sizeof(PRIORITIES));
	
	if ((sigemptyset(&intmask) == -1) || (sigaddset(&intmask, SIGINT) == -1)){
       perror("Failed to initialize the signal mask");
       exit(-1);
    }   

}
void schedule(){
	disable_interrupts();
	
#if defined(DEBUG)
		//printf("==============Escalonador ativado=============\n");
#endif
	
	getcontext(current->context);
	
	if(enter_sched){
		enter_sched = 0;
		//setcontext(current->context);
		/*a partir desse ponto o escalonamento acontece
		* Se a thread terminou, ela esta no estado THREAD_ZOMBIE
		* ficam mantida até o pai chamar join, ou terminar
		*/
		if(current->state == THREAD_ZOMBIE){
			// colocar a thread em um local seguro
			rq->active_count--;
			rq->thread_count--;
			//rq->dead(current);
		} else {
			/*Se só existir um thread o quantum nao e alterado
			* ela retoma sua execucao
			*/
			if(rq->thread_count == 1){
				//puts("Somente uma thread");
				setcontext(current->context);
				//return; // otimizacao
			} else {
				current->quantum--; /*o quantum deve ser decrementado*/
				/* O Quantum expirrou?*/
				if(current->quantum > 0){
					setcontext(current->context);
					
				} else {
					inserir_expirada(current);
				}
				/* Se todas as threads expirraram seu quantum
				*  ocorre a mudanca na escala de execucao (new Epoch)
				*/
				if(rq->active_count == 0){
					sched_run_queue(rq);
				}
				/*A partir desse ponto fica facil, uma nova thread é escalonada
				* para o processador, com os sequintes passos:
				* 1 - Procura-se a thread de maior prioridade em active (pode ter varias),
				* 	retira-se ela.
				* 2 - current = thread escalonada
				* 3 - setcontext(current->context).
				* a thread escalonada retoma sua execucao (inicio de schedule), e saira dele
				* pois a flag enter_sched estará em 0	
				*/	
				int prioridade_atual = select_next_prio(rq->active);
				current = remover_ativa(prioridade_atual);
				
				if(current->nova){
				  current->nova = 0;
                  enable_interrupts(); 	
				}
				setcontext(current->context);
			}
		}
	} else {
		enter_sched = 1;
	}
    enable_interrupts();
	return;
	
}
void set_quantum(THREAD *th){
	th->quantum = priority_quantum[th->prio];
}
void sched_run_queue(RQ *r){
	PRIORITIES *exp;
	r->active_count = r->expired_count;
	r->expired_count = 0;

	exp = r->expired;
	r->expired = r->active;
	r->active = exp;
}

void insert_thread(THREAD* th){
	if(th != current){
		enqueue(&rq->active->queues[th->prio], th);
    }	
    rq->thread_count++;
	rq->active_count++;
}	


int select_next_prio(PRIORITIES* prio){
	int i;
	for(i = 0; i < N_PRIO; i++){
		if(prio->queues[i].size > 0){
			return i;
		}	
	}
	return -1;	
}	

THREAD* remover_ativa(int prio){
	THREAD* th;
	th = dequeue(&rq->active->queues[prio]);
	return th;
}	
void inserir_expirada(THREAD* th){
	enqueue(&rq->expired->queues[th->prio], th);
	set_quantum(th);
	rq->active_count--;
	rq->expired_count++;
}
