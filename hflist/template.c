/* template.c - template processing */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "snprintf.h"
#include "template.h"

#define nfree(a) { if (a) { free(a); a = NULL; } }

char *tplVarNames[cNumVar] =
{
  "filename",
  "filesize",
  "fileyear",
  "filemonth",
  "fileday",
  "filedesc",
  "fileexist",
  "arename",
  "areadesc",
  "areapath",
  "areanumfiles",
  "areatotalsize",
  "globalnumfiles",
  "globaltotalsize"
};

char **tplVarValues = NULL;

void tplSetVar(unsigned int id, char *value)
{
  if (!tplVarValues)
  {
    tplVarValues = (char **) calloc(cNumVar, sizeof(char *));
  }

  tplVarValues[id] = value;
}

void tplUnsetVar(unsigned int id)
{
  if (!tplVarValues)
  {
    tplVarValues = (char **) calloc(cNumVar, sizeof(char *));
  }

  tplVarValues[id] = "";
}

void tplFreeVar(unsigned int id)
{
  if (!tplVarValues)
  {
    tplVarValues = (char **) calloc(cNumVar, sizeof(char *));
  }

  nfree(tplVarValues[id]);
}

char *tplLookupVar(char *varName)
{
  unsigned int i;

  for (i = 0; i < cNumVar; i++)
  {
    if (!strcmp(tplVarNames[i], varName))
    {
      if (tplVarValues[i]) return tplVarValues[i];
      return "";
    }
  }

  return "";
}

void tplAddChar(char **buf, unsigned int *bufLen, unsigned int *bufSize, char c)
{
  (*bufLen)++;
  if ((*bufLen) > (*bufSize))
  {
    *bufSize = (*bufLen) + 1024;
    *buf = realloc(*buf, (*bufSize) + 1);
  }
  (*buf)[(*bufLen) - 1] = c;
}

void tplAddString(char **buf, unsigned int *bufLen, unsigned int *bufSize, char *s)
{
  unsigned int oldBufLen;

  oldBufLen = (*bufLen);
  (*bufLen) += strlen(s);
  if ((*bufLen) > (*bufSize))
  {
    *bufSize = (*bufLen) + 1024;
    *buf = realloc(*buf, (*bufSize) + 1);
  }
  strcpy(&((*buf)[oldBufLen]), s);
}

int tplDoCheck(char *check)
{
  char *cmpPos;
  unsigned int cmpType;
  unsigned int cmpLen;
  char *varName;
  char *expVal;
  char *realVal;
  int res;

  // search for comparison operator
  cmpPos = strstr(check, "<=");
  if (!cmpPos)
  {
    cmpPos = strstr(check, "<=");
    cmpLen = 2;
    cmpType = 1;
  }
  if (!cmpPos)
  {
    cmpPos = strstr(check, ">=");
    cmpLen = 2;
    cmpType = 2;
  }
  if (!cmpPos)
  {
    cmpPos = strstr(check, "=");
    cmpLen = 1;
    cmpType = 3;
  }
  if (!cmpPos)
  {
    cmpPos = strstr(check, "<");
    cmpLen = 1;
    cmpType = 4;
  }
  if (!cmpPos)
  {
    cmpPos = strstr(check, ">");
    cmpLen = 1;
    cmpType = 5;
  }
  if (!cmpPos) return 0;

  // get variable name
  varName = malloc(cmpPos - check + 1);
  strncpy(varName, check, cmpPos - check);
  varName[cmpPos - check] = 0;

  // get expected value
  expVal = cmpPos + cmpLen;

  // get real value of variable
  realVal = tplLookupVar(varName);

  // compare
  switch (cmpType)
  {
  case 1:
    res = (strcmp(realVal, expVal) <= 0);
    break;

  case 2:
    res = (strcmp(realVal, expVal) >= 0);
    break;

  case 3:
    res = (strcmp(realVal, expVal) == 0);
    break;

  case 4:
    res = (strcmp(realVal, expVal) < 0);
    break;

  case 5:
    res = (strcmp(realVal, expVal) > 0);
    break;
  }

  free(varName);
  return res;
}

char *processTemplate(char *tpl)
{
  char *buf;
  char *tplTmp;
  char *res;
  unsigned int bufLen = 0;
  unsigned int bufSize = 1024;
  char *sepPos;
  char *varName;
  char *fmtPos;
  char *check;
  char *action;
  char *alt;
  char *value;

  buf = malloc(bufSize + 1);
  tplTmp = tpl;
  while (*tplTmp)
  {
    switch (*tplTmp)
    {
    case '\\':
      tplTmp++;
      if (*tplTmp) tplAddChar(&buf, &bufLen, &bufSize, *tplTmp);
      tplTmp++;
      break;

    case '$':
      // variable expansion
      tplTmp++;
      sepPos = strchr(tplTmp, '$');
      if (sepPos)
      {
        // $$ -> single $
	if (tplTmp == sepPos)
	{
	  tplAddChar(&buf, &bufLen, &bufSize, *tplTmp);
          tplTmp++;
	}
	else
	{
          // check for optional format parameter
	  fmtPos = strchr(tplTmp, ':');

	  if (fmtPos && (fmtPos < (sepPos - 1)))
	  {
	    char *fmtStr;
	    char *fmtTmp;
	    char *valTmp;

	    varName = malloc(fmtPos - tplTmp + 1);
	    strncpy(varName, tplTmp, fmtPos - tplTmp);
	    varName[fmtPos - tplTmp] = 0;

            fmtStr = malloc(sepPos - fmtPos);
	    strncpy(fmtStr, fmtPos + 1, sepPos - fmtPos - 1);
	    fmtStr[sepPos - fmtPos - 1] = 0;

	    // FIXME: possibly dangerous (unchecked custom format string)
            fmtTmp = malloc(sepPos - fmtPos + 2);
	    sprintf(fmtTmp, "%%%ss", fmtStr);
            free(fmtStr);
	    asprintf(&valTmp, fmtTmp, tplLookupVar(varName));
	    tplAddString(&buf, &bufLen, &bufSize, valTmp);
            free(valTmp);
	  }
	  else
	  {
	    varName = malloc(sepPos - tplTmp + 1);
	    strncpy(varName, tplTmp, sepPos - tplTmp);
	    varName[sepPos - tplTmp] = 0;
	    tplAddString(&buf, &bufLen, &bufSize, tplLookupVar(varName));
	  }

          nfree(varName);
	  tplTmp = sepPos + 1;
	}
      }
      break;

    case '?':
      // conditionals: ?<check>?<consequent>?<alternative>?
      tplTmp++;
      sepPos = strchr(tplTmp, '?');
      if (sepPos)
      {
        // ?? -> single ?
	if (tplTmp == sepPos)
	{
	  tplAddChar(&buf, &bufLen, &bufSize, *tplTmp);
          tplTmp++;
	}
	else
	{
	  // extract the check
	  check = malloc(sepPos - tplTmp + 1);
	  strncpy(check, tplTmp, sepPos - tplTmp);
	  check[sepPos - tplTmp] = 0;

          // advance, get consequent
          tplTmp = sepPos + 1;
	  sepPos = strchr(tplTmp, '?');
	  if (!sepPos) break;
	  action = malloc(sepPos - tplTmp + 1);
	  strncpy(action, tplTmp, sepPos - tplTmp);
	  action[sepPos - tplTmp] = 0;

          // advance, get alternative
          tplTmp = sepPos + 1;
	  sepPos = strchr(tplTmp, '?');
	  if (!sepPos) break;
	  alt = malloc(sepPos - tplTmp + 1);
	  strncpy(alt, tplTmp, sepPos - tplTmp);
	  alt[sepPos - tplTmp] = 0;

          // advance
	  tplTmp = sepPos + 1;

	  // execute the check, process sub-template
	  if (tplDoCheck(check)) value = processTemplate(action);
	  else value = processTemplate(alt);

          // add result of sub-template
	  tplAddString(&buf, &bufLen, &bufSize, value);
	  free(value);
	}
      }
      break;

    default:
      tplAddChar(&buf, &bufLen, &bufSize, *tplTmp);
      tplTmp++;
      break;
    }
  }

  buf[bufLen] = 0;
  res = malloc(bufLen + 1);
  strcpy(res, buf);
  free(buf);

  return res;
}

