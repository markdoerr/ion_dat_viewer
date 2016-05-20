
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

#include "well_plot.h"

#include <vector>
#include <algorithm>
#include <iostream>

#include <QtGui>
#include <QtDebug>

WellPlot::WellPlot(int x, int y, std::vector<double>& line_vec, int line_left, int line_offset)
{
    this->x = x;
    this->y = y;
    this->line_left = line_left;
    this->line_offset = line_offset;

    this->line_vec = line_vec;

    //std::vector<double>::const_iterator  max_y = std::max_element(line_vec.begin(), line_vec.end());

    std::vector<double>::const_iterator   max_y = std::max_element(line_vec.begin() + line_left, line_vec.begin() + line_left + line_offset);

    //qDebug() << "ll:" << line_left << " offs:" << line_offset << " max:" << *max_y;

    color = QColor::fromHsv( int(60 - *max_y ), 255, 255,  255 );

    setZValue((x + y) % 2);

    //setFlags(ItemIsSelectable | ItemIsMovable);
    setFlags(ItemIsSelectable );
    setAcceptsHoverEvents(true);
}


//void WellPlot::setLineVector(std::vector<int> &line_vec)
//{
//    this->line_vec = line_vec;
//}

QRectF WellPlot::boundingRect() const
{
    return QRectF(0, 0, 110, 70);
}

QPainterPath WellPlot::shape() const
{
    QPainterPath path;
    //path.addRect(14, 14, 82, 42);
    path.addRect(4, 4, 100, 65);
    return path;
}

void WellPlot::drawFunctionLines(QPainter *painter, qreal lod)
{
    qreal xmin = 15.0;
    qreal ymin = 65.0;

    qreal stepsize = 6.0;

    const int nlines = 12;

    // Draw lines
    QVarLengthArray<QLineF, nlines> lines;

//    if (lod >= 0.5) {
//        for (int i = 0; i <= 10; i += (lod > 0.5 ? 1 : 2)) {
//            lines.append(QLineF(18 + 7 * i, 13, 18 + 7 * i, 5));
//            lines.append(QLineF(18 + 7 * i, 54, 18 + 7 * i, 62));
//        }
//        for (int i = 0; i <= 6; i += (lod > 0.5 ? 1 : 2)) {
//            lines.append(QLineF(5, 18 + i * 5, 13, 18 + i * 5));
//            lines.append(QLineF(94, 18 + i * 5, 102, 18 + i * 5));
//        }
//    }

    if (lod >= 0.4) {
        lines.append(QLineF(xmin-1.0, ymin, xmin, ymin));

        int i = 0;
        int i_left = i + line_left;

        std::vector<double>::size_type vi = 0;

       // lines.append(QLineF( xmin+qreal(0) * stepsize, ymin - this->line_vec[vi + i_left], xmin + qreal(1) * stepsize ,ymin - this->line_vec[i_curr+1]));

        lines.append(QLineF( xmin, ymin, xmin + stepsize, ymin - this->line_vec[vi+i_left]));

        for ( i=1 ; i < line_offset-1 ; ++i )
        {
            lines.append(QLineF( xmin + qreal(i) * stepsize, ymin - this->line_vec[vi+line_left+i], xmin + qreal(i+1) * stepsize, ymin - this->line_vec[vi+line_left+i+1]));
        }

/*
        std::vector<double>::size_type i = 0;
        std::vector<double>::size_type i_curr = i + line_left;

        lines.append(QLineF( xmin+qreal(0) * stepsize ,ymin - this->line_vec[i_curr], xmin + qreal(1) * stepsize ,ymin - this->line_vec[i_curr+1]));

        for ( i_curr+=2; i_curr < i + line_offset; ++i_curr )
        {
            lines.append(QLineF( xmin+qreal(i_curr-1) * stepsize ,ymin - this->line_vec[i_curr-1], xmin + qreal(i_curr) * stepsize ,ymin - this->line_vec[i_curr]));
        }
*/
    }
    painter->drawLines(lines.data(), lines.size());

}

void WellPlot::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QColor fillColor = (option->state & QStyle::State_Selected) ? color.dark(150) : color;
    if (option->state & QStyle::State_MouseOver)
        fillColor = fillColor.light(125);

    const qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());

    // low detail level plotting
    if (lod < 0.2) {
        if (lod < 0.125) {
            painter->fillRect(QRectF(0, 0, 110, 70), fillColor);
            return;
        }

        QBrush b = painter->brush();
        painter->setBrush(fillColor);
        painter->drawRect(13, 13, 97, 57);
        painter->setBrush(b);
        return;
    }

    QPen oldPen = painter->pen();
    QPen pen = oldPen;
    int width = 0;
    if (option->state & QStyle::State_Selected)
        width += 2;

    pen.setWidth(width);
    QBrush b = painter->brush();
    painter->setBrush(QBrush(fillColor.dark(option->state & QStyle::State_Sunken ? 120 : 100)));

    //painter->drawRect(QRect(14, 14, 79, 39));
    // graph
    painter->drawRect(QRect(14, 14, 79, 55));
    painter->setBrush(b);

//    if (lod >= 1) {
//        painter->setPen(QPen(Qt::gray, 1));
//        painter->drawLine(15, 54, 94, 54);
//        painter->drawLine(94, 53, 94, 15);
//        painter->setPen(QPen(Qt::black, 0));
//    }

    // high detail level plotting
    if (lod >= 2) {
        // drawing coord system text
        QFont font("Times", 10);
        font.setStyleStrategy(QFont::ForceOutline);
        painter->setFont(font);
        painter->save();
        painter->scale(0.1, 0.1);
        painter->drawText(170, 180, QString("Voltagram of well: %1x%2").arg(x).arg(y));
        painter->restore();
    }

//    qreal line_vec [] = { 15, 0, 25, 20,
//                          25, 20, 32, 45,
//                          32, 45, 45, 49,
//                          45, 49, 55, 20,
//                          55, 20, 65, 10,
//                          65, 10, 75, 0};
//    if (lod >= 0.4) {
//        qDebug() << "lines" << " x: " << this->line_vec[0] ;
//    }


  drawFunctionLines(painter, lod );

    // Draw red ink
    if (stuff.size() > 1) {
        QPen p = painter->pen();
        painter->setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        //painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundJoin));
        painter->setBrush(Qt::NoBrush);
        QPainterPath path;
        path.moveTo(stuff.first());
        for (int i = 1; i < stuff.size(); ++i)
            path.lineTo(stuff.at(i));
        painter->drawPath(path);
        painter->setPen(p);
    }
    //qDebug() << "lod : " << lod;
}

void WellPlot::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    update();
}

void WellPlot::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->modifiers() & Qt::ShiftModifier) {
        stuff << event->pos();
        update();
        return;
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void WellPlot::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}
