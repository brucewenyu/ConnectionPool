#include <iostream>
#include "MySQLConnection.h"


// ��ʼ��һ���µ�mysql object
MySQLConnection::MySQLConnection():
	mysqlConn_(mysql_init(nullptr)),
	alivetime_(clock())
{
}

// �ͷ�mysql������Դ
MySQLConnection::~MySQLConnection()
{
	if (mysqlConn_ != nullptr)
		mysql_close(mysqlConn_);
}

// ͨ��mysql object����mysql ���ӷ���
bool MySQLConnection::connect(std::string ip, unsigned short port,std::string username, std::string password, std::string dbname)
{
	MYSQL* p = mysql_real_connect(mysqlConn_, ip.c_str(), username.c_str(), password.c_str(), dbname.c_str(), port, nullptr, 0);
	if (p == nullptr) LOG(mysql_error(mysqlConn_));
	
	return p != nullptr;
}

// ����select����
MYSQL_RES* MySQLConnection::query(std::string sql)
{
	if (!mysql_query(mysqlConn_, sql.c_str())) return mysql_use_result(mysqlConn_);

	LOG("��ѯʧ��:" + sql);
	return nullptr;	
}

// �������ݿ���� insert delete updata��handler�Ĳ���
bool MySQLConnection::update(std::string sql)
{
	if (!mysql_query(mysqlConn_, sql.c_str())) return true;
	
	LOG("����ʧ��:" + sql);
	return false;
}


// ˢ�����ӵ���ʼ�Ŀ���ʱ���
void MySQLConnection::refreshAliveTime() 
{ 
	alivetime_ = clock(); 
}

// ���ش���ʱ��
clock_t MySQLConnection::getAliveTime()const 
{ 
	return clock() - alivetime_; 
}