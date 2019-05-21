#ifndef WBLINEVECTOR_H
#define WBLINEVECTOR_H

#include <QGraphicsItem>

class WbLineVector : public QGraphicsItem
{
public:
    explicit WbLineVector(QGraphicsItem * parent = 0);
    ~WbLineVector();
    void setPath(const QPainterPath & path);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) Q_DECL_OVERRIDE;
    QRectF boundingRect() const;
private:
    QPainterPath m_path;
};

#endif // WBLINEVECTOR_H
