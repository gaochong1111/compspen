#
# Makefile in parser dir
#

include ../../Make.properties

OBJS=$(subst .cpp,.o,$(wildcard *.cpp))

all:$(OBJS)

$(DEPS):
	$(CC) -c $^ $(CPPFLAGS)

.PHONY:clean
clean:
	rm -f *.o
