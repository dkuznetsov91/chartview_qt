#pragma once

#include <QObject>
#include <QVector>
#include <QPointF>

#include "ParameterAttributes.h"

class Parameter : public QObject
{
    Q_OBJECT
public:
    Parameter(QObject *parent = nullptr);

    double min() const;
    void setMin(double min);

    double max() const;
    void setMax(double max);

    double level() const;
    void setLevel(double level);

    double range() const;
    void setRange(double range);

    QColor color() const;
    void setcolor(const QColor &color);

    QString name() const;
    void setName(const QString &name);

    LineStyle lineStyle() const;
    void setLineStyle(LineStyle style);

    const QVector<QPointF> *data() const;
    void setData(const QVector<QPointF> &data);

signals:
    void dataReset();

private:
    ParameterAttributes m_attribs;

    QVector<QPointF> m_data;


};
