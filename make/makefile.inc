# common rules for all platforms

VER = 1.4
VERH = 1

commonprogs: poll$(EXE) request$(EXE) send$(EXE) tolower$(EXE) $(SYMBOLIC)
	$(NULL)

poll$(EXE): poll$(OBJ) general$(OBJ)
	$(CC) $(LFLAGS) $(EXENAMEFLAG)poll$(EXE) poll$(OBJ) general$(OBJ) $(LIBS)

request$(EXE): request$(OBJ) general$(OBJ)
	$(CC) $(LFLAGS) $(EXENAMEFLAG)request$(EXE) request$(OBJ) general$(OBJ) $(LIBS)

send$(EXE): send$(OBJ) general$(OBJ)
	$(CC) $(LFLAGS) $(EXENAMEFLAG)send$(EXE) send$(OBJ) general$(OBJ) $(LIBS)

tolower$(EXE): tolower$(OBJ)
	$(CC) $(LFLAGS) $(EXENAMEFLAG)tolower$(EXE) tolower$(OBJ) $(LIBS)


commonclean: $(SYMBOLIC)
	-$(RM) $(RMOPT) *$(OBJ)
	-$(RM) $(RMOPT) core

commondistclean: clean $(SYMBOLIC)
	-$(RM) $(RMOPT) poll$(EXE)
	-$(RM) $(RMOPT) request$(EXE)
	-$(RM) $(RMOPT) send$(EXE)
	-$(RM) $(RMOPT) tolower$(EXE)
