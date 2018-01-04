#pragma once

// ********************************************
// name: 
// function: the encryption  of software
// description: 
// author: Hi ROBOT
// date：2013-04-21 16:04
// 
// how to use the module:
//   CDES des;								// define CDES object.
//   clock_t start, end;
//   encrypt::CDES des;
// 
//   start = clock();
//   des.des_encrypt("1.txt", "key.txt", "2.txt");
//   end = clock();
//   printf("加密消耗%d毫秒\n", end - start);
// 
//   system("pause");
// 
//   start =clock();
//   des.des_decrypt("2.txt", "key.txt", "3.txt");
//   end = clock();
//   printf("解密消耗%d毫秒\n", end - start);
// 
// ********************************************

// DES (Data Encrypt Standard, 数据库加密标准)

#include "encrypt_global.h"

namespace encrypt
{
	enum
	{
		DesResultCipherFileOpenError = -2,
		DesResultPlainFileError = -1,
		DesResultSuccess = 1,

	} DesResult;

	// CDES

	class CDESPrivate;

	class DLLENCRYPT_API CDES
	{
	public:
		explicit CDES(void);
		~CDES(void);

		int encrypt(const char* plain_file, const char* key_str, const char* cipher_file);
		int decrypt(const char* cipher_file, const char* key_str, const char* plain_file);

	private:
		HR_DISABLE_COPY(CDES)
		CDESPrivate* d;
	};
}
