#ifndef _HR_STDDEF_H_
#define _HR_STDDEF_H_

#include <stddef.h>

#define hr_offsetof(s, m) offsetof(s, m)
#define hr_sizeof(s, m) sizeof(((s *)0)->m)

#endif  // _HR_STDDEF_H_
