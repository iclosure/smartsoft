#ifndef KTOOLTITLE_H
#define KTOOLTITLE_H

#include "ktitle.h"

class QLabel;
class QMenu;
class QHBoxLayout;
class QVBoxLayout;
class QSignalMapper;
class KToolButton;
class KPushButton;

class KWT_EXPORT KToolTitle : public KTitle
{
    Q_OBJECT
public:
    explicit KToolTitle(int height, TitleButtons titleButtons = 0, QWidget *parent = 0);

    void addToolButton(QPixmap &pixmap, const QString &text = QString()) Q_DECL_FINAL;
    void setToolButtonText(int id, const QString &text) Q_DECL_FINAL;
    void removeToolButton(int id) Q_DECL_FINAL;
    int toolButtonSize() const { return q_toolButtons.size(); }
    QToolButton* toolButton(int id) const { Q_ASSERT(id < q_toolButtons.size()); return q_toolButtons[id]; }
    QList<QToolButton*> toolButtons() const Q_DECL_FINAL { return q_toolButtons;}

private Q_SLOTS:
    void slotSelectionChanged(int id);

private:
    QList<QToolButton*> q_toolButtons;      // 工具栏按钮组
    QSignalMapper*      q_signalMapper;     // 工具栏按钮选择信号映射表
    QHBoxLayout*        q_horiLayoutTool;   // 工具栏布局器
};

#endif // KTOOLTITLE_H
