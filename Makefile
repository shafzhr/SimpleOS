all: run

bootsector.bin: boot/bootsector.asm
	nasm -f bin $< -o $@

kernel.o: kernel/kernel.c
	i686-elf-gcc -ffreestanding -c $< -o $@

enter_kernel.o: boot/enter_kernel.asm
	nasm -f elf $< -o $@

kernel.bin: kernel.o enter_kernel.o
	i686-elf-ld -o $@ -Ttext=0x1000 $^ --oformat binary

os.img: bootsector.bin kernel.bin
	cat $^ > $@

run: os.img
	qemu-system-i386 -drive file=$<,index=0,if=floppy,format=raw

debug: os.img
	qemu-system-i386 -s -S -fda $<

clean:
	rm *.bin *.o