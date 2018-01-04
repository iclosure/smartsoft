#ifndef KENCRYPT_H
#define KENCRYPT_H

// ********************************************
// name:
// function: the encryption  of software
// description:
// author: Hi ROBOT
// date: 2013-04-21 16:04
//
// how to use the module:
//   KEncrypt des;				 // define KEncrypt object.
//   clock_t start, end;
//
//   start = clock();
//   des.des_encrypt("1.txt", "key.txt", "2.txt");
//   end = clock();
//
//   system("pause");
//
//   start =clock();
//   des.des_decrypt("2.txt", "key.txt", "3.txt");
//   end = clock();
//
// ********************************************

#include "kwt_global.h"
#include <QObject>

class EncryptPrivate;

class KWT_EXPORT KEncrypt : public QObject
{
    Q_OBJECT

public:
    enum DesResult{
        DesResultCipherFileOpenError = -2,
        DesResultPlainFileError = -1,
        DesResultSuccess = 1
    };
public:
    explicit KEncrypt(QObject *parent = 0);
    ~KEncrypt(void);

    int encrypt(const char* plain_file, const char* key_str, const char* cipher_file);
    int decrypt(const char* cipher_file, const char* key_str, const char* plain_file);

private:
    Q_DISABLE_COPY(KEncrypt)
    EncryptPrivate* d;
};

#endif // KENCRYPT_H
