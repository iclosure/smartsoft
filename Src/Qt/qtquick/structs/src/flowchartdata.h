#ifndef FLOWCHARTDATA_H
#define FLOWCHARTDATA_H

#include <QDeclarativeItem>

typedef struct _SHAPE_DATA
{
    QString moduleId;
    int index;
    QString title;
    int number;

} SHAPE_DATA, *PSHAPE_DATA;

class FlowChartData : public QDeclarativeItem
{
    Q_OBJECT
    Q_DISABLE_COPY(FlowChartData)
    Q_PROPERTY(QString moduleId READ moduleId WRITE setModuleId NOTIFY moduleIdChanged)
    Q_PROPERTY(int index READ index WRITE setIndex NOTIFY indexChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(int number READ number WRITE setNumber NOTIFY numberChanged)
    Q_PROPERTY(QDeclarativeListProperty<FlowChartData> shapesData READ shapesData CONSTANT)

public:
    FlowChartData(QDeclarativeItem *parent = 0);
    ~FlowChartData();

    QString moduleId() const { return q_shapeData.moduleId; }
    void setModuleId(const QString& moduleId) { q_shapeData.moduleId = moduleId; Q_EMIT moduleIdChanged(moduleId); }

    int index() const { return q_shapeData.index; }
    void setIndex(int index) { q_shapeData.index = index; Q_EMIT indexChanged(index); }

    QString title() const { return q_shapeData.title; }
    void setTitle(const QString& title) { q_shapeData.title = title; Q_EMIT titleChanged(title); }

    int number() const { return q_shapeData.number; }
    void setNumber(int number) { q_shapeData.number = number; Q_EMIT numberChanged(number); }

    QDeclarativeListProperty<FlowChartData> shapesData()
    {
        return QDeclarativeListProperty<FlowChartData>();
    }

Q_SIGNALS:
    void moduleIdChanged(const QString& moduleId);
    void indexChanged(int index);
    void titleChanged(const QString& title);
    void numberChanged(int number);
    //void shapeDataChanged();

public Q_SLOTS:
    void slotTitleChanged(const QString& title)
    {
        int i = 0;
        i++;
    }

private:
    SHAPE_DATA q_shapeData;
};

QML_DECLARE_TYPE(FlowChartData)

#endif // FLOWCHARTDATA_H

