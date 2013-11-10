#ifndef __ALBERT_DATABASE_DB_H__
#define	__ALBERT_DATABASE_DB_H__
#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <time.h>
using namespace std;

namespace AlDb
{
	typedef void*	HANDLE;

	enum DB_RET
	{
		DB_RET_ERR_DB = -1000,
		DB_RET_ERR_DB_PRIMARY_KEY,
		DB_RET_ERR_DB_ATTRIBUTE,
		DB_RET_ERR_DB_NOT_FOUND,
		DB_RET_ERR_DB_EXIST,
		DB_RET_ERR_DB_EMPTY,
		DB_RET_ERR_DB_FULL,
		DB_RET_ERR_DB_TYPE,				//	Incorrect data type.
		DB_RET_ERR_SYS = -500,
		DB_RET_ERR_SYS_MEMORY,			//	System memory error.
		DB_RET_ERR_PARAMETER,			//	Input parameter incorrect
		DB_RET_ERR_PROCEDURE,			//	Procedure incorrect.
		DB_RET_ERR_INTERNAL,
		DB_RET_SUCCESS = 0
	};

	enum DATA_T
	{
		DATA_T_NONE = 0,
		DATA_T_BOOL,
		DATA_T_DECIMAL,
		DATA_T_INTEGER,
		DATA_T_TIME,
		DATA_T_STRING,
		DATA_T_END
	};

	struct DATA_VAL
	{
		bool			bNull;
		union 
		{
			bool		b;
			double		d;
			int			i;
			time_t		t;
		};
		string			str;
	};

	/*****************************************************************************/
	/****************************** Data Unit Class ******************************/
	/*****************************************************************************/
	//	Version: 1.0.0
	//		Release Data: 2013-11-09
	class DataUnit
	{
	public:
		DataUnit();
		DataUnit(DATA_T DataType);

		//	# Description:
		//		Set the data type. This function will also set data to NULL.
		//	i/p:
		//		DataType	: The type.
		//	o/p:
		//		DB_RET_SUCCESS			: Success.
		//		DB_RET_ERR_PARAMETER	: Input parameter incorrect
		DB_RET setDataType(DATA_T DataType);

		//	# Description:
		//		Get the data type.
		//	i/p:
		//		None.
		//	o/p:
		//		The data type.
		DATA_T getDataType();

		//	# Description:
		//		Set the data.
		//	i/p:
		//		Data	: The data.
		//	o/p:
		//		DB_RET_SUCCESS			: Success.
		//		DB_RET_ERR_PROCEDURE	: Procedure incorrect.
		//		DB_RET_ERR_DB_TYPE		: Incorrect data type.
		//		DB_RET_ERR_SYS_MEMORY	: System memory error.
		DB_RET setData(DATA_VAL *pData);

		//	# Description:
		//		Get the data.
		//	i/p:
		//		pData	: A variable pointer to load data.
		//	o/p:
		//		DB_RET_SUCCESS		: Success.
		//		DB_RET_ERR_PROCEDURE: Procedure incorrect.
		//		DB_RET_ERR_DB_TYPE	: Incorrect data type.
		DB_RET getData(DATA_VAL *pData);

		//	# Description:
		//		Set data to NULL.
		//	i/p:
		//		None.
		//	o/p:
		//		None.
		void clearData();

		bool isNull();

	private:
		DATA_T				m_DataType;
		DATA_VAL			m_Data;
		DB_RET				m_Ret;
	};

	class Column
	{
	public:
		Column();
		Column(HANDLE hTable, string strColName, DATA_T DataType, bool bPriKey, bool bNullable, string strForeKey);
		DB_RET open(HANDLE hTable, string strColName, DATA_T DataType, bool bPriKey, bool bNullable, string strForeKey);
		
		DB_RET addData(DataUnit Data);
		DB_RET deleteData(DataUnit Data);
		DB_RET deleteData(unsigned int uiIndex);
		int searchData(DataUnit *pData);
		DB_RET getData(unsigned int uiIndex, DataUnit *pData);
		DB_RET checkData(DataUnit Data);

	private:
		HANDLE				m_hTable;
		string				m_strColName;
		DATA_T				m_DataType;
		bool				m_bPriKey;
		bool				m_bNullable;
		string				m_strForeKey;
		vector<DataUnit>	m_vDatas;
		struct 
		{
			int			iLast;
			DataUnit	Data;
		}m_Search;
	};

	class Table
	{
	public:
		Table(){}
		Table(HANDLE hDb, string strTableName){ cout << __FUNCTION__ << "()" << endl;}
		DB_RET open(HANDLE hDb, string strTableName){ cout << __FUNCTION__ << "()" << endl; return DB_RET_SUCCESS;}
		
		Column* addColumn(string strColName, DATA_T DataType, bool bPrimaryKey, bool bNullable, string strForeignKey){ cout << __FUNCTION__ << "()" << endl; Column *c; return c;}
		Column* getColumn(string strColName){ cout << __FUNCTION__ << "()" << endl; Column *c; return c;}
		Column* searchColumn(string strColName){ cout << __FUNCTION__ << "()" << endl; Column *c; return c;}
		
		DB_RET addRecord(vector<DataUnit> vData){ cout << __FUNCTION__ << "()" << endl; return DB_RET_SUCCESS;}
		DB_RET deleteRecord(vector<DataUnit> vData){ cout << __FUNCTION__ << "()" << endl; return DB_RET_SUCCESS;}
		DB_RET deleteRecord(unsigned int uiIndex){ cout << __FUNCTION__ << "()" << endl; return DB_RET_SUCCESS;}
		int searchRecord(vector<DataUnit> vData){ cout << __FUNCTION__ << "()" << endl; return 0;}
		DB_RET getRecord(unsigned int uiIndex, vector<DataUnit> *pvData){ cout << __FUNCTION__ << "()" << endl; return DB_RET_SUCCESS;}

	private:
		HANDLE				m_hDb;
		string				m_strTableName;
		unsigned int		m_uiPrimary;
		vector<Column>		m_vCols;
	};

	class Db
	{
	public:
		Db(){}
		Db(string strDbName, string strPath){ cout << __FUNCTION__ << "()" << endl;}
		DB_RET open(string strDbName, string strPath){ cout << __FUNCTION__ << "()" << endl; return DB_RET_SUCCESS;}
		
		Table* addTable(string strTableName){ cout << __FUNCTION__ << "()" << endl; Table *t1 = new Table; return t1;}
		Table* getTable(string strTableName){ cout << __FUNCTION__ << "()" << endl; Table *t1 = new Table; return t1;}
		unsigned int searchTable(string strTableName){ cout << __FUNCTION__ << "()" << endl; return 1;}
		
		Column* addColumn(string strTableName, string strColName, DATA_T DataType, bool bPrimaryKey, bool bNullable, string strForeignKey){ cout << __FUNCTION__ << "()" << endl; Column *c = new Column; return c;}
		Column* getColumn(string strTableName, string strColName){ cout << __FUNCTION__ << "()" << endl; Column *c; return c;}
		Column* searchColumn(string strTableName, string strColName){ cout << __FUNCTION__ << "()" << endl; Column *c; return c;}
		
		DB_RET addRecord(string strTableName, vector<DataUnit> vData){ cout << __FUNCTION__ << "()" << endl; return DB_RET_SUCCESS;}
		DB_RET deleteRecord(string strTableName, vector<DataUnit> vData){ cout << __FUNCTION__ << "()" << endl; return DB_RET_SUCCESS;}
		DB_RET deleteRecord(string strTableName, unsigned int uiIndex){ cout << __FUNCTION__ << "()" << endl; return DB_RET_SUCCESS;}
		int searchRecord(string strTableName, vector<DataUnit> vData){ cout << __FUNCTION__ << "()" << endl; return 0;}
		DB_RET getRecord(string strTableName, unsigned int uiIndex, vector<DataUnit> *pvData){ cout << __FUNCTION__ << "()" << endl; return DB_RET_SUCCESS;}

		DB_RET load(){ cout << __FUNCTION__ << "()" << endl; return DB_RET_SUCCESS;}
		DB_RET commit(){ cout << __FUNCTION__ << "()" << endl; return DB_RET_SUCCESS;}

	private:
		string				m_strDbName;
		string				m_strPath;
		vector<Table>		m_vTable;
	};
}

#endif // !__ALBERT_DATABASE_DB_H__



