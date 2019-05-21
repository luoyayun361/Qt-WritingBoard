#include <QApplication>
#include <QGraphicsView>
#include <QDesktopWidget>
#include "wbgraphicsscene.h"
#include <QScopedPointer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSize desktopSize = qApp->desktop()->screenGeometry().size();
    QScopedPointer<QGraphicsView> view(new QGraphicsView());
    QScopedPointer<WbGraphicsScene> pScene(new WbGraphicsScene(desktopSize));
    view->setScene(pScene.data());
    view->resize(desktopSize);
    view->show();

    return a.exec();
}
