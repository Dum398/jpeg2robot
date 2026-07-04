maingtkfrontend: src/main.c
		gcc `pkg-config --cflags gtk+-3.0` src/main.c -o app `pkg-config --libs gtk+-3.0`



