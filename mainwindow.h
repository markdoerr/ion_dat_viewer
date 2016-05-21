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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/qwidget.h>
#include <QMainWindow>
#include "dat_loader.h"

QT_FORWARD_DECLARE_CLASS(QGraphicsScene)
QT_FORWARD_DECLARE_CLASS(QGraphicsView)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QSlider)
QT_FORWARD_DECLARE_CLASS(QSplitter)

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    
private:
    std::vector<double> raw_data;

    void setupMatrix();
    void populateWellPlots();

    const DatLoader* dat_data;

    QGraphicsScene *scene;
    QSplitter *h1Splitter;
    QSplitter *h2Splitter;
};

#endif
