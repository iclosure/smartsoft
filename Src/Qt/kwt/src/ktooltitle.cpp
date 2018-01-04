#include "precomp.h"
#include "ktooltitle.h"
#include "kpushbutton.h"
#include "ktoolbutton.h"

// KToolTitle

KToolTitle::KToolTitle(int height, TitleButtons titleButtons, QWidget *parent) :
    KTitle(height, titleButtons, parent)
{
    setObjectName("KToolTitle");
    setFixedHeight( KTitle::height() + 80);

    //
    q_signalMapper = new QSignalMapper(this);

    //
    q_horiLayoutTool = new QHBoxLayout();
    q_horiLayoutTool->setSpacing(8);
    q_horiLayoutTool->setContentsMargins(0, 0, 0, 0);

    QLabel *logoLabel = new QLabel();
    QPixmap pixmap(":/kwt/images/global/logo");
    logoLabel->setPixmap(pixmap);
    logoLabel->setFixedSize(pixmap.size());
    logoLabel->setCursor(Qt::PointingHandCursor);

    q_horiLayoutTool->addStretch();
    q_horiLayoutTool->addWidget(logoLabel);
    KTitle::addLayout(q_horiLayoutTool);

    ///////////////////////////////////////////////////////////////

    connect(this, SIGNAL(selectionChanged(int)), this, SLOT(slotSelectionChanged(int)));

    ///////////////////////////////////////////////////////////////
}

void KToolTitle::addToolButton(QPixmap &pixmap, const QString &text)
{
    if (q_toolButtons.isEmpty()) {
        connect(q_signalMapper, SIGNAL(mapped(int)), this, SIGNAL(selectionChanged(int)));  // TODO
    } else {
        QListIterator<QToolButton*> iter(q_toolButtons);
        while (iter.hasNext()) {
            if (iter.next()->text() == text) { // The same toolButton has been in existence
                return;
            }
        }
    }

    KToolButton *toolButton = new KToolButton(pixmap, text, this);
    q_toolButtons.append(toolButton);
    connect(toolButton, SIGNAL(clicked()), q_signalMapper, SLOT(map()));
    q_signalMapper->setMapping(toolButton, q_toolButtons.count() - 1);
    q_horiLayoutTool->insertWidget(q_toolButtons.count() - 1, toolButton, 0, Qt::AlignLeft | Qt::AlignBottom);
}

void KToolTitle::setToolButtonText(int id, const QString &text)
{
    if (0 <= id && id < q_toolButtons.count()) {
        q_toolButtons[id]->setText(text);
    }
}

void KToolTitle::removeToolButton(int id)
{
    if (0 <= id && id < q_toolButtons.count()) {
        q_signalMapper->removeMappings(q_toolButtons[id]);
        q_horiLayoutTool->removeWidget(q_toolButtons[id]);
        delete q_toolButtons[id];
        q_toolButtons.removeAt(id);

        // redistribe button's index
        for (int i = id; i < q_toolButtons.count(); i++) {
            q_signalMapper->setMapping(q_toolButtons[i], i);
        }
    }
}

// slot functions

// selection changed
void KToolTitle::slotSelectionChanged(int id)
{
    for(int i = 0; i < q_toolButtons.count(); i++) {
        reinterpret_cast<KToolButton*>(q_toolButtons[i])->select((id == i) ? true : false);
    }
}
