#pragma once

#include <QListView>

class LegendView : public QListView
{
public:
    LegendView(QWidget *parent = nullptr);

    QSize sizeHint() const
    {
        return QSize(100, QListView::sizeHint().height());
    }
};
