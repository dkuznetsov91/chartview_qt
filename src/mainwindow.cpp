#include "mainwindow.h"
#include "graph.h"
#include "axis.h"
#include "parameter.h"
#include "ui_mainwindow.h"

#include <QPoint>
#include <QFile>
#include <QTextStream>
#include <QActionGroup>
#include <cmath>

QVector<QPointF> generateData(int min, int max)
{
    Q_UNUSED(min);
    Q_UNUSED(max);
    int numPoints = 100001;

    QVector<QPointF> data;
    data.reserve(numPoints);

    double t;

    for(int i = 0; i < numPoints; ++i){
        auto v = i*100.0/numPoints;
        //data.append(QPointF(v, (uint(qrand()) % (max-min) ) + min));
        data.append(QPointF(i, i));

        t = v;
    }

    return data;
}

QVector<QPointF> dataFromFile(const QString &fileName, std::tuple<QString, double, double, double, double, QString> &header)
{
    QFile file(fileName);
    QVector<QPointF> data;

    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);

        //header
        {
            QString line = in.readLine();
            line = in.readLine();
            QStringList pairs = line.split(' ', Qt::SkipEmptyParts);

            header = {pairs[0], pairs[1].toDouble(), pairs[2].toDouble(),
                      pairs[3].toDouble(), pairs[4].toDouble(), pairs[5]};
        }
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList pairs = line.split(' ', Qt::SkipEmptyParts);
            if (pairs.count() == 2) {
                double x = pairs[0].toDouble();
                double y = pairs[1].toDouble();
                data.append(QPointF(x, y));
            }
        }
    }

    return data;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->xAxisView->setAxis(ui->plotter->xAxis());
    ui->xAxisView->setOrientation(Qt::Horizontal);

    ui->legend->setModel(ui->plotter->model());

    connect(ui->legend, &QListView::clicked, ui->plotter, &Plotter::selectGraph);
    connect(ui->plotter, &Plotter::graphicSelected, ui->legend, &QListView::setCurrentIndex);
    connect(ui->plotter, &Plotter::graphicActivated, ui->yAxisView, [this](Graph *graph){
        ui->yAxisView->setAxis(graph->axis());
    });


    QActionGroup *group = new QActionGroup(this);
    group->addAction(ui->actionLines);
    group->addAction(ui->actionDots);
    group->addAction(ui->actionMarkers);
    ui->actionLines->setChecked(true);

    connect(ui->actionLines, &QAction::triggered, [this]() {
        ui->plotter->setLineStyle(LineStyle::Line);
    });
    connect(ui->actionDots, &QAction::triggered, [this]() {
        ui->plotter->setLineStyle(LineStyle::Dot);
    });
    connect(ui->actionMarkers, &QAction::triggered, [this]() {
        ui->plotter->setLineStyle(LineStyle::Marker);
    });


    for(const auto &source : {"NY.txt", "NX.txt"}){
        addGraph(source);
    }
    addGraph("NX_.txt");
//    addManyGraph("NY.txt", 29);
//    addGraph("NY.txt");

    ui->plotter->releaseGraphs();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addGraph(const QString &sourceName)
{
    std::tuple<QString, double, double, double, double, QString> header;

    auto *parameter = new Parameter(this);
    parameter->setData(dataFromFile(sourceName, header));
    parameter->setName(std::get<0>(header));
    parameter->setMin(std::get<1>(header));
    parameter->setMax(std::get<2>(header));
    parameter->setLevel(std::get<3>(header));
    parameter->setRange(std::get<4>(header));
    parameter->setcolor(QColor(std::get<5>(header)));
    ui->plotter->addGraph(parameter);
}

void MainWindow::addManyGraph(const QString &sourceName, int count)
{
    std::tuple<QString, double, double, double, double, QString> header;
    auto data = dataFromFile(sourceName, header);

    for(int i = 0; i < count; ++i){
        auto *parameter = new Parameter(this);
        parameter->setData(data);
        parameter->setName(std::get<0>(header));
        parameter->setMin(std::get<1>(header));
        parameter->setMax(std::get<2>(header));
        parameter->setLevel(std::get<3>(header));
        parameter->setRange(std::get<4>(header));
        parameter->setcolor(QColor(std::get<5>(header)));
        ui->plotter->addGraph(parameter);
    }
}
