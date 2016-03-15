all: master producer consumer

master: master.c producer consumer ibarland-utils.o command-line-options.o
	gcc -Wall master.c -o master ibarland-utils.o command-line-options.o -lm

producer: producer.c ibarland-utils.o
	gcc -Wall producer.c -o producer ibarland-utils.o -lm

consumer: consumer.c ibarland-utils.o
	gcc -Wall consumer.c -o consumer ibarland-utils.o -lm

command-line-options.o: command-line-options.c command-line-options.h ibarland-utils.o
	gcc -Wall -c command-line-options.c

ibarland-utils.o: ibarland-utils.c ibarland-utils.h
	gcc -Wall -c ibarland-utils.c

clean:
	rm -f master producer consumer *.o

run: master
	master --queue-size 3 --produce-time 100 --consume-time 75 --duration 1000