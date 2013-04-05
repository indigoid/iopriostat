/*
 * iopriostat:
 * 	display a breakdown of processes by IO scheduling class
 * 	and priority level in that class
 *
 * No command-line parameters required.
 *
 * Copyright (C) 2013 John Slee <john@slee.id.au>
 * 
 * Released under the terms of the GNU General Public License version 2
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

#include "ioprio.h"

#define MAXLEVEL 7

struct iopriostat {
	int n_none, n_idle, n_besteffort, n_rt;
	int n_besteffort_hist[MAXLEVEL+1], n_rt_hist[MAXLEVEL+1];
};

void investigate(struct iopriostat* state, int pid) {
	/* is a process */
	int ioprio = ioprio_get(IOPRIO_WHO_PROCESS, pid);
	if (ioprio == -1) {
		/* failed, maybe the process went away? */
		perror("ioprio_get");
		return;
	}
	int data = IOPRIO_PRIO_DATA(ioprio);
	int class = IOPRIO_PRIO_CLASS(ioprio);
	if (data > MAXLEVEL) {
		perror("weird IO scheduling level");
		return;
	}	
	switch (class) {
	case IOPRIO_CLASS_NONE:
		state->n_none++;
		break;
	case IOPRIO_CLASS_RT:
		state->n_rt++;
		state->n_rt_hist[data]++;
		break;
	case IOPRIO_CLASS_BE:
		state->n_besteffort++;
		state->n_besteffort_hist[data]++;
		break;
	case IOPRIO_CLASS_IDLE:
		state->n_idle++;
		break;
	default:
		fprintf(stderr, "weird IO scheduling class: %d\n", class);
	}
}

void print_ints(int* p, int n) {
	printf("%d", p[0]);
	for (int i = 1; i < n; i++) {
		printf(" %d", p[i]);
	}
	printf("\n");
}

void display(struct iopriostat* state) {
	printf("none:        total %d\n", state->n_none);
	printf("idle:        total %d\n", state->n_idle);
	printf("best-effort: total %d by-level ", state->n_besteffort);
	print_ints((int*) &(state->n_besteffort), MAXLEVEL+1);
	printf("realtime:    total %d by-level ", state->n_rt);
	print_ints((int*) &(state->n_rt_hist), MAXLEVEL+1);
}

int main(void) {
	struct iopriostat results;
	DIR* dirp = opendir("/proc");
	struct dirent* ent;

	memset(&results, 0, sizeof(results));

	if (!dirp) {
		perror("can't read /proc");
		return 1;
	}
	while ((ent = readdir(dirp)) != NULL) {
		int pid = atoi(ent->d_name);
		if (pid > 1) {
			investigate(&results, pid);
		}
	}
	display(&results);
	return 0;
}
