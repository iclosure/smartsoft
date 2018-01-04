#ifndef KDIALOG_H
#define KDIALOG_H

#include <QDialog>
#include "kwt_global.h"
#include "ktitle.h"

//
typedef QPair<QString/*image*/, QString/*name*/> KSkinType;
class KSkinListType : public QObject
{
    Q_OBJECT
public:
    bool isEmpty() const { return _lst.isEmpty(); }
    void append(const KSkinType& type) { _lst.append(type); }
    void append(const KSkinListType& skins) { _lst.append(skins._lst); }
    int count() const { return _lst.count(); }
    KSkinType& last() { return _lst.last(); }
    KSkinType& operator [](int index) { return _lst[index]; }

private:
    QList<KSkinType> _lst;
};

//
// KDialog
//

class KShake;
class KStatusBar;
class QVBoxLayout;

class KWT_EXPORT KDialog : public QDialog
{
    Q_OBJECT
public:
    explicit KDialog(QString &fileName, QMargins &shadowMargins, int titleHeight, KTitle::TitleTypes titleType,
                     KTitle::TitleButtons titleButtons, QWidget *parent = 0);

    void setCenter(QWidget *parent = 0);

    QLayout *layout() const;
    void setLayout(QLayout *layout);
    void setAttribute(Qt::WidgetAttribute, bool = true);
    inline bool testAttribute(Qt::WidgetAttribute) const;
    void setWindowIcon(const QIcon &icon);
    void setWindowTitle(const QString &text);
    void setTitleAlignment(Qt::Alignment alignment);
    KTitle* titleWidget() const { return q_titleWidget; }
    int titleHeight() const { return q_titleHeight; }
    void setTitleHeight(int height) { q_titleHeight = height; }
    QWidget* clientWidget() const { return q_clientWidget; }
    void setClientWidget(QWidget* widget);
    KStatusBar* statusWidget() const { return q_statusBar; }
    QRect clientRect() const;

    void transparentClientWidget(bool transparent);
    void setStatusWidget(bool enable = true);

    void addSkins(const KSkinType & skin);
    void addSkins(const KSkinListType & skins);

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

Q_SIGNALS:
    void skinChanged(const QString & name = QString());

public Q_SLOTS:
    void showMaximized();
    void showFullScreen();
    void showNormal();
    void showChanged();
    void setSkin(const QString & name = QString());

private Q_SLOTS:
    //void slotShowChanged();

private:
    enum RegionType {
        TopLeft,
        TopRight,
        Left,
        Right,
        BottomLeft,
        Bottom,
        BottomRight,
        Unknown
    };

    void makeRegions();
    void updateGeometry(int x, int y, int w, int h);
    RegionType regionHit(const QPoint &pos);

private:
    int             q_titleHeight;      //
    QMargins        q_shadowMargins;    //
    int             q_borderWidth;      //
    QMargins        q_tmpShadowMargins; //
    int             q_tmpBorderWidth;   //
    QString         q_skinName;         //
    KShake*         q_shake;            //
    bool            q_mousePressed;     //
    bool            q_isMousePress;     //
    RegionType      q_rgnPressed;
    QPoint          q_orgPos;
    QRect           q_orgGeom;
    QRect           q_regions[7];

    QVBoxLayout*    q_mainLayout;       //
    KTitle*         q_titleWidget;      //
    QWidget*        q_clientWidget;     //
    KStatusBar*     q_statusBar;        //
    QPixmap         q_pmTopLeft;        //
    QPixmap         q_pmTop;            //
    QPixmap         q_pmTopRight;       //
    QPixmap         q_pmLeft;           //
    QPixmap         q_pmTitle;          //
    QPixmap         q_pmClient;         //
    QPixmap         q_pmRight;          //
    QPixmap         q_pmBottomLeft;     //
    QPixmap         q_pmBottom;         //
    QPixmap         q_pmBottomRight;    //
    QPixmap         q_pmBackground;     //

    friend class KTitle;
    friend class KToolTitle;
};

//
// KSkinDialog
//

class KSkinDialog : public KDialog
{
    Q_OBJECT
private:
    explicit KSkinDialog(QWidget *parent);

private:
    Q_DISABLE_COPY(KSkinDialog)
    friend class KTitle;
};

#endif // KDIALOG_H
