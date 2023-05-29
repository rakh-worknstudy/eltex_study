all: sender_complete.exe receiver.exe
	
sender.exe:
	gcc -o build/sender.exe src/sender.c
receiver.exe:
	gcc -o build/receiver.exe src/receiver.c
sender_complete.exe:
	gcc -o build/sender_complete.exe src/sender_complete.c src/lib/frameopt.c
