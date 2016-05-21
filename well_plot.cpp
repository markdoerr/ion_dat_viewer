
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

#include "well_plot.h"

WellPlot::WellPlot(int x, int y, std::vector<double>& line_vector, int line_left, int line_offset) : line_vec(line_vector)
{
    this->x = x;
    this->y = y;
    this->line_left = line_left;
    this->line_offset = line_offset;

    std::vector<double>::const_iterator max_y = std::max_element(line_vec.begin() + line_left,
                                                                 line_vec.begin() + line_left + line_offset);

    color = QColor::fromHsv( int(60 - *max_y ), 255, 255,  255 ); // colour between yellow (min) and red (max)

    setZValue((x + y) % 2);

    //setFlags(ItemIsSelectable | ItemIsMovable);
    setFlags(ItemIsSelectable );
    setAcceptsHoverEvents(true);
}

QRectF WellPlot::boundingRect() const
{
    return QRectF(0, 0, 110, 70);
}

QPainterPath WellPlot::shape() const
{
    QPainterPath path;
    path.addRect(4, 4, 100, 65);
    return path;
}

void WellPlot::drawFunctionLines(QPainter *painter, qreal lod)
{
    qreal xmin = 15.0;
    qreal ymin = 65.0;

    // Drawing lines
    if (lod >= 0.4) {
        lines.append(QLineF(xmin, ymin, xmin, ymin));

        int i = 0;
        int i_left = i + line_left;

        std::vector<double>::size_type vi = 0;

        lines.append(QLineF( xmin, ymin, xmin + stepsize, ymin - this->line_vec[vi+i_left]));

        for ( i=1 ; i < line_offset-1 ; ++i )
        {
            lines.append(QLineF( xmin + qreal(i) * stepsize,
                                 ymin - this->line_vec[vi+line_left+i],
                                 xmin + qreal(i+1) * stepsize,
                                 ymin - this->line_vec[vi+line_left+i+1]));
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

    // graph
    painter->drawRect(QRect(14, 14, 79, 55));
    painter->setBrush(b);

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
