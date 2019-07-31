BUILD_DIRS := $(filter-out Makefile README.md, $(wildcard [A-Z]*))

.PHONY: all clean

all:
	@$(foreach dir, ${BUILD_DIRS}, echo -n "Building ${dir} ... "; ${MAKE} -C ${dir} > /dev/null; echo "done";)

clean:
	@$(foreach dir, ${BUILD_DIRS}, echo -n "Cleaning ${dir} ... "; ${MAKE} -C ${dir} clean  > /dev/null; echo "done";)
