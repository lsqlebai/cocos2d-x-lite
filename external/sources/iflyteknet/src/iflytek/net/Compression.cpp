/*
 * Compression.cpp
 *
 *  Created on: Aug 26, 2013
 *      Author: jerome
 */

#include "Compression.h"
#include <cstdio>
#include <cstring>
#include <cassert>



Compression::~Compression() {
	delete[] m_bufferIn;
	delete[] m_bufferOut;
}

int Compression::Init()
{
    int ret;
	m_zstream.zalloc = NULL;
	m_zstream.zfree  = NULL;
	m_zstream.opaque = NULL;
	if(m_IsCompress)
	{
		/*ret = deflateInit2_(&m_zstream, m_compressLevel, Z_DEFLATED, MAX_WBITS,MAX_MEM_LEVEL, Z_DEFAULT_STRATEGY, ZLIB_VERSION, sizeof(z_stream));*/
		ret = deflateInit2_(&m_zstream, m_compressLevel, Z_DEFLATED, m_IsWrap ? MAX_WBITS : -15, MAX_MEM_LEVEL, Z_DEFAULT_STRATEGY, ZLIB_VERSION, sizeof(z_stream));
	}
	else
	{
		//ret = inflateInit2_(&m_zstream, MAX_WBITS, ZLIB_VERSION, sizeof(z_stream));
		ret = inflateInit2_(&m_zstream, m_IsWrap ? MAX_WBITS  : - 15, ZLIB_VERSION, sizeof(z_stream));
	}

	m_bufferIn = new Bytef[m_bufferInCapa];
	m_bufferOut = new Bytef[m_bufferOutCapa];

	return ret;
}


int Compression::doCompress(const Bytef* inData, const int& inDataLen, std::vector<Bytef>& outData)
{
	int ret;
	int flush;

	size_t lastByte = inDataLen;
	int have = 0;
	while (lastByte > 0)
	{
		if (lastByte > m_bufferInCapa)
		{
			memcpy(m_bufferIn, &inData[inDataLen - lastByte], m_bufferInCapa);
			lastByte -= m_bufferInCapa;
			m_zstream.avail_in = m_bufferInCapa;
			//flush               = Z_NO_FLUSH;
			flush = m_IsSynFlush ? Z_SYNC_FLUSH : Z_NO_FLUSH;
		}
		else
		{
			memcpy(m_bufferIn, &inData[inDataLen - lastByte], lastByte);
			m_zstream.avail_in = lastByte;
			lastByte = 0;
			//flush              = Z_FINISH;
			flush = m_IsSynFlush ? Z_SYNC_FLUSH : Z_NO_FLUSH;
		}
		m_zstream.next_in = m_bufferIn;

		do
		{
			m_zstream.avail_out = m_bufferOutCapa;
			m_zstream.next_out = m_bufferOut;

			ret = deflate(&m_zstream, flush);
			//assert(ret != Z_STREAM_ERROR);
			have = m_bufferOutCapa - m_zstream.avail_out;
			outData.insert(outData.end(), m_bufferOut, m_bufferOut + have);
		} while (m_zstream.avail_out == 0);
		//assert(m_zstream.avail_in == 0);
	}

	// Finish deflate
	if (!m_IsSynFlush)
	{
		(void)deflateEnd(&m_zstream);
	}

	return Z_OK;
}

int Compression::doUncompress(const Bytef* inData, const int& inDataLen, std::vector<Bytef>& outData)
{
	int ret;

	int lastByte = inDataLen;
	int have = 0;
	while (lastByte > 0)
	{
		if (lastByte > m_bufferInCapa)
		{
			memcpy(m_bufferIn, &inData[inDataLen - lastByte], m_bufferInCapa);
			lastByte -= m_bufferInCapa;
			m_zstream.avail_in = m_bufferInCapa;
		}
		else
		{
			memcpy(m_bufferIn, &inData[inDataLen - lastByte], lastByte);
			m_zstream.avail_in = lastByte;
			lastByte = 0;
		}
		m_zstream.next_in = m_bufferIn;

		do
		{
			m_zstream.next_out = m_bufferOut;
			m_zstream.avail_out = m_bufferOutCapa;

			//ret = inflate(&m_zstream, Z_NO_FLUSH);
			ret = inflate(&m_zstream, m_IsSynFlush ? Z_SYNC_FLUSH : Z_NO_FLUSH);

			assert(ret != Z_STREAM_ERROR);
			switch (ret)
			{
			case Z_NEED_DICT:
				ret = Z_DATA_ERROR;
			case Z_DATA_ERROR:
			case Z_MEM_ERROR:
				(void)inflateEnd(&m_zstream);
				return ret;
			case Z_STREAM_END:
				inflateReset(&m_zstream);
			}

			have = m_bufferOutCapa - m_zstream.avail_out;
			outData.insert(outData.end(), m_bufferOut, m_bufferOut + have);
		} while (m_zstream.avail_out == 0);
	}

	if (!m_IsSynFlush)
	{
		(void)inflateEnd(&m_zstream);
		return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
	}
	else
	{
		return ret;
	}
}

void Compression::reset()
{
	if (m_IsCompress)
	{
		deflateReset(&m_zstream);
	}
	else
	{
		inflateReset(&m_zstream);
	}

}

//int Compression::Deflate(const StlVecUnChar &inStr, StlVecUnChar &outStr)
//{
//    int ret;
//    int flush;
//
//    size_t lastByte = inStr.size ();
//	int have  = 0;
//    while (lastByte > 0)
//	{
//        if (lastByte > m_bufferInCapa)
//		{
//            memcpy (m_bufferIn, &inStr[inStr.size () - lastByte], m_bufferInCapa);
//            lastByte           -= m_bufferInCapa;
//            m_zstream.avail_in  = m_bufferInCapa;
//            //flush               = Z_NO_FLUSH;
//			flush = m_IsSynFlush ? Z_SYNC_FLUSH : Z_NO_FLUSH;
//		}
//		else
//		{
//            memcpy (m_bufferIn, &inStr[inStr.size () - lastByte], lastByte);
//            m_zstream.avail_in = lastByte;
//            lastByte           = 0;
//            //flush              = Z_FINISH;
//			flush = m_IsSynFlush ? Z_SYNC_FLUSH : Z_NO_FLUSH;
//		}
//        m_zstream.next_in  = m_bufferIn;
//
//		do
//		{
//            m_zstream.avail_out = m_bufferOutCapa;
//            m_zstream.next_out  = m_bufferOut;
//
//			ret = deflate(&m_zstream, flush);
//			//assert(ret != Z_STREAM_ERROR);
//            have = m_bufferOutCapa - m_zstream.avail_out;
//            outStr.insert (outStr.end (), m_bufferOut, m_bufferOut + have);
//		}while(m_zstream.avail_out == 0);
//		//assert(m_zstream.avail_in == 0);
//	}
//
//	// Finish deflate
//	if (!m_IsSynFlush)
//	{
//		(void)deflateEnd(&m_zstream);
//	}
//	
//	return Z_OK;
//}
//
//int Compression::Inflate(const StlVecUnChar &inStr, StlVecUnChar &outStr)
//{
//
//
//	/*z_stream *zs = &m_zstream;
//	
//	int MAX_LEN = 1024 * 1024 * 1;
//	char* outbuf = new char[MAX_LEN];
//	memset(outbuf, 0, MAX_LEN);
//
//	Bytef* buf = new Bytef[inStr.size()];
//	for (int i = 0; i < inStr.size(); ++i)
//	{
//		buf[i] = inStr.at(i);
//	}
//
//	zs->next_in = (Bytef *)buf;
//	zs->avail_in = inStr.size();
//
//	while (1)
//	{
//		int ret;
//
//		zs->next_out = (Bytef *)outbuf;
//		zs->avail_out = MAX_LEN;
//
//		ret = inflate(zs, Z_SYNC_FLUSH);
//		if (ret != Z_OK && ret != Z_STREAM_END)
//		{
//			printf("inflate returned %d: %s\n",
//				ret, zs->msg ? zs->msg : "<no-error>");
//			return -1;
//		}
//
//		if (ret == Z_STREAM_END)
//		{
//			printf("Reached end of stream\n");
//			inflateReset(zs);
//		}
//
//		if (zs->avail_in == 0)
//			break;
//	}
//	int have = zs->total_out;
//	outStr.insert(outStr.end(), outbuf, outbuf + have);
//	return 0;*/
//
//    int ret;
//
//	int lastByte = inStr.size();
//	int have  = 0;
//	while (lastByte > 0)
//	{
//        if (lastByte > m_bufferInCapa)
//		{
//            memcpy(m_bufferIn, &inStr[inStr.size () - lastByte], m_bufferInCapa);
//            lastByte          -= m_bufferInCapa;
//            m_zstream.avail_in = m_bufferInCapa;
//		}
//		else
//		{
//            memcpy(m_bufferIn, &inStr[inStr.size () - lastByte], lastByte);
//			m_zstream.avail_in = lastByte;
//			lastByte = 0;
//		}
//        m_zstream.next_in  = m_bufferIn;
//
//		do
//		{
//            m_zstream.next_out  = m_bufferOut;
//            m_zstream.avail_out = m_bufferOutCapa;
//
//			//ret = inflate(&m_zstream, Z_NO_FLUSH);
//			ret = inflate(&m_zstream, m_IsSynFlush ? Z_SYNC_FLUSH : Z_NO_FLUSH);
//			
//			assert(ret != Z_STREAM_ERROR);
//			switch(ret)
//			{
//			case Z_NEED_DICT:
//				ret = Z_DATA_ERROR;
//			case Z_DATA_ERROR:
//			case Z_MEM_ERROR:
//				(void)inflateEnd(&m_zstream);
//				return ret;
//			case Z_STREAM_END:
//				inflateReset(&m_zstream);
//			}
//
//            have  = m_bufferOutCapa - m_zstream.avail_out;
//            outStr.insert (outStr.end (), m_bufferOut, m_bufferOut + have);
//		}while(m_zstream.avail_out == 0);
//	}
//
//	if (!m_IsSynFlush)
//	{
//		(void)inflateEnd(&m_zstream);
//		return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
//	}
//	 else
//	 {
//		 return ret;
//	 }
//
//}