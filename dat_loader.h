/****************************************************************************
 *
 * * ion_dat_viewer - an Ion Torrent Dat file viewer based on Qt
 *
 * * version: v0.1.1
 *
 * * author: mark doerr (mark.doerr@uni-greifswald.de)
 *
 * * date: 160521
 *
 * * Inspired by chip Qt4 example from Trolltech A/S.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 ***************************************************************************/

#ifndef DAT_LOADER_H
#define DAT_LOADER_H

#include <fstream>
#include <vector>
#include <iostream>
#include <random>

#define NUM_DAT_POINTS 12
#define NUM_X 1200
#define NUM_Y 833

class DatLoader
{
public:
    DatLoader(std::vector<double>& raw_data );

    int numX(void) const {return num_x; }
    int numY(void) const  {return num_y; }
    int numDataPoints(void) const {return num_data_points; }

private:
    const int num_x = NUM_X;
    const int num_y = NUM_Y;
    const int num_data_points  = NUM_DAT_POINTS; // remove hardcoded stuff
    int p[NUM_DAT_POINTS] = {};  // !!! remove hardcoded arrays
};

#endif // DAT_LOADER_H



