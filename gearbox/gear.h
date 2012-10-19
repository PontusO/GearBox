/*
 * gear.h - Part of an open source gearbox controller
 *
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
 */
#ifndef GEAR_H_INCLUDED
#define GEAR_H_INCLUDED

/**
 * \addtogroup gear
 * @{
 */

/**
 * \file
 * Gear controller process
 * \author
 * Pontus oldberg <pontus.oldberg@invector.se>
 *
 */

/**
 * Defines the maximum gear positions, this must include neutral
 * and reverse, meaning a 6 gear car should have the value 8 here
 */
#define MAX_GEARS     8

/**
 * Defines the highest forward going gear.
 */
#define MAX_GEAR      6

/**
 * Signal to start engaging a new gear. Before engaging a new gear the
 * gear_ctrl structure inside the pt_gear instance must be filled in with
 * proper data.
 */
extern signal_t GEAR_SHIFT;

/**
 * List of different states the gear controller can be in.
 */
enum gear_state {
  READY = 0x00, /**< Ready with previous command and to accept a new command */
  SHIFTING,     /**< In the process of shifting between two gears */
  ERROR,        /**< There was an error in the previous shift request */
};

/**
 * Data structure that describes the current state of the gear box
 */
struct _gear_ctrl {
  enum gear_state state;  /**< State of the gear box */
  unsigned char gear;     /**< Current gear */
  unsigned char new_gear; /**< Requested new gear, used when sending the
                               GEAR_SHIFT signal */
};

/* Create gear basic interface */
PT_INTERFACE_START(gear);
  unsigned char mask[2];        /**< Mask values used, internal data only */
  struct _gear_ctrl gear_ctrl;  /**< Gear box control data */
PT_INTERFACE_END(gear);

/**
 * \hideinitializer
 */
typedef struct _gear_ctrl gear_ctrl;

/* Export the external API */
/**
 * Return the maximum number of gears this gearbox supports.
 *
 * \return The maximum number od supported gears.
 */
unsigned char get_max_gear(void);

#endif // GEAR_H_INCLUDED
