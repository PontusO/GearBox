/*
 * Copyright (C) 2010  Pontus Oldberg
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * This file is part of the protothreads library.
 *
 */

/**
 * \addtogroup pt
 * @{
 */

/**
 * \file
 * Factory macros for protothreads
 * \author
 * Pontus Oldberg <pontus.oldberg@invector.se>
 *
 */

#ifndef PT_FACTORY_H_INCLUDED
#define PT_FACTORY_H_INCLUDED
/**
 * \name Protothread factory macros
 * @{
 */

/**
 * Start an interface declaration.
 *
 * This macro begins the declaration of a protothread interface. Additional
 * member variables can be defined directly between the PT_INTERFACE_START and
 * PT_INTERFACE_END macros.
 * The macro sequence generates the necesseray protothread structure, with
 * user added instance member variables (in the above example "i") as well as
 * the prototype for the actual protothread and the init function. The generated
 * init function gets the name "init_" + the name that was given to the macro.
 * In this example you will get an init function called "init_clutch". The
 * function has the type void and takes no arguments. The generated protothread
 * declaration works basically the same, the name generated is "thread_" + the
 * name and follows the protothread naming of threads. In addition to this
 * the macro also creates a type declaration of the generated instance structure
 * which is in the form of "pt_" + the name.
 *
 * \param name A name for the protothread.
 *
 * Example:
  \code
PT_INTERFACE_START(clutch);
  int i;
PT_INTERFACE_END(clutch);
 \endcode
 *
 * \hideinitializer
 * \see PT_INTERFACE_END, PT_FACTORY_INIT_START, PT_FACTORY_INIT_END
 */
#define PT_INTERFACE_START(name) \
struct _pt_##name { \
  struct pt pt

/**
 * Ends an interface declaration.
 *
 * This macro ends the declaration of a protothread interface. The name given
 * here must be the same name as in the preceding PT_INTERFACE_START macro.
 *
 * \param name A name for the protothread.
 * \hideinitializer
 * \see PT_INTERFACE_START, PT_FACTORY_INIT_START, PT_FACTORY_INIT_END
 */
#define PT_INTERFACE_END(name) \
}; \
typedef struct _pt_ ## name pt_ ## name; \
PT_THREAD(thread_ ## name (pt_ ## name *ptr)); \
void init_ ## name (pt_ ## name *ptr)

/**
 * Starts the creation of a protothread init function.
 *
 * This macro will create the first and basic section of an init function for
 * a named protothread. It will clear the entire instance structure and
 * initialize the protothread variable.
 *
 * \param name A name for the protothread.
 *
 * Example:
 *
  \code
PT_FACTORY_INIT_START(clutch)
{
  CLUTCH_ENGAGE = 0;
  CLUTCH_DISENGAGE = 0;
  clutch_state = CLUTCH_DISENGAGED;
  set_clutch_servo (CLUTCH_DISENGAGED_POSITION);
}
PT_FACTORY_INIT_END(clutch)
 \endcode
 *
 * \hideinitializer
 * \see PT_FACTORY_INIT_END, PT_INTERFACE_START, PT_INTERFACE_END
 */
#define PT_FACTORY_INIT_START(name) \
void init_ ## name (pt_ ## name *ptr) { \
  memset (ptr, 0, sizeof (ptr)); \
  PT_INIT(&ptr->pt);

/**
 * Ends the init macro.
 *
 * This macro will end a factory_init section. The programmer can freely add
 * code between the PT_FACTORY_INIT_START and the PT_FACTORY_INIT_END macros
 * to initialize any other resources necessary.
 *
 * \param name A name for the protothread.
 * \hideinitializer
 * \see PT_FACTORY_INIT_START, PT_INTERFACE_START, PT_INTERFACE_END
 */
#define PT_FACTORY_INIT_END(name) }

/** @} */
#endif // PT_FACTORY_H_INCLUDED
/** @} */
