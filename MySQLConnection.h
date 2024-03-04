#pragma once
#include <ctime>
#include <iostream>
#include <mysql.h>

#define LOG(str) std::cout << __FILE__ << ":" << __LINE__ << " " << \
	__TIMESTAMP__ << " : " << str << std::endl;

/**
 *MySQL数据库实现及其相关操作
**/
class MySQLConnection
{
public:
	// 构造函数 初始化数据库连接
	MySQLConnection();
	// 析构函数 释放数据库连接资源
	~MySQLConnection();

	MYSQL_RES* query(std::string sql);
	bool update(std::string sql);
	void refreshAliveTime();
	clock_t getAliveTime()const;
	bool connect(std::string ip,unsigned short port,std::string user,std::string password,std::string dbname);
private:
	MYSQL* mysqlConn_; // 与MySQL Server的服务连接
	clock_t alivetime_; // 进入空闲状态后的起始存活时间
};