/*
 * parameters.h
 *
 *  Created on: Dec 17, 2009
 *      Author: pontuso
 */
#ifndef PARAMETERS_H_
#define PARAMETERS_H_

/**
 * \addtogroup parameters
 * @{
 */

/**
 * \file
 * Parameter Store Manager
 * \author
 * Pontus oldberg <pontus.oldberg@invector.se>
 *
 */

/**
 * A type for returning result codes from the parameter handler. On host systems
 * this is typically an int and on target systems this can range from a char
 * to an int depending on CPU architecture.
 */
#ifdef HOST
typedef int           parm_result;
#else
typedef unsigned char parm_result;
#endif

/**
 * \enum parameter_results
 * Parameter functions result codes
 */
enum parameter_results {
  PARM_OK = 0x00,       /**< Operation was OK */
  PARM_NULL_PARAMETER,  /**< User supplied a null parameter */
  PARM_PARAMETER_ERROR, /**< User supplied an invalid parameter */
  PARM_MEMORY,          /**< Not enough memory to perform operation */
  PARM_NOT_FOUND,       /**< Parameter not found in store */
  PARM_FAILED           /**< General failure to get parameter */
};

/**
 * Initialize the parameter handler
 *
 * The parameter handler takes care of reading and writing the parameter store
 * used in the system. The function init_parameters is called upon system start
 * and must initialize the parameter library.
 */
parm_result init_parameters(void);

/**
 * Get integer value from the parameter store
 *
 * This function returns an integer value from the parameter store. It uses a
 * return status code interface where the function returns a status code to
 * indicate the status of the operation. The return value is supplied in the
 * output parameter of the function.
 *
 * @param [in]  name The name of the parameter to return
 * @param [out] value A pointer to the integer to return. Must be a pointer to an int.
 *
 * @return The status of the operation
 */
parm_result get_integer (int name, int *value);

/**
 * Put integer value to the parameter store
 *
 * This function writes an integer value to the parameter store. It uses a
 * return status code interface where the function returns a status code to
 * indicate the status of the operation.
 *
 * @param [in]  name The name of the parameter to return
 * @param [in]  value The value to write to the parameter store.
 *
 * @return The status of the operation
 */
parm_result put_integer (int name, int value);

/**
 * Get string value from the parameter store
 *
 * This function returns a string value from the parameter store. Ths user must
 * supply a pointer to an array with sufficient room to store the string to
 * fetch from the parameter store. The max_len variable defines the maximum
 * length to get. It uses a return status code interface where the function
 * returns a status code to indicate the status of the operation.
 *
 * @param [in]  name The name of the parameter to return
 * @param [in]  string A pointer to the string where to copy string data.
 * @param [in]  max_len The maximum length of the string to retrieve.
 *
 * @return The status of the operation
 */
parm_result get_string (int name, char *string, int max_len);

/**
 * Put a string value to the parameter store
 *
 * This function writes a string value to the parameter store. It uses a
 * return status code interface where the function returns a status code to
 * indicate the status of the operation.
 *
 * @param [in]  name The name of the parameter to return
 * @param [in]  string A pointer to the string to write to the store. If the
 *              supplied string is to long for the parameter store it will be
 *              truncated.
 *
 * @return The status of the operation
 */
parm_result put_string (int name, char *string);

/** @} */

#endif /* PARAMETERS_H_ */
