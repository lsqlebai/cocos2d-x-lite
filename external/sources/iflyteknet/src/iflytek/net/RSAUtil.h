
//
//  TcpConnection.hpp
//
//  Created by SULEI on 17/2/9.
//  Copyright © 2017年 SULEI. All rights reserved.
//

#ifndef RSA_UTIL_H
#define RSA_UTIL_H

/************************************************************************/
/* RSA加解密工具                                                                     */
/************************************************************************/
class RSAUtil
{
public:

	/**
	 * 计算加密后的数据大小
	 */
	static int getEncryptOutputLength(const int& dataLen);	

	/**
	* 公钥加密数据
	*/
	static int public_encrypt(unsigned char * data, int data_len, unsigned char * key, unsigned char *encrypted);

	/**
	* 私钥解密数据
	*/
	static int private_decrypt(unsigned char * enc_data, int data_len, unsigned char * key, unsigned char *decrypted);

	/**
	* 私钥加密数据
	*/
	static int private_encrypt(unsigned char * data, int data_len, unsigned char * key, unsigned char *encrypted);

	/**
	* 公钥解密数据
	*/
	static int public_decrypt(unsigned char * enc_data, int data_len, unsigned char * key, unsigned char *decrypted);
};
#endif
