
#ifndef HR_ENCRYPT_GLOBAL_H
#define HR_ENCRYPT_GLOBAL_H

//#ifdef ENCRYPT_DLL

#ifdef DLLENCRYPT_EXPORTS
#define DLLENCRYPT_API	__declspec(dllexport)
#else
#define DLLENCRYPT_API	__declspec(dllimport)
#endif  // DLLENCRYPT_EXPORTS

//#endif // ENCRYPT_DLL

#ifndef DLLENCRYPT_EXPORTS
#define DLLENCRYPT_EXPORTS
#endif

/*
Some classes do not permit copies to be made of an object. These
classes contains a private copy constructor and assignment
operator to disable copying (the compiler gives an error message).
*/
#ifndef HR_DISABLE_COPY
#define HR_DISABLE_COPY(Class) \
	Class(const Class &); \
	Class& operator=(const Class &);
#endif  // HR_DISABLE_COPY

#endif	// HR_ENCRYPT_GLOBAL_H
