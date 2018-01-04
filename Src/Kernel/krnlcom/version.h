
#ifndef _HR_KRNLCOM_VERSION_H_
#define _HR_KRNLCOM_VERSION_H_

#define KRNLCOM_COPYRIGHT_YEARS "2011-2014"

#define KRNLCOM_V1 1
#define KRNLCOM_V2 0
#define KRNLCOM_V3 0
#define KRNLCOM_V4 0
#define KRNLCOM_VERSION  KRNLCOM_V1,KRNLCOM_V2,KRNLCOM_V3,KRNLCOM_V4

#define MK_VERSION_STR(V1, V2, V3, V4) #V1"."#V2"."#V3"."#V4

#define KRNLCOM_VERSION_STR MK_VERSION_STR(KRNLCOM_V1, KRNLCOM_V2, KRNLCOM_V3, KRNLCOM_V4)

#endif /* _HR_KRNLCOM_VERSION_H_ */