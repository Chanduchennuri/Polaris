#include <debug/debug.h>
#include <devices/console.h>
#include <fb/fb.h>
#include <fs/devtmpfs.h>
#include <fs/ramdisk.h>
#include <fs/streams.h>
#include <fs/tmpfs.h>
#include <fs/vfs.h>
#include <ipc/pipe.h>
#include <kernel.h>
#include <klibc/event.h>
#include <klibc/module.h>
#include <mm/mmap.h>
#include <net/arp.h>
#include <net/net.h>
#include <sched/sched.h>
#include <sys/prcb.h>
#include <sys/timer.h>

void module_thread(uint64_t *ret) {
	uint64_t mod_ret = module_load("/lib/modules/hello.ko");
	if (mod_ret)
		kprintf("Failed to load module :(\n");
	else
		module_unload("/lib/modules/hello.ko");

	*ret = mod_ret;

	thread_kill(prcb_return_current_cpu()->running_thread, 1);
}

void kernel_main(void *args) {
	vfs_init();
	tmpfs_init();
	devtmpfs_init();
	vfs_mount(vfs_root, NULL, "/", "tmpfs");
	vfs_create(vfs_root, "/dev", 0755 | S_IFDIR);
	vfs_mount(vfs_root, NULL, "/dev", "devtmpfs");
	streams_init();

	kprintf("Hello I am %s running on CPU%u\n",
			prcb_return_current_cpu()->running_thread->mother_proc->name,
			prcb_return_current_cpu()->cpu_number);

	if (args != NULL) {
		uint64_t *module_info = (uint64_t *)args;
		kprintf("Ramdisk located at 0x%p\n", module_info[0]);
		ramdisk_install(module_info[0], module_info[1]);
	}

	uint64_t module_load_ret = 0;
	thread_create((uintptr_t)module_thread, (uintptr_t)&module_load_ret, 0,
				  prcb_return_current_cpu()->running_thread->mother_proc);
	if (module_load_ret)
		panic("Failed to load required kernel modules\n");
	/*
	syscall_register_handler(0x0, syscall_read);
	syscall_register_handler(0x1, syscall_write);
	syscall_register_handler(0x2, syscall_open);
	syscall_register_handler(0x3, syscall_close);
	syscall_register_handler(0x8, syscall_seek);
	syscall_register_handler(0x9, syscall_mmap);
	syscall_register_handler(0xb, syscall_munmap);
	syscall_register_handler(0x10, syscall_ioctl);
	syscall_register_handler(0x48, syscall_fcntl);
	syscall_register_handler(0x4f, syscall_getcwd);
	syscall_register_handler(0x50, syscall_chdir);
	syscall_register_handler(0x59, syscall_readdir);
	syscall_register_handler(0x101, syscall_openat);
	syscall_register_handler(0x102, syscall_mkdirat);
	syscall_register_handler(0x106, syscall_fstatat);
	syscall_register_handler(0x107, syscall_unlinkat);
	syscall_register_handler(0x109, syscall_linkat);
	syscall_register_handler(0x10b, syscall_readlinkat);
	syscall_register_handler(0x10c, syscall_fchmodat);
	syscall_register_handler(0x124, syscall_dup3);
	syscall_register_handler(0x125, syscall_pipe);

	// console_init();
	fbdev_init();

	// net_init();

		std_console_device =
			(vfs_get_node(vfs_root, "/dev/console", true))->resource;


	char *argv[] = {"/bin/init.elf", NULL};

	kprintf("Running init binary %s\n", argv[0]);

	if (!process_create_elf(
			"init", PROCESS_READY_TO_RUN, 1000, argv[0],
			prcb_return_current_cpu()->running_thread->mother_proc))
		panic("Failed to run init binary!\n");
	*/

	for (;;)
		;
}
