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

#include "well_plot.h"

#include <vector>
#include <algorithm>
#include <iostream>

#include <QtGui>
#include <QtDebug>

WellPlot::WellPlot(int x, int y, std::vector<double>& line_vec)
{
    this->x = x;
    this->y = y;
    this->line_vec = line_vec;

    std::vector<double>::const_iterator  max_y = std::max_element(line_vec.begin(), line_vec.end());

    color = QColor::fromHsv( int(*max_y), 255, 255,  255 );

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
    int xmin = 15;
    int xmax = 75;
    int ymin = 65;
    int ymax = 15;

    qreal stepsize = 10.0;

    const int nlines = 3;

//    for (std::vector<double>::iterator it=line_vec.begin() ; it != line_vec.end() ; ++it) {
//          std::cout << "item:" << *it << std::endl;
//        }

   // qDebug() << "nlines" << nlines << ": " << this->line_vec[0] ;

    // Draw lines
    QVarLengthArray<QLineF, nlines+3> lines;

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
        lines.append(QLineF(14,65,15,65));
        // line_vec.size()

        std::vector<int>::size_type i = 0;
        std::vector<int>::size_type i_max = i + nlines;

        lines.append(QLineF( xmin+qreal(0) * stepsize ,ymin - this->line_vec[i], xmin + qreal(1) * stepsize ,ymin - this->line_vec[i+1]));

        for ( i+=2; i < i_max; ++i )
        {
            lines.append(QLineF( xmin+qreal(i-1) * stepsize ,ymin - this->line_vec[i-1], xmin + qreal(i) * stepsize ,ymin - this->line_vec[i]));
        }
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
