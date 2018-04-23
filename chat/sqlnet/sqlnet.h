#ifndef __SQLNET_H
#define __SQLNET_H
#include <list>
#include <string>

#ifdef __CLUDESQL
#include "mysql.h"
#else
class MYSQL;
#endif // __CLUDESQL


using namespace std;

#define SQL_API __declspec(dllexport)

class SQL_API cMySql
{
public:
	cMySql();
	~cMySql();

	//连接数据库
	bool connect(char* pAddr, int port, char* databaseName, char* account, char* password);

	//执行sql语句
	bool sqlExecute(char* sql);
	//获得查询结果
	list<list<string>> getExecuteResults()
	{
		return m_listResult;
	}

private:
	MYSQL* m_sql;
	list<list<string>> m_listResult;
};


#endif // !__SQLNET_H
