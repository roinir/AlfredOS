FILES = ./build/kernel.asm.o ./build/kernel.o
FLAGS = -g -ffreestanding -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc

all:
	nasm -f bin ./src/boot.asm -o ./bin/boot.bin -O0
	nasm -f elf -g ./src/kernel.asm -o ./build/kernel.asm.o
	i686-linux-gnu-gcc -I./src $(FLAGS) -std=gnu99 -c ./src/kernel.c -o ./build/kernel.o
	i686-linux-gnu-gcc -g -r $(FILES) -o ./build/completeKernel.o
	i686-linux-gnu-gcc -ffreestanding -O0 -nostdlib -T ./src/linkerScript.ld ./build/completeKernel.o -o ./bin/kernel.elf
	i686-linux-gnu-objcopy -O binary ./bin/kernel.elf ./bin/kernel.bin	
	
	dd if=./bin/boot.bin >> ./bin/os.bin	
	dd if=./bin/kernel.bin >> ./bin/os.bin
	dd if=/dev/zero bs=512 count=8 >> ./bin/os.bin

clean:
	rm -f ./bin/boot.bin
	rm -f ./bin/kernel.bin
	rm -f ./bin/os.bin
	rm -f ./build/kernel.asm.o
	rm -f ./build/kernel.o
	rm -f ./build/completeKernel.o
