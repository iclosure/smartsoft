// kstring.h: interface of the kstring class.
//
//////////////////////////////////////////////////////////////////////
//////////			Created by		:	ding jian zhong		//////////
//////////			Email			:	djzbj@163.com		//////////
//////////			Creation Date	:	2011-11-1			//////////
//////////			Last Update		:	2011-11-1			//////////
//////////			Description		:	kstring class		//////////
//////////////////////////////////////////////////////////////////////

#ifndef SK_KSTRING_H 
#define SK_KSTRING_H 

#include <stdlib.h>
#include <ostream>
#include "vccore_global.h"

class VCCORE_EXPORT kstring
{
public:
	// - Constructions:
	kstring(void);
	kstring(const char* lpszStr);
	kstring(const char* lpszStr, size_t nlen);
	kstring(const kstring &sStr);

	// - Destruction
	~kstring(void);

	// - Operators Overloading
	kstring& operator =(const kstring &sStr);			// - Operator = overloading for kstring
	kstring& operator =(const char* lpszStr);				// - Operator = overloading for string
	kstring  operator +(const kstring &sStr);			// - Operator + overloading : kstring + kstring
	kstring  operator +(const char* lpszStr);				// - Operator + overloading : kstring + const char*
	kstring& operator+=(const kstring &sStr);			// - Operator +=overloading for kstring
	kstring& operator+=(const char* lpszStr);				// - Operator +=overloading for const char*

	// - Operations
	bool operator <(const kstring &sStr);				// - Operator < overloading
	bool operator <=(const kstring &sStr);				// - Operator <= overloading
	bool operator >(const kstring &sStr);				// - Operator > overloading
	bool operator >=(const kstring &sStr);				// - Operator >= overloading
	bool operator==(const kstring &sStr);				// - Operator ==overloading for kstring
	bool operator==(const char* sStr);						// - Operator ==overloading for const char*
	bool operator!=(const kstring &sStr);				// - Operator !=overloading for kstring
	bool operator!=(const char* sStr);						// - Operator !=overloading for const char*
	char& operator[](size_t index);						//sStr[2]='c'
	const char& operator[](size_t index) const;         //const kstring d("123"); char ch=d[2]

	bool		reserve(size_t nlen);						//assert nlen byte space 
	void		assign(const char* lpszStr, size_t nlen);   //copy str to this
	void		append(const char* lpszStr, size_t nlen);   //append str to this
	void		append(const kstring & sStr){ append(sStr.m_sString, sStr.m_iLength); };//append kstring to this
	void		clear();
	char		getat(size_t iPos)  const;						// - Returns char in specified position
	int			find(char ch);							// - Finds first position of specified char
	int			find(const kstring &sStr);				// - Finds first position of specified kstring
	int			find(const char* lpszStr);					// - Finds first position of specified string
	size_t		replace(char cOld, char cNew);		// - replace an old char with a new char
	size_t		replace(const char* sOld, const char* sNew);	// - replace an old string with a new string
	size_t		replace(const char *buf, size_t buf_len, const char *byte_sequence, size_t byte_sequence_len); //Ìæ»»×Ö·û´®;
	size_t		replace(const kstring &sOld, const kstring &sNew);// - replace an old kstring with a new kstring
	kstring		mid(size_t iStart, size_t iCount);			// - Returns iCount chars from iStart
	kstring		mid(size_t iStart);						// - Returns chars from nStart till the end
	kstring		left(size_t iCount);						// - Returns iCount left chars
	kstring		right(size_t iCount);						// - Returns iCount right chars
	bool		erase(size_t iPos);						// - Removes a char in specified position
	size_t		erase(size_t iStart, size_t iEnd);			// - Removes chars from nStart to nEnd
	kstring&	trimleft(const char *targets = NULL);				// - Trims left specified char
	kstring&	trimright(const char *targets = NULL);				// - Trims right specified char
	kstring&	trim(const char *targets = NULL);					// - Trims left and right specified char
	kstring&	insert(const kstring &sStr, size_t iPos);	// - Inserts a string in specified position
	kstring&	makeupper();							// - Converts to upper case
	kstring&	makelower();							// - Converts to lower case
	bool		empty() const;							// check is or not empty

	size_t		size() const{ return m_iLength; }			// - Return length of the string
	size_t		length() const{ return m_iLength; }		// - Return size of the string
	const char* c_str()  const;


	void		Format(const char* pszCharSet, ...);		// - Format string

private:
	static char *memmem(const char *buf, size_t buf_len, const char *byte_sequence, size_t byte_sequence_len);

private:// - Data Members
	char*		m_sString;								// - Holds the string data
	size_t		m_iLength;								// data length
	size_t		m_iBufLen;								// m_sString buffer size
};

//for stream output
std::ostream& operator<<(std::ostream &temp, const kstring& sStr);

#endif  // SK_KSTRING_H
