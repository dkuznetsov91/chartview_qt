#include "doublescreenbuffer.h"

#include <QVector>
#include <QPoint>

PixValue::PixValue()
{
    m_min = std::numeric_limits<decltype(m_min)>::min();
    m_max = std::numeric_limits<decltype(m_min)>::max();
    m_first = 0;
    m_last = 0;
}

PixValue::PixValue(double min, double max, double first, double last)  :
    m_min(min)
  , m_max(max)
  , m_first(first)
  , m_last(last)
{

}

DoubleScreenBuffer::DoubleScreenBuffer(double range)
{
    m_length = 2000.0;
    m_range = range;
    m_compress = m_range / m_length;
}

const QMap<int, PixValue> *DoubleScreenBuffer::data() const
{
    return &m_data;
}

void DoubleScreenBuffer::addPoint(const QPointF *point)
{
    int key = static_cast<int>(point->x() / m_compress);

    if(m_data.contains(key)){
        PixValue &pixel = m_data[key];
        pixel.m_max = qMax(pixel.m_max, point->y());
        pixel.m_min = qMin(pixel.m_min, point->y());
        pixel.m_last = point->y();
    }
    else{
        PixValue pv(point->y(), point->y(), point->y(), point->y());

        m_data.insert(key, pv);
    }
}

void DoubleScreenBuffer::createFromPoints(const QVector<QPointF> &points)
{
    clear();
    for(const QPointF &point : points) {
    //for(auto it = points.begin(); it != points.end(); ++it){
//    int count = points.count();
//    for(int i = 0; i < count; ++i){
//        addPoint(points.data() + i);
        addPoint(&point);
    }
}

void DoubleScreenBuffer::clear()
{
    m_data.clear();
}

double DoubleScreenBuffer::length() const
{
    return m_length;
}
