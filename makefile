CC = gcc
CFLAGS = -Wall -Wextra -pthread -Iinclude

all:
	$(CC) src/patient_simulator.c src/ipc.c src/memory_manager.c $(CFLAGS) -o patient_simulator
	$(CC) src/admissions.c src/threading.c src/ipc.c src/scheduler.c src/memory_manager.c src/queue.c $(CFLAGS) -o admissions

run:
	./admissions

clean:
	rm -f admissions patient_simulator
