all:
	gcc -Wall -Wextra main.c -o main; ./main
clean:
	rm main