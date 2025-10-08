# Alfred-OS project

# How to compile and run the project on an emulator
* main project dir:
	make clean
     	./build.sh
	gdb
	* in gdb:
		add-symbol-file ./build/completeKernel.o 0x100000
		(hit y)
		b kmain
		target remote | qemu-system-x86_64 -hda ./bin/os.bin -gdb stdio -S
		(hit c to continue)
