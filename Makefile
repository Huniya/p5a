all: mfs.c mfs.h
	gcc -c -fPIC mfs.c -Wall -Werror 
	gcc -c -fPIC udp.c -Wall -Werror 
	gcc -shared -o libmfs.so mfs.o udp.o -fPIC
server: server.c udp.o
	# gcc -c -fpic udp.c -Wall -Werror
	gcc server.c -Wall -Werror -o server udp.o
runserver: server
	server 4000 data.img
clean:
	rm -rf *.o *.so server
client: all
	gcc -g -L. -lmfs -Xlinker -rpath=. -o test.o test.c
runclient:
	test.o
# test: 
	# python ~cs537-2/testing/p3a/MemTest.pyc .