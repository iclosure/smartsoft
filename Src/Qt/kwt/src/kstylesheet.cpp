#include "precomp.h"
#include "kstylesheet.h"

KStyleSheet::KStyleSheet(QObject *parent) :
    QObject(parent)
{
}

void KStyleSheet::setButtonStyle(QWidget* widget, const QString &image, int cutSec, int width, int height)
{
    Q_ASSERT(widget);
    QPixmap pixmap(image);
    int singleWidth = pixmap.width() / cutSec;
    widget->setStyleSheet(QString("QPushButton{ width: %5px; height: %6px; border-image: url(%1) 0 %4 0 0 stretch stretch;}")
                          .append("QPushButton::hover{border-image: url(%1) 0 %3 0 %2  stretch stretch;}")
                          .append("QPushButton::pressed{border-image: url(%1) 0 %2 0 %3 stretch stretch;}")
                          .append("QPushButton::checked{border-image: url(%1) 0 %2 0 %3 stretch stretch;}")
                          .append("QPushButton::disabled{border-image: url(%1) 0 0 0 %4 stretch stretch;}")
                          .arg(image)
                          .arg(singleWidth)
                          .arg(singleWidth * 2)
                          .arg(singleWidth * 3)
                          .arg(width == -1 ? singleWidth : width)
                          .arg(height == -1 ? pixmap.height() : height));
}
