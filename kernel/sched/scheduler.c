/*
 * Copyright 2021 Sebastian
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "scheduler.h"
#include "../cpu/apic.h"
#include "../cpu/cpu.h"
#include "../klibc/lock.h"
#include "../klibc/printf.h"

lock_t sched_lock;

extern void context_switch(struct process_context **old,
						   struct process_context *new);

void sched_init(void) {
	while (1) {
		asm volatile("sti");
		LOCK(sched_lock);
		// Primitive priority system
		struct process *toproc = kmalloc(sizeof(struct process));
		for (int i = 0; i < ptable.length; i++) {
			struct process *proc = ptable.data[i];
			if (proc->state != READY)
				continue;

			if (proc->priority >= toproc->priority) {
				toproc = proc;
			}
		}

		size_t next_sched_tick = timer_tick + toproc->timeslice;
		while (timer_tick < next_sched_tick && toproc->state == READY) {
			this_cpu->cpu_state->running_proc = toproc;
			toproc->state = RUNNING;

			context_switch(&this_cpu->cpu_state->scheduler, toproc->context);

			this_cpu->cpu_state->running_proc = NULL;
		}
		// Free memory used for top-most process
		kfree(toproc);
		UNLOCK(sched_lock);
	}
}

inline struct process *running_proc(void) {
	asm volatile("cli");
	struct process *proc = this_cpu->cpu_state->running_proc;
	asm volatile("sti");

	return proc;
}

void yield_to_scheduler(void) {
	asm volatile("cli");
	struct process *proc = running_proc();
	if (proc->state != RUNNING) {
		context_switch(&proc->context, this_cpu->cpu_state->scheduler);
	}
}