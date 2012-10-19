/*
 * rules.c - An open source gearbox controller
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
#include <system/system.h>
#include <rules/rules.h>
#include <gearbox/gear.h>

/**
 * The evaluator.
 *
 * This function takes a command and a new gear that the controller whishes
 * to employ and evaluates if it is a valid combination at the time.
 * Necessary parameters that need to be taken into consideration are:
 *
 *   - Current gear.
 *   - Current rpm
 *   - The state of the brake pedal
 *
 * Other parameters may be added.
 *
 * For instance, consider a driver that is going at high speed at a track and
 * starts to break before a turn. He will want to shift down among the gears
 * at the same time that he is breaking to reduce the speed. If the down
 * shifting occurs to fast the engine might over rev when hitting a gear which
 * is to low for the current speed.
 *
 * Another scenario is when the driver is trying to perform a gear shift
 * action, while the gear controller is still performing the last requested
 * action.
 *
 * There might be many more scenarios that I can't think of right now but they
 * should be pretty straight forward to implement.
 *
 */
char evalute_rules (pt_controller *controller)
{
  /* The first thing we check is whether the selected gear is allowed or not */
  if (controller->cmd.command == SHIFT_UP) {
    /* Check for erronous gear shift up changes */
    if (controller->cmd.gear == MAX_GEARS ||
        controller->gear <= controller->cmd.gear ||
        controller->gear > controller->cmd.gear + 1) {
          return RULE_INVALID_SHIFT;
    }
  }

  return 0;
}
