#include "precomp.h"
#include "kdialog.h"
#include "kshake.h"
#include "ktooltitle.h"
#include "kskinbutton.h"
#include "kpanetitle.h"
#include "kribbontitle.h"
#include "kcustomtitle.h"
#include "kstatusbar.h"

// KDialog

KDialog::KDialog(QString &fileName, QMargins &shadowMargins, int titleHeight,
                 KTitle::TitleTypes titleType, KTitle::TitleButtons titleButtons, QWidget *parent) :
    QDialog(parent, Qt::FramelessWindowHint),
    q_titleHeight(titleHeight),
    q_shadowMargins(shadowMargins),
    q_tmpShadowMargins(q_shadowMargins),
    q_borderWidth(4),
    q_tmpBorderWidth(q_borderWidth),
    q_skinName(fileName),
    //q_shake(new KShake(this)),
    q_mousePressed(false),
    q_isMousePress(false),
    q_rgnPressed(Unknown)
{
    setObjectName("KDialog");
    QDialog::setAttribute(Qt::WA_TranslucentBackground);
    setMouseTracking(true);

    q_clientWidget = 0;

    if (q_skinName.isEmpty()) {
        q_skinName = ":/kwt/images/skins/widget_shadow";
        q_shadowMargins = QMargins(21, 16, 21, 26);
        q_tmpShadowMargins = q_shadowMargins;
        q_titleHeight = 24;
    }

    //
    // load image
    //

    QPixmap image(q_skinName);

    QRect r;

    // TopLeft
    r.setRect(0, 0, q_shadowMargins.left(), q_shadowMargins.top());
    if (!r.isEmpty()) { q_pmTopLeft = image.copy(r); }
    // Top
    r.setRect(q_shadowMargins.left(), 0, image.width() - q_shadowMargins.left() - q_shadowMargins.right(), q_shadowMargins.top());
    if (!r.isEmpty()) { q_pmTop = image.copy(r); }
    // TopRight
    r.setRect(image.width() - q_shadowMargins.right(), 0, q_shadowMargins.right(), q_shadowMargins.top());
    if (!r.isEmpty()) { q_pmTopRight = image.copy(r); }
    // Left
    r.setRect(0, q_shadowMargins.top(), q_shadowMargins.left(), image.height() - q_shadowMargins.top() - q_shadowMargins.bottom());
    if (!r.isEmpty()) { q_pmLeft = image.copy(r); }
    // Title
    r.setRect(q_shadowMargins.left(), q_shadowMargins.top(), image.width() - q_shadowMargins.left() - q_shadowMargins.right(), q_titleHeight);
    if (!r.isEmpty()) { q_pmTitle = image.copy(r); }
    // Client
    if (q_skinName.contains(QRegExp(":/(kwt/)?images/skins/*"))) {
        q_pmClient.load(QString(":/kwt/images/skins/client_bkg"));
    } else {
        r.setRect(q_shadowMargins.left(), q_shadowMargins.top() + q_titleHeight, image.width() - q_shadowMargins.left() - q_shadowMargins.right(),
                  image.height() - q_shadowMargins.top() - q_shadowMargins.bottom() - q_titleHeight);
        if (!r.isEmpty()) { q_pmClient = image.copy(r); }
    }
    // Right
    r.setRect(image.width() - q_shadowMargins.right(), q_shadowMargins.top(), q_shadowMargins.right(),
              image.height() - q_shadowMargins.top() - q_shadowMargins.bottom());
    if (!r.isEmpty()) { q_pmRight = image.copy(r); }
    // BottomLeft
    r.setRect(0, image.height() - q_shadowMargins.bottom(), q_shadowMargins.left(), q_shadowMargins.bottom());
    if (!r.isEmpty()) { q_pmBottomLeft = image.copy(r); }
    // Bottom
    r.setRect(q_shadowMargins.left(), image.height() - q_shadowMargins.bottom(),
              image.width() - q_shadowMargins.left() - q_shadowMargins.right(), q_shadowMargins.bottom());
    if (!r.isEmpty()) { q_pmBottom = image.copy(r); }
    // BottomRight
    r.setRect(image.width() - q_shadowMargins.right(), image.height() - q_shadowMargins.bottom(),
              q_shadowMargins.right(), q_shadowMargins.bottom());
    if (!r.isEmpty()) { q_pmBottomRight = image.copy(r); }

    //
    makeRegions();

    //
    setSkin();

    //
    switch(titleType) {
    case KTitle::NoTitle: q_titleWidget = 0; break;
    case KTitle::Normal: q_titleWidget = new KTitle(q_titleHeight, titleButtons, this); break;
    case KTitle::Tool: q_titleWidget = new KToolTitle(q_titleHeight, titleButtons, this); break;
    case KTitle::Pane: q_titleWidget = new KPaneTitle(q_titleHeight, titleButtons, this); break;
    case KTitle::Ribbon: q_titleWidget = new KRibbonTitle(q_titleHeight, titleButtons, this); break;
    case KTitle::Custom: q_titleWidget = new KCustomTitle(q_titleHeight, titleButtons, this); break;
    default: Q_ASSERT(false); break;
    }

    // client widget
    q_clientWidget = new QWidget(this);
    q_clientWidget->setCursor(Qt::ArrowCursor);
    setWindowIcon(parent ? parent->windowIcon() : QIcon(":/kwt/images/global/robot"));

    // status bar
    q_statusBar = new KStatusBar(this);
    q_statusBar->setCursor(Qt::ArrowCursor);
    q_statusBar->hide();

    q_mainLayout = new QVBoxLayout(this);
    q_mainLayout->setSpacing(0);
    q_mainLayout->setContentsMargins(q_shadowMargins);
    if (q_titleWidget) { q_mainLayout->addWidget(q_titleWidget); }
    q_mainLayout->addWidget(q_clientWidget);
    q_mainLayout->addWidget(q_statusBar);

    ///////////////////////////////////////////////////////////

    if (q_titleWidget && q_skinName.contains(QRegExp(":/(kwt/)?images/skins/*"))) {
        connect(q_titleWidget, SIGNAL(showClicked()), this, SLOT(showChanged()));
        connect(this, SIGNAL(skinChanged(QString)), this, SLOT(setSkin(QString)));
    }
}

void KDialog::setClientWidget(QWidget *widget)
{
    if (q_clientWidget) {
        q_mainLayout->removeWidget(q_clientWidget);
        delete q_clientWidget;
    }
    q_clientWidget = widget;
    if (q_titleWidget) {
        q_mainLayout->insertWidget(1, q_clientWidget);
    } else {
        q_mainLayout->insertWidget(2, q_clientWidget);
    }
}

void KDialog::setCenter(QWidget *parent)
{
    if (parent) {
    } else if (parentWidget()) {
        parent = parentWidget();
    } else {
        parent = reinterpret_cast<QWidget*>(QApplication::desktop());
    }

    move((parent->width() - width()) / 2, (parent->height() - height()) / 2);
}

QLayout *KDialog::layout() const
{
    return q_titleWidget ? q_clientWidget->layout() : QDialog::layout();
}

void KDialog::setLayout(QLayout *layout)
{
    q_clientWidget->setLayout(layout);
}

void KDialog::setAttribute(Qt::WidgetAttribute attribute, bool on)
{
    q_clientWidget->setAttribute(attribute, on);
}

bool KDialog::testAttribute(Qt::WidgetAttribute attribute) const
{
    return q_clientWidget->testAttribute(attribute);
}

void KDialog::setWindowIcon(const QIcon &icon)
{
    QDialog::setWindowIcon(icon);
    if (q_titleWidget) { q_titleWidget->setTitleIcon(icon); }
}

void KDialog::setWindowTitle(const QString &text)
{
    QDialog::setWindowTitle(text);
    if (q_titleWidget) { q_titleWidget->setWindowTitle(text); }
}

void KDialog::setTitleAlignment(Qt::Alignment alignment)
{
    Q_ASSERT(q_titleWidget);
    if (q_titleWidget) { q_titleWidget->setTitleAlignment(alignment); }
}

QRect KDialog::clientRect() const
{
    return q_clientWidget->rect();
}

void KDialog::setStatusWidget(bool enable/* = true*/)
{
    if (enable) {
        q_mainLayout->addWidget(q_statusBar);
        q_statusBar->show();
    } else {
        q_statusBar->hide();
        q_mainLayout->removeWidget(q_statusBar);
    }
}

void KDialog::addSkins(const KSkinType & skin)
{
    KSkinListType* skinList = qObjectProperity<KSkinListType*>(this, "skinList");
    if (!skinList) {
        Q_ASSERT(false);
        return;
    }
    skinList->append(skin);
}

void KDialog::addSkins(const KSkinListType & skins)
{
    KSkinListType* skinList = qObjectProperity<KSkinListType*>(this, "skinList");
    if (!skinList) {
        Q_ASSERT(false);
        return;
    }
    skinList->append(skins);
}

void KDialog::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);

    if (!isMaximized()) {
        // TopLeft
        if(!q_pmTopLeft.isNull()) {
            painter.drawPixmap(0, 0, q_pmTopLeft);
        }
        // Top
        if(!q_pmTop.isNull()) {
            painter.drawPixmap(q_pmTopLeft.width(), 0, width() - q_pmTopLeft.width() - q_pmTopRight.width(), q_pmTop.height(), q_pmTop);
        }
        // TopRight
        if(!q_pmTopRight.isNull()) {
            painter.drawPixmap(width() - q_pmTopRight.width(), 0, q_pmTopRight);
        }
        // Left
        if(!q_pmTopRight.isNull()) {
            painter.drawPixmap(0, q_pmTopLeft.height(), q_pmLeft.width(), height() - q_pmTopLeft.height() - q_pmBottomLeft.height(), q_pmLeft);
        }
        // Right
        if (!q_pmRight.isNull()) {
            painter.drawPixmap(width() - q_pmRight.width(), q_pmTopRight.height(), q_pmRight.width(),
                           height() - q_pmTopRight.height() - q_pmBottomRight.height(), q_pmRight);
        }
        // BottomLeft
        if (!q_pmBottomLeft.isNull()) {
            painter.drawPixmap(0, height() - q_pmBottomLeft.height(), q_pmBottomLeft);
        }
        // Bottom
        if (!q_pmBottom.isNull()) {
            painter.drawPixmap(q_pmBottomLeft.width(), height() - q_pmBottom.height(),
                           width() - q_pmBottomLeft.width() - q_pmBottomRight.width(), q_pmBottom.height(), q_pmBottom);
        }
        // BottomRight
        if (!q_pmBottomRight.isNull()) {
            painter.drawPixmap(width() - q_pmBottomRight.width(), height() - q_pmBottomRight.height(), q_pmBottomRight);
        }
    }

    // Title
    if (!q_pmTitle.isNull()) {
        painter.drawPixmap(q_shadowMargins.left(), q_shadowMargins.top(),
                           width() - q_shadowMargins.left() - q_shadowMargins.right(), q_pmTitle.height(), q_pmTitle);
    }
    // Client
    if (!q_pmBackground.isNull()) {
        QRect clientRect(q_shadowMargins.left(), q_shadowMargins.top(), width() - q_shadowMargins.left() - q_shadowMargins.right(),
                         height() - q_shadowMargins.top() - q_shadowMargins.bottom());
        painter.drawPixmap(clientRect, q_pmBackground, q_pmBackground.rect() &
                           (clientRect.translated(-clientRect.topLeft() + q_pmBackground.rect().center() - clientRect.center())));
    }
    if (!q_pmClient.isNull() && !q_clientWidget->testAttribute(Qt::WA_TranslucentBackground)) {
        QRect clientRect(q_shadowMargins.left(), q_shadowMargins.top() + q_titleHeight, width() - q_shadowMargins.left() - q_shadowMargins.right(),
                         height() - q_shadowMargins.top() - q_shadowMargins.bottom() - q_titleHeight);
        painter.drawPixmap(clientRect, q_pmClient, q_pmClient.rect() &
                           (clientRect.translated(-clientRect.topLeft() + q_pmClient.rect().center() - clientRect.center())));
    }
}

void KDialog::resizeEvent(QResizeEvent *event)
{
    makeRegions();

    QDialog::resizeEvent(event);
}

void KDialog::leaveEvent(QEvent *event)
{
    setCursor(Qt::ArrowCursor);

    QDialog::leaveEvent(event);
}

void KDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        q_mousePressed = true;
        q_orgPos = event->pos();
        q_rgnPressed = regionHit(event->pos());
        q_orgGeom = geometry();
    }

    QDialog::mousePressEvent(event);
}

void KDialog::mouseMoveEvent(QMouseEvent *event)
{
    if (q_mousePressed) {
        QPoint curPos = event->pos();
        QRect curGeom = geometry();
        switch (q_rgnPressed) {
        case TopLeft:
        {
            QPoint v = curPos - q_orgPos;
            updateGeometry(curGeom.x() + v.x(), curGeom.y() + v.y(),
                           curGeom.width() - v.x(), curGeom.height() - v.y());
            break;
        }
        case TopRight:
        {
            QPoint v = curPos - q_orgPos;
            updateGeometry(q_orgGeom.x(), curGeom.y() + v.y(),
                           q_orgGeom.width() + v.x(), curGeom.height() - v.y());
            break;
        }
        case Left:
        {
            int dx = curPos.x() - q_orgPos.x();
            updateGeometry(curGeom.x() + dx, curGeom.y(),
                           curGeom.width() - dx, curGeom.height());
            break;
        }
        case Right:
        {
            int dx = curPos.x() - q_orgPos.x();
            updateGeometry(q_orgGeom.x(), q_orgGeom.y(),
                           q_orgGeom.width() + dx, q_orgGeom.height());
            break;
        }
        case BottomLeft:
        {
            QPoint v = curPos - q_orgPos;
            updateGeometry(curGeom.x() + v.x(), q_orgGeom.y(),
                           curGeom.width() - v.x(), q_orgGeom.height() + v.y());
            break;
        }
        case Bottom:
        {
            int dy = curPos.y() - q_orgPos.y();
            updateGeometry(q_orgGeom.x(), q_orgGeom.y(),
                           q_orgGeom.width(), q_orgGeom.height() + dy);
            break;
        }
        case BottomRight:
        {
            QPoint v = curPos - q_orgPos;
            updateGeometry(q_orgGeom.x(), q_orgGeom.y(),
                           q_orgGeom.width() + v.x(), q_orgGeom.height() + v.y());
            break;
        }
        default:
            break;
        }
    } else {
        switch (regionHit(event->pos())) {
        case TopLeft:
        case BottomRight:
            setCursor(Qt::SizeFDiagCursor);
            break;
        case TopRight:
        case BottomLeft:
            setCursor(Qt::SizeBDiagCursor);
            break;
        case Left:
        case Right:
            setCursor(Qt::SizeHorCursor);
            break;
        case Bottom:
            setCursor(Qt::SizeVerCursor);
            break;
        default:
            setCursor(Qt::ArrowCursor);
            break;
        }
    }

    QDialog::mouseMoveEvent(event);
}

void KDialog::mouseReleaseEvent(QMouseEvent *event)
{
    q_mousePressed = false;
    q_rgnPressed = Unknown;
    setCursor(Qt::ArrowCursor);

    QDialog::mouseReleaseEvent(event);
}

void KDialog::makeRegions()
{
    q_regions[TopLeft]      = QRect(q_shadowMargins.left(), q_shadowMargins.top(), q_borderWidth * 2, q_borderWidth);
    q_regions[TopRight]     = QRect(width() - q_shadowMargins.right() - q_borderWidth, q_shadowMargins.top(), q_borderWidth, q_borderWidth);
    q_regions[Left]         = QRect(q_shadowMargins.left(), q_shadowMargins.top() + q_borderWidth, q_borderWidth,
                                    height() - q_borderWidth * 2 - q_shadowMargins.top() - q_shadowMargins.bottom());
    q_regions[Right]        = QRect(width() - q_shadowMargins.right() - q_borderWidth, q_shadowMargins.top() + q_borderWidth, q_borderWidth,
                                    height() - q_borderWidth * 2 - q_shadowMargins.top() - q_shadowMargins.bottom());
    q_regions[BottomLeft]   = QRect(q_shadowMargins.left(), height() - q_shadowMargins.bottom() - q_borderWidth, q_borderWidth, q_borderWidth);
    q_regions[Bottom]       = QRect(q_shadowMargins.left() + q_borderWidth, height() - q_shadowMargins.bottom() - q_borderWidth,
                                    width() - q_borderWidth * 2 - q_shadowMargins.left() - q_shadowMargins.right(), q_borderWidth);
    q_regions[BottomRight]  = QRect(width() - q_shadowMargins.right() - q_borderWidth, height() - q_shadowMargins.bottom() - q_borderWidth,
                                    q_borderWidth, q_borderWidth);
}

void KDialog::updateGeometry(int x, int y, int w, int h)
{
    if (minimumWidth() <= w && w <= maximumWidth()) {
        setGeometry(x, pos().y(), w, height());
    }

    if (minimumHeight() <= h && h <= maximumHeight()) {
        setGeometry(pos().x(), y, width(), h);
    }
}

KDialog::RegionType KDialog::regionHit(const QPoint &pos)
{
    for (int i = 0; i < 7; i++) {
        if (q_regions[i].contains(pos)) {
            return KDialog::RegionType(i);
        }
    }

    return Unknown;
}

void KDialog::showMaximized()
{
    q_tmpShadowMargins = q_shadowMargins;
    q_tmpBorderWidth = q_borderWidth;
    q_shadowMargins = QMargins();
    q_borderWidth = 0;
    q_mainLayout->setContentsMargins(q_shadowMargins);
    repaint();
    QDialog::showMaximized();

    if (q_titleWidget) {
        Q_EMIT q_titleWidget->showChanged();
    }
}

void KDialog::showFullScreen()
{
    showMaximized();
}

void KDialog::showNormal()
{
    q_shadowMargins = q_tmpShadowMargins;
    q_borderWidth = q_tmpBorderWidth;
    q_mainLayout->setContentsMargins(q_shadowMargins);
    repaint();
    QDialog::showNormal();

    if (q_titleWidget) {
        Q_EMIT q_titleWidget->showChanged();
    }
}

void KDialog::showChanged()
{
    isMaximized() ? showNormal() : showMaximized();
}

void KDialog::setSkin(const QString & name)
{
    if (!q_skinName.isEmpty() && !q_skinName.contains(QRegExp(":/(kwt/)?images/skins/*"))) {
        return ;
    }

    q_pmBackground.detach();
    q_pmBackground = QPixmap();

    if (!name.isEmpty() && !name.contains(QRegExp(":/(kwt/)?images/skins/*"))) {
        q_skinName = name;
    } else {
        QSettings settings;
        settings.beginGroup("Smartsoft/Skin");
        if (name.isEmpty()) {
            q_skinName = settings.value("default", ":/kwt/images/skins/default").toString();
        } else {
            q_skinName = name;
        }
        settings.setValue("default", q_skinName);
        settings.endGroup();
    }

    if (!q_pmBackground.load(q_skinName)) {
        q_pmBackground.detach();
        q_pmBackground = QPixmap();
    }

    update();

    QListIterator<KDialog*> iter(findChildren<KDialog*>());
    while (iter.hasNext()) {
        Q_EMIT iter.next()->skinChanged(name);
    }
}

//
// KSkinDialog
//

KSkinDialog::KSkinDialog(QWidget *parent) :
    KDialog(QString(), QMargins(), 0, KTitle::Normal, KTitle::Default, parent)
{
    setMinimumSize(805, 550);
    setWindowTitle(tr("Set skin"));

    KSkinListType* skinList = qObjectProperity<KSkinListType*>(parent, "skinList");
    Q_ASSERT(skinList);

    if (!skinList->isEmpty() && (skinList->last().first != ":/kwt/images/skins/default")) {
        skinList->append(KSkinType(":/kwt/images/skins/default", tr("Elegant time")));
    }

    QWidget* clientWidget = new QWidget(this);
    clientWidget->setObjectName("ClientWidget");
    clientWidget->setStyleSheet("QWidget#ClientWidget { background-color: rgba(0,0,0,0);border: 0px; }");

    QGridLayout* gridLayoutClient = new QGridLayout(clientWidget);
    QSignalMapper* signalMapper = new QSignalMapper(this);
    for (int i = 0; i < skinList->count(); i++) {
        KSkinButton* skinButton = new KSkinButton((*skinList)[i].second, QPixmap((*skinList)[i].first), clientWidget);
        gridLayoutClient->addWidget(skinButton, i / 5, i % 5);
        connect(skinButton, SIGNAL(clicked()), signalMapper, SLOT(map()));
        signalMapper->setMapping(skinButton, (*skinList)[i].first);
    }

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setObjectName("scrollAreaClient");
    scrollArea->setStyleSheet("QScrollArea#scrollAreaClient { background-color: rgba(0,0,0,0);border: 0px; }");
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setWidget(clientWidget);
    clientWidget->adjustSize();

    QLayout *mainLayout = new QHBoxLayout(KDialog::clientWidget());
    mainLayout->setContentsMargins(0, 5, 0, 5);
    mainLayout->addWidget(scrollArea);
    setCenter(QApplication::desktop());

    ////////////////////////////////////////////////////////////////////////

    connect(signalMapper, SIGNAL(mapped(QString)), reinterpret_cast<KDialog*>(parent), SLOT(setSkin(QString)));

    ////////////////////////////////////////////////////////////////////////
}
