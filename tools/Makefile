include ../../huskymak.cfg

all: poll$(EXE) request$(EXE) send$(EXE) tolower$(EXE)

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


.c$(OBJ):
	$(CC) $(CFLAGS) $(CDEFS) -c $<

poll$(EXE): poll$(OBJ) general$(OBJ)
	$(CC) $(LFLAGS) -o poll$(EXE) poll$(OBJ) general$(OBJ) $(LIBS)

request$(EXE): request$(OBJ) general$(OBJ)
	$(CC) $(LFLAGS) -o request$(EXE) request$(OBJ) general$(OBJ) $(LIBS)

send$(EXE): send$(OBJ) general$(OBJ)
	$(CC) $(LFLAGS) -o send$(EXE) send$(OBJ) general$(OBJ) $(LIBS)

tolower$(EXE): tolower$(OBJ)
	$(CC) $(LFLAGS) -o tolower$(EXE) tolower$(OBJ) $(LIBS)

clean:
	-$(RM) $(RMOPT) *~
	-$(RM) $(RMOPT) *$(OBJ)

distclean: clean
	-$(RM) $(RMOPT) poll$(EXE)
	-$(RM) $(RMOPT) request$(EXE)
	-$(RM) $(RMOPT) send$(EXE)
	-$(RM) $(RMOPT) tolower$(EXE)

install:
	$(INSTALL) $(RMOPT) $(IBOPT) poll$(EXE) $(BINDIR)
	$(INSTALL) $(RMOPT) $(IBOPT) request$(EXE) $(BINDIR)
	$(INSTALL) $(RMOPT) $(IBOPT) send$(EXE) $(BINDIR)
	$(INSTALL) $(RMOPT) $(IBOPT) tolower$(EXE) $(BINDIR)

uninstall:
	-$(RM) $(RMOPT) $(BINDIR)$(DIRSEP)poll$(EXE) 
	-$(RM) $(RMOPT) $(BINDIR)$(DIRSEP)request$(EXE)
	-$(RM) $(RMOPT) $(BINDIR)$(DIRSEP)send$(EXE)
	-$(RM) $(RMOPT) $(BINDIR)$(DIRSEP)tolower$(EXE)

