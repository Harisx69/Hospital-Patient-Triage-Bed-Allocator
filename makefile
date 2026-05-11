CC = gcc

CFLAGS = -Wall -Wextra -pthread

SRC = src/admissions.c \
      src/threading.c \
      src/queue.c \
      src/memory_manager.c \
      src/ipc.c \
      src/logger.c \
      src/stats.c \
      src/semaphores.c \
      src/nurse.c \
      src/scheduler.c

PATIENT_SRC = src/patient_simulator.c

INCLUDE = -Iinclude

all:
	$(CC) $(CFLAGS) $(SRC) $(INCLUDE) -o admissions
	$(CC) $(CFLAGS) $(PATIENT_SRC) $(INCLUDE) -o patient_simulator

run: all
	./admissions

test: all
	./scripts/stress_test.sh

clean:
	rm -f admissions
	rm -f patient_simulator
	rm -f logs/*.log
	rm -f /tmp/discharge_fifo
