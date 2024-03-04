#pragma once
#include <atomic>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <queue>
#include <thread>
#include "MySQLConnection.h"


#define LOG(str) std::cout << __FILE__ << ":" << __LINE__ << " " << \
	__TIMESTAMP__ << " : " << str << std::endl;

/*
 *实现连接池功能模块, 目前基于mysql实现的连接池，后续增加其他类型数据库
*/
class ConnectionPool
{
public:
	// 获取连接池单个对象实例
	static ConnectionPool* getConnectionPool();
	// 从连接池中获取一个可用连接
	std::shared_ptr<MySQLConnection> getConnection();
private:
	ConnectionPool();
	~ConnectionPool() = default;
	bool loadConfigFile();
	void produceConnectionPoolTask();

	// 扫描超过maxIdleTime时间的空闲连接，进行对于的连接回收
	void scanConnectionPoolTask();

	std::string mysqlIP_; // mysql的ip地址
	unsigned short mysqlPort_; // mysql的端口号 3306
	std::string userName_; // mysql登录用户名
	std::string password_; // mysql登录密码
	std::string dbName_; // 连接的数据库名称
	int initConnSize_; // 连接池的初始连接量
	int maxConnSize_; // 连接池的最大连接量
	int maxIdleTime_; // 连接池最大空闲时间
	int connectionTimeout_; // 连接池获取连接的超时时间

	std::queue<MySQLConnection*> connectionQueue_; // 存储mysql连接队列
	std::mutex queueMutex_; // 维护连接队列的线程安全互斥锁
	std::atomic_int connectionCount_; // 记录连接所创建的connection连接的总数量 
	std::condition_variable cv; // 设置条件变量，用于连接生产线程和连接消费线程的通信
};