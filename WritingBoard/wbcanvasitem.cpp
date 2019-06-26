#include "wbcanvasitem.h"
#include <QDebug>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsPathItem>
#include "wbcommondef.h"
#include <QPointer>
#include <QTime>


WbCanvasItem::WbCanvasItem(const QSizeF & size,QGraphicsObject * parent):
    QGraphicsObject (parent),
    m_size(size)
{
    initCanvas();
}

WbCanvasItem::~WbCanvasItem()
{
    SAFE_DELETE(m_pRealPainter);
    SAFE_DELETE(m_pRealCanvas);
    qDebug() << "--->Lynn<---" << __FUNCTION__ << "after";
}

void WbCanvasItem::drawPress(int id, const QPointF &p)
{
    CLineObj * obj = new CLineObj(p);
    obj->addToPath(p,p);
    m_lineObjs.insert(id,obj);
    m_pTempLayer->drawToTemp(obj);
}

void WbCanvasItem::drawMove(int id, const QPointF &lastPoint, const QPointF &curPoint)
{
    CLineObj * obj = m_lineObjs.value(id,nullptr);
    if(!obj) return;
    obj->addToPath(lastPoint,curPoint);

    if(m_curMode == Mode_DrawLine){
#if 0
        if(obj->elementCount() < 300){
            m_pTempLayer->drawToTemp(obj);
        }
        else{
#ifndef DRAW_VECTOR
            drawToReal(obj);       //绘制真实层 非矢量线
#else
            drawToRealByVector(obj);       //绘制真实层，矢量线
#endif
            obj->createNewPath();   //清空画线
        }
#else
#ifndef DRAW_VECTOR
        drawToReal(obj);
        obj->createNewPath();   //清空画线
#else
        m_pTempLayer->drawToTemp(obj);
        obj->createNewPath();   //清空画线
#endif

#endif
    }
    else{
        doErase(lastPoint,curPoint,30);
    }
}

void WbCanvasItem::drawRelease(int id, const QPointF &point)
{
    CLineObj * obj = m_lineObjs.value(id,nullptr);
    if(!obj) return;
    obj->addToPath(point,point);
    if(m_curMode == Mode_DrawLine){
#ifdef DRAW_VECTOR
        drawToRealByVector(obj);  //绘制真实层，矢量线
        obj->createNewRealPath();   //清空画线
#else
        drawToReal(obj);  //绘制真实层 非矢量线
#endif
    }
    else{
        doErase(point,point,30);
    }

    m_lineObjs.remove(id);
    delete obj;
    obj = nullptr;
}

void WbCanvasItem::setBackgroundColor(const QColor &color)
{
    m_bgColor = color;
    m_pRealCanvas->fill(color);
}

void WbCanvasItem::setMode(WbCanvasItem::DrawMode mode)
{
    m_curMode = mode;
}

void WbCanvasItem::clear()
{
#ifdef DRAW_VECTOR
    QList<QGraphicsItem*> items = this->childItems();
    QList<QGraphicsItem*>::iterator it = items.begin();
    while(it != items.end()){
        QGraphicsItem* item = *it++;
        if(item->type() == Type_LineVector){
            delete item;
            item = nullptr;
        }
    }
#else
    m_pRealCanvas->fill(m_bgColor);
#endif
    update();
}

void WbCanvasItem::zoomIn(qreal sc)
{
#ifdef DRAW_VECTOR
    QList<QGraphicsItem*> items = this->childItems();
    QList<QGraphicsItem*>::iterator it = items.begin();
    while(it != items.end()){
        QGraphicsItem* item = *it++;
        if(item->type() == Type_LineVector){
            item->setScale(item->scale() + sc);
        }
    }
#else
    this->setScale(this->scale() + sc);
#endif
}

void WbCanvasItem::zoomOut(qreal sc)
{
#ifdef DRAW_VECTOR
    QList<QGraphicsItem*> items = this->childItems();
    QList<QGraphicsItem*>::iterator it = items.begin();
    while(it != items.end()){
        QGraphicsItem* item = *it++;
        if(item->type() == Type_LineVector){
            item->setScale(item->scale() - sc);
        }
    }
#else
    this->setScale(this->scale() - sc);
#endif
}

QRectF WbCanvasItem::boundingRect() const
{
    return  QRectF(0,0,m_size.width(),m_size.height());
}

void WbCanvasItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
#ifndef DRAW_VECTOR
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->drawImage(0,0,*m_pRealCanvas);
#endif
}

void WbCanvasItem::resize(const QSizeF &size)
{
    m_size = size;
}

void WbCanvasItem::drawToReal(CLineObj *obj)
{
    m_pRealPainter->setRenderHint(QPainter::Antialiasing, true);
    m_pRealPainter->setCompositionMode(QPainter::CompositionMode_Source);
    m_pRealPainter->setPen(QPen(Qt::red,5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    QPainterPath path = obj->StrokePath(5);
    m_pRealPainter->fillPath(path,Qt::red);//填充轮廓
    //清空临时层
    m_pTempLayer->clear();
    this->update(path.boundingRect());
}
void WbCanvasItem::drawToRealByVector(CLineObj *obj)
{
    qDebug() << "--->>>Lynn<<<---" << __FUNCTION__;
    QPainterPath path = obj->StrokeRealPath(5);
    QPointer<WbLineVector> item = new WbLineVector(this);
    item->setPath(path);
    //清空临时层
    m_pTempLayer->clear();
}

void WbCanvasItem::initCanvas()
{
    m_pRealCanvas = new QImage(m_size.toSize(),QImage::Format_ARGB32_Premultiplied);
    m_pRealCanvas->fill(Qt::white);
    m_pRealPainter = new QPainter(m_pRealCanvas);

    //临时绘画层
    m_pTempLayer = new WbTempCanvasLayer(m_size,this);
    m_pTempLayer->setZValue(10);

//    this->setTransformOriginPoint(this->boundingRect().center());

}

void WbCanvasItem::doErase(QPointF pos1, QPointF pos2,int width)
{
    QPainterPath path = createStrokePath(pos1, pos2, width);

#ifdef DRAW_VECTOR  //矢量线擦除
    scene()->setSelectionArea(path);

    QList<QGraphicsItem*> selItems = scene()->selectedItems();
    qDebug()<<__FUNCTION__<<selItems.size();
    QList<QGraphicsItem*>::iterator it = selItems.begin();
    while(it != selItems.end()){
        QGraphicsItem* item = *it++;

        if(item->type() == Type_LineVector){
            WbLineVector *lineroamer = dynamic_cast<WbLineVector *>(item);
            eraseVectorLineRoamer(lineroamer, pos1, pos2, width);
        }
    }

#else //非矢量线擦除
    m_pRealPainter->setRenderHint(QPainter::Antialiasing, true);
    m_pRealPainter->setCompositionMode(QPainter::CompositionMode_Source);
    m_pRealPainter->setPen(QPen(Qt::white,width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    m_pRealPainter->fillPath(path,Qt::white);//填充轮廓
    update();
#endif

}

void WbCanvasItem::eraseVectorLineRoamer(WbLineVector *l, const QPointF &p1, const QPointF &p2, int width)
{
    if(!l->vectorPath().isEmpty()){
        l->doEraseLine(p1, p2, width);
        if(l->vectorPath().isEmpty()){
            l->deleteLater();
        }
    }else{
        //路径为空时，需要把lineroamer删除
        l->deleteLater();
    }
}

QPainterPath WbCanvasItem::createStrokePath(const QPointF &p1, const QPointF &p2, int width)
{
    QPainterPath path;
    path.moveTo(p1);
    path.lineTo(p2);
    if(path.isEmpty()){
        path.addRegion(QRegion(QRect(p1.x()-0.5, p1.y()-0.5, 1, 1),
                                    QRegion::Ellipse));
    }
    QPainterPathStroker stroker;
    stroker.setWidth(width);
    stroker.setCapStyle(Qt::RoundCap);
    stroker.setJoinStyle(Qt::RoundJoin);
    stroker.createStroke(path);
    return stroker.createStroke(path);
}
