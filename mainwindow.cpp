/****************************************************************************
 *
 * * ion_dat_viewer - an Ion Torrent Dat file viewer based on Qt
 *
 * * version: v0.1.0
 *
 * * author: mark doerr (mark.doerr@uni-greifswald.de)
 *
 * * date: 160518
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

#include "mainwindow.h"
#include "well_view.h"
#include "well_plot.h"
#include "dat_loader.h"
#include <iostream>
#include <QtGui>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
    scene = new QGraphicsScene;

    dat_data = new DatLoader(raw_data);

    //std::vector<double>* line_vec = dat_data->data();

    std::cout << "raw data created"  << std::endl;

    std::cout << "main window normal_distribution (5.0,2.0) - size:" << raw_data.size() << std::endl;

//    for (std::vector<double>::iterator it=raw_data.begin() ; it != raw_data.end() ; ++it) {
//          std::cout << "ret. item:" << *it << std::endl;
//        }

    populateWellPlots();

    h1Splitter = new QSplitter(this);

    QSplitter *vSplitter = new QSplitter;
    vSplitter->setOrientation(Qt::Vertical);
    vSplitter->addWidget(h1Splitter);

    WellView *view = new WellView("Ion Torrent Chip");
    view->view()->setScene(scene);
    h1Splitter->addWidget(view);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(vSplitter);
    setLayout(layout);

    // final finish of the MainWindow
    setWindowTitle(tr("Ion Torrent Viewer v0.0.1"));
    setCentralWidget(h1Splitter);

}

void MainWindow::populateWellPlots()
{
    const int max_points = 12;
    const int min_well_coordinate_x = 2200; //= 11000;
    const int min_well_coordinate_y = 1400; //7000;
    const int well_plot_distance_x = 110;
    const int well_plot_distance_y = 70;

//    std::vector<double>::iterator it=raw_data.begin();
//    std::vector<double>::iterator it_begin = it;
//    std::vector<double>::iterator it_max = it + max_points;

    // || it != raw_data.end()
    //for ( it ; it < it_max  ; ++it) line_vec.push_back(*it);

    // Populating the scene
    int n_items = 0;
    int x_item_pos = 0;

    const int point_offset = 12;

    int line_left = 0;
    int max_rep = 7699;


    qDebug() << "pop";

    for (int i = -min_well_coordinate_x; i < min_well_coordinate_x; i +=  well_plot_distance_x) {
        ++x_item_pos;

        int y_item_pos = 0;

        if (line_left > max_rep) { line_left = 0;}

        for (int j = -min_well_coordinate_y; j < min_well_coordinate_y; j += well_plot_distance_y) {
            ++y_item_pos;

            if (line_left > max_rep) { line_left = 0;}

          // qDebug() << "line:" << *it << "-" << *(it+1) << "-" << *(it+2) << "-" << *(it+3) << "-" << *(it+4) << "-" << *(it+5)  << "y-item pos:" << y_item_pos ;

//           for(int lp=line_left; lp < line_left + point_offset; lp ++ ){
//               qDebug() << "line-[" << lp << "]" << raw_data[lp] ;
//           }
           WellPlot *item = new WellPlot(x_item_pos, y_item_pos, raw_data, line_left, point_offset);

           item->setPos(QPointF(i, j));
           scene->addItem(item);

           ++n_items;
           line_left = line_left + point_offset;
        }
        line_left = line_left + point_offset;
    }
}

/*

void MainWindow::populateWellPlots()
{
    int min = 0;
    int max = 254;
    int size = 50;

    const int min_well_coordinate_x = 11000;
    const int min_well_coordinate_y = 7000;
    const int well_plot_distance_x = 110;
    const int well_plot_distance_y = 70;

    int vi = 0;
    //static std::vector<int> line_vec(10*sizeof(int));
std::cout << "pop" << std::endl;
//    std::vector<int>::iterator it = v.begin();
    // Populate scene
    srand(time(NULL));
    int xx = 0;
    int nitems = 0;
    for (int i = -11000; i < 11000; i +=  well_plot_distance_x) {
        ++xx;
        int yy = 0;
        for (int j = -7000; j < 7000; j += well_plot_distance_y) {
            ++yy;
            qreal x = (i + 11000) / 22000.0;
            qreal y = (j + 7000) / 14000.0;

//            qreal x = (i + min_well_coordinate_x) / (2 * min_well_coordinate_x);
//            qreal y = (j + min_well_coordinate_y) / (2 * min_well_coordinate_y);

            qreal rand_num = rand()%(max-min + 1) + min;

            //qDebug() << "rand num : " << rand_num;

            //line_vec[4] = line_vec[4] * rand_num;
            // QColor color(image.pixel(int(image.width() * x), int(image.height() * y)));

//            for (int k=0; k<10; k++) {
//                //line_vec[k] = *it;
//                line_vec[k] = 1;
//               // it++;
//            }

            QColor color = QColor::fromHsv( rand_num, 255, 255,  255 );

       //     Chip *item = new Chip(color, xx, yy); //QGraphicsItem
           // WellPlot *item = new WellPlot(color, xx, yy);
            std::cout << "wells crea" << std::endl;
           // item->setLineVector(this->line_vec);
std::cout << "lines set" << std::endl;
           // item->setPos(QPointF(i, j));
            //scene->addItem(item);

            ++nitems;
        }
    }
}
*/
