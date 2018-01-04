#include "stdafx.h"
#include "kstring.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <algorithm>

#ifdef _WIN32
#define  vsnprintf _vsnprintf_s
#endif

kstring::kstring(void)
{
	m_sString = NULL;
	m_iLength = 0;
	m_iBufLen = 0;
}

kstring::kstring(const char* lpszStr)
{
	m_sString = NULL;
	m_iLength = 0;
	m_iBufLen = 0;
	if (lpszStr != NULL)
	{
		assign(lpszStr, strlen(lpszStr));
	}
}

kstring::kstring(const char* lpszStr, size_t nlen)
{
	m_sString = NULL;
	m_iLength = 0;
	m_iBufLen = 0;
	assign(lpszStr, nlen);
}


kstring::kstring(const kstring &sStr)
{
	m_sString = NULL;
	m_iLength = 0;
	m_iBufLen = 0;
	assign(sStr.m_sString, sStr.m_iLength);
}

kstring::~kstring(void)
{
	if (m_sString)
		free(m_sString);

	m_sString = NULL;
	m_iBufLen = 0;
	m_iLength = 0;
}

kstring& kstring::operator=(const kstring &sStr)
{
	assign(sStr.m_sString, sStr.m_iLength);

	return *this;
}

kstring& kstring::operator=(const char* lpszStr)
{

	assign(lpszStr, strlen(lpszStr));

	return *this;
}


// - Operator + overloading for : kstring + kstring
kstring kstring::operator +(const kstring &sStr)
{
	kstring	sTemp;
	sTemp.reserve(m_iLength + sStr.m_iLength);//防止多次malloc
	sTemp.append(m_sString, m_iLength);
	sTemp.append(sStr.m_sString, sStr.m_iLength);

	return sTemp;
}

// - Operator + overloading for : kstring + const char*
kstring kstring::operator +(const char* lpszStr)
{
	kstring	sTemp;

	size_t alen = strlen(lpszStr);
	sTemp.reserve(m_iLength + alen + 1);
	sTemp.append(m_sString, m_iLength);
	sTemp.append(lpszStr, alen);

	return sTemp;
}


// - Operator +=overloading for kstring

kstring& kstring::operator +=(const kstring &sStr)
{
	append(sStr.m_sString, sStr.m_iLength);
	return *this;
}

// - Operator +=overloading for const char*
kstring& kstring::operator +=(const char* lpszStr)
{
	if (lpszStr != NULL)
	{
		size_t alen = strlen(lpszStr);
		append(lpszStr, alen);
	}
	return *this;
}

// - Operator < overloading : return true if specified kstring's length is less than
//   this class string's length , otherwise false
bool kstring::operator <(const kstring &sStr)
{
	if (empty() && sStr.empty())
	{
		return false;
	}

	if (empty())
	{
		return true;
	}
	else if (sStr.empty())
	{
		return false;
	}
	else
	{
		size_t usize = min(m_iLength, sStr.m_iLength) + 1;
		if (memcmp(m_sString, sStr.m_sString, usize)<0)
		{
			return true;
		}
	}

	return false;

}

// - Operator <=overloading : return true if specified kstring's length is less than
//   or equal to this class string's length , otherwise false
bool kstring::operator <=(const kstring &sStr)
{
	if (empty() && sStr.empty())
	{
		return true;
	}

	if (empty())
	{
		return true;
	}
	else if (sStr.empty())
	{
		return false;
	}
	else
	{
		size_t usize = min(m_iLength, sStr.m_iLength) + 1;
		if (memcmp(m_sString, sStr.m_sString, usize) <= 0)
		{
			return true;
		}
	}

	return false;
}

// - Operator > overloading : return true if specified kstring's length is greater than
//   this class string's length , otherwise false
bool kstring::operator >(const kstring &sStr)
{
	if (empty() && sStr.empty())
	{
		return false;
	}

	if (empty())
	{
		return false;
	}
	else if (sStr.empty())
	{
		return true;
	}
	else
	{
		size_t usize = min(m_iLength, sStr.m_iLength) + 1;
		if (memcmp(m_sString, sStr.m_sString, usize)>0)
		{
			return true;
		}
	}

	return false;
}

// - Operator >=overloading : return true if specified kstring's length is greater than
//   or equal to this class string's length , otherwise false
bool kstring::operator >=(const kstring &sStr)
{
	if (empty() && sStr.empty())
	{
		return true;
	}

	if (empty())
	{
		return false;
	}
	else if (sStr.empty())
	{
		return true;
	}
	else
	{
		size_t usize = min(m_iLength, sStr.m_iLength) + 1;
		if (memcmp(m_sString, sStr.m_sString, usize) >= 0)
		{
			return true;
		}
	}

	return false;
}

// - Operator==overloading : return true if specified kstring length is equal to
//   this class string's length , otherwise return false
bool kstring::operator==(const kstring &sStr)
{
	if (empty() && sStr.empty())
	{
		return true;
	}
	if (!empty() && !sStr.empty())
	{
		size_t usize = min(m_iLength, sStr.m_iLength) + 1;
		if (memcmp(m_sString, sStr.m_sString, usize) == 0)
		{
			return true;
		}
	}

	return false;
}

// - Operator==overloading : return true if specified const char* length is equal to
//   this class string's length , otherwise return false
bool kstring::operator==(const char* lpszStr)
{
	if (empty() && !lpszStr)
	{
		return true;
	}
	if (!empty() && lpszStr != NULL&&lpszStr[0] != 0)
	{
		size_t usize = min(m_iLength, strlen(lpszStr)) + 1;
		if (memcmp(m_sString, lpszStr, usize) == 0)
		{
			return true;
		}
	}

	return false;
}

// - Operator !=overloading : return true if specified kstring length is equal to
//   this class string's length , otherwise return false
bool kstring::operator !=(const kstring &sStr)
{

	if (*this == sStr)
	{
		return false;
	}
	return true;
}

bool kstring::operator !=(const char* lpszStr)
{
	if (*this == lpszStr)
	{
		return false;
	}
	return true;
}

char& kstring::operator[](size_t index)
{
	if (index >= m_iLength)
	{
		throw 0;
	}
	return m_sString[index];
}

const char& kstring::operator[](size_t index) const
{
	if (index >= m_iLength)
	{
		throw 0;
	}
	return m_sString[index];
}


bool kstring::reserve(size_t nlen)
{
	size_t tlen = m_iLength + nlen + 1;
	if (m_iBufLen<tlen)
	{
		char *pt = (char*)realloc(m_sString, tlen);
		if (pt == NULL)
		{
			return false;
		}
		m_iBufLen = tlen;
		m_sString = pt;
	}
	return true;
}

void kstring::assign(const char* lpszStr, size_t nlen)
{
	if (lpszStr != NULL&&nlen>0)
	{
		if (m_iBufLen<nlen + 1)
		{
			m_iBufLen = nlen + 1;
			if (m_sString != NULL)
			{
				free(m_sString);
			}
			m_sString = (char*)malloc(m_iBufLen);
		}
		m_iLength = nlen;
		memcpy(m_sString, lpszStr, m_iLength);
		m_sString[m_iLength] = 0;
	}

}

void kstring::append(const char* lpszStr, size_t nlen)
{
	if (lpszStr != NULL&&nlen>0)
	{
		reserve(nlen);
		memcpy(m_sString + m_iLength, lpszStr, nlen);
		m_iLength += nlen;
		m_sString[m_iLength] = 0;
	}
}


void kstring::clear()
{
	m_iLength = 0;
	if (m_sString != NULL)
		m_sString[0] = 0;
}


// - Returns char in specified position

char kstring::getat(size_t iPos)  const
{
	if (iPos >= 0 && iPos < m_iLength)
		return m_sString[iPos];

	return 0;
}

// - Finds position of first specified char , if did not found return -1

int kstring::find(char ch)
{
	if (empty())
	{
		return -1;
	}

	char	*c = (char*)memchr(m_sString, ch, m_iLength);
	if (c != NULL)
	{
		return (int)(c - m_sString);
	}

	return -1;
}

// - Finds first position of specified kstring , if did not found return -1

int kstring::find(const kstring &sStr)
{
	if (empty())
	{
		return -1;
	}

	if (sStr.empty())
	{
		return 0;
	}

	char	*c = memmem(m_sString, m_iLength, sStr.m_sString, sStr.m_iLength);
	if (c != NULL)
	{
		return (int)(c - m_sString);
	}

	return -1;
}

// - Finds first position of specified string , if did not found return -1

int kstring::find(const char* lpszStr)
{

	if (empty())
	{
		return -1;
	}

	if (!lpszStr || !lpszStr[0])
	{
		return 0;
	}

	char	*c = memmem(m_sString, m_iLength, lpszStr, strlen(lpszStr));
	if (c != NULL)
	{
		return (int)(c - m_sString);
	}

	return -1;

}

// - replace an old char with a new char

size_t kstring::replace(char cOld, char cNew)
{
	size_t i, iTotal = 0;
	for (i = 0; i <= m_iLength; ++i)
	{
		if (m_sString[i] == cOld)
		{
			m_sString[i] = cNew;
			++iTotal;
		}
	}

	return iTotal;
}

// - replace an old string with a new string

size_t kstring::replace(const char* sOld, const char* sNew)
{
	if (!sNew || !sOld)
	{
		return 0;
	}

	return replace(sOld, strlen(sOld), sNew, strlen(sNew));
}


size_t kstring::replace(const kstring &sOld, const kstring &sNew)
{
	return replace(sOld.m_sString, sNew.m_sString);
}

size_t kstring::replace(const char *polddata, size_t oldlen, const char *pnewdata, size_t newlen)
{
	if (empty())
	{
		return 0;
	}

	size_t nnum(0);

	kstring sTemp;

	char *pbuf = m_sString;
	size_t buflen = m_iLength;
	while (buflen>0)
	{
		char *pc = memmem(pbuf, buflen, polddata, oldlen);
		if (pc == NULL)
		{
			if (sTemp.empty())
			{
				return 0;
			}

			sTemp.append(pbuf, buflen);
			break;
		}
		else
		{
			++nnum;
			size_t cplen = pc - pbuf;
			sTemp.append(pbuf, cplen);
			sTemp.append(pnewdata, newlen);

			pbuf = pc + oldlen;
			buflen = buflen - cplen - oldlen;
		}
	}
	*this = sTemp;

	return nnum;

}


// - Returns iCount chars from iStart

kstring kstring::mid(size_t iStart, size_t iCount)
{
	kstring	sTemp;

	if (iStart > m_iLength)
		return sTemp;

	if (iStart + iCount > m_iLength)
		return mid(iStart);

	sTemp.append(m_sString + iStart, iCount);

	return sTemp;
}

// - Returns chars from nStart till the end

kstring kstring::mid(size_t iStart)
{
	kstring	sTemp;

	if (iStart > m_iLength)
		return sTemp;

	sTemp.append(m_sString + iStart, m_iLength - iStart);

	return sTemp;
}

// - Returns iCount left chars

kstring kstring::left(size_t iCount)
{
	if (iCount > m_iLength)
		iCount = m_iLength;

	return mid(0, (int)iCount);
}

// - Returns iCount right chars

kstring kstring::right(size_t iCount)
{
	if (iCount > m_iLength)
		iCount = m_iLength;

	return mid(m_iLength - iCount, iCount);
}

// - Removes a char in specified position

bool kstring::erase(size_t iPos)
{
	return erase(iPos, iPos) == 1;
}

// - Removes chars from iStart to iEnd

size_t kstring::erase(size_t iStart, size_t iEnd)
{
	if (iStart >= m_iLength)
	{
		return 0;
	}

	if (iEnd >= m_iLength)
		iEnd = m_iLength - 1;

	size_t	iDelCount = iEnd - iStart + 1;

	memcpy(m_sString + iStart, m_sString + iEnd + 1, m_iLength - iEnd - 1);
	m_iLength -= iDelCount;
	m_sString[m_iLength] = 0;

	return iDelCount;
}



// - Trims left specified char

kstring& kstring::trimleft(const char *targets)
{
	if (targets == NULL)
	{
		targets = " \t\n\r";
	}
	if (!empty())
	{
		size_t npos;
		for (npos = 0; npos<m_iLength; ++npos)
		{
			if (strchr(targets, m_sString[npos]) == NULL)
			{
				--npos;
				break;
			}
		}
		if (npos>0)
		{
			erase(0, npos);
		}
	}

	return *this;
}


// - Trims right specified char

kstring& kstring::trimright(const char *targets)
{
	if (targets == NULL)
	{
		targets = " \t\n\r";
	}

	if (!empty())
	{
		for (size_t i = m_iLength - 1; i >= 0; --i)
		{
			if (strchr(targets, m_sString[i]) == NULL)
			{
				m_iLength = i + 1;
				break;
			}
		}
		m_sString[m_iLength] = 0;

	}

	return *this;
}

// - Trims left and right specified char
kstring& kstring::trim(const char *targets)
{
	//先trimright可减少内存拷贝
	trimright(targets);
	trimleft(targets);

	return *this;
}


// - Inserts a string in specified position
kstring& kstring::insert(const kstring &sStr, size_t iPos)
{
	if (!sStr.empty())
	{
		if (iPos >= m_iLength)
		{
			append(sStr);
		}
		else
		{
			reserve(sStr.m_iLength);

			kstring sTemp = mid(iPos);
			erase(iPos, -1);
			append(sStr);
			append(sTemp);
		}
	}

	return *this;
}

// - Converts to upper case
kstring& kstring::makeupper()
{
	if (!empty())
	{
		for (size_t i = 0; i<m_iLength; ++i)
		{
			if (m_sString[i] >= 'a'&&m_sString[i] <= 'z')
			{
				m_sString[i] -= 32;
			}
		}
	}
	return *this;
}

// - Converts to lower case
kstring& kstring::makelower()
{
	if (!empty())
	{
		for (size_t i = 0; i<m_iLength; ++i)
		{
			if (m_sString[i] >= 'A'&&m_sString[i] <= 'Z')
			{
				m_sString[i] += 32;
			}
		}
	}
	return *this;
}


// - Clears the string
bool kstring::empty()  const
{
	if (!m_sString || !m_iLength)
	{
		return true;
	}
	return false;

}

const char* kstring::c_str() const
{
	static const char* szempstr = "";
	if (empty())
	{
		return szempstr;
	}
	m_sString[m_iLength] = 0;
	return m_sString;
}


void kstring::Format(const char* pszCharSet, ...)
{

	char*	pszTemp = NULL;
	int		nRetVal = -1;

	va_list	vl;
	va_start(vl, pszCharSet);

	for (int nBufferSize = 1024; nRetVal < 0; nBufferSize *= 2)
	{
		if (pszTemp != NULL)
		{
			free(pszTemp);
		}
		pszTemp = (char*)malloc(nBufferSize + 1);
		nRetVal = vsnprintf(pszTemp, nBufferSize, _TRUNCATE, pszCharSet, vl);
	}
	va_end(vl);

	assign(pszTemp, nRetVal);
	free(pszTemp);

}




// memmem()
//
// Purpose:     Find a byte sequence within a memory buffer
//
// Parameters:  buf               - pointer to buffer
//              buf_len           - size of buffer in bytes
//              byte_sequence     - byte sequence to look for
//              byte_sequence_len - size of byte sequence in bytes
//
// Returns:     void * - if successful, returns a pointer to the first 
//                       occurrence of byte_sequence in buf;  otherwise, 
//                       returns NULL
//
// Notes;       Characters in byte_sequence and characters in buf will be
//              compared "as is", with no case conversion.

char *kstring::memmem(const char *buf, size_t buf_len, const char *byte_sequence, size_t byte_sequence_len)
{
	char *bf = (char *)buf;
	char *bs = (char *)byte_sequence;
	char *p = bf;

	while (byte_sequence_len <= (buf_len - (p - bf)))
	{
		unsigned int b = *bs & 0xFF;
		if ((p = (char *)memchr(p, b, buf_len - (p - bf))) != NULL)
		{
			if ((memcmp(p, byte_sequence, byte_sequence_len)) == 0)
				return p;
			else
				++p;
		}
		else
		{
			break;
		}
	}
	return NULL;
}

std::ostream& operator<<(std::ostream &temp, const kstring& sStr)
{
	temp.rdbuf()->sputn(sStr.c_str(), (std::streamsize)sStr.size());
	return temp;
}

