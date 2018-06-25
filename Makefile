
include ./Make.properties

all:
	(cd $(SRCDIR) && $(MAKE) -f $(MAKEFILE)) || exit 1;

clean:
	(cd $(SRCDIR) && $(MAKE) clean -f $(MAKEFILE)) || exit 1;
