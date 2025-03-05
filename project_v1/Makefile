all:
	gcc -o button help_button.c -lwiringPi
	gcc `pkg-config --cflags gtk+-3.0` -o main main.c trigger_alarms.c window.c compartment.c get_hw.c `pkg-config --libs gtk+-3.0` -lwiringPi -lcjson -pthread -lpthread
clean:
	rm -rf button