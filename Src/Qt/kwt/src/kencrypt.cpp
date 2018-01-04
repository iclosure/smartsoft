#include "precomp.h"
#include "kencrypt.h"
#include <memory.h>
#include <stdio.h>

typedef unsigned char	byte;
typedef unsigned int	uint32;

#pragma warning (disable : 4996)		// warning C4996 : xxx to xxx_s
#pragma warning (disable : 4819)		// warning C4819

// 初始值换表 ( IP )
const byte ip_table[64] =
{
    57, 49, 41, 33, 25, 17,  9,  1,
    59, 51, 43, 35, 27, 19, 11,  3,
    61, 53, 45, 37, 29, 21, 13,  5,
    63, 55, 47, 39, 31, 23, 15,  7,
    56, 48, 40, 32, 24, 16,  8,  0,
    58, 50, 42, 34, 26, 18, 10,  2,
    60, 52, 44, 36, 28, 20, 12,  4,
    62, 54, 46, 38, 30, 22, 14,  6,
};

// 逆初始值换表 ( IP^-1 )
const byte ip_1_table[64] =
{
    39,  7, 47, 15, 55, 23, 63, 31,
    38,  6, 46, 14, 54, 22, 62, 30,
    37,  5, 45, 13, 53, 21, 61, 29,
    36,  4, 44, 12, 52, 20, 60, 28,
    35,  3, 43, 11, 51, 19, 59, 27,
    34,  2, 42, 10, 50, 18, 58, 26,
    33,  1, 41,  9, 49, 17, 57, 25,
    32,  0, 40,  8, 48, 16, 56, 24,
};

// 扩充置换表 ( E )
const byte e_table[48] =
{
    31,  0,  1,  2,  3,  4,
     3,  4,  5,  6,  7,  8,
     7,  8,  9, 10, 11, 12,
    11, 12, 13, 14, 15, 16,
    15, 16, 17, 18, 19, 20,
    19, 20, 21, 22, 23, 24,
    23, 24, 25, 26, 27, 28,
    27, 28, 29, 30, 31,  0,
};

// 置换函数 ( P )
const byte p_table[32] =
{
    15,  6, 19, 20, 28, 11, 27, 16,
     0, 14, 22, 25,  4, 17, 30,  9,
     1,  7, 23, 13, 31, 26,  2,  8,
    18, 12, 29,  5, 21, 10,  3, 24,
};

// S 盒
const byte s_box[8][4][16] =
{
    // S1
    {{ 14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7 },
    {   0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8 },
    {   4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0 },
    {  15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13 }},
    // S2
    {{ 15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10 },
    {   3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5 },
    {   0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15 },
    {  13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9 }},
    // S3
    {{ 10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8 },
    {  13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1 },
    {  13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7 },
    {   1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12 }},
    // S4
    {{  7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15 },
    {  13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9 },
    {  10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4 },
    {   3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14 }},
    // S5
    {{  2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9 },
    {  14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6 },
    {   4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14 },
    {  11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3 }},
    // S6
    {{ 12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11 },
    {  10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8 },
    {   9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6 },
    {   4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13 }},
    // S7
    {{  4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1 },
    {  13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6 },
    {   1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2 },
    {   6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12 }},
    // S8
    {{ 13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7 },
    {   1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2 },
    {   7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8 },
    {   2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11 }},
};

// 置换选择 1
const byte pc_1[56] =
{
    56, 48, 40, 32, 24, 16,  8,
     0, 57, 49, 41, 33, 25, 17,
     9,  1, 58, 50, 42, 34, 26,
    18, 10,  2, 59, 51, 43, 35,
    62, 54, 46, 38, 30, 22, 14,
     6, 61, 53, 45, 37, 29, 21,
    13,  5, 60, 52, 44, 36, 28,
    20, 12,  4, 27, 19, 11,  3,
};

// 置换选择 48
const byte pc_2[56] =
{
    13, 16, 10, 23,  0,  4,  2, 27,
    14,  5, 20,  9, 22, 18, 11,  3,
    25,  7, 15,  6, 26, 19, 12,  1,
    40, 51, 30, 36, 46, 54, 29, 39,
    50, 44, 32, 46, 43, 48, 38, 55,
    33, 52, 45, 41, 49, 35, 28, 31,
};

// 对左移次数的规定
const byte move_times[16] =
{
     1,  1,  2,  2,  2,  2,  2,  2,
     1,  2,  2,  2,  2,  2,  2,  1,
};


//
// EncryptPrivate
//

class EncryptPrivate
{
public:
    explicit EncryptPrivate();
    ~EncryptPrivate();

    int byte_to_bit(const byte* src, byte *des, size_t n);
    int bit_to_byte(const byte* src, byte *des, size_t n);
    int des_transform(const byte* src, byte* des, const byte* table, size_t table_size);
    int des_pc1_transform(const byte src[64], byte des[56]);
    int des_pc2_transform(const byte src[56], byte des[48]);
    int des_ip_transform(byte data[64]);
    int des_ip_1_transform(byte data[64]);
    int des_e_transform(byte data[48]);
    int des_p_transform(byte data[32]);
    int des_s_box(const byte src[48], byte des[32]);
    int des_rol(byte data[56], int time);
    int des_xor(byte* a, const byte* b, size_t size);
    int des_swap(byte left[32], byte right[32]);
    int des_make_sub_keys(byte key[64], byte sub_keys[16][48]);
    int des_encrypt_block(byte plain_block[8], byte sub_keys[16][48], byte cipher_file[8]);
    int des_decrypt_block(byte cipher_block[8], byte sub_keys[16][48], byte plain_block[8]);

    int encrypt(const char* plain_file, const char* key_str, const char* cipher_file);
    int decrypt(const char* cipher_file, const char* key_str, const char* plain_file);

private:
    byte _temp[256];
};

EncryptPrivate::EncryptPrivate()
{
}

EncryptPrivate::~EncryptPrivate()
{
}

// 字节转换成二进制
int EncryptPrivate::byte_to_bit(const byte* src, byte *des, size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        des[i] = (src[i >> 3] >> (i & 0x07)) & 0x01;
    }

    return 0;
}

// 二进制转换成字节
int EncryptPrivate::bit_to_byte(const byte* src, byte *des, size_t n)
{
    memset(des, 0, (n >> 3));
    for (size_t i = 0; i < n; i++)
    {
        des[i >> 3] |= src[i] << (i & 0x07);
    }

    return 0;
}

// 通用化置换函数
int EncryptPrivate::des_transform(const byte* src, byte* des, const byte* table, size_t table_size)
{
    for (size_t i = 0; i < table_size; i++)
    {
        _temp[i] = src[table[i]];
    }

    memcpy(des, _temp, table_size);

    return 0;
}

// 密钥置换1
int EncryptPrivate::des_pc1_transform(const byte src[64], byte des[56])
{
    des_transform(src, des, pc_1, 56);

    return 0;
}

// 密钥置换2
int EncryptPrivate::des_pc2_transform(const byte src[56], byte des[48])
{
    des_transform(src, des, pc_2, 48);

    return 0;
}

// IP 置换
int EncryptPrivate::des_ip_transform(byte data[64])
{
    des_transform(data, data, ip_table, 64);

    return 0;
}

// IP 逆置换
int EncryptPrivate::des_ip_1_transform(byte data[64])
{
    des_transform(data, data, ip_1_table, 64);

    return 0;
}

// 扩展置换
int EncryptPrivate::des_e_transform(byte data[48])
{
    des_transform(data, data, e_table, 48);

    return 0;
}

// P 置换
int EncryptPrivate::des_p_transform(byte data[32])
{
    des_transform(data, data, p_table, 32);

    return 0;
}

// S盒置换
int EncryptPrivate::des_s_box(const byte src[48], byte des[32])
{
    for (size_t i = 0, r, c; i < 8 ; i++, src += 6, des += 4)
    {
        // 计算在S盒中的行、列
        r = (src[0] << 1) + src[5];
        c = (src[1] << 3) + (src[2] << 2) + (src[3] << 1) + src[4];

        // 转换为二进制
        byte_to_bit(&s_box[i][r][c], des, 4);
    }

    return 0;
}

// 循环左移
int EncryptPrivate::des_rol(byte data[56], int time)
{
    // 保存将要循环移动到右边的位
    memcpy(_temp, data, time);
    memcpy(_temp + time, data + 28, time);

    // 前28位移动
    memcpy(data, data + time, 28 - time);
    memcpy(data + 28 - time, _temp, time);

    // 后28位移动
    memcpy(data + 28, data + 28 + time, 28 - time);
    memcpy(data + 56 - time, _temp + time, time);

    return 0;
}

// 异或运算
int EncryptPrivate::des_xor(byte* a, const byte* b, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        a[i] ^= b[i];
    }

    return 0;
}

// 交换运算
int EncryptPrivate::des_swap(byte left[32], byte right[32])
{
    memcpy(_temp, left, 32);
    memcpy(left, right, 32);
    memcpy(right, _temp, 32);

    return 0;
}

// 生成子密钥
int EncryptPrivate::des_make_sub_keys(byte key[64], byte sub_keys[16][48])
{
    // PC1置换
    des_pc1_transform(key, _temp);

    // 16轮迭代，产生16个子密钥
    for (int i=0; i < 16; i++)
    {
        des_rol(_temp, move_times[i]);			// 循环左移
        des_pc2_transform(_temp, sub_keys[i]);	// PC2置换，产生子密钥
    }

    return 0;
}

// 加密单个分组
int EncryptPrivate::des_encrypt_block(byte plain_block[8], byte sub_keys[16][48], byte cipher_block[8])
{
    byte pain_bits[64];
    byte copy_right[48];

    byte_to_bit(plain_block, pain_bits, 64);

    // 初始值换 ( IP 置换 )
    des_ip_transform(pain_bits);

    // 16轮迭代
    for (int i = 0; i < 16 ; i++)
    {
        memcpy(copy_right, pain_bits + 32, 32);

        // 将右半部分进行扩展置换，从32位扩展到48位
        des_e_transform(copy_right);

        // 将右半部分与子密钥进行异或运算
        des_xor(copy_right, sub_keys[i], 48);

        // 异或结果进入S盒，输出32位结果
        des_s_box(copy_right, copy_right);

        // P 置换
        des_p_transform(copy_right);

        // 将明文左半部分与右半部分进行异或运算
        des_xor(pain_bits, copy_right, 32);

        if (i != 15)
        {
            // 最终完成左右部分的交换
            des_swap(pain_bits, pain_bits + 32);
        }
    }

    // 逆初始置换 ( IP^-1置换 )
    des_ip_1_transform(pain_bits);
    bit_to_byte(pain_bits, cipher_block, 64);

    return 0;
}

// 解密单个分组
int EncryptPrivate::des_decrypt_block(byte cipher_block[8], byte sub_keys[16][48], byte plain_block[8])
{
    byte cipher_bits[64];
    byte copy_right[48];

    byte_to_bit(cipher_block, cipher_bits, 64);

    // 初始置换 ( IP 置换 )
    des_ip_transform(cipher_bits);

    // 16轮迭代
    for (int i = 15; i >= 0 ; i--)
    {
        memcpy(copy_right, cipher_bits + 32, 32);

        // 将右半部分进行扩展置换，从32位扩展到48位
        des_e_transform(copy_right);

        // 将右半部分与子密钥进行异或运算
        des_xor(copy_right, sub_keys[i], 48);

        // 异或结果进入S盒，输出32位结果
        des_s_box(copy_right, copy_right);

        // P 置换
        des_p_transform(copy_right);

        // 将明文左半部分与右半部分进行异或运算
        des_xor(cipher_bits, copy_right, 32);

        if (i != 0)
        {
            // 最终完成左右部分的交换
            des_swap(cipher_bits, cipher_bits + 32);
        }
    }

    // 逆初始置换 ( IP^-1置换 )
    des_ip_1_transform(cipher_bits);
    bit_to_byte(cipher_bits, plain_block, 64);

    return 0;
}

// 加密文件
int EncryptPrivate::encrypt(const char* plain_file, const char* key_str, const char* cipher_file)
{
    FILE *plain, *cipher;
    int count;
    byte plain_block[8], cipher_block[8], key_block[8];
    byte key[64];
    byte sub_keys[16][48];

    if ((plain = fopen(plain_file, "rb")) == 0)
    {
        return KEncrypt::DesResultPlainFileError;
    }

    if ((cipher = fopen(cipher_file, "wb")) == 0)
    {
        return KEncrypt::DesResultCipherFileOpenError;
    }

    // 设置密钥
    memcpy(key_block, key_str, 8);

    // 将密钥转换为二进制流
    byte_to_bit(key_block, key, 64);

    // 生成子密钥
    des_make_sub_keys(key, sub_keys);

    while (!feof(plain))
    {
        // 每次读取8个字节，并返回成功读取的字节数
        if ((count = fread(plain_block, sizeof(byte), 8, plain)) == 8)
        {
            des_encrypt_block(plain_block, sub_keys, cipher_block);
            fwrite(cipher_block, sizeof(byte), 8, cipher);
        }
    }

    if (count > 0)
    {
        // 填充
        memset(plain_block + count, '\0', 7 - count);

        // 最后一个字节保存包括最后一个字符在内的所填充的字符数
        plain_block[7] = 8 - count;
        des_encrypt_block(plain_block, sub_keys, cipher_block);
        fwrite(cipher_block, sizeof(byte), 8, cipher);
    }

    fclose(plain);
    fclose(cipher);
    return KEncrypt::DesResultSuccess;
}

// 解密文件
int EncryptPrivate::decrypt(const char* cipher_file, const char* key_str, const char* plain_file)
{
    FILE *plain, *cipher;
    int count, times = 0;
    long file_len;
    byte plain_block[8], cipher_block[8], key_block[8];
    byte key[64];
    byte sub_keys[16][48];

    if ((cipher = fopen(cipher_file, "rb")) == 0)
    {
        return KEncrypt::DesResultCipherFileOpenError;
    }

    if ((plain = fopen(plain_file, "wb")) == 0)
    {
        return KEncrypt::DesResultPlainFileError;
    }

    // 设置密钥
    memcpy(key_block, key_str, 8);

    // 将密钥转换为二进制流
    byte_to_bit(key_block, key, 64);

    // 生成子密钥
    des_make_sub_keys(key, sub_keys);

    // 取文件长度
    fseek(cipher, 0, SEEK_END);		// 将文件指针置尾
    file_len = ftell(cipher);		// 读取文件指针的当前位置
    rewind(cipher);					// 将文件指针重新指向文件头位置

    while (true)
    {
        // 密文的字节数一定是8的整数倍
        fread(cipher_block, sizeof(byte), 8, cipher);

        des_decrypt_block(cipher_block, sub_keys, plain_block);

        times += 8;
        if (times < file_len)		// 是否已处理到文件末尾
        {
            fwrite(plain_block, sizeof(byte), 8, plain);
        }
        else
        {
            break;					// 已处理到文件末尾，退出循环
        }
    }

    // 判断末尾是否被填充
    if (plain_block[7] < 8)
    {
        for (count = 8 - plain_block[7]; count < 7; count++)
        {
            if (plain_block[count] != '\0')
            {
                break;
            }
        }
    }

    if (count == 7)
    {
        // 有填充
        fwrite(plain_block, sizeof(byte), 8 - plain_block[7], plain);
    }
    else
    {
        // 无填充
        fwrite(plain_block, sizeof(byte), 8, plain);
    }

    fclose(plain);
    fclose(cipher);

    return KEncrypt::DesResultSuccess;
}

//
// KEncrypt
//


KEncrypt::KEncrypt(QObject *parent) :
    QObject(parent)
{
    d = new EncryptPrivate();
}

KEncrypt::~KEncrypt(void)
{
    delete d;
}

// 加密文件
int KEncrypt::encrypt(const char* plain_file, const char* key_str, const char* cipher_file)
{
    return d->encrypt(plain_file, key_str, cipher_file);
}

// 解密文件
int KEncrypt::decrypt(const char* cipher_file, const char* key_str, const char* plain_file)
{
    return d->decrypt(cipher_file, key_str, plain_file);
}
