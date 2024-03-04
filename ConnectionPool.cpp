#include "ConnectionPool.h"

// �̰߳�ȫ�������ص���
ConnectionPool* ConnectionPool::getConnectionPool()
{
	static ConnectionPool connPool;
	return &connPool;
}

// �������ļ��м��ظ�������
bool ConnectionPool::loadConfigFile()
{
	FILE* myIniFile = nullptr;
	errno_t err = fopen_s(&myIniFile, "mysql.ini", "r");
	if (err != 0)
	{
		LOG("open mysql.ini file failed, check it exist!");
		return false;
	}

	while (!feof(myIniFile))
	{
		char line[1024] = { 0 };
		fgets(line, 1024, myIniFile);
		std::string str = line;
		size_t kvi = str.find('=', 0);
		if (kvi == -1) continue;

		size_t end_kvi = str.find('\n', kvi);
		std::string key = str.substr(0, kvi);
		std::string value = str.substr(kvi + 1, end_kvi - kvi - 1);

		if (key == "ip")
		{
			mysqlIP_ = value;
		}
		else if (key == "port")
		{
			mysqlPort_ = atoi(value.c_str());
		}
		else if (key == "username")
		{
			userName_ = value;
		}
		else if (key == "password")
		{
			password_ = value;
		}
		else if (key == "dbname")
		{
			dbName_ = value;
		}
		else if (key == "initConnSize")
		{
			initConnSize_ = atoi(value.c_str());
		}
		else if (key == "maxConnSize")
		{
			maxConnSize_ = atoi(value.c_str());
		}
		else if (key == "maxIdleTime")
		{
			maxIdleTime_ = atoi(value.c_str());
		}
		else if (key == "connectionTimeOut")
		{
			connectionTimeout_ = atoi(value.c_str());
		}
	}
	return true;
}

ConnectionPool::ConnectionPool()
{
	if (!loadConfigFile()) return;

	for (int i = 0; i < initConnSize_; ++i)
	{
		MySQLConnection* p(new MySQLConnection());
		p->connect(mysqlIP_, mysqlPort_, userName_, password_, dbName_);
		connectionQueue_.push(p);
		connectionCount_++;
	}

	std::thread pTask(std::bind(&ConnectionPool::produceConnectionPoolTask, this));
	pTask.detach();

	std::thread sTask(std::bind(&ConnectionPool::scanConnectionPoolTask, this));
	sTask.detach();
}

// ���������ӵ��߳�
void ConnectionPool::produceConnectionPoolTask()
{
	for (;;)
	{
		std::unique_lock<std::mutex> lock(queueMutex_);
		while (!connectionQueue_.empty()) cv.wait(lock);

		if (connectionCount_ < maxConnSize_)
		{
			MySQLConnection* p(new MySQLConnection());
			p->connect(mysqlIP_, mysqlPort_, userName_, password_, dbName_);
			connectionQueue_.push(p);
			connectionCount_++;
		}

		cv.notify_all();
	}
}

// �����ӳ��л�ȡһ�����õ�mysql��������
std::shared_ptr<MySQLConnection> ConnectionPool::getConnection()
{
	std::unique_lock<std::mutex> lock(queueMutex_);
	while (connectionQueue_.empty())
	{
		if (std::cv_status::timeout == cv.wait_for(lock, std::chrono::milliseconds(connectionTimeout_)))
		{
			if (connectionQueue_.empty())
			{
				LOG("get idleConn timeout......");
				return nullptr;
			}
		}
	}

	std::shared_ptr<MySQLConnection> sp(connectionQueue_.front(),
		[&](MySQLConnection* mysqlConn) {
			std::unique_lock<std::mutex> lock(queueMutex_);
	        mysqlConn->refreshAliveTime();
	        connectionQueue_.push(mysqlConn);
	});

	connectionQueue_.pop();
	cv.notify_all();

	return sp;
}

// ɨ�賬��maxIdleTimeʱ��Ŀ������ӣ����ӻ���
void ConnectionPool::scanConnectionPoolTask()
{
	for (;;)
	{
		std::this_thread::sleep_for(std::chrono::seconds(maxIdleTime_));

		std::unique_lock<std::mutex> lock(queueMutex_);
		while (connectionCount_ > initConnSize_)
		{
			MySQLConnection* p = connectionQueue_.front();
			if (p->getAliveTime() < (maxIdleTime_ * 1000)) break;
            connectionQueue_.pop();
			connectionCount_--;
			delete p;
		}
	}
}