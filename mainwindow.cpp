/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

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

    std::cout << "normal_distribution (5.0,2.0):" << std::endl;

    for (std::vector<double>::iterator it=raw_data.begin() ; it != raw_data.end() ; ++it) {
         // std::cout << i << "-" << (i+1) << ": ";
          std::cout << "item:" << *it << std::endl; //<< std::string(p[i]*nstars/nrolls,'*') << std::endl;
        }

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
    const int max_points = 10;
    const int min_well_coordinate_x = 11000;
    const int min_well_coordinate_y = 7000;
    const int well_plot_distance_x = 110;
    const int well_plot_distance_y = 70;

    std::vector<double> line_vec;

    std::vector<double>::iterator it=raw_data.begin();
    std::vector<double>::iterator it_max = it + max_points;

    for ( it ; it < it_max || it != raw_data.end() ; ++it) line_vec.push_back(*it);

    // Populating the scene
    int n_items = 0;
    int xx = 0;

    for (int i = -min_well_coordinate_x; i < min_well_coordinate_x; i +=  well_plot_distance_x) {
        ++xx;
        int yy = 0;
        for (int j = -min_well_coordinate_y; j < min_well_coordinate_y; j += well_plot_distance_y) {
            ++yy;

            WellPlot *item = new WellPlot(xx, yy, line_vec);

           item->setPos(QPointF(i, j));
           scene->addItem(item);

            ++n_items;
        }
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
