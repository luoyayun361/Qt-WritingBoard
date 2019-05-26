#ifndef WBLINEVECTOR_H
#define WBLINEVECTOR_H

#include <QGraphicsObject>

class WbLineVector : public QGraphicsObject
{
public:
    explicit WbLineVector(QGraphicsObject * parent = 0);
    ~WbLineVector();
    void setPath(const QPainterPath & path);
    QPainterPath vectorPath(){return m_path;}
    void doEraseLine(const QPointF &p1, const QPointF &p2, int width);
    int type()const;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) Q_DECL_OVERRIDE;
    QRectF boundingRect() const;
private:
    void doErasePath(const QPainterPath &path, bool selfMap = false);
    QPainterPath CreateStrokePath(const QPointF &p1, const QPointF &p2, int width);
private:
    QPainterPath m_path;
    QList<QPainterPath> m_erasePathList;
    int m_currentEraseIndex = 0;
    int m_ignoreCount;
//    QPainterPath m_vertorPath;
};

#endif // WBLINEVECTOR_H
