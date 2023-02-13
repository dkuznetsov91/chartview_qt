#include "plotter.h"
#include "parameter.h"
#include "parametermodel.h"
#include "graph.h"
#include "axis.h"
#include "vizir.h"

#include <QStylePainter>
#include <QMouseEvent>
#include <QTime>
#include <QDebug>

Plotter::Plotter(QWidget *parent) :
    QWidget(parent)
  , m_vizir(new Vizir(Qt::Vertical|Qt::Horizontal, this))
  , m_paramModel(new ParameterModel(this))
  , m_xAxis(new ConstAxis(this))
  , m_activeParameter(-1)
  , timerId(0)
{
    connect(m_xAxis, &Axis::ticsChanged, this, [this](){
        //setDirty();
        //refreshPixmap();
        update();
    });
    connect(m_vizir, &Vizir::vizirMoved, this, [this](){
        for(int i = 0; i < m_graphics.count(); ++i){
            m_paramModel->setData(m_paramModel->index(i),
                                  m_graphics[i]->getValue(static_cast<int>(m_vizir->pos().x() - 1)),
                                  ParameterModel::ValueRole);
        }
        update();
    });
}

Axis *Plotter::xAxis()
{
    return m_xAxis;
}

QAbstractItemModel *Plotter::model() const
{
    return m_paramModel;
}

void Plotter::selectGraph(const QModelIndex &index)
{
    if(index.row() == m_activeParameter){
        return;
    }

    if(index.row() > m_graphics.count()){
        m_activeParameter = -1;
    }

    m_activeParameter = index.row();

    if(m_activeParameter > -1){
        emit graphicSelected(m_paramModel->index(m_activeParameter));
        emit graphicActivated(m_graphics[m_activeParameter]);

    }

    update();
}

void Plotter::setLineStyle(LineStyle style)
{
    for(auto graph : m_graphics) graph->setLineStyle(style);

}

void Plotter::releaseGraphs()
{
    for(const auto graph : m_graphics)
        graph->resetCache();

//    refreshPixmap();

//    m_vizir->setPos(m_vizir->pos());
}

Graph *Plotter::addGraph(Parameter *parameter)
{
   Graph *graph = new Graph(parameter, m_xAxis, this);
   m_graphics.append(graph);


   int rowCount = m_paramModel->rowCount();
   m_paramModel->insertRow(rowCount);
   m_paramModel->setData(m_paramModel->index(rowCount), QVariant::fromValue(parameter), ParameterModel::ParameterRole);

   if(m_activeParameter == -1){
       m_activeParameter = 0;
       emit graphicSelected(m_paramModel->index(m_activeParameter));
       emit graphicActivated(m_graphics[m_activeParameter]);
   }

   //refreshPixmap();

   connect(graph, &Graph::graphChanged, this, [this](){
       refreshPixmap();
   });

   return graph;
}

Graph *Plotter::graph()
{
    if(m_activeParameter < 0 || m_activeParameter > m_graphics.count()){
        return nullptr;
    }
    return m_graphics[m_activeParameter];
}

QSize Plotter::minimumSizeHint() const
{
    return QSize(200, 200);
}

QSize Plotter::sizeHint() const
{
    return QSize(250, 250);
}


void Plotter::paintEvent(QPaintEvent *event)
{
//    QTime timer;
//    timer.start();

    QStylePainter painter(this);
    drawBox(&painter);
    drawGrid(&painter);
    if(timerId){
        qDebug() << rect().width();
        painter.drawPixmap(1, 1, pixmap.scaled(rect().adjusted(1,1,-1,-1).size()));
    }
    else{
         painter.drawPixmap(1, 1, pixmap);
    }

    m_vizir->draw(&painter);

    QWidget::paintEvent(event);
//    qDebug() << "paintEvent" << timer.elapsed();
}
#include <QTimer>
void Plotter::resizeEvent(QResizeEvent *event)
{
    if (timerId){
        killTimer(timerId);
    }
    timerId = startTimer(50);

    QSizeF oldSize = event->oldSize();
    QSizeF newSize = event->size();

    if(oldSize.isValid() == false){
        m_vizir->setPos(QPointF(newSize.width()/2, newSize.height()/2));
        return;
    }

    QPointF oldPoint = m_vizir->pos();
    QPointF newPoint(oldPoint.x()/oldSize.width()*newSize.width(),
                    oldPoint.y()/oldSize.height()*newSize.height());

    m_vizir->setPos(newPoint);
}

void Plotter::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() != Qt::LeftButton){
        return;
    }

    m_vizir->setPos(ev->localPos());
}

void Plotter::mouseMoveEvent(QMouseEvent *ev)
{
    if (!ev->buttons().testFlag(Qt::LeftButton)) {
        return;
    }

    m_vizir->setPos(ev->localPos());
}

void Plotter::refreshPixmap()
{
    if(timerId) return;

//    QTime timer;
//    timer.start();
    pixmap = QPixmap(size() - QSize(2,2));
    pixmap.fill(QColor(0,0,0,0));

    QPainter painter(&pixmap);
//    painter.initFrom(this);

//    qDebug() << "refreshPixmap() start" << timer.elapsed();
    drawCurves(&painter);

    m_vizir->setPos(m_vizir->pos());
//    qDebug() << "refreshPixmap() end" << timer.elapsed();
    update();
//    qDebug() << "refreshPixmap() end" << timer.elapsed();
}

void Plotter::refreshPixmap2()
{
    if(timerId) return;

//    QTime timer;
//    timer.start();
    pixmap = QPixmap(size() - QSize(2,2));
    pixmap.fill(QColor(0,0,0,0));

    QPainter painter(&pixmap);
//    painter.initFrom(this);

//    qDebug() << "refreshPixmap2() start" << timer.elapsed();
    quickDrawCurves(&painter);
//    qDebug() << "refreshPixmap2() end" << timer.elapsed();
    update();
//    qDebug() << "refreshPixmap2() end" << timer.elapsed();
}

void Plotter::drawBox(QPainter *painter)
{
    QPen pen = QColor(Qt::black);
    painter->setPen(pen);
    painter->drawRect(rect().adjusted(0, 0, -1, -1));
}

void Plotter::drawGrid(QPainter *painter)
{
    QPen pen = m_xAxis->color();

    pen.setStyle(Qt::DashLine);

    painter->save();
    painter->setPen(pen);

    QRect rect = painter->viewport().adjusted(1,1,-1,-1);

    //vertical
    {
        const auto &tics = m_xAxis->tics();
        int count = tics.count();

        for(int i = 0; i < count; ++i){
            int x = static_cast<int>(tics[i]);
            painter->drawLine(x, rect.top(), x, rect.bottom());
        }
    }

    auto *graphic = graph();
    pen.setColor(QColor(graphic? graph()->axis()->color() : QColor()));
    painter->setPen(pen);

    //horizontal
    {
        if(m_activeParameter != -1){
            const auto &tics = m_graphics[m_activeParameter]->axis()->tics();
            int count = tics.count();

            for(int i = 0; i < count; ++i){
                int y = static_cast<int>(tics[i]);
                painter->drawLine(rect.left(), y, rect.right(), y);
            }
        }
    }

    painter->restore();
}

void Plotter::drawCurves(QPainter *painter)
{
    QRect rect = painter->viewport();

    if (!rect.isValid()){
        return;
    }

    int index = 0;
    for(Graph *graph : m_graphics){
        if(index++ != m_activeParameter){
            graph->draw(painter);
        }
    }
    if(m_activeParameter != -1){
        m_graphics[m_activeParameter]->draw(painter);
    }
}

void Plotter::quickDrawCurves(QPainter *painter)
{
    QRect rect = painter->viewport();

    if (!rect.isValid()){
        return;
    }

    int index = 0;
    for(Graph *graph : m_graphics){
        if(index++ != m_activeParameter){
            graph->quickDraw(painter);
        }
    }
    if(m_activeParameter != -1){
        m_graphics[m_activeParameter]->quickDraw(painter);
    }
}

void Plotter::timerEvent(QTimerEvent *te)
{
    for(auto graph : m_graphics){
        graph->setDirty();
    }

    killTimer(te->timerId());
    timerId = 0;

    refreshPixmap();
}
