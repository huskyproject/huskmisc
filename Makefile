all: all-tools all-huskyui all-nmcopy all-hflist
clean: clean-tools clean-huskyui clean-nmcopy clean-hflist
distclean: distclean-tools distclean-huskyui distclean-nmcopy distclean-hflist
install: install-tools install-huskyui install-nmcopy install-hflist
uninstall: uninstall-tools uninstall-huskyui uninstall-nmcopy uninstall-hflist

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


all-huskyui:
	$(MAKE) -C huskyui all

clean-huskyui:
	$(MAKE) -C huskyui clean

distclean-huskyui:
	$(MAKE) -C huskyui distclean

install-huskyui:
	$(MAKE) -C huskyui install

uninstall-huskyui:
	$(MAKE) -C huskyui uninstall


all-nmcopy:
	$(MAKE) -C nmcopy all

clean-nmcopy:
	$(MAKE) -C nmcopy clean

distclean-nmcopy:
	$(MAKE) -C nmcopy distclean

install-nmcopy:
	$(MAKE) -C nmcopy install

uninstall-nmcopy:
	$(MAKE) -C nmcopy uninstall

all-hflist:
#	$(MAKE) -C hflist all

clean-hflist:
#	$(MAKE) -C hflist clean

distclean-hflist:
#	$(MAKE) -C hflist distclean

install-hflist:
#	$(MAKE) -C hflist install

uninstall-hflist:
#	$(MAKE) -C hflist uninstall

