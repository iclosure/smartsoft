#include "precomp.h"
#include "kwt_global.h"

//
// KwtCorePrivate
//

class KwtCorePrivate : public QObject
{
    friend class KwtCore;
public:
    explicit KwtCorePrivate(QObject *parent = 0);
    ~KwtCorePrivate();

private:
    Q_DISABLE_COPY(KwtCorePrivate)
    QVector<QTranslator*> translators;
};

KwtCorePrivate::KwtCorePrivate(QObject *)
{
}

KwtCorePrivate::~KwtCorePrivate()
{
    if (!qApp) {
        return;
    }

    Q_FOREACH (QTranslator *translator, translators) {
        qApp->removeTranslator(translator);
        delete translator;
    }
}

//
// KwtCore
//

KwtCore::KwtCore()
{
    Q_INIT_RESOURCE(kwt_images);
    Q_INIT_RESOURCE(kwt_lang);
    Q_INIT_RESOURCE(kwt_qss);
    Q_INIT_RESOURCE(kwt_forms);

    d = new KwtCorePrivate();
}

KwtCore::~KwtCore()
{
    Q_CLEANUP_RESOURCE(kwt_images);
    Q_CLEANUP_RESOURCE(kwt_lang);
    Q_CLEANUP_RESOURCE(kwt_qss);
    Q_CLEANUP_RESOURCE(kwt_forms);

    delete d;
}

bool KwtCore::load(const QString &systemName)
{
    if (!qApp) {
        return false;
    }

    //
    QFile file(":/kwt/qss/stylesheet_1");
    file.open(QFile::ReadOnly);
    qApp->setStyleSheet(file.readAll());
    qApp->setFont(QFont("微软雅黑", 10));

    QString name = systemName.isEmpty() ? QLocale::system().name() : systemName;

    //
    d->translators.append(new QTranslator());
    bool result = d->translators.last()->load(":/kwt/lang/qt_" + name);
    if (result) {
        qApp->installTranslator(d->translators.last());
    }

    //
    d->translators.append(new QTranslator());
    result = d->translators.last()->load(":/kwt/lang/" + name);
    if (result) {
        qApp->installTranslator(d->translators.last());
    }

    return true;
}
