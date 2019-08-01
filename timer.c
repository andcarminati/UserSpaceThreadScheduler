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

#include "timer.h"
#include "scheduler.h"

void start_timer(){
	time.it_interval.tv_sec = 0;
	time.it_interval.tv_usec = 1000;
	time.it_value.tv_sec = 0;
	time.it_value.tv_usec = 1000;

	signal(SIGVTALRM, (void *)schedule);

	setitimer(ITIMER_VIRTUAL, &time, NULL); 
}


