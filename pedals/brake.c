/*
 * brake.c - Part of an open source gearbox controller
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

#include <pedals/brake.h>

/* This is just to avoid a warning, remove when it is ok to do so */
void brake (void)
{

}

#define PID_MAX_I -1
static int PID_Do_Once; /* Local to this routine */

void PID_Initiate(void)
{
  PID_Do_Once = 1;            /* Make it disable D-part on first regulation */
}

unsigned short PID_Regulate(short Current, short Target, unsigned short P_Factor,
  unsigned short I_Factor, unsigned short D_factor, unsigned short Factor_Divisor)
{
  static unsigned short Previous, I_Value; /* Holds previous "Current"-value, used by D-part  */
  long  Work_S, Work_P, Work_I, Work_D;
  short Diff; /* Could be positive or negative */
  Diff = Target - Current; /* Positive = the need for speed */
  /* Now calculate the P-part */
  Work_P  = Diff * P_Factor;
  Work_P /= Factor_Divisor;
  if (Work_P > (unsigned)PID_MAX_I) Work_P = (unsigned)PID_MAX_I; /* Top limit */
  if (Work_P < 0)                   Work_P = 0;                  /* Bottom limit */
  /* Now calculate the I-part */
  Work_I  = Diff * I_Factor;
  Work_I /= Factor_Divisor;
  if (Work_I > (unsigned)PID_MAX_I) Work_I = (unsigned)PID_MAX_I; /* Top limit */
  if (Work_I < 0)                   Work_I = 0;                  /* Bottom limit */
  I_Value += Work_I;
  /* And lastly the D-part */
  Diff    = Current - Previous; /* Negative = hold back a little */
  Work_D  = Diff * D_factor;
  Work_D /= Factor_Divisor;
  Previous = Current; /* remember to next time */
  /* Now put it all together */
  if (PID_Do_Once)
  {
    PID_Do_Once = 0; /* OK, register that it's done */
    Work_D      = 0; /* Set to known state as no Prevoius exists */
    I_Value     = 0; /* Set to known state as it's the starting point */
  }
  Work_S  = Work_P; /* Start putting it together */
  Work_S += I_Value;
  Work_S += Work_D;
  if (Work_S > (unsigned short)PID_MAX_I) Work_S = (unsigned short)PID_MAX_I;
  if (Work_S < 0)                         Work_S = 0;

  return((unsigned short)Work_S);
}
