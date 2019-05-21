#ifndef WBCANVASITEM_H
#define WBCANVASITEM_H

#include <QGraphicsObject>

class CLineObj
{
public:
    CLineObj(const QPointF & point) {
        m_path.moveTo(point);
    }

    int elementCount(){
        return m_path.elementCount();
    }
    void createNewPath()
    {
        QPointF endPoint;
        endPoint = m_path.currentPosition();
        m_path &= QPainterPath();
        m_path.moveTo(endPoint);
    }
    QPainterPath& path(){
        return m_path;
    }
    void addToPath(const QPointF & p1,const QPointF & p2)
    {
        QPointF pt1 = (p1);
        QPointF pt2 = (p2);
        QPointF d = pt2 - pt1;
        if(qAbs(d.x()) > 0 || qAbs(d.y()) >0)
        {
            m_path.quadTo(pt1,(pt1+pt2)/2);
        }
        QRectF r(p1,p2);
        QRectF fixRect = r.normalized();
        m_updateRect = fixRect.adjusted(-200,-200,400,400);
    }
    QPainterPath StrokePath(int width)
    {
        QPainterPathStroker stroker;
        stroker.setWidth(width);
        stroker.setCapStyle(Qt::RoundCap);
        stroker.setJoinStyle(Qt::RoundJoin);
        return  stroker.createStroke(m_path);
    }
    QRectF updateRect(){
        return m_updateRect;
    }


private:
    QPainterPath m_path;
    QRectF m_updateRect;
};

class WbCanvasItem : public QGraphicsObject
{
public:
    explicit WbCanvasItem(const QSizeF & size,QGraphicsObject * parent = nullptr);
    ~WbCanvasItem();

    void drawPress(int id,const QPointF &p);
    void drawMove(int id,const QPointF &lastPoint,const QPointF &curPoint);
    void drawRelease(int id, const QPointF &point);
    void setBackgroundColor(const QColor & color);
protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    void resize(const QSizeF & size);
    void drawToTemp(CLineObj * obj);  //在临时层绘图
    void drawToReal(CLineObj * obj);  //在真实层绘非矢量图
    void drawToRealByVector(CLineObj * obj); //在真实层绘矢量图
    void initCanvas();                //初始化图层
private:
    QSizeF m_size;
    QImage   *m_pTempCanvas;//临时画布
    QPainter *m_pTempPainter = nullptr;//临时画布的painter

    QImage   *m_pRealCanvas;//真实画布
    QPainter *m_pRealPainter = nullptr;//真实画布的painter

    QMap<int,CLineObj*> m_lineObjs;

    QColor  m_bgColor = QColor(Qt::white);
};

#endif // WBCANVASITEM_H
