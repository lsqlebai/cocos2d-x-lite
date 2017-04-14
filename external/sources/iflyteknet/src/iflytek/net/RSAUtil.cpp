#include "RSAUtil.h"
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <stdio.h>
#include <string> 
#include <iostream> 

#include <math.h>

#pragma comment(lib, "libeay32.lib")
#pragma comment(lib, "ssleay32.lib") 
int padding = RSA_PKCS1_PADDING;

RSA * createRSA(unsigned char * key, int isPublic)
{
	RSA *rsa = NULL;
	BIO *keybio;
	keybio = BIO_new_mem_buf(key, -1);
	if (keybio == NULL)
	{
		printf("Failed to create key BIO");
		return 0;
	}
	if (isPublic)
	{
		rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
	}
	else
	{
		rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);
	}
	if (rsa == NULL)
	{
		printf("Failed to create RSA");
	}

	return rsa;
}

int RSAUtil::getEncryptOutputLength(const int& dataLen)
{
	return ceil(dataLen / 117.0f) * 128;
}

int RSAUtil::public_encrypt(unsigned char * data, int data_len, unsigned char * key, unsigned char *encrypted)
{
	RSA * rsa = createRSA(key, 1);
	const int MAX_ENCRYPT_BLOCK = 117;
	int inputLen = data_len;
	int offSet = 0;
	unsigned char cache[1024];
	int i = 0;
	int outLen = 0;

	try
	{

		// 对数据分段加密
		while (inputLen - offSet > 0) {
			if (inputLen - offSet > MAX_ENCRYPT_BLOCK) {
				int len = RSA_public_encrypt(MAX_ENCRYPT_BLOCK, data + offSet, cache, rsa, padding);

				if (len > 0)
				{
					memcpy(encrypted + outLen, cache, len);
					outLen += len;
				}
				else
				{
					return -1;
				}
			}
			else {
				int len = RSA_public_encrypt(inputLen - offSet, data + offSet, cache, rsa, padding);

				if (len > 0)
				{
					memcpy(encrypted + outLen, cache, len);
					outLen += len;
				}
				else
				{
					return -1;
				}
			}
			i++;
			offSet = i * MAX_ENCRYPT_BLOCK;
		}
	}
	catch (...)
	{
		return -1;
	}

	

	//int result = RSA_public_encrypt(data_len, data, encrypted, rsa, padding);
	return outLen;
}

int RSAUtil::private_decrypt(unsigned char * enc_data, int data_len, unsigned char * key, unsigned char *decrypted)
{
	RSA * rsa = createRSA(key, 0);

	const int MAX_DECRYPT_BLOCK = 128;
	int inputLen = data_len;
	int offSet = 0;
	unsigned char cache[1024];
	int i = 0;
	int outLen = 0;
	try
	{

		// 对数据分段解密
		while (inputLen - offSet > 0) {
			if (inputLen - offSet > MAX_DECRYPT_BLOCK) {
				int len = RSA_private_decrypt(MAX_DECRYPT_BLOCK, enc_data + offSet, cache, rsa, padding);

				if (len > 0)
				{
					memcpy(decrypted + outLen, cache, len);
					outLen += len;
				}
				else
				{
					return -1;
				}
			}
			else {
				int len = RSA_private_decrypt(inputLen - offSet, enc_data + offSet, cache, rsa, padding);

				if (len > 0)
				{
					memcpy(decrypted + outLen, cache, len);
					outLen += len;
				}
				else
				{
					return -1;
				}
			}
			i++;
			offSet = i * MAX_DECRYPT_BLOCK;
		}
	}
	catch (...)
	{
		return -1;
	}
	
	//int  result = RSA_private_decrypt(data_len, enc_data, decrypted, rsa, padding);
	return outLen;
}

int RSAUtil::private_encrypt(unsigned char * data, int data_len, unsigned char * key, unsigned char *encrypted)
{
	RSA * rsa = createRSA(key, 0);

	const int MAX_ENCRYPT_BLOCK = 117;
	int inputLen = data_len;
	int offSet = 0;
	unsigned char cache[1024];
	int i = 0;
	int outLen = 0;

	try
	{
		// 对数据分段加密
		while (inputLen - offSet > 0) {
			if (inputLen - offSet > MAX_ENCRYPT_BLOCK) {
				int len = RSA_private_encrypt(MAX_ENCRYPT_BLOCK, data + offSet, cache, rsa, padding);

				if (len > 0)
				{
					memcpy(encrypted + outLen, cache, len);
					outLen += len;
				}
				else
				{
					return -1;
				}
			}
			else {
				int len = RSA_private_encrypt(inputLen - offSet, data + offSet, cache, rsa, padding);

				if (len > 0)
				{
					memcpy(encrypted + outLen, cache, len);
					outLen += len;
				}
				else
				{
					return -1;
				}
			}
			i++;
			offSet = i * MAX_ENCRYPT_BLOCK;
		}
	}
	catch (...)
	{
		return -1;
	}

	
	

	//int result = RSA_private_encrypt(data_len, data, encrypted, rsa, padding);
	return outLen;
}

int RSAUtil::public_decrypt(unsigned char * enc_data, int data_len, unsigned char * key, unsigned char *decrypted)
{
	RSA * rsa = createRSA(key, 1);

	const int MAX_DECRYPT_BLOCK = 128;
	int inputLen = data_len;
	int offSet = 0;
	unsigned char cache[1024];
	int i = 0;
	int outLen = 0;

	try
	{

		// 对数据分段解密
		while (inputLen - offSet > 0) {
			if (inputLen - offSet > MAX_DECRYPT_BLOCK) {
				int len = RSA_public_decrypt(MAX_DECRYPT_BLOCK, enc_data + offSet, cache, rsa, padding);

				if (len > 0)
				{
					memcpy(decrypted + outLen, cache, len);
					outLen += len;
				}
				else
				{
					return -1;
				}
			}
			else {
				int len = RSA_public_decrypt(inputLen - offSet, enc_data + offSet, cache, rsa, padding);

				if (len > 0)
				{
					memcpy(decrypted + outLen, cache, len);
					outLen += len;
				}
				else
				{
					return -1;
				}
			}
			i++;
			offSet = i * MAX_DECRYPT_BLOCK;
		}
	}
	catch (...)
	{
		return -1;
	}
	
	//int  result = RSA_public_decrypt(data_len, enc_data, decrypted, rsa, padding);
	return outLen;
}
