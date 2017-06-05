/*
 * Compression.h
 *
 *  Created on: Aug 26, 2013
 *      Author: jerome
 */

#ifndef COMPRESSION_H_
#define COMPRESSION_H_

#include <zlib.h>
#include <zconf.h>
#include <iostream>
#include <vector>

#define COMPRESSION_LEVEL Z_DEFAULT_COMPRESSION
#define COMPRESSION_BUFFER_SIZE 16384 * 10

typedef std::vector<Bytef> StlVecUnChar;

class Compression
{
public:
    enum CompressLevel
    {
        NO_COMPRESSION      = Z_NO_COMPRESSION,
        BEST_SPEED          = Z_BEST_SPEED,
        BEST_COMPRESSION    = Z_BEST_COMPRESSION,
        DEFAULT_COMPRESSION = Z_DEFAULT_COMPRESSION
    };
	
	/************************************************************************/
	/* @param isCompress true-压缩器，false-解压器
	 * @param isWrap 是否wapper模式
	 * @pamra isSynFlush 是否是synFlush模式，使用synFlush模式，需要一直用当前对象编码和解码，中途不调用inflateEnd或deflateEnd
	 * @param level 压缩等级
	/************************************************************************/
    Compression(bool isCom, bool isWrap, bool isSynFlush, CompressLevel level = DEFAULT_COMPRESSION)
		: m_zstream(), m_IsCompress(isCom), m_IsWrap(isWrap), m_IsSynFlush(isSynFlush), m_bufferInCapa(COMPRESSION_BUFFER_SIZE), m_bufferOutCapa(COMPRESSION_BUFFER_SIZE), m_compressLevel(level)
	{
		Init();
	}
	virtual ~Compression();

	
	int doCompress(const Bytef* inData, const int& inDataLen, std::vector<Bytef>& outData);
	int doUncompress(const Bytef* inData, const int& inDataLen, std::vector<Bytef>& outData);
	void reset();

    //int Deflate(const StlVecUnChar &inStr, StlVecUnChar &outStr);
    //int Inflate(const StlVecUnChar &inStr, StlVecUnChar &outStr);

private:
	int Init();

private:


    z_stream      m_zstream;               // Stream structure used by zlib
    bool          m_IsCompress;            // True: compress. False: decompress
	bool		  m_IsWrap; // true：wrapper mode， false：no wrapper
	bool          m_IsSynFlush;

    Bytef* m_bufferIn; // Input buffer for zlib
	Bytef* m_bufferOut;// Output Buffer for zlib
    const int     m_bufferInCapa;          // Input buffer capacity
    const int     m_bufferOutCapa;         // Output buffer capacity
    CompressLevel m_compressLevel;         // Compress level
};

#endif /* COMPRESSION_H_ */