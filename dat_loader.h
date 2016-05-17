#ifndef DAT_LOADER_H
#define DAT_LOADER_H

#include <vector>

class DatLoader
{

public:
    DatLoader(std::vector<double>& raw_data );

    // std::vector<double>* data(void);

    int* gaussDistribution(void);
};

#endif // DAT_LOADER_H



