#ifndef WBCANVASITEM_H
#define WBCANVASITEM_H

#include <QGraphicsObject>
#include "clineobj.h"
#include "wbtempcanvaslayer.h"
#include <wblinevector.h>


class WbCanvasItem : public QGraphicsObject
{
public:
    enum DrawMode{
        Mode_DrawLine,
        Mode_Eraser
    };
    explicit WbCanvasItem(const QSizeF & size,QGraphicsObject * parent = nullptr);
    ~WbCanvasItem();

    void drawPress(int id,const QPointF &p);
    void drawMove(int id,const QPointF &lastPoint,const QPointF &curPoint);
    void drawRelease(int id, const QPointF &point);
    void setBackgroundColor(const QColor & color);
    void setMode(DrawMode mode);
    void clear();
    void zoomIn(qreal sc);  //放大
    void zoomOut(qreal sc); //缩小
    qreal scaleR()const;
protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    void resize(const QSizeF & size);
    void drawToReal(CLineObj * obj);  //在真实层绘非矢量图
    void drawToRealByVector(CLineObj * obj); //在真实层绘矢量图
    void initCanvas();                //初始化图层

    //线擦除
    void doErase(QPointF pos1, QPointF pos2, int width );
    void eraseVectorLineRoamer(WbLineVector *l, const QPointF &p1, const QPointF &p2, int width);
    QPainterPath createStrokePath(const QPointF &p1, const QPointF &p2, int width);
private:
    QSizeF m_size;

    WbTempCanvasLayer * m_pTempLayer = nullptr;

    QImage   *m_pRealCanvas;//真实画布
    QPainter *m_pRealPainter = nullptr;//真实画布的painter

    QMap<int,CLineObj*> m_lineObjs;

    QColor  m_bgColor = QColor(Qt::white);

    DrawMode m_curMode = Mode_DrawLine;
};

#endif // WBCANVASITEM_H
