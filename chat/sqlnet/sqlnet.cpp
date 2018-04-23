// sqlnet.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include <winsock2.h>

#define __CLUDESQL
#include "sqlnet.h"

#include <iostream>
using namespace std;
#pragma comment(lib,"libmysql.lib")

cMySql::cMySql()
{
	m_sql = new MYSQL;
	mysql_init(m_sql);
	mysql_options(m_sql, MYSQL_SET_CHARSET_NAME, "gbk");
}

cMySql::~cMySql()
{
	delete m_sql;
}

//连接数据库
bool cMySql::connect(char* pAddr, int port, char* databaseName, char* account, char* password)
{
	//连接数据库
	if (!mysql_real_connect(m_sql, pAddr, account, password, databaseName, port, NULL, 0))
	{
		cout << "连接数据库失败" << endl;
		cout << "错误代码：" << mysql_error(m_sql) << endl;
		return false;
	}
	return true;
}

//执行sql语句
bool cMySql::sqlExecute(char* sql)
{
	if(mysql_query(m_sql, sql))
	{
		cout << "查询失败" << endl;
		return 0;
	}



	MYSQL_RES* result;
	result = mysql_store_result(m_sql);
	if (result == NULL)
	{
		return true;
	}
	m_listResult.clear();
	MYSQL_ROW row;
	if (mysql_num_rows(result))
	{
		//获得一个记录
		row = mysql_fetch_row(result);
		while (row)
		{
			//不知道里面有多少个字段
			int count = mysql_field_count(m_sql);
			int index = 0;
			list<string> strList;
			//把一个记录中的所有字段加入到strlist
			while (index < count)
			{
				if (row[index] == NULL)
				{
					strList.push_back("");
				}
				else
				{
					strList.push_back(row[index]);
				}
				++index;
			}
			m_listResult.push_back(strList);
			row = mysql_fetch_row(result);
		}
	}
	return true;
}