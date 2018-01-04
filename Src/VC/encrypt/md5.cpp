
#include "md5.h"

// ROTATE_LEFT rotates x left n bits.
#define MD5_ROTATE_LEFT(x, n)	(((x) << (n)) | ((x) >> (32 - (n))))

// F, G, H and I are basic MD5 functions.
#define MD5_F(x, y, z)		(((x) & (y)) | ((~x) & (z)))
#define MD5_G(x, y, z)		(((x) & (z)) | ((y) & (~z)))
#define MD5_H(x, y, z)		((x) ^ (y) ^ (z))
#define MD5_I(x, y, z)		((y) ^ ((x) | (~z)))

// FF, GG, HH and II transformations for rounds 1, 2, 3 and 4.
// Rotation is separate from addition to prevent recomputation.
#define MD5_FF(a, b, c, d, x, s, ac)	{		\
	(a) += MD5_F((b), (c), (d)) + (x) + (ac);	\
	(a)  = MD5_ROTATE_LEFT((a), (s));			\
	(a) += (b);									\
}

#define MD5_GG(a, b, c, d, x, s, ac)	{		\
	(a) += MD5_G((b), (c), (d)) + (x) + (ac);	\
	(a)  = MD5_ROTATE_LEFT((a), (s));			\
	(a) += (b);									\
}

#define MD5_HH(a, b, c, d, x, s, ac)	{		\
	(a) += MD5_H((b), (c), (d)) + (x) + (ac);	\
	(a)  = MD5_ROTATE_LEFT((a), (s));			\
	(a) += (b);									\
}

#define MD5_II(a, b, c, d, x, s, ac)	{		\
	(a) += MD5_I((b), (c), (d)) + (x) + (ac);	\
	(a)  = MD5_ROTATE_LEFT((a), (s));			\
	(a) += (b);									\
}

namespace encrypt
{
	typedef unsigned char	byte;
	typedef unsigned int	uint32;

	// constants for MD5 Transform routine
	enum {
		S11 =  7, S12 = 12, S13 = 17, S14 = 22,
		S21 =  5, S22 =  9, S23 = 14, S24 = 20,
		S31 =  4, S32 = 11, S33 = 16, S34 = 23,
		S41 =  6, S42 = 10, S43 = 15, S44 = 21,
	};

	// 
	// MD5Private
	// 

	class MD5Private
	{
	public:
		MD5Private();
		~MD5Private();

		void update(const byte* input, size_t length);
		void final();
		void transform(const byte block[64]);
		void encode(const uint32* input, byte* output, size_t length);
		void decode(const byte* input, uint32* output, size_t length);
		std::string bytesToHexString(const byte* input, size_t length);

		void reset();

	private:
		uint32	_state[4];		// state (ABCD)
		uint32	_count[2];		// number of bit, module 2^64 (low-order word first)
		byte	_buffer[64];	// input buffer
		byte	_digest[16];	// message digest
		bool	_finished;		// calculate finished ?

		static const byte MD5_PADDING[64];	// padding for calculate
		static const char MD5_HEX[16];		// 

		enum { BUFFER_SIZE = 1024, };

		friend class MD5;
	};

	// 
	const byte MD5Private::MD5_PADDING[64] = { 0x80 };
	const char MD5Private::MD5_HEX[16] =
	{
		'0', '1', '2', '3',
		'4', '5', '6', '7',
		'8', '9', 'a', 'b',
		'c', 'd', 'e', 'f',
	};

	MD5Private::MD5Private()
	{
	}

	MD5Private::~MD5Private()
	{
	}

	// Return the calculate state.
	void MD5Private::reset()
	{
		_finished = false;

		std::memset(_state, 0, sizeof(_state));
		std::memset(_count, 0, sizeof(_count));		// // reset number of bits.
		std::memset(_buffer, 0, sizeof(_buffer));
		std::memset(_digest, 0, sizeof(_digest));

		// Load magic initialization constants.
		_state[0] = 0x67452301;
		_state[1] = 0xefcdab89;
		_state[2] = 0x98badcfe;
		_state[3] = 0x10325476;
	}

	// MD5 block update operation. Continues an MD5 message-digest
	// operation, processing another message block, and updating the 
	// context.
	void MD5Private::update(const byte* input, size_t length)
	{
		uint32 i, index, partLen;
		
		_finished = false;

		// Compute number of bytes mod 64.
		index = (uint32) ((_count[0] >> 3) & 0x3f);

		// update number of bits.
		if ((_count[0] += ((uint32) length << 3)) < ((uint32) length << 3))
		{
			_count[1] ++;
		}

		_count[1] += (uint32) length >> 29;

		partLen = 64 - index;
		// transform as many times as possible.
		if (length >= partLen)
		{
			std::memcpy(&_buffer[index], input, partLen);
			transform(_buffer);
			
			for (i=partLen; i + 63 < length; i += 64)
			{
				transform(&input[i]);
			}

			index = 0;
		}
		else
		{
			i = 0;
		}

		// Buffer remaining input.
		std::memcpy(&_buffer[index], &input[i], length - i);
	}

	// MD5 finalization Ends as MD5 message-digest operation writing the
	// the message-digest and zeroing the context.
	void MD5Private::final()
	{
		byte bits[8];
		uint32 oldState[4];
		uint32 oldCount[2];
		uint32 index, padLen;

		// Pad out to 56 mod 64.
		std::memcpy(oldState, _state, sizeof(oldState));
		std::memcpy(oldCount, _count, sizeof(oldCount));

		// Save number of bits.
		encode(_count, bits, sizeof(bits));

		// Pad out to 56 mod 64.
		index = (uint32) ((_count[0] >> 3) & 0x3f);
		padLen = (index < 56) ? (56 - index) : (120 - index);
		update(MD5_PADDING, padLen);

		// Append length (before padding).
		update(bits, sizeof(bits));

		// Store state in digest.
		encode(_state, _digest, sizeof(_digest));

		// Restore current state and count.
		std::memcpy(_state, oldState, sizeof(oldState));
		std::memcpy(_count, oldCount, sizeof(oldCount));
	}

	// MD5 basic transformation. Transforms _state based on block.
	void MD5Private::transform(const byte block[64])
	{
		uint32 a = _state[0];
		uint32 b = _state[1];
		uint32 c = _state[2];
		uint32 d = _state[3];
		uint32 x[16];

		decode(block, x, sizeof(x));

		// Round 1
		MD5_FF(a, b, c, d, x[ 0], S11, 0xd76aa478);	// 1
		MD5_FF(d, a, b, c, x[ 1], S12, 0xe8c7b756);	// 2
		MD5_FF(c, d, a, b, x[ 2], S13, 0x242070db);	// 3
		MD5_FF(b, c, d, a, x[ 3], S14, 0xc1bdceee);	// 4

		MD5_FF(a, b, c, d, x[ 4], S11, 0xf57c0faf);	// 5
		MD5_FF(d, a, b, c, x[ 5], S12, 0x4787c62a);	// 6
		MD5_FF(c, d, a, b, x[ 6], S13, 0xa8304613);	// 7
		MD5_FF(b, c, d, a, x[ 7], S14, 0xfd469501);	// 8

		MD5_FF(a, b, c, d, x[ 8], S11, 0x698098d8);	// 9
		MD5_FF(d, a, b, c, x[ 9], S12, 0x8b44f7af);	// 10
		MD5_FF(c, d, a, b, x[10], S13, 0xffff5bb1);	// 11
		MD5_FF(b, c, d, a, x[11], S14, 0x895cd7be);	// 12

		MD5_FF(a, b, c, d, x[12], S11, 0x6b901122);	// 13
		MD5_FF(d, a, b, c, x[13], S12, 0xfd987193);	// 14
		MD5_FF(c, d, a, b, x[14], S13, 0xa679438e);	// 15
		MD5_FF(b, c, d, a, x[15], S14, 0x49b40821);	// 16

		// Round 2
		MD5_GG(a, b, c, d, x[ 1], S21, 0xf61e2562);	// 17
		MD5_GG(d, a, b, c, x[ 6], S22, 0xc040b340);	// 18
		MD5_GG(c, d, a, b, x[11], S23, 0x265e5a51);	// 19
		MD5_GG(b, c, d, a, x[ 0], S24, 0xe9b6c7aa);	// 20

		MD5_GG(a, b, c, d, x[ 5], S21, 0xd62f105d);	// 21
		MD5_GG(d, a, b, c, x[10], S22, 0x02441453);	// 22
		MD5_GG(c, d, a, b, x[15], S23, 0xd8a1e681);	// 23
		MD5_GG(b, c, d, a, x[ 4], S24, 0xe7d3fbc8);	// 24

		MD5_GG(a, b, c, d, x[ 9], S21, 0x21e1cde6);	// 25
		MD5_GG(d, a, b, c, x[14], S22, 0xc33707d6);	// 26
		MD5_GG(c, d, a, b, x[ 3], S23, 0xf4d50d87);	// 27
		MD5_GG(b, c, d, a, x[ 8], S24, 0x455a14ed);	// 28

		MD5_GG(a, b, c, d, x[13], S21, 0xa9e3e905);	// 29
		MD5_GG(d, a, b, c, x[ 2], S22, 0xfcefa3f8);	// 30
		MD5_GG(c, d, a, b, x[ 7], S23, 0x676f02d9);	// 31
		MD5_GG(b, c, d, a, x[12], S24, 0x8d2a4c8a);	// 32

		// Round 3
		MD5_HH(a, b, c, d, x[ 5], S31, 0xfffa3942);	// 33
		MD5_HH(d, a, b, c, x[ 8], S32, 0x8771f681);	// 34
		MD5_HH(c, d, a, b, x[11], S33, 0x6d9d6122);	// 35
		MD5_HH(b, c, d, a, x[14], S34, 0xfde5380c);	// 36

		MD5_HH(a, b, c, d, x[ 1], S31, 0xa4beea44);	// 37
		MD5_HH(d, a, b, c, x[ 4], S32, 0x4bdecfa9);	// 38
		MD5_HH(c, d, a, b, x[ 7], S33, 0xf6bb4b60);	// 39
		MD5_HH(b, c, d, a, x[10], S34, 0xbebfbc70);	// 40

		MD5_HH(a, b, c, d, x[13], S31, 0x289b7ec6);	// 41
		MD5_HH(d, a, b, c, x[ 0], S32, 0xeaa127fa);	// 42
		MD5_HH(c, d, a, b, x[ 3], S33, 0xd4ef3085);	// 43
		MD5_HH(b, c, d, a, x[ 6], S34, 0x04881d05);	// 44

		MD5_HH(a, b, c, d, x[ 9], S31, 0xd9d4d039);	// 45
		MD5_HH(d, a, b, c, x[12], S32, 0xe6db99e5);	// 46
		MD5_HH(c, d, a, b, x[15], S33, 0x1fa27cf8);	// 47
		MD5_HH(b, c, d, a, x[ 2], S34, 0xc4ac5665);	// 48

		// Round 4
		MD5_II(a, b, c, d, x[ 0], S41, 0xf4292244);	// 49
		MD5_II(d, a, b, c, x[ 7], S42, 0x432aff97);	// 50
		MD5_II(c, d, a, b, x[14], S43, 0xab9423a7);	// 51
		MD5_II(b, c, d, a, x[ 5], S44, 0xfc93a039);	// 52

		MD5_II(a, b, c, d, x[12], S41, 0x655b59c3);	// 53
		MD5_II(d, a, b, c, x[ 3], S42, 0x8f0ccc92);	// 54
		MD5_II(c, d, a, b, x[10], S43, 0xffeff47d);	// 55
		MD5_II(b, c, d, a, x[ 1], S44, 0x85845dd1);	// 56

		MD5_II(a, b, c, d, x[ 8], S41, 0x6fa87e4f);	// 57
		MD5_II(d, a, b, c, x[15], S42, 0xfe2ce6e0);	// 58
		MD5_II(c, d, a, b, x[ 6], S43, 0xa3014314);	// 59
		MD5_II(b, c, d, a, x[13], S44, 0x4e0811a1);	// 60

		MD5_II(a, b, c, d, x[ 4], S41, 0xf7537e82);	// 61
		MD5_II(d, a, b, c, x[11], S42, 0xbd3af235);	// 62
		MD5_II(c, d, a, b, x[ 2], S43, 0x2ad7d2bb);	// 63
		MD5_II(b, c, d, a, x[ 9], S44, 0xeb86d391);	// 64

		// 
		_state[0] += a;
		_state[1] += b;
		_state[2] += c;
		_state[3] += d;
	}

	// Encode input (ulong) into output (byte). Assumes length is a multiple of 4.
	void MD5Private::encode(const uint32* input, byte* output, size_t length)
	{
		for (size_t i = 0, j = 0; j < length; i++, j += 4)
		{
			output[    j] = (byte) ( input[i]        & 0xff);
			output[j + 1] = (byte) ((input[i] >>  8) & 0xff);
			output[j + 2] = (byte) ((input[i] >> 16) & 0xff);
			output[j + 3] = (byte) ((input[i] >> 24) & 0xff);
		}
	}

	// Decode input (byte) into output (ulong). Assumes length is a multiple of 4.
	void MD5Private::decode(const byte* input, uint32* output, size_t length)
	{
		for (size_t i = 0, j = 0; j < length; i++, j += 4)
		{
			output[i] = 
				( (uint32) input[j]) | 
				(((uint32) input[j + 1]) << 8) | 
				(((uint32) input[j + 2]) << 16) | 
				(((uint32) input[j + 3]) << 24);
		}
	}

	// Convert byte array to hex string.
	std::string MD5Private::bytesToHexString(const byte* input, size_t length)
	{
		std::string str;

		str.reserve(length << 1);
		for (size_t i = 0; i < length; i++)
		{
			int t = input[i];
			str.append(1, MD5_HEX[t / 16]);
			str.append(1, MD5_HEX[t % 16]);
		}

		return str;
	}

	// 
	// MD5
	// 

	// Default construct.
	MD5::MD5()
	{
		reset();
	}

	// Construct a MD5 object with a input buffer.
	MD5::MD5(const void* input, size_t length)
	{
		reset();
		update(input, length);
	}
	
	// Construct a MD5 object with a string.
	MD5::MD5(const std::string& input)
	{
		reset();
		update(input);
	}

	// Construct a MD5 object with a file.
	MD5::MD5(std::ifstream& in)
	{
		reset();
		update(in);
	}

	// deconstruction
	MD5::~MD5()
	{
	}

	// Return the calculate state.
	void MD5::reset()
	{
		d->reset();
	}

	// Updating the context with a input buffer.
	void MD5::update(const void* input, size_t length)
	{
		d->update((const byte*)input, length);
	}

	// Updating the context with a string.
	void MD5::update(const std::string& input)
	{
		d->update((const byte*)input.c_str(), input.length());
	}

	// Updating the context with a file.
	void MD5::update(std::ifstream& input)
	{
		if (! input)
		{
			return ;
		}

		std::streamsize length;
		char buffer[MD5Private::BUFFER_SIZE];
		while(! input.eof())
		{
			input.read(buffer, MD5Private::BUFFER_SIZE);
			length = input.gcount();
			if (length > 0)
			{
				d->update((byte*)buffer, (size_t)length);
			}
		}

		input.close();
	}

	// Return the message-digest
	const byte* MD5::digest()
	{
		if (!d->_finished)
		{
			d->_finished = true;
			d->final();
		}

		return d->_digest;
	}

	// Convert digest to string value.
	std::string MD5::toString()
	{
		return d->bytesToHexString(digest(), sizeof(d->_digest));
	}
}

