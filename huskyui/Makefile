include ../../huskymak.cfg

all: huskyui$(_EXE)

ifeq ($(DEBUG), 1)
  CFLAGS=-I$(INCDIR) $(DEBCFLAGS) $(WARNFLAGS)
  LFLAGS=$(DEBLFLAGS)
else
  CFLAGS=-I$(INCDIR) $(OPTCFLAGS) $(WARNFLAGS)
  LFLAGS=$(OPTLFLAGS)
endif

ifeq ($(SHORTNAME), 1)
  LIBS=-L$(LIBDIR) -lncurses -lfidoconf -lsmapi -lhusky
else
  LIBS=-L$(LIBDIR) -lncurses -lfidoconfig -lsmapi -lhusky
endif

CDEFS=-D$(OSTYPE) $(ADDCDEFS)

.c$(_OBJ):
	$(CC) $(CFLAGS) $(CDEFS) -c $<

huskyui$(_EXE): huskyui$(_OBJ) showlog$(_OBJ) showoutb$(_OBJ)\
  menu$(_OBJ) cfg$(_OBJ) screen$(_OBJ)
	$(CC) $(LFLAGS) -o huskyui$(_EXE) huskyui$(_OBJ) \
	  showlog$(_OBJ) showoutb$(_OBJ) menu$(_OBJ) cfg$(_OBJ) \
	  screen$(_OBJ) $(LIBS)

clean:
	-$(RM) $(RMOPT) *~
	-$(RM) $(RMOPT) *$(_OBJ)

distclean: clean
	-$(RM) $(RMOPT) huskyui$(_EXE)

install: huskyui$(_EXE)
	$(INSTALL) $(IBOPT) huskyui$(_EXE) $(BINDIR)

uninstall:
	$(RM) $(RMOPT) $(BINDIR)$(DIRSEP)huskyui$(_EXE)

