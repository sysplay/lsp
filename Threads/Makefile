SEM_DIR := Semaphores
CC := gcc
CFLAGS := -pthread
LDFLAGS := -pthread # implicitly does -lpthread

SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)
TGTS := $(SRCS:.c=)

default: ${TGTS}

all: default
	${MAKE} -C ${SEM_DIR}

.depend: ${SRCS}
	${CC} -MM ${CFLAGS} $^ > $@

clean:
	${RM} *.log ${TGTS} ${OBJS} .depend

allclean: clean
	${MAKE} -C ${SEM_DIR} clean

ifneq (${MAKECMDGOALS},clean)
-include .depend
endif
