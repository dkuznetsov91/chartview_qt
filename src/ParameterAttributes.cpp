#include "ParameterAttributes.h"

ParameterAttributes::ParameterAttributes() :
    m_min       {0.0}
  , m_max       {100.0}
  , m_level     {50.0}
  , m_range     {100.0}
  , m_lineStyle {LineStyle::Line}
  , m_type      {ParamType::Analog}
  , m_color     {QColor(Qt::darkGreen)}
  , m_name      {}
  , m_baseLine  {false}
{

}

double ParameterAttributes::min() const
{
    return m_min;
}

void ParameterAttributes::setMin(double min)
{
    m_min = min;
}

double ParameterAttributes::max() const
{
    return m_max;
}

void ParameterAttributes::setMax(double max)
{
    m_max = max;
}

double ParameterAttributes::level() const
{
    return m_level;
}

void ParameterAttributes::setLevel(double level)
{
    m_level = level;
}

double ParameterAttributes::range() const
{
    return m_range;
}

void ParameterAttributes::setRange(double range)
{
    m_range = range;
}

LineStyle ParameterAttributes::lineStyle() const
{
    return m_lineStyle;
}

void ParameterAttributes::setLineStyle(LineStyle style)
{
    m_lineStyle = style;
}

ParamType ParameterAttributes::type() const
{
    return m_type;
}

void ParameterAttributes::setType(ParamType type)
{
    m_type = type;
}

QColor ParameterAttributes::color() const
{
    return m_color;
}

void ParameterAttributes::setColor(const QColor &color)
{
    m_color = color;
}

QString ParameterAttributes::name() const
{
    return m_name;
}

void ParameterAttributes::setName(const QString &name)
{
    m_name = name;
}

bool ParameterAttributes::isBaseLine() const
{
    return m_baseLine;
}

void ParameterAttributes::setBaseLine(bool baseLine)
{
    m_baseLine = baseLine;
}
