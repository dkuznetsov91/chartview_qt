#pragma once

#include <QMap>

class QPointF;

class PixValue
{
public:
    PixValue();

    PixValue(double min, double max, double first, double last);

    double m_min,        //!< Минимальное значение
           m_max,        //!< Максимальное значение
           m_first,      //!< Первое значение
           m_last;       //!< Последнее значение
    //bool   bLineBreak, //!< Флаг разрыва

};

class DoubleScreenBuffer
{
public:
    DoubleScreenBuffer() = delete;

    DoubleScreenBuffer(double range);

    const QMap<int, PixValue> *data() const;

    void addPoint(const QPointF *point);

    void createFromPoints(const QVector<QPointF> &points);

    void clear();

    double length() const;

private:
    QMap<int, PixValue> m_data;
    double m_length;
    double m_compress;
    double m_range;
};
