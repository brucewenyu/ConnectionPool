#pragma once
#include <ctime>
#include <iostream>
#include <mysql.h>

#define LOG(str) std::cout << __FILE__ << ":" << __LINE__ << " " << \
	__TIMESTAMP__ << " : " << str << std::endl;

/**
 *MySQL���ݿ�ʵ�ּ�����ز���
**/
class MySQLConnection
{
public:
	// ���캯�� ��ʼ�����ݿ�����
	MySQLConnection();
	// �������� �ͷ����ݿ�������Դ
	~MySQLConnection();

	MYSQL_RES* query(std::string sql);
	bool update(std::string sql);
	void refreshAliveTime();
	clock_t getAliveTime()const;
	bool connect(std::string ip,unsigned short port,std::string user,std::string password,std::string dbname);
private:
	MYSQL* mysqlConn_; // ��MySQL Server�ķ�������
	clock_t alivetime_; // �������״̬�����ʼ���ʱ��
};