#
# Makefile in src
#
include ../Make.properties
TARGET=sepsolver
# subdirs
SUBDIRS=$(foreach dir,$(shell find . -mindepth 1 -type d),$(shell echo $(dir) ' '))

# objs

all: subdirs $(TARGET)
	mv $(TARGET) $(DISTDIR)

subdirs:
	@for i in $(SUBDIRS); do \
		echo "===> $$i"; \
		(cd $$i && $(MAKE) -f $(MAKEFILE)) || exit 1; \
		echo "<=== $$i"; \
	done

ALL_OBJS = $(subst .cpp,.o,$(foreach dir,$(SUBDIRS),$(wildcard $(dir)/*.cpp))) main.o

$(DEPS):
	$(CC) -c $^ $(CPPFLAGS)

$(TARGET):$(ALL_OBJS)
	$(CC) $(CPP_FLAGS) -o $@ $^ \
	$(F_LIB)




.PHONY:clean
clean:
	@for i in $(SUBDIRS); do \
		echo "===> $$i"; \
		(cd $$i && $(MAKE) clean -f $(MAKEFILE)) || exit 1; \
		echo "<=== $$i"; \
	done
	rm *.o
	rm -f $(DISTDIR)/$(TARGET)

