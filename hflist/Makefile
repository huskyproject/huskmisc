include ../../huskymak.cfg

all: hflist$(EXE)

ifeq ($(DEBUG), 1)
  CFLAGS=-I$(INCDIR) $(DEBCFLAGS) $(WARNFLAGS)
  LFLAGS=$(DEBLFLAGS)
else
  CFLAGS=-I$(INCDIR) $(OPTCFLAGS) $(WARNFLAGS)
  LFLAGS=$(OPTLFLAGS)
endif

ifeq ($(SHORTNAME), 1)
  LIBS=-L$(LIBDIR) -lfidoconf -lsmapi
else
  LIBS=-L$(LIBDIR) -lfidoconfig -lsmapi
endif

CDEFS=-D$(OSTYPE) $(ADDCDEFS)

OBJS=hflist$(OBJ) snprintf$(OBJ) template$(OBJ)

.c$(OBJ):
	$(CC) $(CFLAGS) $(CDEFS) -c $<

hflist$(EXE): $(OBJS)
	$(CC) $(LFLAGS) -o hflist$(EXE) $(OBJS) $(LIBS)

clean:
	-$(RM) $(RMOPT) *$(OBJ)
	-$(RM) $(RMOPT) *~

distclean: clean
	-$(RM) $(RMOPT) hflist$(EXE)

install: hflist$(EXE)
	$(INSTALL) $(IBOPT) hflist$(EXE) $(BINDIR)

uninstall:
	-$(RM) $(RMOPT) $(BINDIR)$(DIRSEP)hflist$(EXE)

