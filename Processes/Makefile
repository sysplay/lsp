CC := gcc
CFLAGS :=
LDFLAGS :=

SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)
TGTS := $(SRCS:.c=)

all: ${TGTS}

.depend: ${SRCS}
	${CC} -MM ${CFLAGS} $^ > $@

clean:
	${RM} ${TGTS} ${OBJS} .depend

ifneq (${MAKECMDGOALS},clean)
-include .depend
endif
