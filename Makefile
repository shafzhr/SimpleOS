SRCFILES = $(wildcard kernel/*.c drivers/*.c utils/*.c)
HDRFILES = $(wildcard kernel/*.h drivers/*.h utils/*.h)

OBJFILES = ${SRCFILES:.c=.o}

WARNINGS = -Wall
CFLAGS = -g -masm=intel ${WARNINGS}

CC = i686-elf-gcc


os.img: boot/bootsector.bin kernel.bin
	cat $^ > $@

kernel.bin: boot/enter_kernel.o ${OBJFILES}
	i686-elf-ld -o $@ -Ttext=0x1000 $^ --oformat binary

kernel.elf: boot/enter_kernel.o ${OBJFILES}
	i686-elf-ld -o $@ -Ttext=0x1000 $^

run: os.img
	qemu-system-i386 -drive file=$<,index=0,if=floppy,format=raw

debug: os.img kernel.elf
	qemu-system-i386 -s -S -drive file=$<,index=0,if=floppy,format=raw -d guest_errors

%.o: %.c
	${CC} ${CFLAGS} -ffreestanding -c $^ -o $@

%.o: %.asm
	nasm -f elf $< -o $@

%.bin: %.asm
	nasm -f bin $< -o $@

clean:
	rm -f *.bin *.o *.elf *.img
	rm -f boot/*.o boot/*.bin
	rm -f kernel/*.o
	rm -f drivers/*.o