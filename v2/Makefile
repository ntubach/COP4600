obj-m += cop.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	$(CC) testcop.c -o test

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
