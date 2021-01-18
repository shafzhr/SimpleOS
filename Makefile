all: run

bootsector.bin: boot/bootsector.asm
	nasm -f bin $< -o $@

kernel.o: kernel/kernel.c
	i686-elf-gcc -g -ffreestanding -c $< -o $@

enter_kernel.o: boot/enter_kernel.asm
	nasm -f elf $< -o $@

kernel.bin: kernel.o enter_kernel.o
	i686-elf-ld -o $@ -Ttext=0x1000 $^ --oformat binary

kernel.elf: kernel.o enter_kernel.o
	i686-elf-ld -o $@ -Ttext=0x1000 $^

os.img: bootsector.bin kernel.bin
	cat $^ > $@

run: os.img
	qemu-system-i386 -drive file=$<,index=0,if=floppy,format=raw

debug: os.img kernel.elf
	qemu-system-i386 -s -S -drive file=$<,index=0,if=floppy,format=raw -d guest_errors

clean:
	rm *.bin *.o *.elf