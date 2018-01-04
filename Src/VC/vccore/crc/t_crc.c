/*CRC Ëã·¨*/
#include "t_crc.h"
#include "lib_crc.h"

const ruint16 __stdcall make_crc_16(ruint16 icrc, ruint8 *icp, ruint32 icnt)
{
    ruint16 crc  = icrc;

    while( icnt-- )
    {
        crc = update_crc_16(crc, *icp++);
    }

    return crc ;
}

const ruint32 __stdcall make_crc_32(ruint32 icrc, ruint8 *icp, ruint32 icnt)
{
    ruint32 crc  = icrc;

    while( icnt-- )
    {
        crc = update_crc_32(crc, *icp++);
    }

    return crc ;
}

const ruint16 __stdcall make_crc_ccitt(ruint16 icrc, ruint8 *icp, ruint32 icnt)
{
    ruint16 crc  = icrc;

    while( icnt-- )
    {
        crc = update_crc_ccitt(crc, *icp++);
    }

    return crc ;
}

const ruint16 __stdcall make_crc_dnp(ruint16 icrc, ruint8 *icp, ruint32 icnt)
{
    ruint16 crc  = icrc;

    while( icnt-- )
    {
        crc = update_crc_dnp(crc, *icp++);
    }

    return crc ;
}

const ruint16 __stdcall make_crc_kermit(ruint16 icrc, ruint8 *icp, ruint32 icnt)
{
    ruint16 crc  = icrc;

    while( icnt-- )
    {
        crc = update_crc_kermit(crc, *icp++);
    }

    return crc ;
}
