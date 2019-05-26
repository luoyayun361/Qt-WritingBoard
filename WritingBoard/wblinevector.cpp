#include "wblinevector.h"
#include <QPainter>
#include <QDebug>
#include "wbcommondef.h"

WbLineVector::WbLineVector(QGraphicsObject *parent):
    QGraphicsObject(parent)
{
    setCacheMode(ItemCoordinateCache);
    setFlag(QGraphicsItem::ItemIsSelectable);
}

WbLineVector::~WbLineVector()
{
    qDebug() << "--->Lynn<---" << __FUNCTION__ ;
}

void WbLineVector::setPath(const QPainterPath &path)
{
    m_path = path;
    m_erasePathList << m_path;
    update();
}

void WbLineVector::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setCompositionMode(QPainter::CompositionMode_Source);
    painter->setPen(QPen(Qt::red,2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->setBrush(Qt::red);
//    painter->drawPath(m_path);
    painter->fillPath(m_path,Qt::red);
}

QRectF WbLineVector::boundingRect() const
{
    return m_path.boundingRect();
}

QPainterPath WbLineVector::CreateStrokePath(const QPointF &p1, const QPointF &p2, int width)
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

void WbLineVector::doEraseLine(const QPointF &p1, const QPointF &p2, int width)
{
    QPainterPath path = CreateStrokePath(p1, p2, width);
    doErasePath(path);
}

int WbLineVector::type() const
{
    return Type_LineVector;
}

void WbLineVector::doErasePath(const QPainterPath &path, bool selfMap)
{
    prepareGeometryChange();//改变boundingrect之前必须调用
    QPainterPath epath;
    Q_ASSERT(!path.isEmpty());
    if(selfMap){
        epath = path;//自身坐标系不用转换
    }else{
        epath = mapFromScene(path);//转换成内部坐标
    }
    if(m_currentEraseIndex < m_erasePathList.size()){
        //保留之前数据,删除后边的
        m_currentEraseIndex = (m_currentEraseIndex <= 1 ? 2 : m_currentEraseIndex);
        int count = m_erasePathList.count();
        for(int i = count; i >= m_currentEraseIndex; i--){
            m_erasePathList.removeLast();
        }
        Q_ASSERT(m_erasePathList.size()>=1);
    }
    //以下这句很耗时，导致擦除卡顿
    m_path -= epath;

    m_ignoreCount--;
    if(m_ignoreCount<=0){
        m_erasePathList<<m_path;
        m_currentEraseIndex = m_erasePathList.size();
        m_ignoreCount = 2;
    }
    update();
}
