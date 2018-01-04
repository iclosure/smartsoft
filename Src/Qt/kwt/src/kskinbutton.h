#ifndef KSKINBUTTON_H
#define KSKINBUTTON_H

#include "kwidget.h"

class KWT_EXPORT KSkinButton : public KWidget
{
    Q_OBJECT
public:
    explicit KSkinButton(const QString &name, QPixmap &pixmap, QWidget *parent = 0);

Q_SIGNALS:
    void clicked(bool checked = false);

private Q_SLOTS:
    void slotThisClicked(bool checked);

protected:
    void paintEvent(QPaintEvent *event);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    
private:
    bool            q_mouseEnter;
    int             q_downloadCount;    //
};

#endif // KSKINBUTTON_H
