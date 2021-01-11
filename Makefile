all: run

bootsector.bin: boot/bootsector.asm
	nasm -f bin $< -o $@

os.bin: bootsector.bin
	cat $^ > $@

run: os.bin
	qemu-system-i386 -drive file=$<,format=raw
