#include "axis.h"
#include "parameter.h"

Axis::Axis(QObject *parent) :
    QObject (parent)
{

}

double Axis::span() const
{
    return max() - min();
}

void Axis::calcTics(double length, double offset, int minTicDistance)
{
    tics().clear();

    double rangeLength = length / 100.0 * range();
    double levelLength = length / 100.0 * (100.0  - level());
    double levelStart = levelLength - rangeLength / 2;

    int ticCount = maxTics();
    double ticStep = (rangeLength / ( ticCount - 1));
    double step = (span()) / ( ticCount - 1);

    while((ticStep < minTicDistance) ){
        ticCount--;
        if(ticCount > 1){
            ticStep = (rangeLength/( ticCount - 1));
            step = (span()) / (ticCount - 1);
        }
        else{
            ticStep = rangeLength / 2;
            step = (span()) / 2;
            break;
        }
    }

    for(int i = 0; i < ticCount; ++i){
        if(ticCount == 1){
            tics().append(levelLength + offset);
        }
        else{
            tics().append(ticStep * i + levelStart + offset);
        }
    }

    emit ticsChanged();
}

ConstAxis::ConstAxis(QObject *parent) :
    Axis(parent)
{
    m_max = 100;
    m_min = 0;
    m_level = 50;
    m_range = 100;

    m_maxTics = 11;
}

ConstAxis::~ConstAxis()
{

}

double ConstAxis::min() const
{
    return m_min;
}

void ConstAxis::setMin(double min)
{
    m_min = min;
}

double ConstAxis::max() const
{
    return m_max;
}

void ConstAxis::setMax(double max)
{
    m_max = max;
}

double ConstAxis::level() const
{
    return m_level;
}

void ConstAxis::setLevel(double level)
{
    m_level = level;
}

double ConstAxis::range() const
{
    return m_range;
}

void ConstAxis::setRange(double range)
{
    m_range = range;
}

QColor ConstAxis::color() const
{
    return QColor(Qt::darkGreen);
}

bool ConstAxis::isScalable()
{
    return false;
}

int ConstAxis::maxTics() const
{
    return m_maxTics;
}

void ConstAxis::setMaxTics(int maxTics)
{
    m_maxTics = maxTics;
}

QList<double> &ConstAxis::tics()
{
    return m_ticCoordinates;
}

ParameterAxis::ParameterAxis(Parameter *parameter, QObject *parent) :
    ConstAxis(parent)
  , m_parameter(parameter)
{

}

ParameterAxis::~ParameterAxis()
{

}

double ParameterAxis::min() const
{
    return m_parameter->min();
}

void ParameterAxis::setMin(double min)
{
    m_parameter->setMin(min);
    m_min = min;

    emit axisChanged();
}

double ParameterAxis::max() const
{
    return m_parameter->max();
}

void ParameterAxis::setMax(double max)
{
    m_parameter->setMin(max);
    m_max = max;

    emit axisChanged();
}

double ParameterAxis::level() const
{
    return m_parameter->level();
}

void ParameterAxis::setLevel(double level)
{
    m_parameter->setLevel(level);
    m_level = level;

    emit axisChanged();
}

double ParameterAxis::range() const
{
    return m_parameter->range();
}

void ParameterAxis::setRange(double range)
{
    m_parameter->setRange(range);
    m_range = range;

    emit axisChanged();
}

QColor ParameterAxis::color() const
{
    return m_parameter->color();
}

bool ParameterAxis::isScalable()
{
    return true;
}

Parameter *ParameterAxis::parameter()
{
    return m_parameter;
}
