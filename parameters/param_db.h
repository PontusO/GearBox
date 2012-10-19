/*
 * param_db.h
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
#ifndef PARAM_DB_H_INCLUDED
#define PARAM_DB_H_INCLUDED

/* enum that holds all parameters */
enum parameter_types {
  PAR_TYP_RESERVED = 0x00,
  PAR_TYP_INT,
  PAR_TYP_STRING,
};

enum parameter_entries {
  PAR_RESERVED = 0x00,
  PAR_INT_CLUTCH_ENGAGED_POSITION,
  PAR_INT_CLUTCH_DISENGAGED_POSITION,
  PAR_INT_ACC_SERVO_MAX,
  PAR_INT_ACC_SERVO_MIN,
  PAR_INT_ACC_PEDAL_MAX,
  PAR_INT_ACC_PEDAL_MIN,
  PAR_END_MARKER,
};

struct parameter_table {
  enum parameter_entries entry;
  enum parameter_types   type;
  char *default_value;
};

#endif // PARAM_DB_H_INCLUDED
