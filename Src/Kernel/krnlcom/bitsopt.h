
#ifndef _HR_BITS_OPERATION_H_
#define _HR_BITS_OPERATION_H_

#include <ntdef.h>

/* need not to return */

#define CLR_REG_BIT(_reg_, _bit_)  ((_reg_) &= ~(1UL << (_bit_)))
#define SET_REG_BIT(_reg_, _bit_)  ((_reg_) |=  (1UL << (_bit_)))

#define CONFIG_REG_BIT_ULONG(_reg_, _bit_, _val_) \
    (((_val_) & 0x01UL) ? SET_REG_BIT((_reg_), (_bit_)) : CLR_REG_BIT((_reg_), (_bit_)))

#define CONFIG_REG_BITS_ULONG(_reg_, _offset_, _mask_, _val_) \
    ((_reg_) = (_reg_) & (~(_mask_)) | ((_val_) << (_offset_)))

#define GET_REG_BIT(_reg_, _bit_)                  ( (_reg_) &  (1UL << (_bit_)))
#define GET_REG_BIT_VAL(_reg_, _bit_)              (((_reg_) >> (_bit_)) & 1UL)
#define GET_REG_BITS_VAL(_reg_, _offset_, _mask_)  (((_reg_) &  (_mask_)) >> (_offset_))

/* need to return */

#define CLR_REG_BIT_RETURN(_reg_, _bit_)  ((_reg_) & (~(1UL << (_bit_))))
#define SET_REG_BIT_RETURN(_reg_, _bit_)  ((_reg_) | (  1UL << (_bit_)))

#define CONFIG_REG_BIT_RETURN(_reg_, _bit_, _val_) \
	(((_val_)& 0x01UL) ? SET_REG_BIT_RETURN((_reg_), (_bit_)) : CLR_REG_BIT_RETURN((_reg_), (_bit_)))

#define CONFIG_REG_BITS_RETURN(_reg_, _offset_, _mask_, _val_) \
	((_reg_) & (~(_mask_)) | ((_val_) << (_offset_)))
/*
union BIT8
{
	struct
	{
		BOOLEAN bit0 : 1;
		BOOLEAN bit1 : 1;
		BOOLEAN bit2 : 1;
		BOOLEAN bit3 : 1;
		BOOLEAN bit4 : 1;
		BOOLEAN bit5 : 1;
		BOOLEAN bit6 : 1;
		BOOLEAN bit7 : 1;
	} bits;
	UINT8 all;
};

union BIT16
{
	struct
	{
		BOOLEAN bit0 : 1;
		BOOLEAN bit1 : 1;
		BOOLEAN bit2 : 1;
		BOOLEAN bit3 : 1;
		BOOLEAN bit4 : 1;
		BOOLEAN bit5 : 1;
		BOOLEAN bit6 : 1;
		BOOLEAN bit7 : 1;
		BOOLEAN bit8 : 1;
		BOOLEAN bit9 : 1;
		BOOLEAN bit10 : 1;
		BOOLEAN bit11 : 1;
		BOOLEAN bit12 : 1;
		BOOLEAN bit13 : 1;
		BOOLEAN bit14 : 1;
		BOOLEAN bit15 : 1;
	} bits;
	UINT16 all;
};

union BIT32
{
	struct
	{
		BOOLEAN bit0 : 1;
		BOOLEAN bit1 : 1;
		BOOLEAN bit2 : 1;
		BOOLEAN bit3 : 1;
		BOOLEAN bit4 : 1;
		BOOLEAN bit5 : 1;
		BOOLEAN bit6 : 1;
		BOOLEAN bit7 : 1;
		BOOLEAN bit8 : 1;
		BOOLEAN bit9 : 1;
		BOOLEAN bit10 : 1;
		BOOLEAN bit11 : 1;
		BOOLEAN bit12 : 1;
		BOOLEAN bit13 : 1;
		BOOLEAN bit14 : 1;
		BOOLEAN bit15 : 1;
		BOOLEAN bit16 : 1;
		BOOLEAN bit17 : 1;
		BOOLEAN bit18 : 1;
		BOOLEAN bit19 : 1;
		BOOLEAN bit20 : 1;
		BOOLEAN bit21 : 1;
		BOOLEAN bit22 : 1;
		BOOLEAN bit23 : 1;
		BOOLEAN bit24 : 1;
		BOOLEAN bit25 : 1;
		BOOLEAN bit26 : 1;
		BOOLEAN bit27 : 1;
		BOOLEAN bit28 : 1;
		BOOLEAN bit29 : 1;
		BOOLEAN bit30 : 1;
		BOOLEAN bit31 : 1;
	} bits;
	UINT32 all;
};

union BIT64
{
	struct
	{
		BOOLEAN bit0 : 1;
		BOOLEAN bit1 : 1;
		BOOLEAN bit2 : 1;
		BOOLEAN bit3 : 1;
		BOOLEAN bit4 : 1;
		BOOLEAN bit5 : 1;
		BOOLEAN bit6 : 1;
		BOOLEAN bit7 : 1;
		BOOLEAN bit8 : 1;
		BOOLEAN bit9 : 1;
		BOOLEAN bit10 : 1;
		BOOLEAN bit11 : 1;
		BOOLEAN bit12 : 1;
		BOOLEAN bit13 : 1;
		BOOLEAN bit14 : 1;
		BOOLEAN bit15 : 1;
		BOOLEAN bit16 : 1;
		BOOLEAN bit17 : 1;
		BOOLEAN bit18 : 1;
		BOOLEAN bit19 : 1;
		BOOLEAN bit20 : 1;
		BOOLEAN bit21 : 1;
		BOOLEAN bit22 : 1;
		BOOLEAN bit23 : 1;
		BOOLEAN bit24 : 1;
		BOOLEAN bit25 : 1;
		BOOLEAN bit26 : 1;
		BOOLEAN bit27 : 1;
		BOOLEAN bit28 : 1;
		BOOLEAN bit29 : 1;
		BOOLEAN bit30 : 1;
		BOOLEAN bit31 : 1;
		BOOLEAN bit32 : 1;
		BOOLEAN bit33 : 1;
		BOOLEAN bit34 : 1;
		BOOLEAN bit35 : 1;
		BOOLEAN bit36 : 1;
		BOOLEAN bit37 : 1;
		BOOLEAN bit38 : 1;
		BOOLEAN bit39 : 1;
		BOOLEAN bit40 : 1;
		BOOLEAN bit41 : 1;
		BOOLEAN bit42 : 1;
		BOOLEAN bit43 : 1;
		BOOLEAN bit44 : 1;
		BOOLEAN bit45 : 1;
		BOOLEAN bit46 : 1;
		BOOLEAN bit47 : 1;
		BOOLEAN bit48 : 1;
		BOOLEAN bit49 : 1;
		BOOLEAN bit50 : 1;
		BOOLEAN bit51 : 1;
		BOOLEAN bit52 : 1;
		BOOLEAN bit53 : 1;
		BOOLEAN bit54 : 1;
		BOOLEAN bit55 : 1;
		BOOLEAN bit56 : 1;
		BOOLEAN bit57 : 1;
		BOOLEAN bit58 : 1;
		BOOLEAN bit59 : 1;
		BOOLEAN bit60 : 1;
		BOOLEAN bit61 : 1;
		BOOLEAN bit62 : 1;
		BOOLEAN bit63 : 1;
	} bits;
	UINT64 all;
};
*/
#endif	// _HR_BITS_OPERATION_H_
