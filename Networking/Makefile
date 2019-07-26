SOCK_ADV_DIR := SocketsAdvanced
CC := gcc
CFLAGS :=
LDFLAGS :=

SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)
TGTS := $(SRCS:.c=)

default: ${TGTS} links

all: default
	${MAKE} -C ${SOCK_ADV_DIR}

links: pipe_window0 pipe_window1

pipe_window0 pipe_window1:
	ln -s pipe_window $@

.depend: ${SRCS}
	${CC} -MM ${CFLAGS} $^ > $@

clean: clean_links
	${RM} ${TGTS} ${OBJS} .depend

allclean: clean
	${MAKE} -C ${SOCK_ADV_DIR} clean

clean_links:
	${RM} pipe_win? pipe_window?

ifneq (${MAKECMDGOALS},clean)
-include .depend
endif
