all:
	gcc -o button help_button.c -lwiringPi
	gcc `pkg-config --cflags gtk+-3.0` -o window window.c `pkg-config --libs gtk+-3.0`
	gcc -o trigger_alarms trigger_alarms.c -lwiringPi -jsoncpp

clean:
	rm -rf button