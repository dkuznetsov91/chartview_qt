#include "graph.h"
#include "axis.h"
#include "parameter.h"

#include "rest_marker.h"

#include <QPainter>

Graph::Graph(Parameter *parameter, Axis *xAxis, QObject *parent) :
    QObject(parent)
  , m_yAxis(new ParameterAxis(parameter, this))
  , m_xAxis(xAxis)
  , m_cache(0)
  , m_dirty(true)
{
    m_xAxis->setMin(0.0);
    m_xAxis->setMax(std::max(parameter->data()->last().x(), m_xAxis->max()));

    connect(parameter, &Parameter::dataReset, this, &Graph::resetCache);
    connect(m_yAxis, &Axis::axisChanged, this, [this](){
        //&Graph::graphChanged);
        setDirty();
//        qDebug() << m_yAxis->parameter()->name();
        emit graphChanged();
    });

//    resetCache();
}

void Graph::resetCache()
{
    auto cnt = m_xAxis->span();

    m_cache = DoubleScreenBuffer(cnt);
    m_cache.createFromPoints(*data());

    setDirty();
}

bool Graph::isValid()
{
    return m_yAxis != nullptr;
}

Axis *Graph::axis()
{
    return m_yAxis;
}

const QVector<QPointF> *Graph::data() const
{
    if(m_yAxis == nullptr) return nullptr;
    if(m_yAxis->parameter() == nullptr) return nullptr;
    return m_yAxis->parameter()->data();
}

const DoubleScreenBuffer *Graph::cache() const
{
    if(m_yAxis == nullptr) return nullptr;
    if(m_yAxis->parameter() == nullptr) return nullptr;
    return &m_cache;
}

void Graph::calkConsts(const QRect &rect, double &y1Const, double &y2Const)
{
    const double minMaxRange = m_yAxis->span();

    y1Const = rect.height() / ( minMaxRange * ( 100.0 / m_yAxis->range() ) );

    y2Const = (minMaxRange / 100.0) *
            ( ( m_yAxis->level() - 50.0) + ( 100.0 - m_yAxis->range() ) / 2 ) *
            rect.height() / minMaxRange;
}

QPoint Graph::createPoint(const QRect &rect, double _x, double _y, double xConst, double y1Const, double y2Const)
{
    double x = rect.left() + (_x * xConst);
    double y = rect.bottom() - (_y - m_yAxis->min()) * y1Const - y2Const;

    static int key = -1;
    static auto it = m_screenCache.end();

    if(key == static_cast<int>(x)){  //contains
        PixValue &pixel = it.value();//m_screenCache[key];
        pixel.m_max = qMax(pixel.m_max, _y);
        pixel.m_min = qMin(pixel.m_min, _y);
        pixel.m_last = _y;
    }
    else{
        key = static_cast<int>(x);
        it = m_screenCache.insert(key, PixValue(_y, _y, _y, _y));
    }

    return QPoint(static_cast<int>(x), static_cast<int>(y));
}

double Graph::getValue(int x)
{
    if(x < m_xAxis->min()) x = static_cast<decltype(x)>(m_xAxis->min());
    if(x > m_xAxis->max()) x = static_cast<decltype(x)>(m_xAxis->max());

    if(m_screenCache.contains(x)){
//        qDebug() << m_screenCache[x].m_max << m_screenCache[x].m_min;
        return m_screenCache[x].m_max;//min first last
    }
    else if(m_screenCache.isEmpty()){
        return 0;
    }
    else{
        return m_screenCache.last().m_max;
    }
}

bool Graph::isDirty() const
{
    return m_dirty;
}

void Graph::setDirty()
{
    m_dirty = true;
}

void Graph::setLineStyle(LineStyle style)
{
    m_yAxis->parameter()->setLineStyle(style);
    setDirty();
    emit graphChanged();
}

void Graph::fill(QPainter *painter)
{
    if(!isDirty()){
        return;
    }

    switch(m_yAxis->parameter()->lineStyle()){
        case LineStyle::Line:   fillLines(painter);    break;
        case LineStyle::Dot:    fillDots(painter);     break;
        case LineStyle::Marker: fillMarkers(painter);  break;
        case LineStyle::DotLine:                    break;
    }

    m_dirty = false;
}

void Graph::fillDots(QPainter *painter)
{
    const auto *data = m_yAxis->parameter()->data();

    using vectorType = std::remove_reference<decltype (*data)&>::type;
    QVectorIterator<vectorType::value_type> it(*data);

    QRect rect = painter->viewport();

    double range = m_xAxis->span();
    QPoint last;

    double xConst, y1Const, y2Const;
    xConst = rect.width() / range;
    calkConsts(rect, y1Const, y2Const);

    while (it.hasNext()){
        const auto &point = it.next();
        auto p = createPoint(rect, point.x(), point.y(), xConst, y1Const, y2Const);
        if(!(last.x() == p.x() && last.y() == p.y()) ){
            painter->drawPoint(p);
        }
        last = p;
    }
}

void Graph::fillLines(QPainter *painter)
{
    const auto *data = m_cache.data();

    using mapType = std::remove_reference<decltype (*data)&>::type;
    QMapIterator<mapType::key_type, mapType::mapped_type> it(*data);

    QRect rect = painter->viewport();

    double range = m_cache.length();

    double xConst, y1Const, y2Const;
    xConst = rect.width() / range;
    calkConsts(rect, y1Const, y2Const);

    QLine lines[4];
    lines[0].setP1(createPoint(rect, data->begin().key(), data->begin().value().m_first, xConst, y1Const, y2Const));

    while (it.hasNext()) {
        it.next();

        auto p = createPoint(rect, it.key(), it.value().m_first, xConst, y1Const, y2Const);
        lines[0].setP2(p);
        lines[1].setP1(p);

        p = createPoint(rect, it.key(), it.value().m_min, xConst, y1Const, y2Const);
        lines[1].setP2(p);
        lines[2].setP1(p);

        p = createPoint(rect, it.key(), it.value().m_max, xConst, y1Const, y2Const);
        lines[2].setP2(p);
        lines[3].setP1(p);

        p = createPoint(rect, it.key(), it.value().m_last, xConst, y1Const, y2Const);
        lines[3].setP2(p);

        painter->drawLines(lines, 4);

        lines[0].setP1(p);
    }
}
void Graph::fillMarkers(QPainter *painter)
{
    const auto *data = m_yAxis->parameter()->data();

    using vectorType = std::remove_reference<decltype (*data)&>::type;
    QVectorIterator<vectorType::value_type> it(*data);

    QRect rect = painter->viewport();

    double range = m_xAxis->span();

    double xConst, y1Const, y2Const;
    xConst = rect.width() / range;
    calkConsts(rect, y1Const, y2Const);

    QPixmap mrk(11,11);
    mrk.fill(QColor(0,0,0,0));
    QPainter mrkPaint(&mrk);
    mrkPaint.setPen(painter->pen());

    DrawMarker(&mrkPaint, 5, 5, 14, 10);

    QPoint last;

    while (it.hasNext()){
        const auto &point = it.next();
        auto p = createPoint(rect, point.x(), point.y(), xConst, y1Const, y2Const);

        if(!(last.x() == p.x() && last.y() == p.y()) ){
            painter->drawPixmap(p - QPoint(5, 5), mrk);
        }
        last = p;
    }
}

void Graph::draw(QPainter *painter)
{
    if(isDirty()){
        m_screenCache.clear();

        m_pixmap = QPixmap(painter->viewport().size());
        m_pixmap.fill(QColor(0,0,0,0));
    }
    QPainter p(&m_pixmap);

    QPen pen(m_yAxis->color());
    p.setPen(pen);

    fill(&p);

    painter->drawPixmap(0, 0, m_pixmap);
}

void Graph::quickDraw(QPainter *painter)
{
    m_screenCache.clear();

    QPen pen(m_yAxis->color());
    painter->setPen(pen);

    fill(painter);

    //fill(painter);
    //painter->drawPixmap(0, 0, m_pixmap);
}
