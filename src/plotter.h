#pragma once

#include <QWidget>
#include <QPointF>

class Parameter;
class ParameterModel;
class QAbstractItemModel;
class Graph;
class Vizir;
class Axis;
enum class LineStyle;

class Plotter : public QWidget
{
    Q_OBJECT
public:
    explicit Plotter(QWidget *parent = nullptr);

    Graph *addGraph(Parameter *parameter);
    Graph *graph();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    Axis *xAxis();

    QAbstractItemModel *model() const;

    void selectGraph(const QModelIndex &index);

    void setLineStyle(LineStyle style);

    void releaseGraphs();

signals:
    void graphicActivated(Graph *graph);
    void graphicSelected(const QModelIndex &index);

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);

    void timerEvent(QTimerEvent *te);

private:
    void refreshPixmap();
    void refreshPixmap2();
    void drawBox(QPainter *painter);
    void drawGrid(QPainter *painter);
    void drawCurves(QPainter *painter);
    void quickDrawCurves(QPainter *painter);

private:
    QPixmap pixmap;
    QVector<Graph*> m_graphics;
    Vizir *m_vizir;
    ParameterModel *m_paramModel;

    Axis *m_xAxis;
    int m_activeParameter;
    int timerId;
};
