test.bin: test.c
	/opt/riscv/bin/riscv64-unknown-elf-gcc -S test.c
	/opt/riscv/bin/riscv64-unknown-elf-gcc -Wl,-Ttext=0x0 -nostdlib -march=rv64i -mabi=lp64 -o test test.s
	/opt/riscv/bin/riscv64-unknown-elf-objcopy -O binary test test.bin

mac: test.c
	/opt/homebrew/Cellar/riscv-gnu-toolchain/main/bin/riscv64-unknown-elf-gcc -S test.c
	/opt/homebrew/Cellar/riscv-gnu-toolchain/main/bin/riscv64-unknown-elf-gcc -Wl,-Ttext=0x0 -nostdlib -march=rv64i -mabi=lp64 -o test test.s
	/opt/homebrew/Cellar/riscv-gnu-toolchain/main/bin/riscv64-unknown-elf-objcopy -O binary test test.bin

linux: test.c
	/usr/bin/riscv64-linux-gnu-gcc -S test.c
	/usr/bin/riscv64-linux-gnu-gcc -Wl,-Ttext=0x0 -nostdlib -march=rv64i -mabi=lp64 -o test test.s
	/usr/bin/riscv64-linux-gnu-objcopy -O binary test test.bin

clean:
	rm -f test
	rm -f test.bin
	rm -f test.s