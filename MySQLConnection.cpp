#include <iostream>
#include "MySQLConnection.h"


// 初始化一个新的mysql object
MySQLConnection::MySQLConnection():
	mysqlConn_(mysql_init(nullptr)),
	alivetime_(clock())
{
}

// 释放mysql链接资源
MySQLConnection::~MySQLConnection()
{
	if (mysqlConn_ != nullptr)
		mysql_close(mysqlConn_);
}

// 通过mysql object创建mysql 链接服务
bool MySQLConnection::connect(std::string ip, unsigned short port,std::string username, std::string password, std::string dbname)
{
	MYSQL* p = mysql_real_connect(mysqlConn_, ip.c_str(), username.c_str(), password.c_str(), dbname.c_str(), port, nullptr, 0);
	if (p == nullptr) LOG(mysql_error(mysqlConn_));
	
	return p != nullptr;
}

// 处理select操作
MYSQL_RES* MySQLConnection::query(std::string sql)
{
	if (!mysql_query(mysqlConn_, sql.c_str())) return mysql_use_result(mysqlConn_);

	LOG("查询失败:" + sql);
	return nullptr;	
}

// 更新数据库操作 insert delete updata等handler的操作
bool MySQLConnection::update(std::string sql)
{
	if (!mysql_query(mysqlConn_, sql.c_str())) return true;
	
	LOG("更新失败:" + sql);
	return false;
}


// 刷新连接的起始的空闲时间点
void MySQLConnection::refreshAliveTime() 
{ 
	alivetime_ = clock(); 
}

// 返回存活的时间
clock_t MySQLConnection::getAliveTime()const 
{ 
	return clock() - alivetime_; 
}