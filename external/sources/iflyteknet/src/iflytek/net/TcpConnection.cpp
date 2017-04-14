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


#define publicKey "-----BEGIN PUBLIC KEY-----\n"\
	"MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCrIAwtYCCsgPr6wai8kX8etPCZ\n"\
	"lO1dS4UqJohFwDA1dd3zbV8BWCNFPsg9OrW5qH8vzp1Yo7+T3/DE/Y+FtHm+f5No\n"\
	"tx9cpNBfI+ZDuTX2gCsoSWTy8nj9mdn6oWhp1367QbFM7tQBqK3EsDuo1nG+KK32\n"\
	"twb+Xpw3inl1KoMQmwIDAQAB\n"\
	"-----END PUBLIC KEY-----\n"

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

TcpConnection::TcpConnection():_socket(_ioService)
{
    _isConnected = false;
    _isConnecting = false;
    _thread = nullptr;
	_refPtr = nullptr;
	_isEnableCrypt = true;
	_isEnableDecodeProto = true;
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
#ifdef COCOS2D_DEBUG // 调试模式的包，允许使用非加密模式
	this->_isEnableCrypt = isEnableCrypt;
#endif
}

void TcpConnection::setEnableDecodeProto(const bool& isDecodeProto)
{
	this->_isEnableDecodeProto = isDecodeProto;
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
			//CCLOG("-------------encrypt len:%d", encrypted_length);

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
			msg->setBody(data, len);
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

void TcpConnection::doReadHeader()
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
                                 if(result == 1) // 继续解析头
                                 {
                                     if(curDataLen>=MAX_HEADER_LENGTH) // 未解析到头，丢掉包
                                     {
                                         cond = false;
                                     }
                                     
                                     std::size_t recLen = asio::read(_socket, asio::buffer((*msgPtr).data() + curDataLen, RECEIVE_LEN));
                                     if(recLen>0)
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
                                 else if(result == 0) // 解析成功，开始接收body
                                 {
                                     cond = false;
                                     doReadBody(msgPtr);
                                 }
                                 else // 头解析失败，丢掉这个包，重新接收新包
                                 {
                                     cond = false;
                                     loopRead();
                                 }
                                 
                             }while(cond);
                         }
                         else
                         {
                             doDisconnect();
                         }
                     });
}

void TcpConnection::doReadBody(std::shared_ptr<ReceiveMsg> receiveMsg)
{
    asio::async_read(_socket,
                     asio::buffer((*receiveMsg).receiveBody(), (*receiveMsg).receiveBodyLength()),
                     [this, receiveMsg](std::error_code ec, std::size_t len/*length*/)
                     {
                         if (!ec)
                         {
                             
                             int8_t* msgData = (*receiveMsg).body();
                             std::size_t bodyLen = (*receiveMsg).bodyLength();
                             
                             //std::cout << "receive len:" << bodyLen << std::endl;


							 if (_isEnableCrypt)
							 {

								 // 对接收的数据进行解密
								 uint8_t* decrypted = new uint8_t[bodyLen];

								 int decrypted_length = RSAUtil::public_decrypt((unsigned char*)msgData, bodyLen, (unsigned char*)publicKey, decrypted);
								// CCLOG("-------------decrypt len:%d", decrypted_length);

								 if (decrypted_length > 0) // 解密成功
								 {
									 if (_receiveCallback)
									 {

										 string jsonStr = "";
										 if (_isEnableDecodeProto) // 解析proto
										 {
											 if (decrypted_length > 0)
											 {
												 try
												 {
													 MessageInfo* msg = new MessageInfo();

													 parseMessage(msg, decrypted, decrypted_length);
													 jsonStr = google::protobuf::JsonFormat::Utf8DebugJsonString(*msg);
													 delete msg;
												 }
												 catch (...)
												 {
												 }
											 }
										 }
										 
										 _receiveCallback(decrypted, decrypted_length, jsonStr); // 派发消息
									 }
									 delete[] decrypted;
								 }
								 else // 解密数据失败
								 {
									 std::cerr << "Decrypt data failed" << endl;
								 }
							 }
							 else
							 {
								 if (_receiveCallback)
								 {
									 string jsonStr = "";
									 if (_isEnableDecodeProto) // 解析proto
									 {
										 if (bodyLen > 0)
										 {
											 try
											 {
												 MessageInfo* msg = new MessageInfo();

												 parseMessage(msg, msgData, bodyLen);
												 jsonStr = google::protobuf::JsonFormat::Utf8DebugJsonString(*msg);
												 delete msg;
											 }
											 catch (...)
											 {
											 }
										 }
									 }
									 _receiveCallback(msgData, bodyLen, jsonStr); // 派发消息
								 }
							 }
							
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
												size_t sendSize = asio::write(_socket, asio::buffer(buffer2, 4)); // 询问服务器是否支持代理

												if (sendSize <= 0) // 发送失败
												{
													break;
												}

												int8_t recData[2];
												size_t rs = asio::read(_socket, asio::buffer(recData, 2));

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

												auto sendSize2 = asio::write(_socket, asio::buffer(buffer, 10));

												if (sendSize2 <= 0) // 发送失败
												{
													break;
												}

												int8_t recData2[10];
												size_t rs2 = asio::read(_socket, asio::buffer(recData2, 10));

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

