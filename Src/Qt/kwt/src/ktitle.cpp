#include "precomp.h"
#include "ktitle.h"
#include "kpushbutton.h"
#include "kskinbutton.h"
#include "ksystemmenu.h"
#include "kdialog.h"

//
// KIconButton
//

KIconButton::KIconButton(QWidget *parent) :
    QPushButton(parent)
{
    setObjectName("KIconButton");
    setAttribute(Qt::WA_TranslucentBackground);
}

void KIconButton::setPixmap(const QString &name)
{
    q_pixmap.load(name);
    //setFixedSize(parentWidget()->size());
}

void KIconButton::setPixmap(QPixmap &pixmap)
{
    q_pixmap = pixmap;
    //setFixedSize(parentWidget()->size());
}

void KIconButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), q_pixmap, q_pixmap.rect());
}

// KTitleLabel

KTitleLabel::KTitleLabel(QWidget *parent, Qt::WindowFlags f) :
    QLabel(parent, f)
{
    setObjectName("KTitleLabel");
}

KTitleLabel::KTitleLabel(const QString &text, QWidget *parent, Qt::WindowFlags f) :
    QLabel(text, parent, f)
{
    setObjectName("KTitleLabel");
}

void KTitleLabel::enterEvent(QEvent *event)
{
    setCursor(Qt::ArrowCursor);

    QLabel::enterEvent(event);
}

//
// KTitle
//

KTitle::KTitle(int height, TitleButtons titleButtons, QWidget *parent) :
    KWidget(parent),
    q_mousePressed ( false ),
    q_rgnPressed (Unknown),
    q_titleButtons(titleButtons),
    q_menuMain(Q_NULLPTR)
{
    Q_ASSERT(parent->inherits("KDialog"));

    setObjectName("KTitle");
    setAttribute(Qt::WA_TranslucentBackground);
    setMouseTracking(true);
    setFixedHeight(height);

    //
    setRegions();

    //
    q_buttonIcon = new KIconButton(this);
    q_buttonIcon->setFixedSize(height * 2 / 3, height * 2 / 3);
    q_buttonIcon->setCheckable(true);
    connect(q_buttonIcon, SIGNAL(toggled(bool)), this, SLOT(slotShowSystemMenu(bool)));

    // layout
    q_horiLayoutTitle = new QHBoxLayout();
    q_horiLayoutTitle->setSpacing(0);
    q_horiLayoutTitle->setContentsMargins(4, 0, 4, 0);
    q_horiLayoutTitle->addWidget(q_buttonIcon, 0, Qt::AlignVCenter);
    q_horiLayoutTitle->addStretch();

    // window title
    q_labelTitle = new KTitleLabel(parentWidget()->windowTitle(), this);
    q_labelTitle->setStyleSheet("QLabel { color:white; }");
    q_labelTitle->setContentsMargins(5, 0, 2, 0);
    q_horiLayoutTitle->addWidget(q_labelTitle, 0, Qt::AlignCenter);
    q_horiLayoutTitle->addStretch();

    // button: Skins
    if (titleButtons & SkinButton) {
        q_buttonSkins = new KPushButton(this);
        q_buttonSkins->setToolTip(tr("Change skin"));
        q_buttonSkins->setPixmap(":/kwt/images/title_button/skin");
        q_horiLayoutTitle->addWidget(q_buttonSkins, 0, Qt::AlignTop);
        qObjectSetProperity<KSkinListType*>(parent, new KSkinListType, "skinList");
        connect(q_buttonSkins, SIGNAL(clicked()), this, SLOT(slotShowSkinsDialog()));
    } else {
        q_buttonSkins = Q_NULLPTR;
    }

    // button: MainMenu
    if (titleButtons & MainMenu) {
        q_buttonMainMenu = new KPushButton(this);
        q_buttonMainMenu->setVisible(false);
        q_buttonMainMenu->setToolTip(tr("Main menu"));
        q_buttonMainMenu->setPixmap(":/kwt/images/title_button/title_bar_menu");
        q_horiLayoutTitle->addWidget(q_buttonMainMenu, 0, Qt::AlignTop);
    } else {
        q_buttonMainMenu = Q_NULLPTR;
    }

    // button: WhatsThis
    if (titleButtons & WhatsThis) {
        q_buttonWhatsThis = new KPushButton(this);
        q_buttonWhatsThis->setToolTip(tr("What is this"));
        q_buttonWhatsThis->setPixmap(":/kwt/images/title_button/arrow");  // TODO
        q_horiLayoutTitle->addWidget(q_buttonWhatsThis, 0, Qt::AlignTop);
        //connect(q_buttonWhatsThis, SIGNAL(clicked()), this, SLOT());
    } else {
        q_buttonWhatsThis = Q_NULLPTR;
    }

    // button: Minimized
    if (titleButtons & Minimized) {
        q_buttonMinimized = new KPushButton(this);
        q_buttonMinimized->setToolTip(tr("Minimize"));
        q_horiLayoutTitle->addWidget(q_buttonMinimized, 0, Qt::AlignTop);
        connect(q_buttonMinimized, SIGNAL(clicked()), parent, SLOT(showMinimized()));
        if (titleButtons & TitleButton(MainMenu)) {
            q_buttonMinimized->setPixmap(":/kwt/images/title_button/min");
        } else {
            q_buttonMinimized->setPixmap(":/kwt/images/title_button/min2");
        }
    } else {
        q_buttonMinimized = Q_NULLPTR;
    }

    // button: Maximized
    if (titleButtons & Maximized) {
        q_buttonMaximized = new KPushButton(this);
        q_buttonMaximized->setToolTip(tr("Maximize"));
        q_buttonMaximized->setCheckable(true);
        q_buttonMaximized->setPixmap(":/kwt/images/title_button/max");
        q_horiLayoutTitle->addWidget(q_buttonMaximized, 0, Qt::AlignTop);
        connect(q_buttonMaximized, SIGNAL(clicked()), this, SLOT(slotShowClicked()));
        connect(this, SIGNAL(showChanged()), this, SLOT(slotShowChanged()));
    } else {
        q_buttonMaximized = Q_NULLPTR;
    }

    // button: Close
    if (titleButtons & Close) {
        q_buttonClose = new KPushButton(this);
        q_buttonClose->setToolTip(tr("Close"));
        if (titleButtons == Close) {
            q_buttonClose->setPixmap(":/kwt/images/title_button/dlg_close");
        } else {
            q_buttonClose->setPixmap(":/kwt/images/title_button/close");
        }
        q_horiLayoutTitle->addWidget(q_buttonClose, 0, Qt::AlignTop);
        connect(q_buttonClose, SIGNAL(clicked()), parent, SLOT(close()));
    } else {
        q_buttonClose = Q_NULLPTR;
    }

    q_vertLayoutMain = new QVBoxLayout(this);
    q_vertLayoutMain->setSpacing(0);
    q_vertLayoutMain->setContentsMargins(0, 0, 0, 0);
    q_vertLayoutMain->addLayout(q_horiLayoutTitle);

    setTitleAlignment(Qt::AlignVCenter);
}

void KTitle::addLayout(QLayout *layout)
{
    Q_ASSERT(layout);
    layout->setParent(this);
    q_vertLayoutMain->addLayout(layout);
}

void KTitle::addWidget(QWidget *widget)
{
    Q_ASSERT(widget);
    widget->setParent(this);
    q_vertLayoutMain->addWidget(widget);
}

void KTitle::setTitleIcon(const QIcon &icon) const
{
    q_buttonIcon->setPixmap(icon.pixmap(q_buttonIcon->size()));
}

void KTitle::setTitleFont(const QFont & font) const
{
    q_labelTitle->setFont(font);
}

void KTitle::setTitleAlignment(Qt::Alignment alignment)
{
    q_horiLayoutTitle->setStretch(2, (alignment & Qt::AlignHCenter) ? 0 : 1);
}

void KTitle::setWindowTitle(const QString &text)
{
    KWidget::setWindowTitle(text);
    q_labelTitle->setText(text);
}

void KTitle::setMainMenu(QMenu *menu)
{
    if (q_menuMain || !(q_titleButtons & MainMenu)) {  // have been existence
        Q_ASSERT(false);
        return;
    }

    Q_ASSERT(menu);
    q_menuMain = menu;
    q_buttonMainMenu->setVisible(true);
    connect(q_buttonMainMenu, SIGNAL(clicked()), this, SLOT(slotShowMainMenu()));
}

void KTitle::setEnabled(TitleButtons titleButtons, bool enable) const
{
    KPushButton* button = Q_NULLPTR;

    if (titleButtons & MainMenu) { button = q_buttonMainMenu; }
    if (titleButtons & SkinButton) { button = q_buttonSkins; }
    if (titleButtons & WhatsThis) { button = q_buttonWhatsThis; }
    if (titleButtons & Minimized) { button = q_buttonMinimized; }
    if (titleButtons & Maximized) { button = q_buttonMainMenu; }
    if (titleButtons & Close) { button = q_buttonClose; }

    if (button) {
        button->setEnabled(enable);
    }
}

void KTitle::setVisible(TitleButtons titleButtons, bool visible) const
{
    KPushButton* button = Q_NULLPTR;

    if (titleButtons & MainMenu) { button = q_buttonMainMenu; }
    if (titleButtons & SkinButton) { button = q_buttonSkins; }
    if (titleButtons & WhatsThis) { button = q_buttonWhatsThis; }
    if (titleButtons & Minimized) { button = q_buttonMinimized; }
    if (titleButtons & Maximized) { button = q_buttonMainMenu; }
    if (titleButtons & Close) { button = q_buttonClose; }

    if (button) {
        button->setVisible(visible);
    }
}

void KTitle::addTitleWidget(QWidget *widget, const QString &objectName)
{
    Q_ASSERT(widget);
    if (!objectName.isEmpty()) {
        widget->setObjectName(objectName);
    }

    q_horiLayoutTitle->insertWidget(4, widget, 0, Qt::AlignTop);
}

void KTitle::addTitleSpacing(int size)
{
    q_horiLayoutTitle->insertSpacing(4, size);
}

void KTitle::removeTitleWidget(QWidget *widget)
{
    Q_ASSERT(widget);
    q_horiLayoutTitle->removeWidget(widget);
    widget->setVisible(false);
}

void KTitle::resizeEvent(QResizeEvent *)
{
    setRegions();
}

void KTitle::leaveEvent(QEvent *)
{
    setCursor(Qt::ArrowCursor);
}

void KTitle::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        q_mousePressed = true;
        q_rgnPressed = regionHit(event->pos());
        q_orgPos = mapToParent(event->pos());
        q_orgGeom = parentWidget()->geometry();
    }
}

void KTitle::mouseMoveEvent(QMouseEvent *event)
{
    if (!parentWidget()->isMaximized()) {
        if (q_mousePressed) {
            QPoint curPos = mapToParent(event->pos());
            QRect curGeom = parentWidget()->geometry();
            switch (q_rgnPressed) {
            case TopLeft:
            {
                QPoint v = curPos - q_orgPos;
                updateGeometry(curGeom.x() + v.x(), curGeom.y() + v.y(),
                               curGeom.width() - v.x(), curGeom.height() - v.y());
                break;
            }
            case Top:
            {
                int dy = curPos.y() - q_orgPos.y();
                updateGeometry(q_orgGeom.x(), curGeom.y() + dy,
                               q_orgGeom.width(), curGeom.height() - dy);
                break;
            }
            case TopRight:
            {
                QPoint v = curPos - q_orgPos;
                updateGeometry(q_orgGeom.x(), curGeom.y() + v.y(),
                               q_orgGeom.width() + v.x(), curGeom.height() - v.y());
                break;
            }
            case Inner:
            {
                parentWidget()->move(curGeom.topLeft() + curPos - q_orgPos);
                break;
            }
            default:
                break;
            }
        } else {
            switch (regionHit(event->pos())) {
            case TopLeft:
                setCursor(Qt::SizeFDiagCursor);
                break;
            case Top:
                setCursor(Qt::SizeVerCursor);
                break;
            case TopRight:
                setCursor(Qt::SizeBDiagCursor);
                break;
            default:
                setCursor(Qt::ArrowCursor);
                break;
            }
        }
    }
}

void KTitle::mouseReleaseEvent(QMouseEvent *)
{
    q_mousePressed = false;
    q_rgnPressed = Unknown;
    setCursor(Qt::ArrowCursor);

    if (parentWidget()->y() < - ((KDialog*)parentWidget())->q_shadowMargins.top()) {
        parentWidget()->move(parentWidget()->geometry().x(),
                             parentWidget()->geometry().y() - parentWidget()->y()
                             - ((KDialog*)parentWidget())->q_shadowMargins.top());
    }
}

void KTitle::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (q_titleButtons & Maximized) {
        if (event->button() == Qt::LeftButton) {
            if (regionHit(event->pos()) != Unknown)
            {
                q_mousePressed = false;
                q_rgnPressed = Unknown;
                Q_EMIT showClicked();
            }
        }
    }
}

void KTitle::setRegions()
{
    KDialog* parentDlg = (KDialog*)parentWidget();
    int borderWidth = parentDlg->q_borderWidth;

    q_regions[TopLeft]  = QRect(0, 0, borderWidth * 2, borderWidth * 2);
    q_regions[Top]      = QRect(borderWidth * 2, 0, width() - borderWidth * 4, borderWidth);
    q_regions[TopRight] = QRect(width() - borderWidth * 4, 0, borderWidth * 2, borderWidth);
    q_regions[Inner]    = QRect(0, borderWidth, width(), parentDlg->q_titleHeight - borderWidth);
}

void KTitle::updateGeometry(int x, int y, int w, int h)
{
    QWidget* parent = parentWidget();

    if (parent->minimumWidth() <= w && w <= parent->maximumWidth()) {
        parent->setGeometry(x, parent->pos().y(), w, parent->height());
    }

    if (parent->minimumHeight() <= h && h <= parent->maximumHeight()) {
        parent->setGeometry(parent->pos().x(), y, parent->width(), h);
    }
}

KTitle::RegionType KTitle::regionHit(const QPoint &pos)
{
    for (int i = 0; i < RegionTypeCount; i++) {
        if (q_regions[i].contains(pos)) {
            return KTitle::RegionType(i);
        }
    }

    return Unknown;
}

// slot functions

// show system menu
void KTitle::slotShowSystemMenu(bool checked)
{
    if (checked) {
        KSystemMenu(q_titleButtons, parentWidget()).exec(mapToGlobal(rect().topLeft() + QPoint(10, 30)));
    }
}

// show skins dialog
void KTitle::slotShowSkinsDialog()
{
    KSkinDialog(parentWidget()).exec();
}

// show-clicked
void KTitle::slotShowClicked()
{
    Q_EMIT KTitle::showClicked();
}

// show-changed
void KTitle::slotShowChanged()
{
    if (parentWidget()->isMaximized()) {
        q_buttonMaximized->setPixmap(":/kwt/images/title_button/restore");
    } else {
        q_buttonMaximized->setPixmap(":/kwt/images/title_button/max");
    }
    q_buttonMaximized->update();
}

// show main menu
void KTitle::slotShowMainMenu()
{
    q_menuMain->move(mapToGlobal(q_buttonMainMenu->pos() + q_buttonMainMenu->rect().bottomLeft() + QPoint(0, 2)));
    q_menuMain->show();
}
