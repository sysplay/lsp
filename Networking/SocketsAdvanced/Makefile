CC := gcc
CFLAGS := -I .
LDFLAGS := -pthread -L . -lmysock
LIB_LDFLAGS :=

LIB_SRCS := socket_layer.c
LIB_OBJS := $(LIB_SRCS:.c=.o)
LIB_TGT := libmysock.a
SRCS := $(filter-out ${LIB_SRCS}, $(wildcard *.c))
OBJS := $(SRCS:.c=.o)
TGTS := $(SRCS:.c=)

all: ${TGTS}

${TGTS}: ${LIB_TGT}

${LIB_TGT}: ${LIB_OBJS}
	${AR} ${ARFLAGS} $@ $^ 2> /dev/null

.depend: ${SRCS} ${LIB_SRCS}
	${CC} -MM ${CFLAGS} $^ > $@

clean:
	${RM} ${TGTS} ${OBJS} ${LIB_TGT} ${LIB_OBJS} .depend

ifneq (${MAKECMDGOALS},clean)
-include .depend
endif
