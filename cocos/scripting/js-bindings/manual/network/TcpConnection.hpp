//
//  TcpConnection.hpp
//
//  Created by SULEI on 17/1/19.
//  Copyright © 2017年 SULEI. All rights reserved.
//

#ifndef TCP_CONNECTION_HPP
#define TCP_CONNECTION_HPP

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <cstdlib>
#include <thread>
#include <deque>
#include <mutex>
#include <memory>
#include "boost/asio.hpp"

using namespace std;
using asio::ip::tcp;

class SendMsg;
class ReceiveMsg;

#define MAX_HEADER_LENGTH 5 // 数据头最大字节数
typedef std::deque<std::shared_ptr<SendMsg>> SendMsgQueue; // 消息发送队列

/**
 * Tcp长连接
 */
class TcpConnection
{
public:
    
    /**
     * 建立连接结果回调
     * @param errorCode 错误码
     * @param errorMsg 错误描述
     */
    typedef std::function<void(int, const char*)> ConnectCallback;
    
    /**
     * 发送数据结果回调
	 * @param sendId 本次发送数据id
     * @param errorCode 0：成功，-1：失败
     */
    typedef std::function<void(int, int)> SendCallback;
    
    /**
     * 接收数据回调,收到的数据已经进行分包
     * @param data 接收到的数据
     * @param dataLen 接收到的数据长度
     */
    typedef std::function<void(void*, std::size_t)> ReceiveCallback;
    
    /**
     * 连接断开回调，主动断开或者网络异常断开都会触发
     * @param bool 是否是主动断开连接
     */
    typedef std::function<void(bool)> DisconnectCallback;
    
    TcpConnection();
    ~TcpConnection();

	/**
	 * 设置是否启用加解密
	 */
	void setEnableCrypt(const bool& isEnableCrypt);

	/**
	 * 设置代理服务器ip和port
	 */
	void setProxy(string proxyHost, int proxyPort);
    
    /**
     * 连接服务器
     * @param host 服务器地址
     * @param port 端口
     */
    void asynConnect(string host, int port, ConnectCallback callback);

	
    
    /**
     * 断开已建立的连接
     */
    void disconnect();
    
    /**
     * 是否已经建立了连接
     */
    bool isConnected();
    
    /**
     * 是否正在连接
     */
    bool isConnecting();
    
    /**
     * 异步发送数据
     * @param data 数据
     * @param len
     */
    void asynSend(void* data, std::size_t len, SendCallback sendCallback = nullptr, const int32_t& sendId = -1);
    
    /**
     * 注册接收数据回调
     */
    void registerReceiveCallback(ReceiveCallback callback);
    /**
     * 注册断开连接回调
     */
    void registerDisconnectCallback(DisconnectCallback callback);
    

	void setRefPtr(void* refPtr);

	void* getRefPtr();
private:

    void loopRead();
    void doReadHeader();
    void doReadBody(std::shared_ptr<ReceiveMsg> receiveMsg);
    void doSend();
    void doDisconnect(const bool& isPrivate = true);

	string intToString(const int& value);

	/**
	* 拆分字符串
	* @params s 待拆分的字符串
	* @params delim 以该字符进行拆分
	* @params elems 已有的字符串集合，拆分完毕后将添加到此集合中，最后返回
	*/
	static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}

	void* _refPtr;
	bool _isEnableCrypt;

	string _proxyHost;
	int _proxyPort;
private:
    bool _isConnected;
    bool _isConnecting;
    
    ReceiveCallback _receiveCallback; // 接收消息回调
    DisconnectCallback _disconnectCallback; // 连接断开回调
    
    asio::io_service _ioService;
    tcp::socket _socket;
    
    std::recursive_mutex _mutex; // 线程锁
    
    thread* _thread; // 通信线程
    
    SendMsgQueue _sendMsgQueue; // 消息发送队列
};


/**
 * 接收数据对象
 */
class ReceiveMsg
{
public:
    
    friend class TcpConnection;
    
    ReceiveMsg()
    : _bodyLength(0),_headerLength(0)
    {
        
        _data = new int8_t[MAX_HEADER_LENGTH]; // 预先分配头信息内存空间
    }
    
    ~ReceiveMsg()
    {
        if(_data)
        {
            delete[] _data;
            _data = nullptr;
        }
    }
    
    const int8_t* data() const
    {
        return _data;
    }
    
    int8_t* data()
    {
        return _data;
    }
    
    std::size_t length() const
    {
        return _headerLength + _bodyLength;
    }
    
    
    const int8_t* body() const
    {
        return _data + _headerLength;
    }
    
    
    int8_t* body()
    {
        return _data + _headerLength;
    }
    
    std::size_t bodyLength() const
    {
        return _bodyLength;
    }
    
    int8_t* receiveBody()
    {
        return _data + _headerLength;
    }
    
    std::size_t receiveBodyLength()
    {
        return  _bodyLength;
    }
    
    uint64_t readRawVarint64SlowPath(int8_t* _buffer, std::size_t bufferSize, std::size_t* outRealHeaderLen) {
        uint64_t result = 0L;
        
        std::size_t bufferPos = 0;
        std::size_t i = 0;
        for (std::size_t shift = 0; shift < 64; shift += 7) {
            ++i;
            int8_t b = _buffer[bufferPos++];
            result |= (uint64_t)(b & 127) << shift;
            if ((b & 128) == 0) {
                *outRealHeaderLen = i;
                return result;
            }
            
        }
        return 0;
    }
    
    std::size_t readRawVarint32(int8_t* _buffer, std::size_t bufferSize, std::size_t* outRealHeaderLen, bool* isDecodeOver)
    {
        int bufferPos = 0;
        int pos;
        int32_t var5;
        *isDecodeOver = true;
        do
        {
            pos = bufferPos;
            if(bufferSize != pos) {
                int8_t* buffer = _buffer;
                int8_t x;
                if((x = buffer[pos++]) >= 0) {
                    bufferPos = pos;
                    return x;
                }
                
                if(pos == bufferSize) // 头信息不足，需要继续传递
                {
                    *isDecodeOver = false;
                    return 0;
                }
                
                if((int64_t)(var5 = x ^ buffer[pos++] << 7) < 0L) {
                    var5 = (int32_t)((int64_t)var5 ^ -128L);
                    break;
                }
                
                
                if(pos == bufferSize) // 头信息不足，需要继续传递
                {
                    *isDecodeOver = false;
                    return 0;
                }
                
                if((int64_t)(var5 ^= buffer[pos++] << 14) >= 0L) {
                    var5 = (int32_t)((int64_t)var5 ^ 16256L);
                    break;
                }
                
                if(pos == bufferSize) // 头信息不足，需要继续传递
                {
                    *isDecodeOver = false;
                    return 0;
                }
                
                if((int64_t)(var5 ^= buffer[pos++] << 21) < 0L) {
                    var5 = (int32_t)((int64_t)var5 ^ -2080896L);
                    break;
                }
                
                if(pos == bufferSize) // 头信息不足，需要继续传递
                {
                    *isDecodeOver = false;
                    return 0;
                }
                
                int8_t y = buffer[pos++];
                var5 ^= y << 28;
                var5 = (int32_t)((int64_t)var5 ^ 266354560L);
                if(y >= 0 || buffer[pos++] >= 0 || buffer[pos++] >= 0 || buffer[pos++] >= 0 || buffer[pos++] >= 0 || buffer[pos++] >= 0) {
                    break;
                }
                
            }
            return readRawVarint64SlowPath(_buffer, bufferPos, outRealHeaderLen);
        } while (0);
        
        *outRealHeaderLen = pos;
        return var5;
    }
    
    /**
     * 解析数据头
     * @return -1：解析失败， 0：解析成功，1：需要继续填入数据进行解析
     *
     */
    int decodeHeader(std::size_t headerLen)
    {
        std::size_t realHeaderLen = 0;
        bool isDecodeOver = false; // 是否已经解析完毕
        
        std::size_t tmpBodyLen = readRawVarint32(_data, headerLen, &realHeaderLen, &isDecodeOver);
        if(!isDecodeOver)
        {
            return 1;
        }
        
        _bodyLength = tmpBodyLen;
        _headerLength = realHeaderLen;
        
        if (_bodyLength <= 0)
        {
            return -1;
        }
        int8_t* newData = new int8_t[_bodyLength + _headerLength]; // 重新分配数据内存
        
        memcpy(newData, _data, _headerLength);
        
        delete[] _data; // 释放之前的数据
        _data = nullptr;
        
        _data = newData; // 重新给数据赋值
        
        return 0;
    }
    
private:
    int8_t* _data;
    std::size_t _headerLength;
    std::size_t _bodyLength;
};



/**
 * 发送数据对象
 */
class SendMsg
{
public:
    
    /**
     * 发送消息回调
	 * @param sendId 本次发送的id
     * @param errorCode 发送结果，0：成功，-1：失败
     */
    typedef std::function<void(int sendId,int errorCode)> SendCallback;
    
    friend class TcpConnection;
    
    SendMsg()
    : _bodyLength(0),_headerLength(0)
    {
        _data = nullptr;
		_sendId = -1;
    }
    
    SendMsg(const SendMsg& sendMsg)
    {
        if(this->_data)
        {
            this->_data = new int8_t[sendMsg.bodyLength() + MAX_HEADER_LENGTH];
            std::memcpy(this->_data, sendMsg._data, sendMsg.bodyLength() + MAX_HEADER_LENGTH);
        }
		this->_sendId = sendMsg._sendId;
        this->_sendCallback = sendMsg._sendCallback;
        this->_bodyLength = sendMsg._bodyLength;
        this->_headerLength = sendMsg._headerLength;
    }
    
    ~SendMsg()
    {
        if(_data)
        {
            delete[] _data;
            _data = nullptr;
        }
    }
    
	int32_t getSendId()
	{
		return _sendId;
	}

	void setSendId(const int32_t& sendId)
	{
		this->_sendId = sendId;
	}

    const int8_t* data() const
    {
        return _data;
    }
    
    int8_t* data()
    {
        return _data;
    }
    
    std::size_t length() const
    {
        return _headerLength + _bodyLength;
    }

    
    const int8_t* body() const
    {
        return _data + _headerLength;
    }
    
    
    int8_t* body()
    {
        return _data + _headerLength;
    }
    
    std::size_t bodyLength() const
    {
        return _bodyLength;
    }
    
    void setBody(void* bodyData, std::size_t bodyLength)
    {
        if(_data && _data != bodyData) // 如果之前有数据，释放
        {
            delete[] _data;
            _data = nullptr;
        }
        
        
        _data = new int8_t[bodyLength + MAX_HEADER_LENGTH]; // 分配数据内存
        _bodyLength = bodyLength;
        encodeHeader(); // 编码数据头并进行头填充
        
        std::memcpy(_data + _headerLength, bodyData, _bodyLength); // 填充数据
    }
    
private:
    
    std::size_t computeRawVarint32Size(std::size_t value) {
        return (value & -128) == 0 ? 1 : ((value & -16384) == 0 ? 2 : ((value & -2097152) == 0 ? 3 : ((value & -268435456) == 0 ? 4 : 5)));
    }
    
    void fillHeader(int8_t* header, std::size_t bodyLen)
    {
        std::size_t i = 0;
        while ((bodyLen & -128) != 0) {
            header[i++] = (bodyLen & 127) | 128;
            //headerLen = headerLen / (int)(pow(2, 7));
            bodyLen = bodyLen >> 7;
        }
        
        header[i] = bodyLen;
    }
    
    void encodeHeader()
    {
        std::size_t bodyLen = _bodyLength;
        std::size_t headerLen = computeRawVarint32Size(bodyLen);
        _headerLength = headerLen;
        
        int8_t* header = new int8_t[headerLen];
        fillHeader(header, bodyLen);
        std::memcpy(_data, header, headerLen);
        delete[] header;
    }
    
private:
	int32_t _sendId;
    int8_t* _data;
    std::size_t _headerLength;
    std::size_t _bodyLength;
    
    SendCallback _sendCallback;
};



#endif /* TCP_CONNECTION_HPP */
