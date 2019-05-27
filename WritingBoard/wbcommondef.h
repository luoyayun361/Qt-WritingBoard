#ifndef WBCOMMONDEF_H
#define WBCOMMONDEF_H

#include <QGraphicsItem>

//#define DRAW_VECTOR  //矢量绘图

#define SAFE_DELETE(x) if(x) \
    { \
        delete (x); \
        (x) = nullptr; \
    }

enum WbItemType{
    Type_LineVector = QGraphicsItem::UserType + 10,
};

#endif // WBCOMMONDEF_H
