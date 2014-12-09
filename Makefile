all: mfs.c mfs.h udp.o
	gcc -c -fPIC mfs.c -Wall -Werror 
	gcc -c -fPIC udp.c -Wall -Werror 
	gcc -shared -o libmfs.so mfs.o udp.o -fPIC
	gcc server.c -Wall -Werror -o server udp.o
server: server.c udp.o
	gcc server.c -Wall -Werror -o server udp.o
runserver: all
	rm -f test.img
	touch test.img
	chmod 755 test.img
	server 4000 test.img
clean:
	rm -rf *.o *.so server results
client: all
	gcc -g -L. -lmfs -Xlinker -rpath=. -o test.o test.c
runclient: client
	test.o
test:
	python /u/c/s/cs537-2/testing/p5a/runtests.py > results; cat results
	