all:
	mkdir -p dist/x86_64 && \
	yasm -f bin -o build/x86_64/boot/boot.bin src/impl/x86_64/boot/main.asm
	gcc -ffreestanding -m32 -o build/kernel/kernel.o -c src/impl/kernel/main.c -fno-pie
	ld --oformat binary -Ttext 0x10000 -o build/kernel/main.o --entry=kmain -m elf_i386 build/kernel/kernel.o
	