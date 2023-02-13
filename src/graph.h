#pragma once

#include <QObject>
#include <QPixmap>

#include "doublescreenbuffer.h"

class Parameter;
class ParameterAxis;
class ConstAxis;
class Axis;
class QPainter;
enum class LineStyle;

class Graph : public QObject
{
    Q_OBJECT
public:
    explicit Graph(Parameter *parameter, Axis *xAxis, QObject *parent = nullptr);

    bool isValid();

    Axis *axis();

    const QVector<QPointF> *data() const;
    const DoubleScreenBuffer *cache() const;
    QPoint createPoint(const QRect &rect, double _x, double _y, double xConst, double y1Const, double y2Const);


    void draw(QPainter *painter);
    void quickDraw(QPainter *painter);

    double getValue(int x);

    bool isDirty() const;
    void setDirty();

    void setLineStyle(LineStyle style);

    void resetCache();

signals:
    void graphChanged();

private:
    void fill(QPainter *painter);

    void fillDots(QPainter *painter);
    void fillLines(QPainter *painter);
    void fillMarkers(QPainter *painter);


    void calkConsts(const QRect &rect,
                    double &y1Const, double &y2Const);
private:
    ParameterAxis *m_yAxis;
    Axis *m_xAxis;

    DoubleScreenBuffer m_cache;

    QMap<int, PixValue> m_screenCache;

    bool m_dirty;
    QPixmap m_pixmap;
};
