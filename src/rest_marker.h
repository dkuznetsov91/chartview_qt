#pragma once

#include <QPainter>
#include <QPoint>
#include <vector>

static constexpr int MarkersNumber    = 33;
static constexpr int MAX_POINT_MARKER = 10;

using coords = std::vector<uint8_t>;
const auto pair = [](coords &&l, coords &&r){
    return std::make_pair<coords, coords>(std::move(l),std::move(r));
};

static const std::vector<std::pair<coords, coords>> v =
{
    pair({} ,   {0,0} ),                            //0
    pair({2,2}, {0,0,2,2,0,2,2,0}),                 //1
    pair({2,2}, {1,0,1,2,0,1,2,1} ),                //2
    pair({3,2}, {0,0,1,1,1,2,1,1,2,0} ),            //3
    pair({5},   {1,2,1,1,0,0,2,0,1,1} ),            //4
    pair({5},   {0,1,1,1,2,0,2,2,1,1} ),            //5
    pair({5},   {1,0,1,1,0,2,2,2,1,1} ),            //6
    pair({5},   {2,1,1,1,0,0,0,2,1,1} ),            //7
    pair({5},   {0,0,2,0,2,2,0,2,0,0} ),            //8
    pair({6,2}, {0,0,2,0,2,2,0,2,0,0,2,2,0,2,2,0} ),//9
    pair({5},   {0,1,2,1,1,0,1,2,2,1} ),            //10
    pair({5},   {1,0,1,2,0,1,2,1,1,2}   ),          //11
    pair({5},   {2,1,0,1,1,0,1,2,0,1}   ),          //12
    pair({5},   {1,2,1,0,0,1,2,1,1,0}   ),          //13
    pair({5,2,2},   {0,0,2,0,2,2,0,2,0,0,1,0,1,2,0,1,2,1} ),//14
    pair({5},   {0,0,2,2,2,0,0,2,0,0}  ),           //15
    pair({5},   {0,0,2,2,0,2,2,0,0,0}  ),           //16
    pair({3,2}, {0,2,1,1,2,2,1,1,1,0}  ),           //17
    pair({5},   {0,0,2,2,2,0,0,2,2,2}  ),           //18
    pair({5},   {2,2,0,0,0,2,2,0,0,0}  ),           //19
    pair({5},   {2,0,0,2,0,0,2,2,0,2}  ),           //20
    pair({5},   {0,2,2,0,2,2,0,0,2,0}  ),           //21
    pair({6},   {0,0,2,0,2,2,0,2,0,0,2,2} ),        //22
    pair({6},   {2,0,2,2,0,2,0,0,2,0,0,2} ),        //23
    pair({5,5}, {0,0,2,0,2,2,0,2,0,0,0,1,1,2,2,1,1,0,0,1} ),//24
    pair({5},   {0,1,1,2,2,1,1,0,0,1}  ),           //25
    pair({6},   {1,2,2,1,1,0,0,1,1,2,1,0}  ),       //26
    pair({5},   {0,1,1,2,1,0,2,1,0,1}  ),           //27
    pair({5},   {0,1,1,0,1,2,2,1,0,1}  ),           //28
    pair({7},   {2,1,1,1,1,2,0,2,0,0,1,0,1,1}  ),   //29
    pair({7},   {0,1,1,1,1,2,2,2,2,0,1,0,1,1}  ),   //30
    pair({7},   {1,2,1,1,0,1,0,0,2,0,2,1,1,1}  ),   //31
    pair({7},   {1,0,1,1,0,1,0,2,2,2,2,1,1,1}  )    //32
};

void Polyline(QPainter *gc, const QPoint point[], const uint8_t arMas) {
    if (gc == nullptr)
        return;

    for (uint8_t i = 1; i < arMas; i++){
        gc->drawLine( point[i-1].x(), point[i-1].y(), point[i].x(), point[i].y() );
    }
}

void PolyPolyline(QPainter *gc, const QPoint point[], const uint8_t arMas[], const uint8_t karMas ) {
    for (uint8_t j=0, k=0; j<karMas; j++) {
        Polyline( gc, point + k, arMas[j] );
        k += arMas[j];
    }
}

void DrawMarker(QPainter *gc, int x, int y, int number, int size) {
    size /= 2;
    uint nm = number % MarkersNumber;
    if (!nm) return;
    QPoint  pMark[MAX_POINT_MARKER];
    uint8_t kp = 0;
    int masx[3] = {x-size,x,x+size};
    int masy[3] = {y-size,y,y+size};

    auto pair = v[nm];
    auto count = v[nm].second.size();
    for(uint i = 0; i < count; i+=2){
        pMark[kp].setX(masx[v[nm].second[i]]);
        pMark[kp++].setY(masy[v[nm].second[i+1]]);
    }

    if (v[nm].first.size() == 1){
        Polyline(gc, pMark, kp);
    }
    else{
        PolyPolyline(gc, pMark, v[nm].first.data(), static_cast<uint8_t>(v[nm].first.size()));
    }
}
