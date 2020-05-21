
include ./Make.properties

all:
	(cd $(SRCDIR) && $(MAKE) -f $(MAKEFILE)) || exit 1;

clean:
	rm -rf ComSpen
	rm -rf *.dot *.txt
	(cd $(SRCDIR) && $(MAKE) clean -f $(MAKEFILE)) || exit 1;
