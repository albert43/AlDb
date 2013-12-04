#ifndef __ALBERT_DATABASE_DB_BASE_H__
#define	__ALBERT_DATABASE_DB_BASE_H__
#pragma once
#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

namespace AlDb
{
	typedef void*	HANDLE;

	enum DB_RET
	{
		DB_RET_DATA_ERR = -1000,
		DB_RET_DATA_ERR_PRIMARY_KEY,
		DB_RET_DATA_ERR_NULL,
		DB_RET_DATA_ERR_NOT_FOUND,
		DB_RET_DATA_ERR_EMPTY,
		DB_RET_DATA_ERR_FULL,
		DB_RET_ERR_PARAMETER,
		DB_RET_SUCCESS = 0
	};

	enum DATA_T
	{
		DATA_T_BOOL,
		DATA_T_DECIMAL,
		DATA_T_INTEGER,
		DATA_T_STRING,
		DATA_T_TIME
	};

	class DbBase
	{
	public:
		
		
		
	};

	class TableBase : public DbBase
	{
	public:
		TableBase(){}
		virtual DB_RET open(HANDLE hDb, string strTableName);
		virtual DB_RET addColumn(string strColName, bool bPrimaryKey, bool bNullable, HANDLE ForeignKey);
		virtual DB_RET addRecord();
		virtual DB_RET deleteRecord();
		virtual int searchRecord();
	};

	template <class T>
	class ColumnBase : public TableBase
	{
	public:
		virtual DB_RET openColumn(HANDLE *hTable, string strColName, bool bPriKey, bool bNullable, HANDLE ForeKey);
		virtual DB_RET addData(T Data);
		virtual DB_RET deleteData(T Data);
		virtual int searchData(T *pData);
		virtual DB_RET getData(unsigned uiIndex, T *pData);
		virtual DB_RET checkData(T Data);

		HANDLE			*m_hTable;
		string				m_strColName;
		bool				m_bPriKey;
		bool				m_bNullable;
		HANDLE			m_ForeKey;
		
	};
}
#endif // !__ALBERT_DATABASE_DB_BASE_H__
