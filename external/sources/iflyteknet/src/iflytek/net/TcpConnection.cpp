//
//  TcpConnection.cpp
//
//  Created by SULEI on 17/1/19.
//  Copyright © 2017年 SULEI. All rights reserved.
//

#include "TcpConnection.hpp"
#include "RSAUtil.h"

#include "game.pb.h"
#include "json_format.h"
#include "zlib.h"
#include "logger.h"

#define publicKey "-----BEGIN PUBLIC KEY-----\n"\
	"MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCrIAwtYCCsgPr6wai8kX8etPCZ\n"\
	"lO1dS4UqJohFwDA1dd3zbV8BWCNFPsg9OrW5qH8vzp1Yo7+T3/DE/Y+FtHm+f5No\n"\
	"tx9cpNBfI+ZDuTX2gCsoSWTy8nj9mdn6oWhp1367QbFM7tQBqK3EsDuo1nG+KK32\n"\
	"twb+Xpw3inl1KoMQmwIDAQAB\n"\
	"-----END PUBLIC KEY-----\n"

#define IFLYTEK_NET_VERSION 12 // 通信库版本号

//#pragma comment(lib, "libzlib.lib") 


//int testFun(int8_t* d, size_t len)
//{
//	//char text[] = "zlib compress and uncompress test\nturingo@163.com\n2012-11-05\n";
//	//uLong tlen = strlen(text) + 1;  /* 需要把字符串的结束符'\0'也一并处理 */
//
//	int8_t* text = d;
//	uLong tlen = len + 1;  /* 需要把字符串的结束符'\0'也一并处理 */
//	Bytef* buf = NULL;
//	uLong blen;
//
//	/* 计算缓冲区大小，并为其分配内存 */
//	blen = compressBound(tlen); /* 压缩后的长度是不会超过blen的 */
//	if ((buf = (Bytef*)malloc(sizeof(char)* blen)) == NULL)
//	{
//		printf("no enough memory!\n");
//		return -1;
//	}
//
//	/* 压缩 */
//	if (compress2(buf, &blen, (Bytef*)text, tlen, 9) != Z_OK)
//	{
//		printf("compress failed!\n");
//		return -1;
//	}
//
//
//	/* 解压缩 */
//	if (uncompress((Bytef*)text, &tlen, buf, blen) != Z_OK)
//	{
//		printf("uncompress failed!\n");
//		return -1;
//	}
//
//	/* 打印结果，并释放内存 */
//	printf("%s", text);
//	if (buf != NULL)
//	{
//		free(buf);
//		buf = NULL;
//	}
//
//	return 0;
//}
//
//#include "Compression.h"
//#include "zlib_wrapper.h"
//
//
//#define CHUNK_LEN 256
//
//static z_stream inflate_stream;
//
//static bool isInited = false;
//static int tInit()
//{
//	if (!isInited)
//	{
//		inflate_stream.zalloc = Z_NULL;
//		inflate_stream.zfree = Z_NULL;
//		inflate_stream.opaque = Z_NULL;
//
//		int ret = inflateInit2_(&inflate_stream, -15, ZLIB_VERSION, sizeof(z_stream));
//		//inflateInit(&inflate_stream)
//		if (ret != Z_OK)
//		{
//			return 1;
//		}
//		isInited = true;
//	}
//}
//
//static bool do_decompression(const char *buf, int len)
//{
//	tInit();
//	z_stream *zs = &inflate_stream;
//	char outbuf[1024 * 5];
//
//	zs->next_in = (Bytef *)buf;
//	zs->avail_in = len;
//
//	while (1)
//	{
//		int ret;
//
//		zs->next_out = (Bytef *)outbuf;
//		zs->avail_out = sizeof (outbuf);
//
//		ret = inflate(zs, Z_SYNC_FLUSH);
//		if (ret != Z_OK && ret != Z_STREAM_END)
//		{
//			printf("inflate returned %d: %s\n",
//				ret, zs->msg ? zs->msg : "<no-error>");
//			return FALSE;
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
//
//	return TRUE;
//}
//
//
//StlVecUnChar zTest(int8_t* d, size_t len)
//{
//
//	StlVecUnChar inData;
//	StlVecUnChar outData;
//	StlVecUnChar outData2;
//
//	//{
//	//	const char* s = "你好";
//	//	int len2 = strlen(s);
//	//	for (int i = 0; i < len2; ++i)
//	//	{
//	//		inData.push_back((signed char)s[i]);
//	//	}
//	//	Compression ci(true);
//	//	ci.Init();
//	//	int r = ci.Deflate(inData, outData);
//
//	//}
//
//	
//	for (int i = 0; i < len; ++i)
//	{
//		inData.push_back(d[i]);
//	}
//	static Compression* ci = new Compression(true, false, true);
//	//ci->Init();
//	int r = ci->doCompress((Bytef*)d, len, outData);
//
//
//	//Compression cd(false);
//	//cd.Init();
//	//int rr = cd.Inflate(outData, outData2);
//
//	return outData;
//}
//
//StlVecUnChar unzTest(int8_t* d, size_t len)
//{
//
//	
//	//std::vector<char> outDatat;
//
//	//bool bbb = unzip_buffer((char*)d, (unsigned int)len, outDatat);
//
//	//int8_t* text = (int8_t*)malloc(sizeof(char)* len * 50);
//
//	//uLong tlen = 0;
//
//	///* 解压缩 */
//
//	//int result = uncompress((Bytef*)text, &tlen, (Bytef*)d, len);
//	//if (result != Z_OK)
//	//{
//	//	printf("uncompress failed!\n");
//	//}
//
//
//	StlVecUnChar inData;
//	StlVecUnChar outData;
//	StlVecUnChar outData2;
//	StlVecUnChar outData3;
//
//	for (int i = 0; i < len; ++i)
//	{
//		inData.push_back((signed char)d[i]);
//	}
//
//
//	
//	//Compression ci(true);
//	//ci.Init();
//	//int r = ci.Deflate(inData, outData2);
//
//	//Compression cdtest(false);
//	//cdtest.Init();
//	//int rrtest = cdtest.Inflate(outData2, outData3);
//
//
//	//do_decompression((const char*)d, len);
//
//	static Compression* cd = new Compression(false, false, true);
//	//cd->Init();
//	int rr = cd->doUncompress((Bytef*)d, len, outData);
//
//	//char a[50] = "Hello World!";
//	//char b[50];
//	//char c[50];
//
//	//// deflate
//	//// zlib struct
//	//z_stream defstream;
//	//defstream.zalloc = Z_NULL;
//	//defstream.zfree = Z_NULL;
//	//defstream.opaque = Z_NULL;
//	//defstream.avail_in = (uInt)sizeof(a); // size of input
//	//defstream.next_in = (Bytef *)a; // input char array
//	//defstream.avail_out = (uInt)sizeof(b); // size of output
//	//defstream.next_out = (Bytef *)b; // output char array
//
//	//deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
//	//deflate(&defstream, Z_FINISH);
//	//deflateEnd(&defstream);
//
//	//printf("Deflate:\n%lu\n%s\n", strlen(b), b);
//
//	//// inflate
//	//// zlib struct
//	//z_stream infstream;
//	//infstream.zalloc = Z_NULL;
//	//infstream.zfree = Z_NULL;
//	//infstream.opaque = Z_NULL;
//	//infstream.avail_in = (uInt)sizeof(b); // size of input
//	//infstream.next_in = (Bytef *)b; // input char array
//	//infstream.avail_out = (uInt)sizeof(c); // size of output
//	//infstream.next_out = (Bytef *)c; // output char array
//
//	//inflateInit(&infstream);
//	//inflate(&infstream, Z_NO_FLUSH);
//	//inflateEnd(&infstream);
//
//	//printf("Inflate:\n%lu\n%s\n", strlen(c), c);
//
//
//
//
//
//
//	return outData;
//}

int32_t TcpConnection::getVersion()
{
	return IFLYTEK_NET_VERSION;
}

//#define privateKey "-----BEGIN RSA PRIVATE KEY-----\n"\
//	"MIICeAIBADANBgkqhkiG9w0BAQEFAASCAmIwggJeAgEAAoGBAKsgDC1gIKyA+vrB\n"\
//	"qLyRfx608JmU7V1LhSomiEXAMDV13fNtXwFYI0U+yD06tbmofy/OnVijv5Pf8MT9\n"\
//	"j4W0eb5/k2i3H1yk0F8j5kO5NfaAKyhJZPLyeP2Z2fqhaGnXfrtBsUzu1AGorcSw\n"\
//	"O6jWcb4orfa3Bv5enDeKeXUqgxCbAgMBAAECgYEAgrh9dOActYnlr03mZv3CHKPd\n"\
//	"s4SHeiApflRm/tlcOkpqnPyseWtprgjSj6tcjJGBbvSnT8Huo7iKxQBo9Zsy76ke\n"\
//	"sblxLR9DlRSLmQFSrillBV0dPrt5+Cq95HsHCqEpwXcEUnHqiuRnRtDaPyeU1Pmq\n"\
//	"CW7hWqSxFy4/0q1dRxkCQQDgNtkAxyU/03AqBU9ab3RVHKb/xQb/XqXpnC+hVqc0\n"\
//	"nQ84rHyutQVW8Br1UDTEZbj9VWMzQY+3rWa/7qBrWOyXAkEAw2J/vz9wNX6sQ5Oz\n"\
//	"hT8O31gqw/lBnQSHqQKweLdJsxg6/wCfhF3bl8KLmwQUi4ZRe0QGO5GiZAyFWWEY\n"\
//	"NXzInQJBAKa2AWoDqN2pQBfudM9AWdZDBKTegdJ1NJXbjMrAnHiRY1T3Y4mfjUXU\n"\
//	"J6dJKMLzA7ZRu/3LfKnM475IFr1alCcCQB9cNPLmZMVBUrb6Awt1BpcUmLCh1kU0\n"\
//	"j+2xr8+AY8TqM7XwTKo7Ql7GbA/yhLWsVnG5hmKTSoRSeijRa8hSAgUCQQCYg0UY\n"\
//	"D2Ccg1BH8zXuNOVOWOMIK1J8qldoi+DP8AI+6dV9lXLXG3ly+Nyusi2LXnnZpNIo\n"\
//	"D7HqWVo2mDSc4lbE\n"\
//	"-----END RSA PRIVATE KEY-----\n"

TcpConnection::TcpConnection() :_socket(_ioService), _compress(true, false, true), _uncompress(false, false, true)
{
    _isConnected = false;
    _isConnecting = false;
    _thread = nullptr;
	_refPtr = nullptr;
	_isEnableCrypt = true;
	_isEnableDecodeProto = true;
	_isEnableZlib = true;
	_proxyPort = 0;

	
}

TcpConnection::~TcpConnection()
{
    doDisconnect();
}


void parseMessage(::google::protobuf::Message* msg, void* data, int32_t len)
{
	msg->ParseFromArray(data, len);
}

void TcpConnection::setEnableCrypt(const bool& isEnableCrypt)
{
	this->_isEnableCrypt = isEnableCrypt;
}

void TcpConnection::setEnableDecodeProto(const bool& isDecodeProto)
{
	this->_isEnableDecodeProto = isDecodeProto;
}

void TcpConnection::setEnableZlib(const bool& isEnableZlib)
{
	this->_isEnableZlib = isEnableZlib;
}

void TcpConnection::setProxy(string proxyHost, int proxyPort)
{
	this->_proxyHost = proxyHost;
	this->_proxyPort = proxyPort;
}

void TcpConnection::doDisconnect(const bool& isPrivate /*=true*/)
{

    if(_thread)
    {
        if (_thread->joinable())
        {
            _thread->detach();
        }
        _thread = nullptr;
    }
    
    if(_isConnected)
    {
		this->_isConnected = false;

        _ioService.post([this]() { _socket.close(); });
        
        // 通知连接断开
        if(this->_disconnectCallback)
        {
            this->_disconnectCallback(!isPrivate);
		}
	}
}

string TcpConnection::intToString(const int& value)
{
	char temp[64];
	sprintf(temp, "%d", value);
	return string(temp);
}



void TcpConnection::asynSend(void* data, std::size_t len, SendCallback sendCallback /*=nullptr*/, const int32_t& sendId/* = -1*/)
{

    SendMsg* msg = new SendMsg();
	msg->setSendId(sendId);
    msg->_sendCallback = sendCallback; // 设置回调函数
    
    std::shared_ptr<SendMsg> msgPtr(msg);
    _ioService.post(
                     [=]()
                     {

		if (_isEnableCrypt)
		{

			// 对发送的数据进行加密
			int outLen = RSAUtil::getEncryptOutputLength(len);

			uint8_t* encrypted = new uint8_t[outLen];

			int encrypted_length = RSAUtil::public_encrypt((unsigned char*)data, len, (unsigned char*)publicKey, encrypted);

			if (encrypted_length > 0) // 加密成功
			{
				msg->setBody(encrypted, encrypted_length);

				delete[] encrypted;

				bool writeInProgress = !_sendMsgQueue.empty();
				_sendMsgQueue.push_back(msgPtr);
				if (!writeInProgress)
				{
					doSend();
				}
			}
			else // 加密失败
			{
				delete[] encrypted;
				SendMsg msg = *_sendMsgQueue.front();

				// 回调数据加密失败
				if (msg._sendCallback)
				{
					msg._sendCallback(msg.getSendId(), -2);
				}
			}
		}
		else
		{

			if (_isEnableZlib)
			{
				std::vector<Bytef> outData;
				this->_compress.doCompress((Bytef*)data, len, outData); // 进行数据压缩


				auto sendData = new int8_t[outData.size()];
				for (int i = 0; i < outData.size(); ++i)
				{
					sendData[i] = outData[i];
				}
				msg->setBody(sendData, outData.size());
			}
			else
			{
				msg->setBody(data, len);
			}

			bool writeInProgress = !_sendMsgQueue.empty();
			_sendMsgQueue.push_back(msgPtr);
			if (!writeInProgress)
			{
				doSend();
			}
		}

	});
}

void TcpConnection::doSend()
{
	
	try
	{
		
		asio::async_write(_socket,
			asio::buffer((*_sendMsgQueue.front()).data(), (*_sendMsgQueue.front()).length()),
			[this](std::error_code ec, std::size_t length)
		{

			SendMsg msg = *_sendMsgQueue.front();

			if (!ec)
			{
				// 发送成功
				if (msg._sendCallback)
				{
					msg._sendCallback(msg.getSendId(), 0);
				}

				_sendMsgQueue.pop_front();
				if (!_sendMsgQueue.empty())
				{
					doSend();
				}
			}
			else
			{

				// 发送失败
				if (msg._sendCallback)
				{
					msg._sendCallback(msg.getSendId(), -1);
				}

				_sendMsgQueue.pop_front();
				//doDisconnect();
			}
		});
	}
	catch (...)
	{
		//std::cout << "asynwrite data error" << std::endl;
		// 网络异常
	}

		
	


}

void TcpConnection::doReadHeader()
{
   
    
	try
	{
		ReceiveMsg* msg = new ReceiveMsg();
		static int RECEIVE_LEN = 1;
		std::shared_ptr<ReceiveMsg> msgPtr(msg);
		asio::async_read(_socket,
			asio::buffer((*msgPtr).data(), RECEIVE_LEN),
			[this, msgPtr](std::error_code ec, std::size_t len/*length*/)
		{

			if (!ec)
			{
				std::size_t curDataLen = RECEIVE_LEN;

				bool cond = true;

				do
				{
					//std::cout << "receive header curDataLen:" << curDataLen << std::endl;
					int result = (*msgPtr).decodeHeader(curDataLen);
					if (result == 1) // 继续解析头
					{
						if (curDataLen >= MAX_HEADER_LENGTH) // 未解析到头，丢掉包
						{
							cond = false;
						}

						std::size_t recLen = 0;
						try
						{
							recLen = asio::read(_socket, asio::buffer((*msgPtr).data() + curDataLen, RECEIVE_LEN));
						}
						catch (...)
						{
							//std::cout << "read data error" << std::endl;
							// 网络异常
						}

						if (recLen > 0)
						{
							curDataLen += recLen;
							continue;
						}
						else
						{
							cond = false;
							//doDisconnect();
						}
					}
					else if (result == 0) // 解析成功，开始接收body
					{
						cond = false;
						doReadBody(msgPtr);
					}
					else // 头解析失败，丢掉这个包，重新接收新包
					{
						cond = false;
						loopRead();
					}

				} while (cond);
			}
			else
			{
				doDisconnect();
			}
		});
	}
	catch (...)
	{
		//std::cout << "asynread data error" << std::endl;
		// 网络异常
	}

    
}


void TcpConnection::doReadBody(const std::shared_ptr<ReceiveMsg>& receiveMsg)
{
    asio::async_read(_socket,
                     asio::buffer((*receiveMsg).receiveBody(), (*receiveMsg).receiveBodyLength()),
                     [this, receiveMsg](std::error_code ec, std::size_t len/*length*/)
                     {

						DEBUG_LOG("---- msg read body");

						// TODO test code
						if (true)
						{
							if (!ec)
							{
								loopRead();
							}
							else
							{
								doDisconnect();
							}
							return;
						}
						


                         if (!ec)
                         {
                             
                             int8_t* msgData = (*receiveMsg).body();
                             std::size_t bodyLen = (*receiveMsg).bodyLength();
                             
							 DEBUG_LOG("---- msg len:%d", bodyLen);

							 if (_isEnableCrypt)
							 {

								 DEBUG_LOG("---- msg 1");

								 // 对接收的数据进行解密
								 uint8_t* decrypted = new uint8_t[bodyLen];

								 DEBUG_LOG("---- msg 2");

								 int decrypted_length = RSAUtil::public_decrypt((unsigned char*)msgData, bodyLen, (unsigned char*)publicKey, decrypted);
								// CCLOG("-------------decrypt len:%d", decrypted_length);

								 DEBUG_LOG("---- msg 3");

								 if (decrypted_length > 0) // 解密成功
								 {

									 DEBUG_LOG("---- msg 4");

									 if (_receiveCallback)
									 {
										 DEBUG_LOG("---- msg 5");

										 string jsonStr = "";
										 if (_isEnableDecodeProto) // 解析proto
										 {

											 DEBUG_LOG("---- msg 6");
											 if (decrypted_length > 0)
											 {
												 try
												 {
													 DEBUG_LOG("---- msg 7");
													 MessageInfo* msg = new MessageInfo();
													 DEBUG_LOG("---- msg 8");
													 parseMessage(msg, decrypted, decrypted_length);
													 DEBUG_LOG("---- msg 9");
													 jsonStr = google::protobuf::JsonFormat::Utf8DebugJsonString(*msg);
													 DEBUG_LOG("---- msg 10");
													 delete msg;
													 DEBUG_LOG("---- msg 11");
												 }
												 catch (...)
												 {
													 DEBUG_LOG("---- msg 12");
												 }
											 }
										 }
										 
										 DEBUG_LOG("---- msg 13");
										 _receiveCallback(decrypted, decrypted_length, jsonStr); // 派发消息
										 DEBUG_LOG("---- msg 14");
									 }
									 delete[] decrypted;
									 DEBUG_LOG("---- msg 15");
								 }
								 else // 解密数据失败
								 {
									 DEBUG_LOG("---- msg 16");
									 std::cerr << "Decrypt data failed" << endl;
								 }
							 }
							 else
							 {
								 DEBUG_LOG("---- msg 22");
								 if (_receiveCallback && false)
								 {
									 DEBUG_LOG("---- msg 23");
									 string jsonStr = "";
									 if (_isEnableDecodeProto) // 解析proto
									 {
										 DEBUG_LOG("---- msg 24");
										 if (bodyLen > 0)
										 {
											 DEBUG_LOG("---- msg 25");
											 try
											 {

												 DEBUG_LOG("---- msg 26");
												 if (_isEnableZlib)
												 {
													 DEBUG_LOG("---- msg 27");
													 MessageInfo* msg = new MessageInfo();
													 DEBUG_LOG("---- msg 28");
													 std::vector<Bytef> outData;
													 DEBUG_LOG("---- msg 29");
													 _uncompress.doUncompress((Bytef*)msgData, bodyLen, outData); // 进行数据解压
													 DEBUG_LOG("---- msg 30");
													 std::unique_ptr<int8_t[]> p(new int8_t[outData.size()]);
													 DEBUG_LOG("---- msg 31");
													 auto finalData = p.get();
													 DEBUG_LOG("---- msg 32");
													 int finalDataLen = outData.size();
													 DEBUG_LOG("---- msg 33, finalDataLen:%d", finalDataLen);
													 for (int i = 0; i < finalDataLen; ++i)
													 {
														 finalData[i] = outData.at(i);
														 
														 
															 //DEBUG_LOG("---- msg --:%d,val:%d", i, outData.at(i));
														 
														 
													 }
													 DEBUG_LOG("---- msg 34");
													 parseMessage(msg, finalData, finalDataLen);
													 auto byteSize = msg->ByteSize();
													 
													 DEBUG_LOG("---- msg 35");

													 

													 DEBUG_LOG("---- msg 35xxx: byteSize:%d", byteSize);

													 auto str1 = msg->DebugString();
													 DEBUG_LOG("---- msg str1:%s", str1.c_str());
													 auto str = msg->Utf8DebugString();
													 DEBUG_LOG("---- msg str:%s", str.c_str());
													 jsonStr = google::protobuf::JsonFormat::Utf8DebugJsonString(*msg);
													 DEBUG_LOG("---- msg 36");
													 msgData = (int8_t*)finalData;
													 bodyLen = finalDataLen;
													 DEBUG_LOG("---- msg 37");
													 delete msg;
													 DEBUG_LOG("---- msg 38");
												 }
												 else
												 {
													 DEBUG_LOG("---- msg 39");
													 {
														 MessageInfo* msg = new MessageInfo();
														 parseMessage(msg, msgData, bodyLen);
														 DEBUG_LOG("---- msg 40");
														 jsonStr = google::protobuf::JsonFormat::Utf8DebugJsonString(*msg);
														 delete msg;
														 DEBUG_LOG("---- msg 41");
													 }

												 }
											 }
											 catch (...)
											 {
												 DEBUG_LOG("---- msg 42");
											 }
										 }
									 }
									 
									 DEBUG_LOG("---- msg 43");
									//_receiveCallback(msgData, bodyLen, jsonStr); // 派发消息
									DEBUG_LOG("---- msg 44");
								 }
							 }

							 DEBUG_LOG("---- msg final");
							
                            // for (int i = 0; i < bodyLen; ++i)
                            // {
                            //     //char ss[1];
                            //     //std::sprintf(ss, "%c", );
                            //     std::cout << msgData[i]<< std::endl;
                            // }
                            //std::cout << "over" << std::endl;
                             loopRead();
                         }
                         else
                         {
                             doDisconnect();
                         }
                     });
}

void TcpConnection::loopRead()
{
    doReadHeader();
}

void TcpConnection::asynConnect(string host, int port, ConnectCallback callback)
{

	DEBUG_LOG("---- asynConnect");

    if(isConnected() || isConnecting())
    {
        return;
    }
    
    try {
        doDisconnect(); // 释放之前的连接和线程
        
        this->_isConnecting = true;
        

		// 有代理服务器，使用代理服务器进行连接

		bool isUseProxy = false;
		if (!_proxyHost.empty())
		{
			isUseProxy = true;
		}


		tcp::resolver::iterator endpoint_iterator;
		tcp::resolver resolver(_ioService);
		if (isUseProxy)
		{
			string portStr = intToString(_proxyPort);
			endpoint_iterator = resolver.resolve({ _proxyHost, portStr });
			
		}
		else
		{
			string portStr = intToString(port);
			endpoint_iterator = resolver.resolve({ host, portStr });
		}
        
		this->_compress.reset();
		this->_uncompress.reset();

        asio::async_connect(_socket, endpoint_iterator,
                            [=](std::error_code ec, tcp::resolver::iterator it)
                            {
                                
                                
                                this->_isConnecting = false;
                                
                                if (!ec) // 连接成功
                                {
                                    this->_isConnected = true;
                                    

									if (isUseProxy) // 使用代理
									{
										bool isSuccess = false;
										try
										{
											do 
											{

												int8_t buffer2[4] = { 0x05, 0x02, 0x00, 0x02 };

												size_t sendSize = 0;
												try
												{
													sendSize = asio::write(_socket, asio::buffer(buffer2, 4)); // 询问服务器是否支持代理
												}
												catch (...)
												{
													//std::cout << "write data error" << std::endl;
													// 网络异常
												}
												

												if (sendSize <= 0) // 发送失败
												{
													break;
												}

												int8_t recData[2];
												size_t rs = 0;
												try
												{
													rs = asio::read(_socket, asio::buffer(recData, 2));
												}
												catch (...)
												{
													//std::cout << "read data error" << std::endl;
													// 网络异常
												}
												

												if (recData[1] != 0) // 服务器不支持代理，无法建立连接
												{
													break;
												}

												vector<string> ipSplit;
												split(host, '.', ipSplit); // 拆分ip

												if (ipSplit.size() != 4) // 目标ip格式不正确
												{
													break;
												}

												int8_t ipBuffer[4];
												for (int i = 0; i < ipSplit.size(); ++i)
												{

													int ipI = atoi(ipSplit[i].c_str());
													ipBuffer[i] = ipI;
												}

												int8_t portBuffer[2];
												
												portBuffer[0] = ((0xff00 & port) >> 8);
												portBuffer[1] = (0xff & port);

												int8_t buffer[10] = { 0x05, 0x01, 0x00, 0x01 }; // 以ipv4且不需认证的方式告诉代理服务器，需要连接的目标服务器ip和端口
												
												memcpy(buffer + 4, ipBuffer, 4);
												memcpy(buffer + 8, portBuffer, 2);

												auto sendSize2 = 2;
												try
												{
													sendSize2 = asio::write(_socket, asio::buffer(buffer, 10));
												}
												catch (...)
												{
													//std::cout << "write data error" << std::endl;
													// 网络异常
												}
												

												if (sendSize2 <= 0) // 发送失败
												{
													break;
												}

												int8_t recData2[10];
												size_t rs2 = 0;
												try
												{
													rs2 = asio::read(_socket, asio::buffer(recData2, 10));
												}
												catch (...)
												{
													//std::cout << "read data error" << std::endl;
													// 网络异常
												}
												

												if (recData2[1] != 0) // 代理服务器连接目标服务器失败
												{
													break;
												}

												isSuccess = true;
												

											} while (0);

										}
										catch (std::exception& e)
										{
											isSuccess = false;
										}


										if (isSuccess)
										{

											// 成功
											this->loopRead(); // 开始接收服务器消息

											if (callback)
											{
												callback(0, "Success");
											}
										}
										else
										{
											if (callback)
											{
												callback(-1, "Fail");
											}
										}

									}
									else // 直连
									{

										this->loopRead(); // 开始接收服务器消息

										if (callback)
										{
											callback(0, "Success");
										}
									}
                                }
                                else
                                {
                                    
                                    if(callback)
                                    {
                                        callback(-1, ec.message().c_str());
                                    }
                                }
                            });
        
        _thread = new thread(([this]()
                              {
                                  _ioService.run();
                              }));
        
    } catch (std::exception& e) {
        
        this->_isConnecting = false;
        if(callback)
        {
            
            callback(-1, e.what());
        }
    }

}


void TcpConnection::disconnect()
{
	DEBUG_LOG("---- disconnect");

    doDisconnect(false);
}


bool TcpConnection::isConnected()
{
    return this->_isConnected;
}


bool TcpConnection::isConnecting()
{
    return this->_isConnecting;
}


void TcpConnection::registerReceiveCallback(ReceiveCallback callback)
{
    this->_receiveCallback = callback;
}

void TcpConnection::registerDisconnectCallback(DisconnectCallback callback)
{
    this->_disconnectCallback = callback;
}

void* TcpConnection::getRefPtr()
{
	return this->_refPtr;
}

void TcpConnection::setRefPtr(void* refPtr)
{
	this->_refPtr = refPtr;
}
