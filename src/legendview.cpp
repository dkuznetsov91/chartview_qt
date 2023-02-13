#include "legendview.h"

LegendView::LegendView(QWidget *parent) :
    QListView(parent)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
}
