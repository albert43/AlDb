#ifndef __ALBERT_DATABASE_DB_H__
#define	__ALBERT_DATABASE_DB_H__
#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <time.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <direct.h>
using namespace std;

#define	VERSION_LIB_ALDB_MAJOR		1
#define	VERSION_LIB_ALDB_MINOR		0
#define	VERSION_LIB_ALDB_RELEASE	0
#define	VERSION_LIB_ALDB_BUILD		5

#define	ALDB_NAME_LENGTH_MAX		32
#define	ALDB_COLUMN_NUM_MAX			32

#define	NEW_LINE					"\r\n"


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
		DB_RET_ERR_SYS_IO,			//	System i/o error.
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
		union 
		{
			bool		b;
			double		d;
			int			i;
			time_t		t;
		};
		string			str;
	};

	struct COLUMN_ATTR
	{
		string			strColName;
		DATA_T			DataType;
		bool			bPriKey;
		int				iForeKey;
	};

	class Column
	{
	public:
		Column();
		Column(HANDLE hTable, string strColName, DATA_T DataType, bool bPriKey, int iForeKey);
		
		//	# Description:
		//		Open a Column class.
		//	i/p:
		//		hTable		: The table handle.
		//		strColName	: The column name.
		//		DataType	: The column data type.
		//		bPriKey		: It's primary key column.
		//		strForeKey	: The foreign key table name.
		//	o/p:
		//		DB_RET_SUCCESS				: Success
		//		DB_RET_ERR_PARAMETER		: Input parameter incorrect.
		//		DB_RET_ERR_DB_PRIMARY_KEY	: Violate the primary attribution.
		DB_RET open(HANDLE hTable, string strColName, DATA_T DataType, bool bPriKey, int iForeKey);

		//	# Description:
		//		Add a data in column.
		//	i/p:
		//		pData	: The DATA_VAL pointer of the data.
		//	o/p:
		//		DB_RET_SUCCESS				: Success
		//		DB_RET_ERR_PROCEDURE
		//		DB_RET_ERR_DB_ATTRIBUTE		: Violate the column attribution.
		//		DB_RET_ERR_DB_PRIMARY_KEY	: Violate the primary attribution.
		//		DB_RET_ERR_DB_FULL			: The column is full.
		//		DB_RET_ERR_INTERNAL
		DB_RET addData(DATA_VAL *pData);

		//	# Description:
		//		Search the data. If pData is not NULL, it will search from 
		//		the begin of the list, otherwise, it will continue searching 
		//		the data.
		//	i/p:
		//		pData		: The DATA_VAL pointer of the data.
		//		bContiune
		//			TRUE	: Search from the start.(index 0)
		//			FALSE	: Continue the last search.
		//	o/p:
		//		Success	: It returns the index of the data.
		//		Failure	: It returns a number less than 0 which match the DB_RET.
		//			DB_RET_ERR_DB_NOT_FOUND	: Data not found.
		//			DB_RET_ERR_PARAMETER	: Input parameter incorrect.
		int searchData(DATA_VAL *pData, bool bContiune);

		//	# Description:
		//		Delete a data.
		//	i/p:
		//		pData	: The DATA_VAL pointer of the data.
		//	o/p:
		//		DB_RET_SUCCESS			: Success
		//		DB_RET_ERR_PARAMETER	: Input parameter incorrect.
		//		DB_RET_ERR_DB_NOT_FOUND	: Data not found.
		DB_RET deleteData(DATA_VAL *pData);

		//	# Description:
		//		Delete a data.
		//	i/p:
		//		uiIndex	: The index of the vector.
		//	o/p:
		//		DB_RET_SUCCESS			: Success
		//		DB_RET_ERR_DB_NOT_FOUND	: Data not found.
		DB_RET deleteData(unsigned int uiIndex);

		//	# Description:
		//		Get the data.
		//	i/p:
		//		uiIndex	: The vector index.
		//		pData	: 
		//	o/p:
		//		DB_RET_SUCCESS			: Success
		//		DB_RET_ERR_DB_NOT_FOUND	: Data not found.
		DB_RET getData(unsigned int uiIndex, DATA_VAL *pData);

		//	# Description:
		//		Compare the data. Include data type.
		//	i/p:
		//		uiIndex	: The index of the data in the column
		//		pData	: The compared data.
		//	o/p:
		//		TRUE	: It's the same.
		//		FALSE	: It's NOT the same.
		bool compareData(unsigned int uiIndex, DATA_VAL *pData);
		
		//	# Description:
		//		Get data number.
		//	i/p:
		//		None.
		//	o/p:
		//		The data number of the column.
		unsigned int getDataNumber();
		
		//	# Description:
		//		Reset the column.
		//	i/p:
		//		None.
		//	o/p:
		//		None.
		void reset();

		DB_RET getAttribute(COLUMN_ATTR *pAttr);

		DB_RET save(FILE *fp);
		
	private:
		HANDLE				m_hTable;
		COLUMN_ATTR			m_Attr;
		vector<DATA_VAL>	m_vstDatas;
		struct
		{
			int				iLast;
			DATA_VAL		stData;
		}m_Search;
	};//	End of Column class

	struct TABLE_ATTR
	{
		string				strTableName;
		int					iPrimary;
		vector<int>			viForeign;
	};
	
	class Table
	{
	public:
		Table();
		Table(HANDLE hDb, string strTableName);
		~Table();
		//	# Description:
		//		pOen a Table class.
		//	i/p:
		//		hDb			: The DB handle.
		//		strTableName: The table name.
		//	o/p:
		//		DB_RET_SUCCESS				: Success
		//		DB_RET_ERR_PARAMETER		: Input parameter incorrect.
		DB_RET open(HANDLE hDb, string strTableName);

		DB_RET setDbHandle(HANDLE hDb);

		DB_RET getAttribute(TABLE_ATTR *pAttr);

		//	# Description:
		//		Add a column in the table.
		//	i/p:
		//		strColName	: The column name.
		//		DataType	: The column data type.
		//		bPrimaryKey	: It's primary key column.
		//		strForeKey	: The foreign key table name.
		//	o/p:
		//		DB_RET_SUCCESS				: Success
		//		DB_RET_ERR_DB_ATTRIBUTE		: Violate table attribution.
		//		DB_RET_ERR_PARAMETER		: Input parameter incorrect.
		//		DB_RET_ERR_DB_PRIMARY_KEY	: Violate the primary attribution.
		//		DB_RET_ERR_DB_FULL			: The column is full.
		DB_RET addColumn(string strColName, DATA_T DataType, bool bPrimaryKey, int iForeignKey);
		
		//	# Description:
		//		Search the column.
		//	i/p:
		//		strColName	: The column name.
		//	o/p:
		//		Success	: It returns the index of the column.
		//		Failure	: It returns -1.
		int searchColumn(string strColName);

		//	# Description:
		//		Get a column entry.
		//	i/p:
		//		strColName	: The column name.
		//	o/p:
		//		Success	: The Column class pointer of the column.
		//		Failure	: It returns NULL. The reason is NOT FOUND.
		const Column* getColumn(string strColName);
		
		//	# Description:
		//		Add a record in the table.
		//	i/p:
		//		vRecord	: The record. The record is a set of data which must store 
		//				  in the vector with the order the same as the column order.
		//	o/p:
		//		DB_RET_SUCCESS			: Success
		//		DB_RET_ERR_PROCEDURE	: Procedure incorrect.
		//		DB_RET_ERR_PARAMETER	: Input parameter incorrect.
		DB_RET addRecord(DATA_VAL *paRecord);
		
		//	# Description:
		//		Search the record. The input vector must matches all of the data.
		//	i/p:
		//		uiCompareItemNum: The search sets number.
		//		The set are a integer and a DATA_VAL.
		//		The integer is the column index.
		//		The DATA_VAL is the value.
		//	o/p:
		//		Success	: It returns the index of the record.
		//		Failure	: It returns a number less than 0 which match the DB_RET.
		//			DB_RET_ERR_PROCEDURE	: Procedure incorrect.
		//			DB_RET_ERR_PARAMETER	: Input parameter incorrect.
		int searchRecord(unsigned int uiCompareItemNum, ...);
		int searchRecord(unsigned int uiCompareItemNum, va_list List);

		//	# Description:
		//		Delete the record. The input vector must matches all of the data.
		//	i/p:
		//		uiIndex	: The index of the record.
		//	o/p:
		//		DB_RET_SUCCESS									: Success
		//		DB_RET_ERR_PROCEDURE[searchRecord()]			: Procedure incorrect.
		//		DB_RET_ERR_PARAMETER[searchRecord()]			: Input parameter incorrect.
		//		DB_RET_ERR_DB_NOT_FOUND[Column::deleteData()]	: Data not found.
		DB_RET deleteRecord(int uiIndex);

		//	# Description:
		//		Get the record.
		//	i/p:
		//		uiIndex	: The index of the record.
		//		paRecord: A array pointer that receive the record. The number of array
		//				  must match the number of column.
		//	o/p:
		//		DB_RET_SUCCESS			: Success
		//		DB_RET_ERR_DB_NOT_FOUND	: Data not found.
		DB_RET getRecord(unsigned int uiIndex, DATA_VAL *paRecord);
		
		//	# Description:
		//		Add a record in the table.
		//	i/p:
		//		None.
		//	o/p:
		//		The record number of the table.
		unsigned int getRecordNum();

		//	# Description:
		//		Check the table is valid. Including which has primary and the 
		//		column number is larger than 1.
		//	i/p:
		//		None.
		//	o/p:
		//		TRUE	: It's valid.
		//		FALSE	: It's NOT valid.
		bool valid();

		DB_RET save(string strDbPath);
		DB_RET commit(string strDbPath);

	private:
		HANDLE					m_hDb;
		TABLE_ATTR				m_Attr;
		vector<Column>			m_vCols;
		struct 
		{
			struct
			{
				int				ai[ALDB_COLUMN_NUM_MAX];
				DATA_VAL		ast[ALDB_COLUMN_NUM_MAX];
			}Item;
			unsigned int		uiComparedNum;
			int					iLast;
		}m_Search;

		//	# Description:
		//		Reset the column.
		//	i/p:
		//		None.
		//	o/p:
		//		None.
		void reset();
	};//	End of Table class

	class Db
	{
	public:
		Db();
		Db(string strDbName, string strPath);
		~Db();
		DB_RET open(string strDbName, string strPath);
		
		DB_RET addTable(Table *pTable);
		const Table* getTable(string strTableName);
		int searchTable(string strTableName);
		
		DB_RET addRecord(string strTableName, DATA_VAL *paRecord);
		DB_RET deleteRecord(string strTableName, unsigned int uiIndex);
		int searchRecord(string strTableName, unsigned int uiCompareItemNum, ...);
		DB_RET getRecord(string strTableName, unsigned int uiIndex, DATA_VAL *paRecord);

		DB_RET load(){ cout << __FUNCTION__ << "()" << endl; return DB_RET_SUCCESS;}
		DB_RET save();
		DB_RET commit();
		DB_RET revert(){ cout << __FUNCTION__ << "()" << endl; return DB_RET_SUCCESS;}

		bool valid();
	private:
		string				m_strDbName;
		string				m_strPath;
		vector<void *>		m_vTables;
	};
}

#endif // !__ALBERT_DATABASE_DB_H__



