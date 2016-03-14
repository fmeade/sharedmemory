all: master producer consumer

master: master.c producer consumer ibarland-utils.o command-line-options.o
	gcc -Wall master.c -o master ibarland-utils.o -lrt -lm

producer: producer.c ibarland-utils.o
	gcc -Wall producer.c -o producer ibarland-utils.o -lrt -lm

consumer: consumer.c ibarland-utils.o
	gcc -Wall consumer.c -o consumer ibarland-utils.o -lrt -lm

command-line-options.o: command-line-options.c command-line-options.h ibarland-utils.o
	gcc -Wall -c command-line-options.c

ibarland-utils.o: ibarland-utils.c ibarland-utils.h
	gcc -Wall -c ibarland-utils.c

clean:
	rm -f master producer consumer *.o