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

#ifndef CHIP_H
#define CHIP_H

#include <vector>

#include <QtGui/QColor>
#include <QtGui/QGraphicsItem>

class WellPlot : public QGraphicsItem
{
public:
    WellPlot(int x, int y, std::vector<double> &line_vec, int line_left, int line_offset);

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    int x, y;
    int line_left, line_offset;

    std::vector<double> line_vec ;

    QColor color;
    QList<QPointF> stuff;
    void drawFunctionLines(QPainter *painter, qreal lod);
};

#endif
