#include "precomp.h"
#include "jflowchart_p.h"
#include <QGraphicsView>
#include "jview.h"
#include "jscene.h"
#include "jshape.h"
#include "jlinker.h"

JFlowChartPrivate::JFlowChartPrivate(QWidget *parent) :
    JView(parent)
{
    JScene *scene = new JScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(scene);

    //
    JShape *shape1 = new JShape(this);
    JShape *shape2 = new JShape(this);
    //JShape *shape3 = new JShape(this);
    shape1->setArrowType(Qt::RightArrow);
    shape2->setSize(QSize(200, 80));

    scene->addItem(shape1);
    scene->addItem(shape2);
    //scene->addItem(shape3);
    scene->addItem(new JLinker(shape1, shape2));
    //scene->addItem(new JLinker(shape2, shape3));
    //scene->addItem(new JLinker(shape1, shape3));

    //
    shape1->setPos(0, 0);
    shape2->setPos(100, 100);
    //shape3->setPos(200, 200);
}
