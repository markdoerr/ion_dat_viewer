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

#include "dat_loader.h"

DatLoader::DatLoader(std::vector<double>& raw_data )
{    
    const double max_y = 200.0;  // number of experiments = max hight of distribution
    const double min_y = 0.0;

    //srand(time(NULL));

    // Open the data stream for reading
    //    const char *filename_in = "int_file.dat" ;
    //    std::ifstream in_stream; //

    //    in_stream.open(filename_in, ios::binary );

    // Determine the file length
    //    in_stream.seekg(0, std::ios_base::end);
    // std::size_t size = 100 ; // in_stream.tellg();
    //    in_stream.seekg(0, std::ios_base::beg);

    // Create a vector to store the data
   // std::vector<int> v(size/sizeof(int));
   // std::vector<int>::iterator result;

    // Load the data
    //    in_stream.read((char*) &v[0], size);

    // Close the file
    //    in_stream.close();

    // dummy data generation process - should be replaced by the real dat file reading

    std::random_device rd;
    std::default_random_engine generator(rd());

    std::normal_distribution<double> distribution(double(num_data_points)/2, 2.0);

    for (int i=0; i <  num_x * num_y; ++i) {

        std::fill(p, p+num_data_points, 0);

        // generating max_hight of function
        std::uniform_int_distribution<int> uniform_dist(min_y, max_y);
        int rand_num = uniform_dist(generator);

        // generating distribution
        for (int i=0; i<rand_num; ++i) {
          double number = distribution(generator);
          if ((number>=min_y)&&(number<12)) ++p[int(number)];
        }

        for (int i=0; i < num_data_points; ++i) {
             raw_data.push_back(p[i] ); // distribution(generator)
        }
    }
}
