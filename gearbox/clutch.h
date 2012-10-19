/*
 * gearbox - An open source gearbox controller
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

#ifndef CLUTCH_H_INCLUDED
#define CLUTCH_H_INCLUDED

/**
 * \addtogroup clutch
 * @{
 */

/**
 * \file
 * Clutch controller process
 * \author
 * Pontus oldberg <pontus.oldberg@invector.se>
 *
 */

/**
 * Signal that engages the clutch. Set to 1 to engage. The signal will be reset
 * when the clutch has engaged fully
 */
extern signal_t CLUTCH_ENGAGE;
/**
 * Signal to disengage the clutch. Set to 1 to disengage. the signal will be
 * reset when the clutch has disengaged fully.
 */
extern signal_t CLUTCH_DISENGAGE;

/**
 * get_clutch_state can be used to examine the current state of the clutch.
 */
enum CLUTCH_STATES {
  CLUTCH_DISENGAGED   = 0x00, /**< The clutch is fully disengaged */
  CLUTCH_DISENGAGING  = 0x01, /**< The clutch is disengaging (do not shift) */
  CLUTCH_ENGAGING     = 0x02, /**< The clutch is engaging (do not shift) */
  CLUTCH_ENGAGED      = 0x03, /**< The clutch has been fully engaged */
};

/* Create clutch basic interface */
PT_INTERFACE_START(clutch);
  int i;
PT_INTERFACE_END(clutch);

/**
 * Returns wether the clutch has disengaged fully or not.
 *
 * \return True if clutch has disengaged, false if not.
 */
unsigned char clutch_is_disengaged(void);

/** @} */

#endif // CLUTCH_H_INCLUDED
