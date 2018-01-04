#ifndef HR_KOBJECT_H
#define HR_KOBJECT_H

#include "vccore_global.h"

class ObjectPrivate;

class VCCORE_EXPORT KObject
{
public:
	explicit KObject(KObject* parent = 0);
	~KObject(void);

	KObject* parent();
	void setParent(KObject* parent);
	
	tstring objectName() const;
	void setObjectName(const tstring &name);

private:
	HR_DISABLE_COPY(KObject)
	ObjectPrivate* d;
};

#endif  // HR_KOBJECT_H
