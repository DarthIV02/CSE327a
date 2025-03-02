all:
	gcc -o button help_button.c -lwiringPi
	gcc -o trigger_alarms trigger_alarms.c window.c `pkg-config --libs gtk+-3.0` -lwiringPi -lcjson
clean:
	rm -rf button