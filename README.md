# ion_dat_viewer - A fast and simple Ion Torrent raw DAT file viewer

written in C++ and Qt > 4.7

## Requirements
 - gcc
 - Qt > 4.7

## Compilation

### compilation with qmake

  1. cd [ion_dat_viewer_folder]/
  1. qmake ion_dat_viewer.pro
  1. make

On some systems you might need to install qmake, qt-devel and qt-create:

    sudo apt-get install qmake
    sudo apt-get install qt-devel qt-create

### compilation from qtcreator 

For easy, QTCreator IDE based compilation, please install qtcreator:
 
    sudo apt-get install qtcreator

load the project into Qt creator, compile the project with the build button.

## Current features
Qt GUI protototype, which displays currently __1 000 000 wells__ (!) smoothly (zooming/navigating), 
displaying 12 000 000 data points (12 data points per well).


## ToDos:

- remove hard coded variables
- optimize memory usage further
- DAT file loader (**you must change the file path manually** in [dat_loader.cpp](https://github.com/bcaller/ion_dat_viewer/blob/master/dat_loader.cpp#L51))
- nicer colour scheme (blue -> red )
- real QtWindow with file menus ...
- Max-Min-Value display
- browsing through several Ion Torrent flows


mark doerr (mark.doerr@uni-greifswald.de)
