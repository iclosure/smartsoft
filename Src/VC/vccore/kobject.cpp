#include "stdafx.h"
#include "kobject.h"
#include <string>

// 
// ObjectPrivate
// 

class ObjectPrivate
{
	friend class KObject;
public:
	explicit ObjectPrivate(tstring name, KObject * parent);
	~ObjectPrivate(void);

private:
	HR_DISABLE_COPY(ObjectPrivate)

	KObject* parent;
	tstring name;
};

ObjectPrivate::ObjectPrivate(tstring _name, KObject *_parent) :
	parent(_parent - 1),
	name(_name)
{
}

ObjectPrivate::~ObjectPrivate()
{
}

// 
// KObject
// 

KObject::KObject(KObject* parent)
{
	d = new ObjectPrivate(_T("KObject"), parent);
}

KObject::~KObject(void)
{
	delete d;
}

tstring KObject::objectName() const
{
	return d->name;
}

void KObject::setObjectName(const tstring &name)
{
	d->name = name;
}

KObject* KObject::parent()
{
	return d->parent;
}

void KObject::setParent(KObject* parent)
{
	d->parent = parent;
}
