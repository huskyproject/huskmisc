include ../../huskymak.cfg

all: huskyui$(EXE)

ifeq ($(DEBUG), 1)
  CFLAGS=-I$(INCDIR) $(DEBCFLAGS) $(WARNFLAGS)
  LFLAGS=$(DEBLFLAGS)
else
  CFLAGS=-I$(INCDIR) $(OPTCFLAGS) $(WARNFLAGS)
  LFLAGS=$(OPTLFLAGS)
endif

ifeq ($(SHORTNAME), 1)
  LIBS=-L$(LIBDIR) -lncurses -lfidoconf -lsmapi
else
  LIBS=-L$(LIBDIR) -lncurses -lfidoconfig -lsmapi
endif

CDEFS=-D$(OSTYPE) $(ADDCDEFS)

.c$(OBJ):
	$(CC) $(CFLAGS) $(CDEFS) -c $<

huskyui$(EXE): huskyui$(OBJ) showlog$(OBJ) showoutb$(OBJ)\
  menu$(OBJ) cfg$(OBJ) screen$(OBJ)
	$(CC) $(LFLAGS) -o huskyui$(EXE) huskyui$(OBJ) \
	  showlog$(OBJ) showoutb$(OBJ) menu$(OBJ) cfg$(OBJ) \
	  screen$(OBJ) $(LIBS)

clean:
	-$(RM) $(RMOPT) *~
	-$(RM) $(RMOPT) *$(OBJ)

distclean: clean
	-$(RM) $(RMOPT) huskyui$(EXE)

install: huskyui$(EXE)
	$(INSTALL) $(IBOPT) huskyui$(EXE) $(BINDIR)

uninstall:
	$(RM) $(RMOPT) $(BINDIR)$(DIRSEP)huskyui$(EXE)

