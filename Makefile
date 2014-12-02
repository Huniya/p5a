all: mfs.c mfs.h
	gcc -c -fpic mfs.c -Wall -Werror
	gcc -shared -o libmfs.so mfs.o
clean:
	rm -rf *.o *.so
# test: 
	# python ~cs537-2/testing/p3a/MemTest.pyc .