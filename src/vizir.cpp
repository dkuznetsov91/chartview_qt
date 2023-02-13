#include "vizir.h"
#include <QPainter>
#include <QPen>

Vizir::Vizir(Qt::Orientations orientations, QObject *parent) :
    QObject(parent)
  , m_orientation(orientations)
{

}

QColor ColorXorColor(QColor Color1, QColor Color2) {
    return QColor( Color1.red() ^ Color2.red(), Color1.green() ^ Color2.green(), Color1.blue() ^ Color2.blue() );
}

void Vizir::draw(QPainter *painter) const
{
    QRect rect = painter->viewport().adjusted(1,1,-1,-1);

    QColor color = ColorXorColor(QColor(Qt::red), QColor(Qt::white));

    QPen pen(color);
    painter->setPen(pen);
    painter->setCompositionMode(QPainter::RasterOp_SourceXorDestination);

    if(m_orientation.testFlag(Qt::Vertical)){
        int x = static_cast<int>(m_pos.x());
        if(x < rect.left()) x = rect.left();
        if(x > rect.right()) x = rect.right();
        painter->drawLine( x, rect.top(), x, rect.bottom() );
    }
    if(m_orientation.testFlag(Qt::Horizontal)){
        int y = static_cast<int>(m_pos.y());
        if(y < rect.top()) y = rect.top();
        if(y > rect.bottom()) y = rect.bottom();
        painter->drawLine( rect.left(), y, rect.right(), y );
    }

    painter->setCompositionMode(QPainter::CompositionMode_Source);
}
