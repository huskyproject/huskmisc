/* poll.c - create a flowfile for a node/point to call him */

#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <smapi/progprot.h>
#include <smapi/compiler.h>
#include <fidoconf/fidoconf.h>

#include "general.h"

int main(int argc, char *argv[])
{
  s_fidoconfig *config;

  if (argc < 2)
  {
    printf("poll - poll (create a flowfile for) a node/point\n"
           "Syntax: poll <address> [deldlc] [<flavour>]\n"
	   "        where <flavour> is one of: immediate, crash, normal, direct, hold\n");

    return 1;
  }

  config = readConfig(NULL);

  if (config != NULL)
  {
    char *flo = FLOName(config, Str2Addr(argv[1]));
    unsigned int floLen = strlen(flo);
    FILE *f;
    int deldlc = 0;
    int i;

    // default flavour
    flo[floLen - 3] = 'c';

    if (argc > 2)
    {
      for (i = 2; i < argc; i++)
      {
	if (stricmp(argv[i], "deldlc") == 0) deldlc = 1;
	else if (stricmp(argv[i], "immediate") == 0) flo[strlen(flo)-3] = 'i';
	else if (stricmp(argv[i], "crash") == 0) flo[strlen(flo)-3] = 'c';
	else if (stricmp(argv[i], "normal") == 0) flo[strlen(flo)-3] = 'f';
	else if (stricmp(argv[i], "direct") == 0) flo[strlen(flo)-3] = 'd';
	else if (stricmp(argv[i], "hold") == 0) flo[strlen(flo)-3] = 'h';
	else
	{
	  printf("Invalid option '%s'!\n", argv[i]);
	}
      }
    }

    // create FlowFile
    f = fopen(flo, "a");
    if (f == NULL)
    {
      printf("Could not open '%s' (error #%d: %s)!\n", flo, errno,
	     strerror(errno));
    }
    else fclose(f);

    if (deldlc)
    {
      // delete dial-counters
      flo[floLen - 3] = '$'; // Binkley-style
      flo[floLen - 2] = '$';
      flo[floLen - 1] = '0';
      if (fexist(flo)) unlink(flo);

      flo[floLen - 3] = 's'; // ifcico-style
      flo[floLen - 2] = 't';
      flo[floLen - 1] = 's';
      if (fexist(flo)) unlink(flo);

      flo[floLen - 3] = 'h'; // BinkD-style
      flo[floLen - 2] = 'l';
      flo[floLen - 1] = 'd';
      if (fexist(flo)) unlink(flo);
    }

    free(flo);
  }
  else
  {
    printf("Cannot read fidoconfig!\n");

    return 5;
  }

  return 0;
}

