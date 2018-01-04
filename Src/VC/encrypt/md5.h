#pragma once

// ********************************************
// name: 
// function: the encryption  of software
// description: 
// author: Hi ROBOT
// date£º2013-04-21 16:04
// 
// how to use the module:
//   MD5 md5;								// define MD5 object.
//   std::string input= "12345678";			// input string.
//   md5.update(input);						// update the string of input.
//   std::string output = md5.toString();	// get the string output by using md5.toString().
// 
// ********************************************

#include "encrypt_global.h"
#include <string>
#include <fstream>

namespace encrypt
{
	class MD5Private;

	// MD5
	class DLLENCRYPT_API MD5
	{
	public:
		explicit MD5();
		explicit MD5(const void* input, size_t length);
		explicit MD5(const std::string& input);
		explicit MD5(std::ifstream& in);
		~MD5();

		void reset();
		void update(const void* input, size_t length);
		void update(const std::string& input);
		void update(std::ifstream& input);
		const unsigned char* digest();
		std::string toString();
		
	private:
		HR_DISABLE_COPY(MD5)
		MD5Private* d;
	};
}

