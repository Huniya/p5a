all: mfs.c mfs.h
	gcc -c -fpic mfs.c -Wall -Werror
	gcc -shared -o libmfs.so mfs.
server: server.c udp.o
	# gcc -c -fpic udp.c -Wall -Werror
	gcc server.c -Wall -Werror -o server udp.o
runserver: server
	server 4000 data.img
clean:
	rm -rf *.o *.so server
# test: 
	# python ~cs537-2/testing/p3a/MemTest.pyc .