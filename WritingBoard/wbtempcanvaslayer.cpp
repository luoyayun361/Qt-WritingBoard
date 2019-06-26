#include "wbtempcanvaslayer.h"
#include "wbcommondef.h"
#include <QDebug>

WbTempCanvasLayer::WbTempCanvasLayer(const QSizeF &size, QGraphicsObject *parent):
    QGraphicsObject (parent),
    m_size(size)
{
    setCacheMode(DeviceCoordinateCache);
    initCanvas();
}

WbTempCanvasLayer::~WbTempCanvasLayer()
{
    SAFE_DELETE(m_pTempPainter);
    SAFE_DELETE(m_pTempCanvas);
    qDebug() << "--->>>Lynn<<<---" << __FUNCTION__;
}

void WbTempCanvasLayer::clear()
{
    m_pTempCanvas->fill(Qt::transparent);
    this->update();
}

QRectF WbTempCanvasLayer::boundingRect() const
{
    return  QRectF(0,0,m_size.width(),m_size.height());
}

void WbTempCanvasLayer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    qDebug() << "--->Lynn<---" << __FUNCTION__;
    painter->drawImage(0,0,*m_pTempCanvas);
}

void WbTempCanvasLayer::drawToTemp(CLineObj *obj)
{
    m_pTempPainter->setRenderHint(QPainter::Antialiasing, true);
    m_pTempPainter->setCompositionMode(QPainter::CompositionMode_Source);
    m_pTempPainter->setPen(QPen(Qt::red,5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    m_pTempPainter->fillPath(obj->StrokePath(5),Qt::red);
    this->update(obj->updateRect());
}

void WbTempCanvasLayer::initCanvas()
{
    m_pTempCanvas = new QImage(m_size.toSize(),QImage::Format_ARGB32_Premultiplied);
    m_pTempCanvas->fill(Qt::transparent);
    m_pTempPainter = new QPainter(m_pTempCanvas);
}
