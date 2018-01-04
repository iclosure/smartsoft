#ifndef KRESIZEDIALOGPROXY_H
#define KRESIZEDIALOGPROXY_H

#include <QDialog>
#include "kwt_global.h"

class KWT_EXPORT KResizeDialogProxy : public QDialog
{
    Q_OBJECT
public:
    explicit KResizeDialogProxy(QWidget *parent = 0);

private Q_SLOTS:
    void slotValueChanged(int value);
    void slotButtonCancelClicked();

private:
    int q_oldWidth;
};

#endif // KRESIZEDIALOGPROXY_H
