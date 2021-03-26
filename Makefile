SRCFILES = $(wildcard kernel/*.c drivers/*.c utils/*.c cpu/*.c)
HDRFILES = $(wildcard kernel/*.h drivers/*.h utils/*.h cpu/*.h)

OBJFILES = ${SRCFILES:.c=.o cpu/interrupt.o}

WARNINGS = -Wall -Wextra

# intel | att
ASMCONF = att
CFLAGS = -g -ffreestanding -masm=${ASMCONF} ${WARNINGS} -m32 -nostartfiles

CC = i686-elf-gcc


os.img: boot/bootsector.bin kernel.bin
	cat $^ > $@

kernel.bin: boot/enter_kernel.o ${OBJFILES}
	i686-elf-ld -o $@ -Ttext=0x1000 $^ --oformat binary

kernel.elf: boot/enter_kernel.o ${OBJFILES}
	i686-elf-ld -o $@ -Ttext=0x1000 $^

run: os.img
	sudo qemu-system-i386 -drive file=$<,index=0,if=floppy,format=raw \
	-netdev tap,id=mynet0,ifname=tap0,script=no,downscript=no \
	-device rtl8139,netdev=mynet0,mac=52:55:00:d1:55:01 \
	-object filter-dump,id=f1,netdev=mynet0,file=./log/traffic1.pcap \
	-netdev tap,id=mynet1,ifname=tap1,script=no,downscript=no \
	-device rtl8139,netdev=mynet1 \
	-object filter-dump,id=f2,netdev=mynet1,file=./log/traffic2.pcap \
	# https://gist.github.com/extremecoders-re/e8fd8a67a515fee0c873dcafc81d811c

debug: os.img kernel.elf
	sudo qemu-system-i386 -s -S -drive file=$<,index=0,if=floppy,format=raw -d guest_errors \
	-netdev tap,id=mynet0,ifname=tap0,script=no,downscript=no \
	-device rtl8139,netdev=mynet0,mac=52:55:00:d1:55:01 \
	-object filter-dump,id=f1,netdev=mynet0,file=./log/traffic.pcap \
	-net nic,model=rtl8139


grub_os.img: grub/boot.o ${OBJFILES}
	${CC} -T grub/linker.ld -o $@ -ffreestanding -nostdlib $^

grub_os.iso: grub_os.img
	mkdir -p isodir/boot/grub
	cp grub_os.img isodir/boot/$<
	cp grub/grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o $@ isodir

grub: grub_os.iso
	sudo qemu-system-i386 -cdrom $< \
	-net nic,model=rtl8139 \
	-net nic,model=rtl8139


grub/boot.o: grub/boot.asm
	nasm -felf32 $^ -o $@

%.o: %.c
ifeq (${ASMCONF}, intel)
	${CC} ${CFLAGS} -c $^ -o $@ -D"ASMINTEL"
else
	${CC} ${CFLAGS} -c $^ -o $@
endif

%.o: %.asm
	nasm -f elf $< -o $@

%.bin: %.asm
	nasm -f bin $< -o $@

clean:
	rm -f *.bin *.o *.elf *.img *.iso
	rm -f boot/*.o boot/*.bin
	rm -f kernel/*.o
	rm -f drivers/*.o
	rm -f utils/*.o
	rm -f cpu/*.o
	rm -f grub/*.o
	rm -rf isodir