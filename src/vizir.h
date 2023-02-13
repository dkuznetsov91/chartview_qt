#pragma once

#include <QObject>
#include <QPoint>

class QPainter;

class Vizir : public QObject
{
    Q_OBJECT
public:
    explicit Vizir(Qt::Orientations orientations, QObject *parent = nullptr);

    void draw(QPainter *painter) const;

    void setPos(const QPointF &pos)
    {
        m_pos = pos;

        emit vizirMoved();
    }

    QPointF pos() const
    {
        return m_pos;
    }

signals:
    void vizirMoved();

private:
    QPointF m_pos;
    Qt::Orientations m_orientation;
};
