#include "wbcanvasitem.h"
#include <QDebug>
#include <QPainter>
#include <wblinevector.h>
#include <QGraphicsScene>

#define DRAW_VECTOR  //矢量绘图

#define SAFE_DELETE(x) if(x) \
    { \
        delete (x); \
        (x) = nullptr; \
    } \

WbCanvasItem::WbCanvasItem(const QSizeF & size,QGraphicsObject * parent):
    QGraphicsObject (parent),
    m_size(size)
{
    initCanvas();
}

WbCanvasItem::~WbCanvasItem()
{
    qDebug() << "--->Lynn<---" << __FUNCTION__ << "before";
    SAFE_DELETE(m_pRealPainter);
    SAFE_DELETE(m_pRealCanvas);
    SAFE_DELETE(m_pTempPainter);
    SAFE_DELETE(m_pTempCanvas);
    qDebug() << "--->Lynn<---" << __FUNCTION__ << "after";
}

void WbCanvasItem::drawPress(int id, const QPointF &p)
{
//    qDebug() << "--->>>Lynn<<<---" << __FUNCTION__ << p;
    CLineObj * obj = new CLineObj(p);
    obj->addToPath(p,p);
    m_lineObjs.insert(id,obj);
}

void WbCanvasItem::drawMove(int id, const QPointF &lastPoint, const QPointF &curPoint)
{
//    qDebug() << "--->>>Lynn<<<---" << __FUNCTION__ << lastPoint<<curPoint;
    CLineObj * obj = m_lineObjs.value(id,nullptr);
    if(!obj) return;
    obj->addToPath(lastPoint,curPoint);
#ifndef DRAW_VECTOR
    if(obj->elementCount() < 300){
        drawToTemp(obj);       //绘制临时层
    }
    else{
        drawToReal(obj);       //绘制真实层 非矢量线
        obj->createNewPath();   //清空画线
    }
#else
    //这里会不断的创建path 内存会一直增长
    drawToRealByVector(obj);       //绘制真实层，矢量线
    obj->createNewPath();   //清空画线
#endif
}

void WbCanvasItem::drawRelease(int id, const QPointF &point)
{
//    qDebug() << "--->>>Lynn<<<---" << __FUNCTION__ << point;
    CLineObj * obj = m_lineObjs.value(id,nullptr);
    if(!obj) return;
    obj->addToPath(point,point);
//    drawToRealByVector(obj);  //绘制真实层，矢量线
#ifndef DRAW_VECTOR
    drawToReal(obj);  //绘制真实层 非矢量线
#endif
    m_lineObjs.remove(id);
    delete obj;
    obj = nullptr;
    if(m_lineObjs.size() == 0){
        m_pTempCanvas->fill(Qt::transparent);
    }
}

void WbCanvasItem::setBackgroundColor(const QColor &color)
{
    m_bgColor = color;
    m_pRealCanvas->fill(color);
}

QRectF WbCanvasItem::boundingRect() const
{
    return  QRectF(0,0,m_size.width(),m_size.height());
}

void WbCanvasItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
#ifndef DRAW_VECTOR
    painter->drawImage(0,0,*m_pRealCanvas);
    painter->drawImage(0,0,*m_pTempCanvas);
#endif
}

void WbCanvasItem::resize(const QSizeF &size)
{
    m_size = size;
}

void WbCanvasItem::drawToTemp(CLineObj *obj)
{
    qDebug() << "--->Lynn<---" << __FUNCTION__;
    m_pTempPainter->setRenderHint(QPainter::Antialiasing, true);
    m_pTempPainter->setCompositionMode(QPainter::CompositionMode_Source);
    m_pTempPainter->setPen(QPen(Qt::red,5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    m_pTempPainter->fillPath(obj->StrokePath(5),Qt::red);
    this->update(obj->updateRect());
}

void WbCanvasItem::drawToReal(CLineObj *obj)
{
    m_pRealPainter->setRenderHint(QPainter::Antialiasing, true);
    m_pRealPainter->setCompositionMode(QPainter::CompositionMode_Source);
    m_pRealPainter->setPen(QPen(Qt::red,5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    QPainterPath path = obj->StrokePath(5);
    m_pRealPainter->fillPath(path,Qt::red);//填充轮廓
    //清空临时层
    m_pTempCanvas->fill(Qt::transparent);
    this->update(path.boundingRect());
}

void WbCanvasItem::drawToRealByVector(CLineObj *obj)
{
    //内存会一直增长
    QPainterPath path = obj->StrokePath(5);
    WbLineVector * item = new WbLineVector(this);
    item->setPath(path);

    //清空临时层
    m_pTempCanvas->fill(Qt::transparent);
    this->update(path.boundingRect());
}

void WbCanvasItem::initCanvas()
{
    m_pRealCanvas = new QImage(m_size.toSize(),QImage::Format_ARGB32_Premultiplied);
    m_pTempCanvas = new QImage(*m_pRealCanvas);
    m_pTempCanvas->fill(Qt::transparent);

    m_pRealPainter = new QPainter(m_pRealCanvas);
    m_pTempPainter = new QPainter(m_pTempCanvas);

}
