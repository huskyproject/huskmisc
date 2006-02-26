# $Id$
#
# include Husky-Makefile-Config
ifeq ($(DEBIAN), 1)
# Every Debian-Source-Paket has one included.
include /usr/share/husky/huskymak.cfg
else
include ../huskymak.cfg
endif

ifeq ($(DEBUG), 1)
  CFLAGS = -I$(INCDIR) $(DEBCFLAGS) $(WARNFLAGS)
  LFLAGS = $(DEBLFLAGS)
else
  CFLAGS = -I$(INCDIR) $(OPTCFLAGS) $(WARNFLAGS)
  LFLAGS = $(OPTLFLAGS)
endif

ifeq ($(SHORTNAME), 1)
  LIBS = -L$(LIBDIR) -lfidoconf -lsmapi
else
  LIBS = -L$(LIBDIR) -lfidoconfig -lsmapi
endif

CDEFS = -D$(OSTYPE) $(ADDCDEFS)

SRC_DIR = tools$(DIRSEP)


all: poll$(EXE) request$(EXE) send$(EXE) tolower$(EXE)

%$(OBJ): $(SRC_DIR)%.c
	$(CC) $(CFLAGS) $(CDEFS) $(SRC_DIR)$*.c

poll$(EXE): poll$(OBJ) general$(OBJ)
	$(CC) $(LFLAGS) -o poll$(EXE) poll$(OBJ) general$(OBJ) $(LIBS)

request$(EXE): request$(OBJ) general$(OBJ)
	$(CC) $(LFLAGS) -o request$(EXE) request$(OBJ) general$(OBJ) $(LIBS)

send$(EXE): send$(OBJ) general$(OBJ)
	$(CC) $(LFLAGS) -o send$(EXE) send$(OBJ) general$(OBJ) $(LIBS)

tolower$(EXE): tolower$(OBJ)
	$(CC) $(LFLAGS) -o tolower$(EXE) tolower$(OBJ) $(LIBS)


man: poll.1.gz request.1.gz send.1.gz tolower.1.gz

poll.1.gz: man/poll.1
	gzip -9c man/poll.1 > poll.1.gz

request.1.gz: man/request.1
	gzip -9c man/request.1 > request.1.gz

send.1.gz: man/send.1
	gzip -9c man/send.1 > send.1.gz

tolower.1.gz: man/tolower.1
	gzip -9c man/tolower.1 > tolower.1.gz


clean:
	-$(RM) $(RMOPT) *.bak
	-$(RM) $(RMOPT) *.b
	-$(RM) $(RMOPT) *$(OBJ)

distclean: clean
	-$(RM) $(RMOPT) poll$(EXE)
	-$(RM) $(RMOPT) request$(EXE)
	-$(RM) $(RMOPT) send$(EXE)
	-$(RM) $(RMOPT) tolower$(EXE)
	-$(RM) $(RMOPT) *.1.gz

install: all man
	$(INSTALL) $(IBOPT) poll$(EXE) $(BINDIR)
	$(INSTALL) $(IBOPT) request$(EXE) $(BINDIR)
	$(INSTALL) $(IBOPT) send$(EXE) $(BINDIR)
	$(INSTALL) $(IBOPT) tolower$(EXE) $(BINDIR)
ifdef MANDIR
	-$(MKDIR) $(MKDIROPT) $(MANDIR)$(DIRSEP)man1
	$(INSTALL) $(IMOPT) poll.1.gz $(MANDIR)$(DIRSEP)man1
	$(INSTALL) $(IMOPT) request.1.gz $(MANDIR)$(DIRSEP)man1
	$(INSTALL) $(IMOPT) send.1.gz $(MANDIR)$(DIRSEP)man1
	$(INSTALL) $(IMOPT) tolower.1.gz $(MANDIR)$(DIRSEP)man1
endif

uninstall:
	-$(RM) $(RMOPT) $(BINDIR)$(DIRSEP)poll$(_EXE) 
	-$(RM) $(RMOPT) $(BINDIR)$(DIRSEP)request$(_EXE)
	-$(RM) $(RMOPT) $(BINDIR)$(DIRSEP)send$(_EXE)
	-$(RM) $(RMOPT) $(BINDIR)$(DIRSEP)tolower$(_EXE)
ifdef MANDIR
	$(RM) $(RMOPT) $(MANDIR)$(DIRSEP)man1$(DIRSEP)poll.1.gz
	$(RM) $(RMOPT) $(MANDIR)$(DIRSEP)man1$(DIRSEP)request.1.gz
	$(RM) $(RMOPT) $(MANDIR)$(DIRSEP)man1$(DIRSEP)send.1.gz
	$(RM) $(RMOPT) $(MANDIR)$(DIRSEP)man1$(DIRSEP)tolower.1.gz
endif
