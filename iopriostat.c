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

void display(struct iopriostat* state) {
	if (state->n_none > 0)
		printf("%d processes with no scheduling class\n", state->n_none);
	if (state->n_rt > 0) {
		printf("%d processes with real-time IO priority\n", state->n_rt);
		for (int i = 0; i <= MAXLEVEL; i++) {
			int count = state->n_rt_hist[i];
			if(count > 0)
				printf("\t%d processes at real-time priority %d\n", count, i);
		}
	} else {
		printf("No processes with real-time IO priority\n");
	}
	if (state->n_besteffort > 0) {
		printf("%d processes with best-effort IO priority\n", state->n_besteffort);
		for (int i = 0; i <= MAXLEVEL; i++) {
			int count = state->n_besteffort_hist[i];
			if(count > 0)
				printf("\t%d processes at best-effort priority %d\n", count, i);
		}
	} else {
		printf("No processes with best-effort IO priority\n");
	}
	if (state->n_idle > 0) {
		printf("%d processes with idle IO priority\n", state->n_rt);
	} else {
		printf("No processes with idle IO priority\n");
	}
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
