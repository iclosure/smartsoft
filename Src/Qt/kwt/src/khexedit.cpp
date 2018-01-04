#include "precomp.h"
#include "khexedit.h"

KHexEdit::KHexEdit(QWidget *parent, int count, quint64 mask) :
    QLineEdit(parent),
    q_count(count),
    q_mask(mask)
{
    setObjectName("KHexEdit");

    q_regExpValidator = new QRegExpValidator(QRegExp(QString("[A-Fa-f0-9]{%1}").arg(count)));
    setValidator(q_regExpValidator);
    setPlaceholderText(QString("(0~%1)").arg(mask, count , 16).toUpper());
    setToolTip(tr("Input a hex data %1.").arg(placeholderText()));

    /////////////////////////////////////////////////////////////////////

    connect(this, SIGNAL(textChanged(QString)), this, SLOT(slotTextChanged(QString)));

    /////////////////////////////////////////////////////////////////////
}

void KHexEdit::setCount(int count)
{
    Q_ASSERT(count > 0);
    q_count = count;
    q_regExpValidator->setRegExp(QRegExp(QString("[A-Fa-f0-9]{%1}").arg(count)));
}

// slot functions

// text changed
void KHexEdit::slotTextChanged(const QString &text)
{
    if (!text.isEmpty()) {
        qulonglong value = text.toULongLong(0, 16);
        if (value > q_mask) {
            setText(QString("%1").arg(q_mask, text.count(), 16).toUpper());
        } else {
            setText(text.toUpper());
        }
    }
}
