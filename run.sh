rm -rf build dist
make iso
qemu-system-x86_64 -cdrom dist/x86_64/kernel.iso 
