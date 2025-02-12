all: low med high

low:
	gcc -g -o main_section2_low main_section2.c assignment1.c assignment2.c governor.c scheduler.o LOW/workload.o LOW/deadlines.c -lwiringPi -lpthread

med:
	gcc -g -o main_section2_med main_section2.c assignment1.c assignment2.c governor.c scheduler.o MED/workload.o MED/deadlines.c -lwiringPi -lpthread

high:
	gcc -g -o main_section2_high main_section2.c assignment1.c assignment2.c governor.c scheduler.o HIGH/workload.o HIGH/deadlines.c -lwiringPi -lpthread

clean:
	rm -rf main_section2_low main_section2_med main_section2_high
