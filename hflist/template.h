/* template.h - template processing */

void tplSetVar(unsigned int id, char *value);
void tplUnsetVar(unsigned int id);
void tplFreeVar(unsigned int id);
char *tplLookupVar(char *varName);
char *processTemplate(char *tpl);


#define cVarFileName                    0
#define cVarFileSize                    1
#define cVarFileYear                    2
#define cVarFileMonth                   3
#define cVarFileDay                     4
#define cVarFileDesc                    5
#define cVarFileExist                   6
#define cVarAreaName                    7
#define cVarAreaDesc                    8
#define cVarAreaPath                    9
#define cVarAreaNumFiles               10
#define cVarAreaTotalsize              11
#define cVarGlobalNumFiles             12
#define cVarGlobalTotalsize            13
#define cNumVar                        14

