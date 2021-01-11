nasm -f bin boot/bootsector.asm -o bootsector.bin &&
qemu-system-x86_64 -drive file=./bootsector.bin,format=raw