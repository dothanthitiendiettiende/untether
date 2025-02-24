#include <mach/mach.h>
#include <sys/mman.h>
#include <pthread.h>
#include <stdio.h>
#include <dlfcn.h>

void lol(void) {
	puts("hello?");
//	*(uint32_t*)0x41424344 = 0;
}

void* lol2(void* arg) {
	while (1) ;;
}

int main(int argc, char* argv[]) {
	kern_return_t kr;
	thread_t th;
	mach_port_name_t mytask, mythread;
	printf("Hello, world!\n");
	mytask = mach_task_self();
	mythread = mach_thread_self();

	mmap(0x2000000, 0x100000, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, 0, 0);

	char* test = malloc(0x100);
	strcpy(test, "Hello, world! %x %x %x %x %x %x %x\n");

	pthread_t thread;
	pthread_create(&thread, NULL, lol2, NULL);

	puts("test");

//	thread_create(mytask, &th);
	th = pthread_mach_thread_np(thread);
	printf("%x\n", mytask);
	arm_thread_state_t state;
	mach_msg_type_number_t count;
	kr = thread_get_state(mythread, ARM_THREAD_STATE, (thread_state_t)&state, &count);

	uint32_t* stack_above = 0x2001000;
	stack_above[0] = 0x42069;
	stack_above[1] = 0x69420;
	stack_above[3] = 0x13371337;
	stack_above[4] = 0x6969;

//	fprintf(stderr, "%p %p\n", test, dlsym(RTLD_DEFAULT, "puts"));

//	exit(42);

//	*(uint32_t*)0x41414141 = 0;

//	memset(&state, 0, ARM_THREAD_STATE_COUNT * sizeof(uint32_t));

	for (int i = 0; i < 13; i++) {
		fprintf(stderr, "r%d=%x\n", i, state.__r[i]);
	}

	*(uint32_t*)(0x346afc48 + 0x1b4c000) = 0x23d751fc + 0x1b4c000;

	state.__r[0] = test;
	state.__r[1] = 0x1337;
	state.__r[2] = 0x420;
	state.__r[3] = 0x69;
	state.__sp = (uint32_t)stack_above;
	state.__lr = 0x23d751fc + 0x1b4c000;
	state.__pc = ((uint32_t)dlsym(RTLD_DEFAULT, "printf")) | 1;
	state.__cpsr = 0x40000020;
	kr = thread_set_state(th, ARM_THREAD_STATE, (thread_state_t)&state, ARM_THREAD_STATE_COUNT);
	kr = thread_resume(th);
//	thread_call_enter((thread_call_func_t)&lol);

	sleep(1);

	return 0;
}