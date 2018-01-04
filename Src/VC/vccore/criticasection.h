#ifndef HR_CRITICAL_SECTION_H
#define HR_CRITICAL_SECTION_H

#include "vccore_global.h"

// 
// KCriticalSection
// 

class CriticalSectionPrivate;

class VCCORE_EXPORT KCriticalSection
{
public:
	explicit KCriticalSection(void);
	virtual ~KCriticalSection(void);

	void acquire();
	void release();

private:
	HR_DISABLE_COPY(KCriticalSection)
	CriticalSectionPrivate* d;
};

// 
// KCriticalLocker
// 

class VCCORE_EXPORT KCriticalLocker
{
public:
	explicit KCriticalLocker(KCriticalSection * cs);
	virtual ~KCriticalLocker(void);

private:
	HR_DISABLE_COPY(KCriticalLocker)
	KCriticalSection* d;
};

#endif  // HR_CRITICAL_SECTION_H
