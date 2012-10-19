/*
 * This will be a dumb and simple solution for reading and saving
 * parameters.
 * One file for each parameter will be created.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "parameters/parameters.h"
#include "parameters/param_db.h"

#define MAX_PARAM_FILE_NAME_LENGTH    1024

static char *parm_dir     = "parms/";
static char *extension    = ".parm";
static char *type_int     = "parms/Int_";
static char *type_string  = "parms/String_";

struct parameter_table parmtab[] = {
  { PAR_INT_CLUTCH_ENGAGED_POSITION,    PAR_TYP_INT, "500" },
  { PAR_INT_CLUTCH_DISENGAGED_POSITION, PAR_TYP_INT, "500" },
  { PAR_INT_ACC_SERVO_MAX,              PAR_TYP_INT, "500" },
  { PAR_INT_ACC_SERVO_MIN,              PAR_TYP_INT, "500" },
  { PAR_INT_ACC_PEDAL_MAX,              PAR_TYP_INT, "500" },
  { PAR_INT_ACC_PEDAL_MIN,              PAR_TYP_INT, "500" },
};

static void create_filename(char *type, int name, char *out)
{
  sprintf(out, "%s%04d%s\0", type, name, extension);
}

parm_result get_integer (int name, int *value)
{
	char *filename;
	FILE *fd;

	/* First check input parameters */
	if (name == 0 || name >= PAR_END_MARKER || value == NULL) {
		return -PARM_PARAMETER_ERROR;
	}

	/* Make room for our filename */
	filename = malloc(MAX_PARAM_FILE_NAME_LENGTH);
	if (!filename) {
		printf("Could not allocate memory for file name !\n");
		return -PARM_MEMORY;
	}

	/* Create filename */
	create_filename(type_int, name, filename);

	/* open parameter file */
	fd = fopen(filename, "r");
	if (fd == 0) {
		printf("Could not find parameter '%d' !\n", name);
		free(filename);
		return -PARM_NOT_FOUND;
	}

	if (fscanf(fd, "%d", value) < 0) {
    printf("Could not read parameter '%d' data !\n", name);
    fclose(fd);
		free(filename);
		return -PARM_FAILED;
	}

  fclose(fd);
  free(filename);
	return PARM_OK;
}

parm_result put_integer (int name, int value)
{
	char *filename;
	FILE *fd;

	/* First check input parameters */
	if (name == 0 || name >= PAR_END_MARKER) {
		return -PARM_PARAMETER_ERROR;
	}

	/* Make room for our filename */
	filename = malloc(MAX_PARAM_FILE_NAME_LENGTH);
	if (!filename) {
		printf("Could not allocate memory for file name !\n");
		return -PARM_MEMORY;
	}

	/* Create filename */
  create_filename(type_int, name, filename);

  /* open parameter file, for writing (overwriting previous value) */
  fd = fopen(filename, "w");
  if (fd == 0) {
    printf("Could not open parameter file !\n");
    free(filename);
    return -PARM_NOT_FOUND;
  }

  /* Write value to file */
  fprintf(fd, "%d", value);

  fclose(fd);
  free(filename);
  return PARM_OK;
}

parm_result get_string (int name, char *string, int max_len)
{
  char *filename;
  FILE *fd;

  /* First check input parameters */
  if (name == 0 || name >= PAR_END_MARKER || string == NULL) {
    return -PARM_PARAMETER_ERROR;
  }

  /* Make room for our filename */
  filename = malloc(MAX_PARAM_FILE_NAME_LENGTH);
  if (!filename) {
    printf("Could not allocate memory for file name !\n");
    return -PARM_MEMORY;
  }

  /* Create filename */
  create_filename(type_string, name, filename);

  /* open parameter file */
  fd = fopen(filename, "r");
  if (fd == 0) {
    printf("Could not find parameter '%d' !\n", name);
    /* Make sure the empty string is terminated */
    *string = 0x00;
  } else {
    if (fgets(string, max_len, fd) == NULL) {
      printf("Could not read parameter '%d' data !\n", name);
      /* Make sure the empty string is terminated */
      *string = 0x00;
    } else {
      fclose(fd);
    }
  }

  free(filename);
  return PARM_OK;
}

parm_result put_string (int name, char *string)
{
  char *filename;
  FILE *fd;
  int result = PARM_OK;

  /* First check input parameters */
  if (name == 0 || name >= PAR_END_MARKER || string == NULL) {
    return -PARM_PARAMETER_ERROR;
  }

  /* Make room for our filename */
  filename = malloc(MAX_PARAM_FILE_NAME_LENGTH);
  if (!filename) {
    printf("Could not allocate memory for file name !\n");
    return -PARM_MEMORY;
  }

  /* Create filename */
  create_filename(type_string, name, filename);

  /* open parameter file, for writing (overwriting previous value) */
  fd = fopen(filename, "w");
  if (fd == 0) {
    printf("Could not open parameter file !\n");
    result = -PARM_NOT_FOUND;
    goto error;
  }

  /* Write value to file */
  fputs(string, fd);

error:
  if (fd)
    fclose(fd);
  if (filename)
    free(filename);
  return result;
}

parm_result init_parameters(void)
{
  int i;
  struct parameter_table *pat = parmtab;

  if (mkdir(parm_dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0)
    printf("Directory %s already existed !\n", parm_dir);

  for (i=0;i<PAR_END_MARKER-1;i++) {
    /* First try to read the parameter */
    if (pat->type == PAR_TYP_INT) {
      int val;
      parm_result res;
      if ((res = get_integer (pat->entry, &val)) != PARM_OK ) {
        if (res == -PARM_NOT_FOUND) {
          val = atoi (pat->default_value);
          printf("Parameter not found, writing default value !\n");
          if (put_integer(pat->entry, val) != PARM_OK) {
            printf ("Failed to write default parameter %d !\n", pat->entry);
          }
        } else {
          printf("Fatal error while reading parameter %d !\n", pat->entry);
        }
      }
    } else if (pat->type == PAR_TYP_STRING) {
      int val;
      parm_result res;
      if ((res = get_integer (pat->entry, &val)) != PARM_OK ) {
        if (res == -PARM_NOT_FOUND) {
          val = atoi (pat->default_value);
          printf("Parameter not found, writing default value !\n");
          if (put_integer(pat->entry, val) != PARM_OK) {
            printf ("Failed to write default parameter %d !\n", pat->entry);
          }
        } else {
          printf("Fatal error while reading parameter %d !\n", pat->entry);
        }
      }
    }
    pat++;
  }
}

/* EOF */

