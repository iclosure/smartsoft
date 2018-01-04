
/* PComm_c.h	  
 * PComm define file for Win32 on C/C++
 * History:
 * Date	    Author	Comment
 * 10/22/1996  Victor	wrote it.
 * 03/28/1997  Victor	modified. add sio_term_irq function.
 *				add sio_linput function.
 * 04/29/1997  Victor	modified. add sio_putb_x function.
 * 05/21/1997  Victor	modified. add sio_view function.
 * 06/13/1997  Victor	modified. add file transfer function.
 * 06/30/1997  Victor	modified. add sio_TxLowWater function.
 * 11/17/1997  Victor	add sio_AbortWrite, sio_SetWriteTimeouts
 * 11/21/1997  Victor	add sio_SetReadTimeouts, sio_AbortRead
 *				sio_GetReadTimeouts, sio_GetWriteTimeouts
 * 03/08/1998  Casper   support VB 5.0
 *
 * 04/27/1999  Casper   add sio_ActXon, sio_ActXoff, sio_break_ex
 */

/*	BAUD rate setting	*/

#ifndef _HR_PCOMM_H_
#define _HR_PCOMM_H_

#define B50			0x00
#define B75			0x01
#define B110		0x02
#define B134		0x03
#define B150		0x04
#define B300		0x05
#define B600		0x06
#define B1200		0x07
#define B1800		0x08
#define B2400		0x09
#define B4800		0x0A
#define B7200		0x0B
#define B9600		0x0C
#define B19200		0x0D
#define B38400		0x0E
#define B57600		0x0F
#define B115200 	0x10
#define B230400 	0x11
#define B460800 	0x12
#define B921600 	0x13

/*	MODE setting		*/
#define BIT_5		0x00			/* Word length define	*/
#define BIT_6		0x01
#define BIT_7		0x02
#define BIT_8		0x03

#define STOP_1		0x00			/* Stop bits define	*/
#define STOP_2		0x04

#define P_NONE		0x00
#define P_ODD		0x08
#define P_EVEN		0x18			/* Parity define	*/
#define P_MRK		0x28
#define P_SPC		0x38

/*	MODEM CONTROL setting	*/
#define C_DTR		0x01
#define C_RTS		0x02

/*	MODEM LINE STATUS	*/
#define S_CTS		0x01
#define S_DSR		0x02
#define S_RI		0x04
#define S_CD		0x08

/* error code */
#define SIO_OK		0
#define SIO_BADPORT	-1	/* no such port or port not opened */
#define SIO_OUTCONTROL	-2	/* can't control the board */
#define SIO_NODATA	-4	/* no data to read or no buffer to write */
#define SIO_OPENFAIL	-5	/* no such port or port has be opened */
#define SIO_RTS_BY_HW	-6      /* RTS can't set because H/W flowctrl */
#define SIO_BADPARM	-7	/* bad parameter */
#define SIO_WIN32FAIL	-8	/* call win32 function fail, please call */
				/* GetLastError to get the error code */
#define SIO_BOARDNOTSUPPORT -9	/* Does not support this board */
#define SIO_FAIL	-10	/* PComm function run result fail */
#define SIO_ABORTWRITE	-11	/* write has blocked, and user abort write */
#define SIO_WRITETIMEOUT    -12 /* write timeoue has happened */

/* file transfer error code */
#define SIOFT_OK	0
#define SIOFT_BADPORT	-1	/* no such port or port not open */
#define SIOFT_TIMEOUT	-2	/* protocol timeout */
#define SIOFT_ABORT	-3	/* user key abort */
#define SIOFT_FUNC	-4	/* func return abort */
#define SIOFT_FOPEN	-5	/* can not open files */
#define SIOFT_CANABORT	-6	/* Ymodem CAN signal abort */
#define SIOFT_PROTOCOL	-7	/* Protocol checking error abort */
#define SIOFT_SKIP	-8	/* Zmodem remote skip this send file */
#define SIOFT_LACKRBUF	-9	/* Zmodem Recv-Buff size must >= 2K bytes */
#define SIOFT_WIN32FAIL -10	/* OS fail */
				/* GetLastError to get the error code */
#define SIOFT_BOARDNOTSUPPORT -11	   /* Does not support board */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef DLLPCOMM

#ifdef DLLPCOMM_EXPORTS

#define DLLPCOMM_API __declspec(dllexport)
#else
#define DLLPCOMM_API __declspec(dllimport)
#endif	// DLLPCOMM_EXPORTS

#else

#define DLLPCOMM_API

#endif  // !DLLPCOMM

#ifndef CALLBACK
#define CALLBACK	__stdcall
#endif  // CALLBACK

/* basic function phototype */
int  DLLPCOMM_API CALLBACK sio_ioctl(int port, int baud, int mode);
int  DLLPCOMM_API CALLBACK sio_getch(int port);
int  DLLPCOMM_API CALLBACK sio_read(int port, char *buf, int len);
int  DLLPCOMM_API CALLBACK sio_putch(int port, int term);
int  DLLPCOMM_API CALLBACK sio_write(int port, char *buf, int len);
int  DLLPCOMM_API CALLBACK sio_flush(int port, int func);
long DLLPCOMM_API CALLBACK sio_iqueue(int port);
long DLLPCOMM_API CALLBACK sio_oqueue(int port);
int  DLLPCOMM_API CALLBACK sio_lstatus(int port);
int  DLLPCOMM_API CALLBACK sio_lctrl(int port, int mode);
int  DLLPCOMM_API CALLBACK sio_cnt_irq(int port, void (CALLBACK *func)(int port), int count);
int  DLLPCOMM_API CALLBACK sio_modem_irq(int port, void (CALLBACK *func)(int port));
int  DLLPCOMM_API CALLBACK sio_break_irq(int port, void (CALLBACK *func)(int port));
int  DLLPCOMM_API CALLBACK sio_Tx_empty_irq(int port, void (CALLBACK *func)(int port));
int  DLLPCOMM_API CALLBACK sio_break(int port, int time);
int  DLLPCOMM_API CALLBACK sio_break_ex(int port, int time);
int  DLLPCOMM_API CALLBACK sio_flowctrl(int port, int mode);
int  DLLPCOMM_API CALLBACK sio_Tx_hold(int port);
int  DLLPCOMM_API CALLBACK sio_close(int port);
int  DLLPCOMM_API CALLBACK sio_open(int port);
long DLLPCOMM_API CALLBACK sio_getbaud(int port);
int  DLLPCOMM_API CALLBACK sio_getmode(int port);
int  DLLPCOMM_API CALLBACK sio_getflow(int port);
int  DLLPCOMM_API CALLBACK sio_DTR(int port, int mode);
int  DLLPCOMM_API CALLBACK sio_RTS(int port, int mode);
int  DLLPCOMM_API CALLBACK sio_baud(int port, long speed);
int  DLLPCOMM_API CALLBACK sio_data_status(int port);
int  DLLPCOMM_API CALLBACK sio_term_irq(int port, void (CALLBACK *func)(int port), char code);
int  DLLPCOMM_API CALLBACK sio_linput(int port, char *buf, int lne, int term);
int  DLLPCOMM_API CALLBACK sio_putb_x(int port, char *buf, int len, int tick);
int  DLLPCOMM_API CALLBACK sio_putb_x_ex(int port, char *buf, int len, int tms);
int  DLLPCOMM_API CALLBACK sio_view(int port, char *buf, int len);
int  DLLPCOMM_API CALLBACK sio_TxLowWater(int port, int size);
int  DLLPCOMM_API CALLBACK sio_AbortWrite(int port);
int  DLLPCOMM_API CALLBACK sio_SetWriteTimeouts(int port, unsigned long TotalTimeouts);
int  DLLPCOMM_API CALLBACK sio_GetWriteTimeouts(int port, unsigned long *TotalTimeouts);
int  DLLPCOMM_API CALLBACK sio_SetReadTimeouts(int port, unsigned long TotalTimeouts, unsigned long IntervalTimeouts);
int  DLLPCOMM_API CALLBACK sio_GetReadTimeouts(int port, unsigned long *TotalTimeouts, unsigned long *IntervalTimeouts);
int  DLLPCOMM_API CALLBACK sio_AbortRead(int port);

int DLLPCOMM_API CALLBACK sio_ActXoff(int port);
int DLLPCOMM_API CALLBACK sio_ActXon(int port);

/* file transfer function phototype */
int DLLPCOMM_API CALLBACK sio_FtASCIITx(int port, char *fname, int (CALLBACK *func)(long len, int rlen,
			char *buf, long flen), int key);
int DLLPCOMM_API CALLBACK sio_FtASCIIRx(int port, char *fname, int (CALLBACK *func)(long len, int rlen,
			char *buf, long flen), int key, int sec);
int DLLPCOMM_API CALLBACK sio_FtXmodemCheckSumTx(int port, char *fname, int (CALLBACK *func)(long len,
			int rlen, char *buf, long flen), int key);
int DLLPCOMM_API CALLBACK sio_FtXmodemCheckSumRx(int port, char *fname, int (CALLBACK *func)(long len,
			int rlen, char *buf, long flen), int key);
int DLLPCOMM_API CALLBACK sio_FtXmodemCRCTx(int port, char *fname, int (CALLBACK *func)(long len,
			int rlen, char *buf, long flen), int key);
int DLLPCOMM_API CALLBACK sio_FtXmodemCRCRx(int port, char *fname, int (CALLBACK *func)(long len,
			int rlen, char *buf, long flen), int key);
int DLLPCOMM_API CALLBACK sio_FtXmodem1KCRCTx(int port, char *fname, int (CALLBACK *func)(long len,
			int rlen, char *buf, long flen), int key);
int DLLPCOMM_API CALLBACK sio_FtXmodem1KCRCRx(int port, char *fname, int (CALLBACK *func)(long len,
			int rlen, char *buf, long flen), int key);
int DLLPCOMM_API CALLBACK sio_FtYmodemTx(int port, char *fname, int (CALLBACK *func)(long len,
			int rlen, char *buf, long flen), int key);
int DLLPCOMM_API CALLBACK sio_FtYmodemRx(int port, char **ffname, int fno,
			int (CALLBACK *func)(long len, int rlen, char *buf, long flen),
			int key);
int DLLPCOMM_API CALLBACK sio_FtZmodemTx(int port, char *fname, int (CALLBACK *func)(long len,
			int rlen, char *buf, long flen), int key);
int DLLPCOMM_API CALLBACK sio_FtZmodemRx(int port, char **ffname, int fno,
			int (CALLBACK *func)(long len, int rlen, char *buf, long flen),
			int key);
int DLLPCOMM_API CALLBACK sio_FtKermitTx(int port, char *fname, int (CALLBACK *func)(long len,
			int rlen, char *buf, long flen), int key);
int DLLPCOMM_API CALLBACK sio_FtKermitRx(int port, char **ffname, int fno,
			int (CALLBACK *func)(long len, int rlen, char *buf, long flen),
			int key);

#ifdef __cplusplus
}
#endif

#define sio_putb(x, y, z) sio_write(x, y, z)

#endif  // _HR_PCOMM_H_
