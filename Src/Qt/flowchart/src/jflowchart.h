#ifndef JFLOWCHART_H
#define JFLOWCHART_H

/////////////////////////////////////////////////////////
/// version

#define JFLOWCHART_VERSION       0x010000
#define JFLOWCHART_VERSION_STR   "1.0.0"

/////////////////////////////////////////////////////////
/// JFlowChartListener interface

class JFlowChartListener
{
public:
    virtual bool onLampClicked(const std::string &shapeId, const std::string &lampId) = 0;
    virtual bool onShapeClicked(const std::string &shapeId) = 0;
};

/////////////////////////////////////////////////////////
/// import and export

#ifdef WIN32
#ifdef JFLOWCHART_DLL

#ifdef JFLOWCHART_MAKEDLL
#define JFLOWCHART_EXPORT  Q_DECL_EXPORT
#else
#define JFLOWCHART_EXPORT  Q_DECL_IMPORT
#endif

#endif // JFLOWCHART_DLL
#endif // WIN32

#ifndef JFLOWCHART_EXPORT
#define JFLOWCHART_EXPORT
#endif

#include <string>

/////////////////////////////////////////////////////////
/// global JFlowChartCore

class JFlowChartCorePrivate;
class JFLOWCHART_EXPORT JFlowChartCore
{
public:
    JFlowChartCore();
    ~JFlowChartCore();

    bool load(const std::string & systemName = std::string());

private:
    JFlowChartCorePrivate* d;
};

/////////////////////////////////////////////////////////
/// class JFlowChart

class JFlowChartPrivate;

class JFLOWCHART_EXPORT JFlowChart
{
public:
    JFlowChart();
    ~JFlowChart();

    void addListener(JFlowChartListener *listener);
    void removeListener(JFlowChartListener *&listener);

    long winId() const;
    bool setParent(long winId);

private:
    JFlowChartPrivate *d;
};

#endif // JFLOWCHART_H
