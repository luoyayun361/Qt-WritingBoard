#include "wblinevector.h"
#include <QPainter>
#include <QDebug>

WbLineVector::WbLineVector(QGraphicsItem *parent):
    QGraphicsItem(parent)
{

}

WbLineVector::~WbLineVector()
{
    qDebug() << "--->Lynn<---" << __FUNCTION__ ;
}

void WbLineVector::setPath(const QPainterPath &path)
{
    m_path = path;
    update();
}

void WbLineVector::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setCompositionMode(QPainter::CompositionMode_Source);
    painter->setPen(QPen(Qt::red,2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->setBrush(Qt::red);
    painter->drawPath(m_path);
}

QRectF WbLineVector::boundingRect() const
{
    return m_path.boundingRect();
}
