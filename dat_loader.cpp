/*
 * Dummy iontorrent dat file loader
 *
 *
 *
 */

#include <fstream>
#include <vector>
#include <iostream>

#include <random>

#include "dat_loader.h"

DatLoader::DatLoader(std::vector<double>& raw_data )
{
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

    const int n_samples=24;

    std::default_random_engine generator;
    std::normal_distribution<double> distribution(20.0,5.0);

    for (int i=0; i<n_samples; ++i) {
         raw_data.push_back( distribution(generator));
      }
}

int* DatLoader::gaussDistribution(void)
{
     const int nrolls=10000;  // number of experiments
     const int nstars=100;    // maximum number of stars to distribute

     std::default_random_engine generator;
     std::normal_distribution<double> distribution(5.0,2.0);

     int p[10]={};

     for (int i=0; i<nrolls; ++i) {
       double number = distribution(generator);
       if ((number>=0.0)&&(number<10.0)) ++p[int(number)];
     }

     std::cout << "normal_distribution (5.0,2.0):" << std::endl;

     for (int i=0; i<10; ++i) {
       std::cout << i << "-" << (i+1) << ": ";
       std::cout << std::string(p[i]*nstars/nrolls,'*') << std::endl;
     }

     return p;
}
