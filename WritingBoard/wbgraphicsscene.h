#ifndef WBGRAPHICSSCENE_H
#define WBGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QTouchEvent>
#include "wbcanvasitem.h"


class WbGraphicsScene : public QGraphicsScene
{
public:
    explicit WbGraphicsScene(const QSizeF &size, QObject *parent = 0);
    ~WbGraphicsScene();

protected:
    bool event(QEvent *e);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event); 
    void keyPressEvent(QKeyEvent *keyEvent);

private:
    bool touchEvent(QTouchEvent *e);
    bool scenePress(int id, const QPointF &pos);
    bool sceneMove(int id, const QPointF &prepos, const QPointF &pos);
    bool sceneRelease(int id, const QPointF &pos);

private:
    WbCanvasItem * m_pCanvasItem = nullptr;

};

#endif // WBGRAPHICSSCENE_H
