#ifndef CLINEOBJ_H
#define CLINEOBJ_H

#include <QPainterPath>
#include <QObject>

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
    void createNewRealPath()
    {
        m_realPath &= QPainterPath();
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
        m_realPath.addPath(m_path);
    }
    QPainterPath StrokePath(int width)
    {
        QPainterPathStroker stroker;
        stroker.setWidth(width);
        stroker.setCapStyle(Qt::RoundCap);
        stroker.setJoinStyle(Qt::RoundJoin);
        return  stroker.createStroke(m_path);
    }
    QPainterPath StrokeRealPath(int width)
    {
        QPainterPathStroker stroker;
        stroker.setWidth(width);
        stroker.setCapStyle(Qt::RoundCap);
        stroker.setJoinStyle(Qt::RoundJoin);
        return  stroker.createStroke(m_realPath);
    }
    QRectF updateRect(){
        return m_updateRect;
    }


private:
    QPainterPath m_path;
    QPainterPath m_realPath;
    QRectF m_updateRect;
};

#endif // CLINEOBJ_H
