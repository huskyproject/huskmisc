include ../../huskymak.cfg

all: nmcopy$(_EXE)

ifeq ($(DEBUG), 1)
  CFLAGS=-I$(INCDIR) $(DEBCFLAGS) $(WARNFLAGS)
  LFLAGS=$(DEBLFLAGS)
else
  CFLAGS=-I$(INCDIR) $(OPTCFLAGS) $(WARNFLAGS)
  LFLAGS=$(OPTLFLAGS)
endif

ifeq ($(SHORTNAME), 1)
  LIBS=-L$(LIBDIR) -lfidoconf -lsmapi -lhusky
else
  LIBS=-L$(LIBDIR) -lfidoconfig -lsmapi -lhusky
endif

CDEFS=-D$(OSTYPE) $(ADDCDEFS)


.c$(_OBJ):
	$(CC) $(CFLAGS) $(CDEFS) -c $<

nmcopy$(_EXE): nmcopy$(_OBJ)
	$(CC) $(LFLAGS) -o nmcopy$(_EXE) nmcopy$(_OBJ) $(LIBS)

clean:
	-$(RM) $(RMOPT) *~
	-$(RM) $(RMOPT) *$(_OBJ)

distclean: clean
	-$(RM) $(RMOPT) nmcopy$(_EXE)

install:
	$(INSTALL) $(IBOPT) nmcopy$(_EXE) $(BINDIR)

uninstall:
	-$(RM) $(RMOPT) $(BINDIR)$(DIRSEP)nmcopy$(_EXE)

