/*
 * This will be a dumb and simple solution for reading and saving
 * parameters.
 * One file for each parameter will be created.
 */
#include <system/system.h>
#include <stdio.h>
#include <string.h>
//#include <stdlib.h>

#include "parameters.h"


parm_result
get_integer (int name, int *value)
{
  IDENTIFIER_NOT_USED(name);
  IDENTIFIER_NOT_USED(value);

	return PARM_OK;
}

parm_result
put_integer (int name, int value)
{
  IDENTIFIER_NOT_USED(name);
  IDENTIFIER_NOT_USED(value);

  return PARM_OK;
}

parm_result
get_string (int name, char *string, int max_len)
{
  IDENTIFIER_NOT_USED(name);
  IDENTIFIER_NOT_USED(string);
  IDENTIFIER_NOT_USED(max_len);

  return PARM_OK;
}

parm_result
put_string (int name, char *string)
{
  IDENTIFIER_NOT_USED(name);
  IDENTIFIER_NOT_USED(string);

  return PARM_OK;
}

parm_result
init_parameters(void)
{
  return PARM_OK;
}
