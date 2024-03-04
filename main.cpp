#include <iostream>
#include "ConnectionPool.h"
#include "MySQLConnection.h"

int main()
{
	clock_t begin = clock();

	std::thread t1([]() {
		ConnectionPool* cp(ConnectionPool::getConnectionPool());
		for (int i = 0; i < 50; ++i)
		{
			char sql[1024] = { 0 };
			sprintf_s(sql, "insert into student(name,age,sex,score) values('%s',%d,'%s',%d)",
				"wukong", 28, "male", 100);
			std::shared_ptr<MySQLConnection> sp = cp->getConnection();
			sp->update(sql);
		}
	});
	std::thread t2([]() {
		ConnectionPool* cp(ConnectionPool::getConnectionPool());
		for (int i = 0; i < 50; ++i)
		{
			char sql[1024] = { 0 };
			sprintf_s(sql, "insert into student(name,age,sex,score) values('%s',%d,'%s',%d)",
				"wukong", 29, "female", 90);
			std::shared_ptr<MySQLConnection> sp = cp->getConnection();
			sp->update(sql);
		}
	});
	std::thread t3([]() {
		ConnectionPool* cp(ConnectionPool::getConnectionPool());
		for (int i = 0; i < 50; ++i)
		{
			char sql[1024] = { 0 };
			sprintf_s(sql, "insert into student(name,age,sex,score) values('%s',%d,'%s',%d)",
				"wukong", 20, "male", 88);
			std::shared_ptr<MySQLConnection> sp = cp->getConnection();
			sp->update(sql);
		}
	});
	std::thread t4([]() {
		ConnectionPool* cp(ConnectionPool::getConnectionPool());
		for (int i = 0; i < 50; ++i)
		{
			char sql[1024] = { 0 };
			sprintf_s(sql, "insert into student(name,age,sex,score) values('%s',%d,'%s',%d)",
				"wukong", 20, "male", 97);
			std::shared_ptr<MySQLConnection> sp = cp->getConnection();
			sp->update(sql);
		}
	});

	t1.join();
	t2.join();
	t3.join();
	t4.join();

	clock_t end = clock();
	std::cout << (end - begin) << "ms" << std::endl;


#if 0
	for (int i = 0; i < 10; ++i)
	{
		/*MySQLConnection conn;
		char sql[1024] = { 0 };
		sprintf_s(sql,  "insert into student(name,age,sex,score) values('%s',%d,'%s',%d)",
			"wukong", 30, "male", 87);
		conn.connect("127.0.0.1", 3306, "root", "xu2024", "connection_pool");
		conn.update(sql);*/

		ConnectionPool* cp(ConnectionPool::getConnectionPool());
		std::shared_ptr<MySQLConnection> sp = cp->getConnection();
		char sql[1024] = { 0 };
		sprintf_s(sql, "insert into student(name,age,sex,score) values('%s',%d,'%s',%d)",
			"wukong", 30, "male", 89);
		sp->update(sql);
	}
#endif

	return 0;
}