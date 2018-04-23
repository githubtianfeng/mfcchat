// sqlnet.cpp : ���� DLL Ӧ�ó���ĵ���������
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

//�������ݿ�
bool cMySql::connect(char* pAddr, int port, char* databaseName, char* account, char* password)
{
	//�������ݿ�
	if (!mysql_real_connect(m_sql, pAddr, account, password, databaseName, port, NULL, 0))
	{
		cout << "�������ݿ�ʧ��" << endl;
		cout << "������룺" << mysql_error(m_sql) << endl;
		return false;
	}
	return true;
}

//ִ��sql���
bool cMySql::sqlExecute(char* sql)
{
	if(mysql_query(m_sql, sql))
	{
		cout << "��ѯʧ��" << endl;
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
		//���һ����¼
		row = mysql_fetch_row(result);
		while (row)
		{
			//��֪�������ж��ٸ��ֶ�
			int count = mysql_field_count(m_sql);
			int index = 0;
			list<string> strList;
			//��һ����¼�е������ֶμ��뵽strlist
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