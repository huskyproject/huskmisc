/* hflist.c - create filelist */

#include <errno.h>
#include <fidoconf/fidoconf.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include "snprintf.h"
#include "template.h"

#define max(a,b) ((a > b) ? (a) : (b))
#define min(a,b) ((a < b) ? (a) : (b))
#define nfree(a) { if (a) { free(a); a = NULL; } }

typedef struct dirStats
{
  unsigned long int numFiles;
  unsigned long int numBytes;
} s_dirStats;

typedef struct fileInfo
{
  char *name;
  unsigned int size;
  time_t timeModified;
  char *desc;
} s_fileInfo;

typedef enum tplType { ttDir, ttGlobal, ttDirList } e_tplType;

typedef struct tplDef
{
  e_tplType type;
  char *dirHdrFile;       // filename of dir header template (ttDir and ttGlobal)
  char *dirEntryFile;     //             dir entry (ttDir and ttGlobal)
  char *dirFtrFile;       //             dir footer (ttDir and ttGlobal)
  char *globHdrFile;      //             global header (ttGlobal only)
  char *globFtrFile;      //             global footer (ttGlobal only)
  char *dirListHdrFile;   //             dirList header (ttDirList only)
  char *dirListEntryFile; //             dirList entry (ttDirList only)
  char *dirListFtrFile;   //             dirList footer (ttDirList only)
  char *dstFileName;      // name of file to be written

  FILE *dstFile;          // file handle (for internal use only)
  char *dirHdrCont;       // contents of template files (for internal use only)
  char *dirEntryCont;
  char *dirFtrCont;
  char *globHdrCont;
  char *globFtrCont;
  char *dirListHdrCont;
  char *dirListEntryCont;
  char *dirListFtrCont;
} s_tplDef;


s_tplDef *tpls;
unsigned int numTpls;


// read a line from file <f>, allocating memory as needed, stripping CR/LF
char *readLineFromFile(FILE *f)
{
  char *lineBuf;
  char *line;
  unsigned int lineLen;

  lineBuf = malloc(1024);

  // read first 1024 chars of line, if any
  line = NULL;
  *lineBuf = 0;
  fgets(lineBuf, 1024, f);

  // check for empty lines
  if (!(*lineBuf))
  {
    free(lineBuf);
    return strdup("");
  }

  line = strdup(lineBuf);
  lineLen = strlen(line);

  // read rest of line in 1024 char-blocks
  while ((feof(f) == 0) && (line[lineLen - 1] != 10) && (line[lineLen - 1] != 13))
  {
    *lineBuf = 0;
    fgets(lineBuf, 1024, f);
    lineLen += strlen(lineBuf);
    line = realloc(line, lineLen + 1);
    strcat(line, lineBuf);
  }

  // strip cr/lf
  while (((lineLen > 0) && ((line[lineLen - 1] == 10) || (line[lineLen - 1] == 13))))
  {
    lineLen--;
    line[lineLen] = 0;
  }

  free(lineBuf);
  return line;
}

// read files.bbs, put info into <files> and <stats>
void readFilesBBS(char *dir, s_fileInfo ***files, s_dirStats *stats)
{
  FILE *f;
  char *fname;
  char *line;
  char *lineTmp;
  s_fileInfo *curFile = NULL;
  unsigned int curDescLen = 0;
  char *spacePos;
  struct stat st;
  unsigned int dirLen;

  dirLen = strlen(dir);
  fname = malloc(dirLen+10);
  sprintf(fname, "%sfiles.bbs", dir);
  f = fopen(fname, "r");
  if (!f)
  {
    printf("Could not open '%s' for reading!\n", fname);
    free(fname);
    return;
  }
  nfree(fname);

  while (feof(f) == 0)
  {
    line = readLineFromFile(f);
    if (*line)
    {
      switch (*line)
      {
      case '+':
      case ' ':
	// long description
	if (curFile)
	{
	  lineTmp = line + 1;
	  while ((*lineTmp) == ' ') lineTmp++;

	  if (*lineTmp)
	  {
	    curDescLen += strlen(lineTmp) + 1;
	    if (curFile->desc)
	    {
	      curFile->desc = realloc(curFile->desc, curDescLen + 1);
	      strcat(curFile->desc, " ");
	      strcat(curFile->desc, lineTmp);
	    }
	    else
	    {
	      curFile->desc = malloc(curDescLen + 1);
	      strcpy(curFile->desc, " ");
	      strcat(curFile->desc, lineTmp);
	    }
	  }
	}
	break;

      default:
	// new file
	stats->numFiles++;
	*files = realloc(*files, stats->numFiles * sizeof(s_fileInfo *));
	curFile = calloc(sizeof(s_fileInfo), 1);
	(*files)[stats->numFiles - 1] = curFile;
	curDescLen = 0;

        // parse line
	spacePos = strchr(line, ' ');
	if (*spacePos)
	{
          // part before first space is the filename
	  curFile->name = malloc(spacePos - line + 1);
	  strncpy(curFile->name, line, spacePos - line);
	  curFile->name[spacePos - line] = 0;

          // skip spaces
          lineTmp = spacePos + 1;
	  while ((*lineTmp) == ' ') lineTmp++;

	  if (*lineTmp)
	  {
	    // everything following is the description
	    curDescLen = strlen(lineTmp);
	    curFile->desc = strdup(lineTmp);
	  }
	}
	else curFile->name = strdup(line);

	// get infos from filesystem
	fname = malloc(dirLen+strlen(curFile->name)+1);
	strcpy(fname, dir);
        strcat(fname, curFile->name);
	if (!stat(fname, &st))
	{
	  curFile->size = st.st_size;
          stats->numBytes += st.st_size;
	  curFile->timeModified = st.st_mtime;
	}
        nfree(fname);
        break;
      }
    }

    free(line);
  }

  fclose(f);
}

void processFile(s_fileInfo *file, s_dirStats *stats)
{
  char *line = NULL;
  char *tmp;
  struct tm *tm;
  unsigned int i;

  tplSetVar(cVarFileName, file->name);
  tplSetVar(cVarFileDesc, file->desc);
  asprintf(&tmp, "%u", file->size);
  tplSetVar(cVarFileSize, tmp);
  tm = localtime(&file->timeModified);
  asprintf(&tmp, "%04u", 1900 + tm->tm_year);
  tplSetVar(cVarFileYear, tmp);
  asprintf(&tmp, "%02u", tm->tm_mon);
  tplSetVar(cVarFileMonth, tmp);
  asprintf(&tmp, "%02u", tm->tm_mday);
  tplSetVar(cVarFileDay, tmp);
  tplSetVar(cVarFileExist, file->timeModified ? "yes" : "no");

  for (i = 0; i < numTpls; i++)
  {
    if ((tpls[i].type == ttGlobal) || (tpls[i].type == ttDir))
    {
      line = processTemplate(tpls[i].dirEntryCont);
      if (*line) fwrite(line, strlen(line), 1, tpls[i].dstFile);
      nfree(line);
    }
  }

  tplFreeVar(cVarFileDay);
  tplFreeVar(cVarFileMonth);
  tplFreeVar(cVarFileYear);
  tplFreeVar(cVarFileSize);
  tplUnsetVar(cVarFileDesc);
  tplUnsetVar(cVarFileName);
}

// helper function for sortFiles
int sortFilesCmp(const void *file1, const void *file2)
{
  return strcmp(((s_fileInfo **)file1)[0]->name, ((s_fileInfo **)file2)[0]->name);
}

// sort the array of fileinfos <files>
void sortFiles(s_fileInfo **files, unsigned int numFiles)
{
  qsort(files, numFiles, sizeof(s_fileInfo *), sortFilesCmp);
}

// get fileinfos, process templates
void processDir(char *dir, s_dirStats *globalStats)
{
  s_fileInfo **files = NULL;
  unsigned int i;
  char *line = NULL;
  s_dirStats dirStats;
  char *fname;

  memset(&dirStats, 0, sizeof(s_dirStats));

  // get fileinfos
  readFilesBBS(dir, &files, &dirStats);
  sortFiles(files, dirStats.numFiles);

  // open filelist destinations if necessary
  for (i = 0; i < numTpls; i++)
  {
    if (tpls[i].type == ttDir)
    {
      fname = malloc(strlen(dir)+strlen(tpls[i].dstFileName)+1);
      strcpy(fname, dir);
      strcat(fname, tpls[i].dstFileName);
      tpls[i].dstFile = fopen(fname, "w");
      if (!tpls[i].dstFile)
      {
	printf("Could not open file '%s' for writing!\n", tpls[i].dstFileName);
      }
    }
  }

  // process header templates
  for (i = 0; i < numTpls; i++)
  {
    if ((tpls[i].type == ttDir) || (tpls[i].type == ttGlobal))
    {
      if (tpls[i].dstFile)
      {
	line = processTemplate(tpls[i].dirHdrCont);
	fwrite(line, strlen(line), 1, tpls[i].dstFile);
	nfree(line);
      }
    }
  }

  // process files
  for (i = 0; i < dirStats.numFiles; i++)
  {
    processFile(files[i], &dirStats);
  }

  // free memory
  for (i = 0; i < dirStats.numFiles; i++)
  {
    nfree(files[i]->name);
    nfree(files[i]->desc);
    free(files[i]);
  }
  free(files);

  // set statistics variables
  globalStats->numFiles += dirStats.numFiles;
  globalStats->numBytes += dirStats.numBytes;
  asprintf(&line, "%lu", dirStats.numFiles);
  tplSetVar(cVarAreaNumFiles, line);
  asprintf(&line, "%lu", dirStats.numBytes);
  tplSetVar(cVarAreaTotalsize, line);
  line = NULL;

  // process global/dir dirFtr / dirList dirListEntry templates
  for (i = 0; i < numTpls; i++)
  {
    switch (tpls[i].type)
    {
    case ttGlobal:
    case ttDir:
      if (tpls[i].dstFile)
      {
	line = processTemplate(tpls[i].dirFtrCont);
	fwrite(line, strlen(line), 1, tpls[i].dstFile);
	nfree(line);
      }
      break;

    case ttDirList:
      if (tpls[i].dstFile)
      {
	line = processTemplate(tpls[i].dirListEntryCont);
	fwrite(line, strlen(line), 1, tpls[i].dstFile);
	nfree(line);
      }
      break;
    }
  }

  // free statistics variables
  tplFreeVar(cVarAreaTotalsize);
  tplFreeVar(cVarAreaNumFiles);

  // close filelist destinations if necessary
  for (i = 0; i < numTpls; i++)
  {
    if (tpls[i].type == ttDir)
    {
      if (tpls[i].dstFileName)
      {
	fclose(tpls[i].dstFile);
        tpls[i].dstFile = NULL;
      }
    }
  }
}

// set area variables, call processDir
void processArea(char *areaName, char *areaDesc, char *areaPath, s_dirStats *globalStats)
{
  tplSetVar(cVarAreaName, areaName);
  tplSetVar(cVarAreaDesc, areaDesc);

  processDir(areaPath, globalStats);

  tplUnsetVar(cVarAreaDesc);
  tplUnsetVar(cVarAreaName);
}

void processFileArea(s_filearea *area, s_dirStats *globalStats)
{
  processArea(area->areaName, area->description, area->pathName, globalStats);
}

void processBbsArea(s_bbsarea *area, s_dirStats *globalStats)
{
  processArea(area->areaName, area->description, area->pathName, globalStats);
}

void processPublicDir(char *dir, s_dirStats *globalStats)
{
  processArea(dir, NULL, dir, globalStats);
}


char *readFile(char *name)
{
  FILE *f;
  char *res;
  struct stat st;

  f = fopen(name, "r");
  if (!f)
  {
    printf("Could not open file '%s' for reading!\n", name);
    return NULL;
  }

  stat(name, &st);
  res = malloc(st.st_size + 1);
  fread(res, st.st_size, 1, f);
  fclose(f);
  res[st.st_size] = 0;

  return res;
}

void getTplDefs(s_fidoconfig *fc)
{
  unsigned int i;

  numTpls = fc->filelistCount;
  tpls = calloc(fc->filelistCount, sizeof(s_tplDef));
  for (i = 0; i < fc->filelistCount; i++)
  {
    switch (fc->filelists[i].flType)
    {
    case flDir:
      tpls[i].type = ttDir;
      break;

    case flGlobal:
      tpls[i].type = ttGlobal;
      break;

    case flDirList:
      tpls[i].type = ttDirList;
      break;
    }
    tpls[i].dstFileName = fc->filelists[i].destFile;
    tpls[i].dirHdrFile = fc->filelists[i].dirHdrTpl;
    tpls[i].dirEntryFile = fc->filelists[i].dirEntryTpl;
    tpls[i].dirFtrFile = fc->filelists[i].dirFtrTpl;
    tpls[i].globHdrFile = fc->filelists[i].globHdrTpl;
    tpls[i].globFtrFile = fc->filelists[i].globFtrTpl;
    tpls[i].dirListHdrFile = fc->filelists[i].dirListHdrTpl;
    tpls[i].dirListEntryFile = fc->filelists[i].dirListEntryTpl;
    tpls[i].dirListFtrFile = fc->filelists[i].dirListFtrTpl;
  }
}

void readTpls()
{
  unsigned int i;

  for (i = 0; i < numTpls; i++)
  {
    if (tpls[i].dirHdrFile) tpls[i].dirHdrCont = readFile(tpls[i].dirHdrFile);
    if (tpls[i].dirEntryFile) tpls[i].dirEntryCont = readFile(tpls[i].dirEntryFile);
    if (tpls[i].dirFtrFile) tpls[i].dirFtrCont = readFile(tpls[i].dirFtrFile);
    if (tpls[i].globHdrFile) tpls[i].globHdrCont = readFile(tpls[i].globHdrFile);
    if (tpls[i].globFtrFile) tpls[i].globFtrCont = readFile(tpls[i].globFtrFile);
    if (tpls[i].dirListHdrFile) tpls[i].dirListHdrCont = readFile(tpls[i].dirListHdrFile);
    if (tpls[i].dirListEntryFile) tpls[i].dirListEntryCont = readFile(tpls[i].dirListEntryFile);
    if (tpls[i].dirListFtrFile) tpls[i].dirListFtrCont = readFile(tpls[i].dirListFtrFile);
  }
}

void disposeTpls()
{
  unsigned int i;

  for (i = 0; i < numTpls; i++)
  {
    nfree(tpls[i].dirHdrCont);
    nfree(tpls[i].dirEntryCont);
    nfree(tpls[i].dirFtrCont);
    nfree(tpls[i].globHdrCont);
    nfree(tpls[i].globFtrCont);
    nfree(tpls[i].dirListHdrCont);
    nfree(tpls[i].dirListEntryCont);
    nfree(tpls[i].dirListFtrCont);
  }

  free(tpls);
}

int main(int argc, char *argv[])
{
  s_fidoconfig *config;
  unsigned int i;
  s_dirStats globalStats;
  char *line;

  if (argc > 1)
  {
    printf("hflist - create filelists\n");
    printf("Syntax: hflist\n");

    return 1;
  }

  config = readConfig(NULL);

  if (config)
  {
    memset(&globalStats, 0, sizeof(s_dirStats));

    // get filelist definitions from fidoconfig
    getTplDefs(config);

    // read templates
    readTpls();

    // open filelist destinations if necessary
    for (i = 0; i < numTpls; i++)
    {
      if ((tpls[i].type == ttGlobal) || (tpls[i].type == ttDirList))
      {
	tpls[i].dstFile = fopen(tpls[i].dstFileName, "w");
	if (!tpls[i].dstFile)
	{
	  printf("Could not open file '%s' for writing!\n", tpls[i].dstFileName);
	}
      }
    }

    // process header templates
    for (i = 0; i < numTpls; i++)
    {
      // only global and dirlist filelists are already open
      if (tpls[i].dstFile)
      {
        if (tpls[i].type == ttGlobal)
	  line = processTemplate(tpls[i].globHdrCont);
        else
	  line = processTemplate(tpls[i].dirListHdrCont);
	fwrite(line, strlen(line), 1, tpls[i].dstFile);
	nfree(line);
      }
    }

    // process areas
    for (i = 0; i < config->fileAreaCount; i++)
    {
      processFileArea(&config->fileAreas[i], &globalStats);
    }

    for (i = 0; i < config->bbsAreaCount; i++)
    {
      processBbsArea(&config->bbsAreas[i], &globalStats);
    }

    for (i = 0; i < config->publicCount; i++)
    {
      processPublicDir(config->publicDir[i], &globalStats);
    }

    // set statistics variables
    asprintf(&line, "%lu", globalStats.numFiles);
    tplSetVar(cVarGlobalNumFiles, line);
    asprintf(&line, "%lu", globalStats.numBytes);
    tplSetVar(cVarGlobalTotalsize, line);

    // process footer templates
    for (i = 0; i < numTpls; i++)
    {
      // only global and dirList filelists are still open
      if (tpls[i].dstFile)
      {
        if (tpls[i].type == ttGlobal)
	  line = processTemplate(tpls[i].globFtrCont);
        else
	  line = processTemplate(tpls[i].dirListFtrCont);
	fwrite(line, strlen(line), 1, tpls[i].dstFile);
	nfree(line);
      }
    }

    // free statistics variables
    tplFreeVar(cVarGlobalTotalsize);
    tplFreeVar(cVarGlobalNumFiles);

    // close filelist destinations if necessary
    for (i = 0; i < numTpls; i++)
    {
      if (tpls[i].dstFile)
      {
	fclose(tpls[i].dstFile);
	tpls[i].dstFile = NULL;
      }
    }

    // free memory
    disposeTpls();
  }
  else
  {
    printf("Cannot read fidoconfig!\n");

    return 5;
  }

  return 0;
}

