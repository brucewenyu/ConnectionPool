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
 *ʵ�����ӳع���ģ��, Ŀǰ����mysqlʵ�ֵ����ӳأ��������������������ݿ�
*/
class ConnectionPool
{
public:
	// ��ȡ���ӳص�������ʵ��
	static ConnectionPool* getConnectionPool();
	// �����ӳ��л�ȡһ����������
	std::shared_ptr<MySQLConnection> getConnection();
private:
	ConnectionPool();
	~ConnectionPool() = default;
	bool loadConfigFile();
	void produceConnectionPoolTask();

	// ɨ�賬��maxIdleTimeʱ��Ŀ������ӣ����ж��ڵ����ӻ���
	void scanConnectionPoolTask();

	std::string mysqlIP_; // mysql��ip��ַ
	unsigned short mysqlPort_; // mysql�Ķ˿ں� 3306
	std::string userName_; // mysql��¼�û���
	std::string password_; // mysql��¼����
	std::string dbName_; // ���ӵ����ݿ�����
	int initConnSize_; // ���ӳصĳ�ʼ������
	int maxConnSize_; // ���ӳص����������
	int maxIdleTime_; // ���ӳ�������ʱ��
	int connectionTimeout_; // ���ӳػ�ȡ���ӵĳ�ʱʱ��

	std::queue<MySQLConnection*> connectionQueue_; // �洢mysql���Ӷ���
	std::mutex queueMutex_; // ά�����Ӷ��е��̰߳�ȫ������
	std::atomic_int connectionCount_; // ��¼������������connection���ӵ������� 
	std::condition_variable cv; // ���������������������������̺߳����������̵߳�ͨ��
};