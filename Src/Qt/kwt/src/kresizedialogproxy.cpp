#include "precomp.h"
#include "kresizedialogproxy.h"
//#include <QtWinExtras/QtWin>
#include "kstylesheet.h"

KResizeDialogProxy::KResizeDialogProxy(QWidget *parent) :
    QDialog(parent)
{
    setObjectName("KResizeDialogProxy");

#if (QT_VERSION >= 0x050000) && (defined(Q_OS_WIN) || defined(Q_OS_CYGWIN))

    if (QSysInfo::windowsVersion() >= QSysInfo::WV_6_0) {
        QtWin::extendFrameIntoClientArea(this, -1, -1, -1, -1);
        setAttribute(Qt::WA_TranslucentBackground, true);
        setAttribute(Qt::WA_NoSystemBackground, false);
        setStyleSheet("QDialog#KResizeDialogProxy { background: transparent; }");
    }

#endif  //

    QSpinBox* spinBox = new QSpinBox(this);
    QSlider* slider = new QSlider(Qt::Horizontal, this);

    spinBox->setRange(80, qMax<int>(parent->width(), 500));
    spinBox->setSuffix(" px");
    slider->setRange(spinBox->minimum(), spinBox->maximum());
    slider->setMinimumWidth(120);

    QDialogButtonBox* dlgButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                                          Qt::Horizontal, this);
    dlgButtonBox->button(QDialogButtonBox::Ok)->setStyleSheet("QPushButton{width: 80; height: 25; color: white;}");
    dlgButtonBox->button(QDialogButtonBox::Cancel)->setStyleSheet("QPushButton{width: 80; height: 25; color: white;}");
    dlgButtonBox->setFixedWidth(dlgButtonBox->sizeHint().width());

    QHBoxLayout* horiLayoutMain = new QHBoxLayout(this);
    horiLayoutMain->addWidget(spinBox);
    horiLayoutMain->addWidget(slider);
    horiLayoutMain->addWidget(dlgButtonBox);

    /////////////////////////////////////////////////////////////

    connect(spinBox, SIGNAL(valueChanged(int)), slider, SLOT(setValue(int)));
    connect(slider, SIGNAL(valueChanged(int)), spinBox, SLOT(setValue(int)));
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));
    q_oldWidth = parent->width();
    connect(dlgButtonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(accept()));
    connect(dlgButtonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), this, SLOT(slotButtonCancelClicked()));

    /////////////////////////////////////////////////////////////

    slider->setValue(q_oldWidth);
}

// slot functions

// value changed
void KResizeDialogProxy::slotValueChanged(int value)
{
    if (parentWidget()->width() != value) {
        parentWidget()->setFixedWidth(value);
    }
}

// buttonCancel clicked
void KResizeDialogProxy::slotButtonCancelClicked()
{
    parentWidget()->setFixedWidth(q_oldWidth);
    this->reject();
}
