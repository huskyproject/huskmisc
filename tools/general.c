/* general.c - functions common to poll/send/request */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fidoconf/fidoconf.h>
#include <fidoconf/common.h>
#include "general.h"

/* FLOName - generate name of FLO for a given address
   params: outbound     path of outbound
           Addr         address to generate FLO name for
   result: filename of FLO (memory is allocated automatically) */
char *FLOName(s_fidoconfig *config, s_addr Addr)
{
  char *FName;
  char *FName2;
  char *OutDir;

  /* remove trailing directory seperator from outbound */
  OutDir = strdup(config->outbound);
  OutDir[strlen(OutDir)-1] = 0;

  FName = calloc(1, 256);
  if ((Addr.domain != NULL) && (config->addr[0].domain != NULL) &&
   (stricmp(Addr.domain, config->addr[0].domain) != 0))
  {
    char *Tmp;

    Tmp = calloc(1, strlen(OutDir)+strlen(Addr.domain)+1);
    strncpy(Tmp, OutDir, strrchr(OutDir, DirSepC)-OutDir+1);
    Tmp[strrchr(OutDir, DirSepC)-OutDir+1] = '\0';

    if (Addr.point != 0)
    {
      if (Addr.zone != config->addr[1].zone)
      {
	sprintf(FName, "%s%s.%03x" DirSepS "%04x%04x.pnt" DirSepS
		"0000%04x.flo", Tmp, Addr.domain, Addr.zone, Addr.net,
		Addr.node, Addr.point);
      }
      else
      {
	sprintf(FName, "%s%s" DirSepS "%04x%04x.pnt" DirSepS "0000%04x.flo",
		Tmp, Addr.domain, Addr.net, Addr.node, Addr.point);
      }
    }
    else
    {
      if (Addr.zone != config->addr[0].zone)
      {
        sprintf(FName, "%s%s.%03x" DirSepS "%04x%04x.flo", Tmp,
         Addr.domain, Addr.zone, Addr.net, Addr.node);
      }
      else
      {
        sprintf(FName, "%s%s" DirSepS "%04x%04x.flo", Tmp,
         Addr.domain, Addr.net, Addr.node);
      }
    }

    free(Tmp);
  }
  else
  {
    if (Addr.point != 0)
    {
      if (Addr.zone != config->addr[0].zone)
      {
	sprintf(FName, "%s.%03x" DirSepS "%04x%04x.pnt" DirSepS "0000%04x.flo",
		OutDir, Addr.zone, Addr.net, Addr.node, Addr.point);
      }
      else
      {
	sprintf(FName, "%s" DirSepS "%04x%04x.pnt" DirSepS "0000%04x.flo",
		OutDir, Addr.net, Addr.node, Addr.point);
      }
    }
    else
    {
      if (Addr.zone != config->addr[0].zone)
      {
	sprintf(FName, "%s.%03x" DirSepS "%04x%04x.flo", OutDir, Addr.zone,
		Addr.net, Addr.node);
      }
      else
      {
	sprintf(FName, "%s" DirSepS "%04x%04x.flo", OutDir, Addr.net,
		Addr.node);
      }
    }
  }

  free(OutDir);
  FName2 = strdup(FName);
  free(FName);
  return FName2;
}


/* Str2Addr - parse address-string into binary address
   params: s            string to parse
   result: binary address */
s_addr Str2Addr(char *s)
{
  s_addr a;
  string2addr(s, &a);
  return a;
}
