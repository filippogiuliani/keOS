REM qemu-system-arm -kernel output/kernel.img -cpu arm1176 -serial stdio -m 256 -M raspi -nographic
REM qemu-system-arm -kernel output/kernel.img -cpu arm1176 -serial stdio -m 256 -M versatilepb -nographic

qemu-system-arm -kernel output/kernel.img -cpu arm1176 -serial stdio -m 256 -M raspi
