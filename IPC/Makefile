CC := gcc
CFLAGS :=
LDFLAGS :=

SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)
SRCS := $(filter-out binary_sem.c, ${SRCS})
TGTS := $(SRCS:.c=)

all: ${TGTS}

sem_one sem_two: binary_sem.o

.depend: ${SRCS}
	${CC} -MM ${CFLAGS} $^ > $@

clean:
	${RM} ${TGTS} ${OBJS} .depend fifo

ifneq (${MAKECMDGOALS},clean)
-include .depend
endif
