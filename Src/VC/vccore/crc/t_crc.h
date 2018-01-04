/************************************************************************/
/*
    CRC 算法函数库
*/
/************************************************************************/

#ifndef _HR_T_CRC_H_
#define _HR_T_CRC_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char		ruint8;
typedef unsigned short		ruint16;
typedef unsigned int		ruint32;

/************************************************************************/
/*   CRC16                                                              */
/*   特征多项式为: X16+X5+X2+X1                                         */
/************************************************************************/
const ruint16 __stdcall make_crc_16(ruint16 icrc, ruint8 *icp, ruint32 icnt);
const ruint32 __stdcall make_crc_32(ruint32 icrc, ruint8 *icp, ruint32 icnt);
const ruint16 __stdcall make_crc_ccitt(ruint16 icrc, ruint8 *icp, ruint32 icnt);
const ruint16 __stdcall make_crc_dnp(ruint16 icrc, ruint8 *icp, ruint32 icnt);
const ruint16 __stdcall make_crc_kermit(ruint16 icrc, ruint8 *icp, ruint32 icnt);

#ifdef __cplusplus
}
#endif

#endif  // _HR_T_CRC_H_
