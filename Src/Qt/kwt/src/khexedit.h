#ifndef KHEXEDIT_H
#define KHEXEDIT_H

#include <QLineEdit>
#include "kwt_global.h"

class QRegExpValidator;

class KWT_EXPORT KHexEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit KHexEdit(QWidget *parent = 0, int count = 16, quint64 mask = ~0);

    void setCount(int count);
    int getCount() const { return q_count; }

private Q_SLOTS:
    void slotTextChanged(const QString & text);

private:
    QRegExpValidator*       q_regExpValidator;
    int                     q_count;
    quint64                 q_mask;
};

#endif // KHEXEDIT_H
