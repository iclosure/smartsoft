﻿#ifndef _HR_SERIALPORT_H_
#define _HR_SERIALPORT_H_

//////////////////////////////////////////////////////////////
// @file 		serialport.h
// @Copyright	Hi ROBOT@611-ChengDu-AVIC
// @key			Function：MOXA 串口驱动库
// @remark		Remarks :
//				使用步骤：	1).创建CSerialPort对象;
//                          2).调用Initialize()初始化串口参数
//							3).调用Open()打开串口;
//							4).调用Read()和Write()进行数据传输;
//							5).调用Close()关闭串口;
//							6).释放CSerialPort对象.
// @date		2012-11-13 09:41
//////////////////////////////////////////////////////////////

#include "../moxa/pcomm.h"
#include <string>
#include <assert.h>

// Flush any data within the driver's input/output buffer
#define FLUSH_INPUT_BUFFER		0  // flush input buffer
#define FLUSH_OUTPUT_BUFFER		1  // flush output buffer
#define FLUSH_BOTH_BUFFER		2  // flush input and output buffer

// 通用MOXA串口类
class SerialPort
{
public:
    /**
    * @brief
    *	Constructor
    * @param  port, [in] COM Port Number
    *		  baud, [in] true baud rate, e.g. 200, 1200, 9600, 115200, etc.
    *		  dataBits, [in] Data bits settings.
    *		  parity, [in] parity bits settings. e.g.: "None" or "Odd" or "Even" or "Mark" or "Space"
    *		  stopBits, [in] stop settings.
    * @return none.
    * @remark
    */
    SerialPort() :
        q_port(1),
        q_baud(9600),
        q_dataBits(8),
        q_parity("None"),
        q_stopBits(1),
        q_isOpen(false),
        q_errorCode(0)
    {

    }

    virtual ~SerialPort()
    {
        // close serial port
        if (q_isOpen)
        {
            sio_close(q_port);
        }
    }

    // 禁止拷贝和赋值
    SerialPort (const SerialPort & rsh);
    const SerialPort operator=(const SerialPort& rsh);

public:
    // initialize the serial port
    void initialize(int port, long baud=9600, int dataBits=8, const std::string &parity="None", int stopBits=1)
    {
        q_port      = port;
        q_baud      = baud;
        q_dataBits  = dataBits;
        q_parity    = parity;
        q_stopBits  = stopBits;
    }

    // Set the option of serial port and initialize the hardware
    bool set(long baud, int dataBits, const std::string &parity, int stopBits)
    {
        setBaud(baud);
        setMode(dataBits, parity, stopBits);
        if (q_errorCode != SIO_OK)
        {
            return false;
        }
        else
        {
            q_dataBits = dataBits;
            q_parity = parity;
            q_stopBits = stopBits;
            return true;
        }
    }

    /**
    * @brief
    *
    *	the initial status of the COM port is:
    *	Baud rate: 38400 bps	Flow control: None
    *	Data bits: 8			DTR state	: ON
    *	Stop bits: 1			RTS state	: ON
    *	Parity	 : None
    * @param	none.
    * @return	false, (m_errorCode: SIO_BADPORT/SIO_OPENFAIL/SIO_WIN32FAIL)
    *			true , (m_errorCode: SIO_OK)
    * @remark
    */
    bool open()
    {
        // open serial port
        if (! q_isOpen)
        {
            q_errorCode = sio_open(q_port);
            if (q_errorCode != SIO_OK)
            {
                q_isOpen = false;
            }
            else
            {
                // set serial port
                setPort();
                if (q_errorCode != SIO_OK)
                {
                    // close serial port
                    q_isOpen = false;
                    sio_close(q_port);
                }
                else  // fail to set
                {
                    q_isOpen = true;
                }
            }
        }

        return q_isOpen;
    }

    /**
    * @brief
    *	Disable a serial port so that it can't transmit data.
    * @param	none.
    * @return	false, (m_errorCode: SIO_BADPORT/SIO_WIN32FAIL)
    *			true , (m_errorCode: SIO_OK)
    * @remark
    */
    bool close()
    {
        if (q_isOpen)
        {
            // close serial port
            q_errorCode = sio_close(q_port);
            if (SIO_OK == q_errorCode)
            {
                q_isOpen = false;
                return true;
            }
            else  // fail to close
            {
                return false;
            }
        }

        return true;
    }

    /**
    * @brief
    *	Read data from the driver's input buffer.
    * @param	buff, [out] receive buffer pointer
    *			len,  [out] length of data to be read each time
    * @return	>0, length of data received
    *			=0, no data received
    *			Fail, SIO_BADPORT/SIO_BADPARAM/SIO_WIN32FAIL
    * @remark
    */
    int read(void* buff, int len)
    {
        return sio_read(q_port, (char*)buff, len);
    }

    // read a byte
    int read()
    {
        return sio_getch(q_port);
    }

    /**
    * @brief
    *	Put a block of data to the driver's output buffer.
    *	The actual length of data written depends on the amount of free space in the driver's output buffer.
    * @param	buff, [in] transmit buffer pointer
    *			len,  [in] transmit buffer length
    * @return	>=0, length of data transmitted
    *			Fail, SIO_BADPORT/SIO_BADPARAM/SIO_ABORT_WRITE/SIO_WRITETIMEOUT
    * @remark
    */
    int write(const void* buff, int len)
    {
        return sio_write(q_port, (char*)buff, len);
    }

    // write a string
    int write(const char* str, int len)
    {
        return sio_write(q_port, (char*)str, len);
    }

    // write a byte
    int write(char ch)
    {
        return sio_putch(q_port, ch);
    }

    /**
    * @brief
    *	Set timeout values for sio_read and sio_getch. (no timeout in default)
    * @param	total[in]: Total timeout values (milliseconds, Default = MAXDWORD)
    *			interval[in]: Interval timeout values (milliseconds, Default = 0)
    * @return	Succeed, SIO_OK
    *			Fail, SIO_BADPORT/SIO_BADPARAM/SIO_WIN32FAIL
    * @remark
    */
    int setReadTimeouts(unsigned long  total = ~0, unsigned long interval = 0)
    {
        return sio_SetReadTimeouts(q_port,total, interval);
    }

    /**
    * @brief
    *	Set timeout values for sio_write() and sio_putch().(no timeout in default)
    * @param	total, [in] Total timeout values (milliseconds, Default = 0).
    * @return	Succeed, SIO_OK
    *			Fail, SIO_BADPORT/SIO_BADPARAM/SIO_WIN32FAIL
    * @remark
    */
    int setWriteTimeouts(unsigned long  total = 0)
    {
        return sio_SetWriteTimeouts(q_port, total);
    }

    /**
    * @brief
    *	Flush any data within the driver's input/output buffer.
    * @param	func, [in] flush function:
    *			>>0, flush input buffer;
    *			>>1, flush output buffer;
    *			>>2, flush input and output buffer.
    * @return	false: (m_errorCode: SIO_BADPORT/SIO_BADPARM/SIO_WIN32FAIL)
    *			true : (m_errorCode: SIO_OK)
    * @remark
    */
    bool flush(int func)
    {
        q_errorCode = sio_flush(q_port, func);
        if (q_errorCode != SIO_OK)
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    // Set baud rate
    bool setBaud(long baud)
    {
        q_errorCode = sio_baud(q_port, baud);
        if (q_errorCode != SIO_OK)
        {
            return false;
        }
        else
        {
            q_baud = baud;
            return true;
        }
    }

    // Set DataBits
    bool setDataBits(long dataBits)
    {
        setMode(dataBits, q_parity, q_stopBits);
        if (q_errorCode != SIO_OK)
        {
            return false;
        }
        else
        {
            q_dataBits = dataBits;
            return true;
        }
    }

    // Set Parity
    bool setParity(const std::string parity)
    {
        setMode(q_dataBits, parity, q_stopBits);
        if (q_errorCode != SIO_OK)
        {
            return false;
        }
        else
        {
            q_parity = parity;
            return true;
        }
    }

    // Set StopBits
    bool setStopBits(int stopBits)
    {
        setMode(q_dataBits, q_parity, stopBits);
        if (q_errorCode != SIO_OK)
        {
            return false;
        }
        else
        {
            q_stopBits = stopBits;
            return true;
        }
    }

    // Return the count of input buffer by bytes.
    int iqueue()
    {
        return sio_iqueue(q_port);
    }

    // Return the count of output buffer by bytes.
    int oqueue()
    {
        return sio_oqueue(q_port);
    }

    // Get the last error string.
    std::string getLastErr()
    {
        switch(q_errorCode)
        {
        case SIO_OK:
            return "Successful";
        case SIO_BADPORT:
            return "no such port or port not opened";
        case SIO_OUTCONTROL:
            return "can't control the board";
        case SIO_NODATA:
            return "no data to read or no buffer to write";
        case SIO_OPENFAIL:
            return "no such port or port has be opened";
        case SIO_RTS_BY_HW:
            return "RTS can't set because H/W flowctrl";
        case SIO_BADPARM:
            return "bad parameter";
        case SIO_WIN32FAIL:
            return "call win32 function fail, please call";
        case SIO_BOARDNOTSUPPORT:
            return "Does not support this board";
        case SIO_FAIL:
            return "PComm function run result fail";
        case SIO_ABORTWRITE:
            return "write has blocked, and user abort write";
        case SIO_WRITETIMEOUT:
            return "write timeoue has happened";
        default:
            assert(false);
        }

        return "";
    }

private:
    // Set the options of serial port.
    void setMode(int dataBits, const std::string parity, int stopBits)
    {
        // 参数检测
        assert(5<=dataBits && dataBits<=8);
        assert(1<=stopBits && stopBits<=2);
        assert(("None" == parity) || ("Odd" == parity) || ("Even" == parity)
            || ("Mark" == parity) || ("Space" == parity));

        dataBits -= 5;
        int iparity;

        switch(parity[0])
        {
        case 'N':
        default : iparity = P_NONE; break;
        case 'O': iparity = P_ODD; break;
        case 'E': iparity = P_EVEN; break;
        case 'M': iparity = P_MRK; break;
        case 'S': iparity = P_SPC; break;
        }

        switch(stopBits)
        {
        case 1 :
        default: stopBits = STOP_1; break;
        case 2 : stopBits = STOP_2; break;
        }

        // Set the basic options of serial port.
        q_errorCode = sio_ioctl(q_port, B9600, dataBits | iparity | stopBits);
        sio_baud(q_port, q_baud);
    }

    /**
    * @brief
    *	Set properties of one port to work.
    * @param	none.
    * @return	none.
    * @remark
    */
    void setPort(void)
    {
        int	hw = false ? 0x03 : 0;	// bit0 and bit1
        int	sw = false ? 0x0C : 0;	// bit2 and bit3
        bool dtr = true;
        bool rts = true;

        // Set the basic options of serial port.
        setMode(q_dataBits, q_parity, q_stopBits);
        if (q_errorCode != SIO_OK) { return ; }

        // Set the flow-control options of serial port.
        q_errorCode = sio_flowctrl(q_port, hw | sw);
        if (q_errorCode != SIO_OK) { return ; }

        // Set the status of DTR (0--DTR off; 1--DTR on)
        q_errorCode = sio_DTR(q_port, (dtr ? 1 : 0));
        if (q_errorCode != SIO_OK) { return ; }

        // Set the status of RTS (0--RTS off; 1--RTS on)
        if (!hw)
        {
            q_errorCode = sio_RTS(q_port, (rts ? 1 : 0));
            if (q_errorCode != SIO_OK) { return ; }
        }

        // Set the count of timeout to write
        long tout = 1000 / sio_getbaud(q_port);  // ms/byte
        if (tout < 1)
            tout = 1;
        tout *= 3;             // 1 byte; '*3' is for delay
        if (tout < 100)
            tout = 100;
        q_errorCode = sio_SetWriteTimeouts(q_port, tout);
    }

public:
    int getPort() const { return q_port; }
    int getBaud() const { return q_baud; }
    int getDataBits() const { return q_dataBits; }
    const std::string &getParity() const { return q_parity; }
    int getStopBits() const { return q_stopBits; }
    bool isOpen() const { return q_isOpen; }

private:
    int                 q_port;			// serial port number
    int                 q_baud;			// baud
    int                 q_dataBits;		// data bits
    std::string         q_parity;     	// parity
    int                 q_stopBits;		// stop bits
    bool                q_isOpen;		// has the serial port opened ?
    int                 q_errorCode;    // 错误码

public:
    static const int SizeRead  = 4096;  // 每次读取缓冲区字节数
    static const int SizeWrite = 4096;	// 每次写缓冲区字节数
};

#endif // _HR_SERIALPORT_H_
