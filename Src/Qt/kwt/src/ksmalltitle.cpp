#include "ksmalltitle.h"
#include <QtWidgets>
#include "kpushbutton.h"

KSmallTitle::KSmallTitle(QWidget *parent) :
    KTitle(parent)
{
    setFixedHeight(30);
    //
    q_titleIcon = new QLabel(this);
    q_titleIcon->setFixedSize(height() / 2, height() / 2);
    q_titleIcon->setScaledContents(true);

    q_titleLabel = new QLabel(this);
    q_titleLabel->setStyleSheet("color:white;");
    q_titleLabel->setContentsMargins(5, 0, 2, 0);

    q_closeButton = new KPushButton(this);
    q_closeButton->setPixmap(":/images/title_button/full_close");
    connect(q_closeButton, &KPushButton::clicked, [=](){ parent->close(); });

    QHBoxLayout* titleLayout = new QHBoxLayout();
    titleLayout->setSpacing(0);
    titleLayout->setContentsMargins(0, 0, 0, 0);
    titleLayout->addWidget(q_titleIcon, 0, Qt::AlignVCenter);
    titleLayout->addWidget(q_titleLabel, 0, Qt::AlignVCenter);
    titleLayout->addStretch();
    titleLayout->addWidget(q_closeButton, 0, Qt::AlignTop);

    setLayout(titleLayout);

    retranslateLang();
}

void KSmallTitle::retranslateLang()
{
    q_titleLabel->setText(parentWidget()->windowTitle());
    q_closeButton->setToolTip(tr("Cl&ose"));
}

void KSmallTitle::setTitleIcon(const QIcon &icon)
{
    q_titleIcon->setPixmap(icon.pixmap(q_titleIcon->size()));
}
