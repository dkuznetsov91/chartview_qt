#pragma once

#include <QWidget>

class Axis;

class AxisView : public QWidget
{
    Q_OBJECT

public:
    explicit AxisView(QWidget *parent = nullptr);
    ~AxisView();

    void setAxis(Axis *axis);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    Qt::Orientation orientation() const;

    void setOrientation(Qt::Orientation orientation);

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void calcAsisTics();

private:
    Axis *m_axis;

    bool isMovable;

    QPoint moveLevelPos;
    QPoint moveRangePos;
    double moveLevel;
    double moveRange;

    Qt::Orientation m_orientation;

};
