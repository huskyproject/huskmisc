# $Id$
#
# Generic makefile for build miscellaneous tools using GNU C
# Require huskymak.cfg

include ../../huskymak.cfg

all: poll$(_EXE) request$(_EXE) send$(_EXE) tolower$(_EXE)

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

poll$(_EXE): poll$(_OBJ) general$(_OBJ)
	$(CC) $(LFLAGS) -o poll$(_EXE) poll$(_OBJ) general$(_OBJ) $(LIBS)

request$(_EXE): request$(_OBJ) general$(_OBJ)
	$(CC) $(LFLAGS) -o request$(_EXE) request$(_OBJ) general$(_OBJ) $(LIBS)

send$(_EXE): send$(_OBJ) general$(_OBJ)
	$(CC) $(LFLAGS) -o send$(_EXE) send$(_OBJ) general$(_OBJ) $(LIBS)

tolower$(_EXE): tolower$(_OBJ)
	$(CC) $(LFLAGS) -o tolower$(_EXE) tolower$(_OBJ) $(LIBS)

clean:
	-$(RM) $(RMOPT) *.bak
	-$(RM) $(RMOPT) *.b
	-$(RM) $(RMOPT) *$(_OBJ)

distclean: clean
	-$(RM) $(RMOPT) poll$(_EXE)
	-$(RM) $(RMOPT) request$(_EXE)
	-$(RM) $(RMOPT) send$(_EXE)
	-$(RM) $(RMOPT) tolower$(_EXE)

ifdef INSTALL

install:
	$(INSTALL) $(IBOPT) poll$(_EXE) $(BINDIR)
	$(INSTALL) $(IBOPT) request$(_EXE) $(BINDIR)
	$(INSTALL) $(IBOPT) send$(_EXE) $(BINDIR)
	$(INSTALL) $(IBOPT) tolower$(_EXE) $(BINDIR)

uninstall:
	-$(RM) $(RMOPT) $(BINDIR)$(DIRSEP)poll$(_EXE) 
	-$(RM) $(RMOPT) $(BINDIR)$(DIRSEP)request$(_EXE)
	-$(RM) $(RMOPT) $(BINDIR)$(DIRSEP)send$(_EXE)
	-$(RM) $(RMOPT) $(BINDIR)$(DIRSEP)tolower$(_EXE)

endif
