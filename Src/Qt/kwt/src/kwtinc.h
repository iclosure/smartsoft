#ifndef KWTINC_H
#define KWTINC_H

#include "kwt_global.h"
#include "kcheckbox.h"
#include "kcustomtitle.h"
#include "kdialog.h"
#include "kdragproxy.h"
#include "kencrypt.h"
#include "khexedit.h"
#include "kmenu.h"
#include "kpanetitle.h"
#include "kpushbutton.h"
#include "kradiobutton.h"
#include "kresizedialogproxy.h"
#include "kribbontitle.h"
#include "kshake.h"
#include "kskinbutton.h"
#include "kstatusbar.h"
#include "kstylesheet.h"
#include "ksystemmenu.h"
#include "ktabwidget.h"
#include "kthread.h"
#include "ktitle.h"
#include "ktoolbutton.h"
#include "ktooltitle.h"
#include "kwaitcondition.h"
#include "kwidget.h"

#if (QT_VERSION < 0x050000)
#include "serialport/qextserialbase.h"
#include "serialport/qextserialenumerator.h"
#include "serialport/qextserialport.h"
#include "serialport/win_qextserialport.h"

#ifdef QT_DECLARATIVE_LIB
#include "kdeclarativeview.h"
#endif
#else
#ifdef QT_QUICK_LIB
#include "kquickview.h"
#endif
#endif

#endif // KWTINC_H
