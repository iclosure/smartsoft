
#ifndef _HR_KRNLCOM_DEFINE_H_
#define _HR_KRNLCOM_DEFINE_H_

#ifndef HR_EXTERN_C
#ifdef __cplusplus
#define HR_EXTERN_C extern "C"
#else
#define HR_EXTERN_C 
#endif  // !__cplusplus
#endif  // HR_EXTERN_C

#ifndef HR_EXTERN_C_START
#ifdef __cplusplus
#define HR_EXTERN_C_START HR_EXTERN_C {
#else
#define HR_EXTERN_C_START 
#endif  // !__cplusplus
#endif  // HR_EXTERN_C_START

#ifndef HR_EXTERN_C_END
#ifdef __cplusplus
#define HR_EXTERN_C_END }
#else
#define HR_EXTERN_C_END 
#endif  // !__cplusplus
#endif  // HR_EXTERN_C_END

#endif	// _HR_KRNLCOM_DEFINE_H_