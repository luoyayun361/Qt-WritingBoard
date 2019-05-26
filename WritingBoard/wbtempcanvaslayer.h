#ifndef WBTEMPCANVASLAYER_H
#define WBTEMPCANVASLAYER_H

#include <QGraphicsObject>
#include "clineobj.h"
#include <QPainter>

class WbTempCanvasLayer : public QGraphicsObject
{
public:
    explicit WbTempCanvasLayer(const QSizeF & size,QGraphicsObject * parent = nullptr);
    ~WbTempCanvasLayer();
    void clear();  //清空画布
    void drawToTemp(CLineObj * obj);  //在临时层绘图

protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    void initCanvas();                //初始化图层
private:
    QSizeF m_size;
    QImage   *m_pTempCanvas;//临时画布
    QPainter *m_pTempPainter = nullptr;//临时画布的painter
};

#endif // WBTEMPCANVASLAYER_H
