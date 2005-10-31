all: all-tools 
clean: clean-tools 
distclean: distclean-tools
install: install-tools
uninstall: uninstall-tools

all-tools:
	$(MAKE) -C tools all

clean-tools:
	$(MAKE) -C tools clean

distclean-tools:
	$(MAKE) -C tools distclean

install-tools:
	$(MAKE) -C tools install

uninstall-tools:
	$(MAKE) -C tools uninstall

