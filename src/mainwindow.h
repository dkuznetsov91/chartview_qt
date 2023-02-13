#pragma once

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void addGraph(const QString &sourceName);
    void addManyGraph(const QString &sourceName, int count);

private:
    Ui::MainWindow *ui;
};
