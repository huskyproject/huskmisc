include ../../huskymak.cfg

all: hflist$(_EXE)

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

OBJS=hflist$(_OBJ) snprintf$(_OBJ) template$(_OBJ)

.c$(_OBJ):
	$(CC) $(CFLAGS) $(CDEFS) -c $<

hflist$(_EXE): $(OBJS)
	$(CC) $(LFLAGS) -o hflist$(_EXE) $(OBJS) $(LIBS)

clean:
	-$(RM) $(RMOPT) *$(_OBJ)
	-$(RM) $(RMOPT) *~

distclean: clean
	-$(RM) $(RMOPT) hflist$(_EXE)

install: hflist$(_EXE)
	$(INSTALL) $(IBOPT) hflist$(_EXE) $(BINDIR)

uninstall:
	-$(RM) $(RMOPT) $(BINDIR)$(DIRSEP)hflist$(_EXE)

