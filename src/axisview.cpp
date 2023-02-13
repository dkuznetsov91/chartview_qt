#include "axisview.h"
#include "axis.h"


#include <QStylePainter>
#include <QFontMetrics>
#include <QDebug>
#include <QMouseEvent>
#include <QRect>

AxisView::AxisView(QWidget *parent) :
    QWidget(parent)
  , m_axis(nullptr)
{
    isMovable = false;

    setCursor(Qt::OpenHandCursor);

    setOrientation(Qt::Vertical);

}

AxisView::~AxisView()
{
}

void AxisView::setAxis(Axis *axis)
{
    m_axis = axis;

    calcAsisTics();

    update();
}

QSize AxisView::minimumSizeHint() const
{
    QFontMetrics fm(font());

    if(m_orientation == Qt::Vertical){
        if(m_axis == nullptr) return QSize(30, 200);
        return QSize(fm.horizontalAdvance(QString::number(m_axis->max(), 'f', 3))+20, 200);
    }
    else{
        return QSize(200, 10 + fm.height());
    }
}

QSize AxisView::sizeHint() const
{
    QFontMetrics fm(font());

    if(m_orientation == Qt::Vertical){
        if(m_axis == nullptr) return QSize(30, 250);
        return QSize(fm.horizontalAdvance(QString::number(m_axis->max(), 'f', 3)), 250);
    }
    else{
        return QSize(250, 10 + fm.height());
    }
}

Qt::Orientation AxisView::orientation() const
{
    return m_orientation;
}

void AxisView::setOrientation(Qt::Orientation orientation)
{
    m_orientation = orientation;


    if(m_orientation == Qt::Vertical){
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    }
    else {
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    }
}

void AxisView::paintEvent(QPaintEvent *event)
{
    if(m_axis == nullptr){
        QWidget::paintEvent(event);
        return;
    }

    QPainter painter(this);
    QPen pen((QColor(Qt::darkGray)));
    painter.setPen(pen);

    QRect rect = this->rect().adjusted(0,0,-1,-1);
    painter.drawRect(rect);
    rect.adjust(1, 1, 0, 0);

    pen.setColor(m_axis->color());
    painter.setPen(pen);

    QFontMetrics fm(font());
    int th = fm.height()/2;
    int tw = fm.horizontalAdvance(QString::number(m_axis->max(), 'f', 3))/2;

    const auto &tics = m_axis->tics();
    int count = tics.count();

    if(m_orientation == Qt::Vertical){
        for(int i = 0; i < count; ++i){
            if(!rect.contains(rect.right(), static_cast<int>(tics[i]))){
                continue;
            }
            painter.drawLine(rect.right()-8, static_cast<int>(tics[i]),
                             rect.right(), static_cast<int>(tics[i]));

            QRect textRect(rect.left(), static_cast<int>(tics[i] - th), width()-15, th*2);
            if(!rect.contains(textRect)){
                continue;
            }

            double textValue = m_axis->max() - (count==1?  m_axis->span() / 2 :
                                                           (m_axis->span() / (count - 1)) * i);

            QString text = QString::number(textValue, 'f', 3);
            painter.drawText(textRect, Qt::AlignRight, text);
        }

        int penHeigth = 2;
        pen.setWidth(penHeigth);
        painter.setPen(pen);

        for(int i = 1; i < count; ++i){
            double y = (tics[i] - tics[i-1]) / 2 + tics[i-1];
            QRect r(rect.left() + 2, static_cast<int>(y-penHeigth+1), rect.right() - 2, penHeigth);
            if(!rect.contains(r)){
                continue;
            }

            painter.drawLine(rect.right()-4, static_cast<int>(y), rect.right(), static_cast<int>(y));
        }
    }
    else{
        for(int i = 0; i < count; ++i){
            if(!rect.contains(static_cast<int>(tics[i]), rect.top())){
                continue;
            }

            painter.drawLine(static_cast<int>(tics[i]), rect.top()+8,
                             static_cast<int>(tics[i]), rect.top());

            QRect textRect(static_cast<int>(tics[i]), 8, tw*2, th*2);
            if(!rect.contains(textRect)){
                textRect.setX(textRect.x() - textRect.width() - 2);
//                continue;
            }

            double textValue = m_axis->min() + (count==1?  m_axis->span() / 2 :
                                                           (m_axis->span() / (count - 1)) * i);
            QString text = QString::number(textValue, 'f', 3);

            painter.drawText(textRect, Qt::AlignTop|Qt::AlignLeft, text);
        }

        int penHeigth = 2;
        pen.setWidth(penHeigth);
        painter.setPen(pen);

        for(int i = 1; i < count; ++i){
            double x = (tics[i] - tics[i-1]) / 2 + tics[i-1];
            QRect r(static_cast<int>(x-penHeigth+1), rect.top() + 4, penHeigth, rect.top());
            if(!rect.contains(r)){
                continue;
            }

            painter.drawLine(static_cast<int>(x), 0, static_cast<int>(x), 4);
        }
    }

    QWidget::paintEvent(event);
}

void AxisView::mousePressEvent(QMouseEvent *event)
{
    if(m_axis == nullptr) return;

    if(event->buttons() == 0){
        return;
    }

    QRect rect(1, 1, width() - 2, height() - 2);
    if (!rect.contains(event->pos()) && !isMovable){
        return;
    }

    if (event->button() == Qt::LeftButton) {
        moveLevel = m_axis->level();
        moveLevelPos = event->pos();
        isMovable = true;
        setCursor(Qt::ClosedHandCursor);
    }
    else if(event->button() == Qt::RightButton){
        moveRange = m_axis->range();
        isMovable = true;
        moveRangePos = event->pos();
        setCursor(Qt::ClosedHandCursor);
    }
}

void AxisView::mouseMoveEvent(QMouseEvent *event)
{
    if(m_axis == nullptr) return;

    if(!isMovable){
        return;
    }

    if(m_axis == nullptr){
        return;
    }

    if(!m_axis->isScalable()){
        return;
    }

    QRect rect = this->rect().adjusted(1, 1, -1, -2);

    if (event->buttons().testFlag(Qt::LeftButton)) {
        QPoint point = moveLevelPos - event->pos();
        int delta = (m_orientation == Qt::Vertical? point.ry() : point.rx());
        double dd = delta * 100.0 / (m_orientation == Qt::Vertical? rect.height() : rect.width());
        m_axis->setLevel(moveLevel + dd);
    }
    if(event->buttons().testFlag(Qt::RightButton)){
        QPoint point = moveRangePos - event->pos();
        int delta = (m_orientation == Qt::Vertical? point.ry() : point.rx());
        double dd = delta * 100.0 / (m_orientation == Qt::Vertical? rect.height() : rect.width());
        double range = moveRange + dd;
        if(range < 0.0) range = 0.0;
        m_axis->setRange(range);
    }

    calcAsisTics();

    update();
}

void AxisView::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_axis == nullptr) return;

    if (isMovable && event->buttons() == 0 ) {
        isMovable = false;
        setCursor(Qt::OpenHandCursor);
    }
}

void AxisView::resizeEvent(QResizeEvent *event)
{
    calcAsisTics();

    QWidget::resizeEvent(event);
}

void AxisView::calcAsisTics()
{
    if(m_axis == nullptr) return;

    QFontMetrics fm(font());

    if(m_orientation == Qt::Vertical){
        QRect r = rect().adjusted(1, 1, -1, -2);
        m_axis->calcTics(r.height(), r.top(), 2 * fm.height());
    }
    else{
        QRect r = rect().adjusted(1, 1, 0, -2);
        m_axis->calcTics(r.width() - 2, r.left(), fm.horizontalAdvance(QString::number(m_axis->max(), 'f', 3)));
    }
}
