#ifndef KTITLE_H
#define KTITLE_H

#include "kwidget.h"
#include <QPushButton>
#include <QLabel>

class QHBoxLayout;
class QVBoxLayout;
class QMenu;
class QToolButton;
class KPushButton;

class KIconButton : public QPushButton
{
    Q_OBJECT
public:
    explicit KIconButton(QWidget *parent = 0);

    void setPixmap(const QString &name);
    void setPixmap(QPixmap &pixmap);

protected:
    void paintEvent(QPaintEvent *);

private:
    QPixmap q_pixmap;
};

class KTitleLabel : public QLabel
{
    Q_OBJECT
public:
    explicit KTitleLabel(QWidget *parent=0, Qt::WindowFlags f=0);
    explicit KTitleLabel(const QString &text, QWidget *parent=0, Qt::WindowFlags f=0);

protected:
    void enterEvent(QEvent *event);
};

//
// KTitle
//

class KWT_EXPORT KTitle : public KWidget
{
    Q_OBJECT
public:
    enum TitleType {
        NoTitle = 0,
        Normal,
        Tool,
        Pane,
        Ribbon,
        Custom,
        TitleTypeCount = 6
    };
    Q_DECLARE_FLAGS(TitleTypes, TitleType)

    // title buttons enumeration
    enum TitleButton {
        NoButton = 0,
        MainMenu = 0x01,
        SkinButton = 0x02,
        WhatsThis = 0x04,
        Minimized = 0x08,
        Maximized = 0x10,
        Close = 0x20,

        TitleButtonCount = 6,
        TitleButtonMask = 0x3F,

        Default = Close,
        DialogNoWhatsThis = Minimized | Close,
        Dialog = WhatsThis | Minimized | Close,
        WinMainNoWhatsThis = MainMenu | Minimized | Maximized | Close,
        WinMain = MainMenu | WhatsThis | Minimized | Maximized | Close
    };
    Q_DECLARE_FLAGS(TitleButtons, TitleButton)

    explicit KTitle(int height, TitleButtons titleButtons = 0, QWidget *parent = 0);

public:
    // for any title
    void setTitleIcon(const QIcon &icon) const;
    void setTitleFont(const QFont &) const;
    void setTitleAlignment(Qt::Alignment alignment);
    void setWindowTitle(const QString &text);
    void setMainMenu(QMenu *);
    void setEnabled(TitleButtons titleButtons, bool enable) const;
    void setVisible(TitleButtons titleButtons, bool visible) const;
    void addTitleWidget(QWidget *widget, const QString &objectName = QString());
    void addTitleSpacing(int size);
    void removeTitleWidget(QWidget *widget);

    // for tool title
    virtual void addToolButton(QPixmap &, const QString & = QString()) { Q_ASSERT(false); }
    virtual void setToolButtonText(int, const QString &) { Q_ASSERT(false); }
    virtual void removeToolButton(int) { Q_ASSERT(false); }
    virtual int toolButtonSize() const { Q_ASSERT(false); return 0; }
    virtual QToolButton* toolButton(int) const { Q_ASSERT(false); return 0; }
    virtual QList<QToolButton*> toolButtons() const { Q_ASSERT(false); return QList<QToolButton*>(); }

    // for pane title
    virtual void addPaneWidget(QWidget *, Qt::Alignment = Qt::AlignLeft) { Q_ASSERT(false); }
    virtual void removePaneWidget(QWidget *) { Q_ASSERT(false); }
    virtual void addPaneLayout(QLayout *, Qt::Alignment = Qt::AlignLeft) { Q_ASSERT(false); }
    virtual void removePaneLayout(QLayout *) { Q_ASSERT(false); }

    // for ribbon title
    // for custom title

Q_SIGNALS:
    void showClicked();
    void showChanged();
    void selectionChanged(int id);

private Q_SLOTS:
    void slotShowSystemMenu(bool checked);
    void slotShowSkinsDialog();
    void slotShowClicked();
    void slotShowChanged();
    void slotShowMainMenu();

protected:
    void addLayout(QLayout *layout);
    void addWidget(QWidget *widget);

protected:
    void resizeEvent(QResizeEvent *);
    void leaveEvent(QEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *event);

private:
    enum RegionType {
        WinIcon,
        TopLeft,
        Top,
        TopRight,
        Inner,
        Unknown,
        RegionTypeCount = 5
    };
    Q_DECLARE_FLAGS(RegionTypes, RegionType)

    void setRegions();
    void updateGeometry(int x, int y, int w, int h);
    RegionType regionHit(const QPoint &pos);

private:
    Q_DISABLE_COPY(KTitle)

    bool                q_mousePressed;
    RegionType          q_rgnPressed;
    QPoint              q_orgPos;
    QRect               q_orgGeom;
    QRect               q_regions[RegionTypeCount];

    QHBoxLayout*        q_horiLayoutTitle;  //
    QVBoxLayout*        q_vertLayoutMain;   //
    TitleButtons        q_titleButtons;     //
    KIconButton*        q_buttonIcon;       //
    KTitleLabel*        q_labelTitle;       // Button: Window Title
    QMenu*              q_menuMain;         // Menu  : Main menu
    KPushButton*        q_buttonMainMenu;   // Button: Main menu
    KPushButton*        q_buttonSkins;      // Button: Change skin
    KPushButton*        q_buttonWhatsThis;  // Button: WhatsThis
    KPushButton*        q_buttonMinimized;  // Button: Minimized
    KPushButton*        q_buttonMaximized;  // Button: Maximized
    KPushButton*        q_buttonClose;      // Button: Close

    //
    friend class KDialog;
    friend class KSystemMenu;
};

#endif // KTITLE_H
