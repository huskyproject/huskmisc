/* general.h - functions common to poll/send/request */

#ifdef UNIX
#define DirSepC '/'
#define DirSepS "/"
#else
#define DirSepC '\\'
#define DirSepS "\\"
#endif


/* FLOName - generate name of FLO for a given address
   params: outbound     path of outbound
           Addr         address to generate FLO name for
   result: filename of FLO (memory is allocated automatically) */
char *FLOName(s_fidoconfig *config, hs_addr Addr);

/* Str2Addr - parse address-string into binary address
   params: s            string to parse
   result: binary address */
hs_addr Str2Addr(char *s);
