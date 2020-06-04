run: Homework_3_serial.out Homework_3_parallel.out

Homework_3_parallel.out: Homework_3_parallel.c timing.c GetNumbers.c
	gcc -g Homework_3_parallel.c timing.c GetNumbers.c -o Homework_3_parallel.out -lpthread

Homework_3_serial.out: Homework_3_serial.c timing.c GetNumbers.c
	gcc -g Homework_3_serial.c timing.c GetNumbers.c -o Homework_3_serial.out

clean:
	rm *.out