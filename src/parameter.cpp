#include "parameter.h"


Parameter::Parameter(QObject *parent) :
    QObject(parent)
  , m_attribs {}
{

}

double Parameter::min() const
{
    return m_attribs.min();
}

void Parameter::setMin(double min)
{
    m_attribs.setMin(min);
}

double Parameter::max() const
{
    return m_attribs.max();
}

void Parameter::setMax(double max)
{
    m_attribs.setMax(max);
}

double Parameter::level() const
{
    return m_attribs.level();
}

void Parameter::setLevel(double level)
{
    m_attribs.setLevel(level);
}

double Parameter::range() const
{
    return m_attribs.range();
}

void Parameter::setRange(double range)
{
    m_attribs.setRange(range);
}

QColor Parameter::color() const
{
    return m_attribs.color();
}

void Parameter::setcolor(const QColor &color)
{
    m_attribs.setColor(color);
}

QString Parameter::name() const
{
    return m_attribs.name();
}

void Parameter::setName(const QString &name)
{
    m_attribs.setName(name);
}

LineStyle Parameter::lineStyle() const
{
    return m_attribs.lineStyle();
}

void Parameter::setLineStyle(LineStyle style)
{
    m_attribs.setLineStyle(style);
}

const QVector<QPointF> *Parameter::data() const
{
    return &m_data;
}

void Parameter::setData(const QVector<QPointF> &data)
{
    m_data = data;

    emit dataReset();
}
