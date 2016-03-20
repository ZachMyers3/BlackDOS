dd if=/dev/zero of=floppya.img bs=512 count=2880
dd if=bootload of=floppya.img bs=512 count=1 conv=notrunc
dd if=map.img of=floppya.img bs=512 count=1 seek=1 conv=notrunc
dd if=dir.img of=floppya.img bs=512 count=1 seek=2 conv=notrunc
bcc -ansi -c -o kernel.o kernel.c
as86 kernel.asm -o kernel_asm.o
ld86 -o kernel -d kernel.o kernel_asm.o
dd if=kernel of=floppya.img bs=512 conv=notrunc seek=3
chmod +x loadFile
bcc -ansi -c -o fib.o fib.c
as86 lib.asm -o lib_asm.o
ld86 -o fib -d fib.o lib_asm.o
bcc -ansi -c -o kitty.o kitty.c
ld86 -o kitty -d kitty.o lib_asm.o
bcc -ansi -c -o cal.o cal.c
ld86 -o cal -d cal.o lib_asm.o
./loadFile msg
./loadFile test1
./loadFile test2
./loadFile fib
./loadFile kitty
./loadFile cal
