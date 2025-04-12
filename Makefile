obj-m += mmap_module.o

mmap_module-objs := project-4-3-1.o

all:
	sudo make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc -o test_user project-4-3-2.c

clean:
	sudo make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f test_user
