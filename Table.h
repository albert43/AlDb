#ifndef __ALBERT_DATABASE_TABLE_H__
#define	__ALBERT_DATABASE_TABLE_H__
#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include "DB.h"
#include "DataUnit.h"

using namespace std;
class PrimaryKey
{
public:
	PrimaryKey(){}
};

class ForeignKey
{
public:
	ForeignKey(){}
};

class Column : public Table
{
public: 
	string			m_strName;
	PrimaryKey		m_bPriKey;
	ForeignKey		m_ForeKey;		//	NULL: It's not foreign key
	bool			m_bNullable;
	Data			m_Data;
	
	Column(){}
	Column(string strName, DATA_T DataType, bool bPriKey, Table ForeKey, bool bNullable)
	{
		switch (DataType)
		{
			case DU_TYPE_INTEGER:
//				m_Data = (Data *) DataInteger();
			case DU_TYPE_DECIMAL:
//				m_Data = (Data *) DataDecimal();
			case DU_TYPE_BOOL:
//				m_Data = (Data *) DataBool();
			case DU_TYPE_STRING:
				m_Data = (Data) DataString(bPriKey, bNullable);
			case DU_TYPE_DATE:
//				m_Data = (Data *) DataDate();
			case DU_TYPE_TIME:
//				m_Data = (Data *) DataTime();
			;
		}
	}
	~Column(void){}
};

class Table
{
public:
	void				*m_hDb;
	string				m_strName;
	Column              m_Col;
	Record              m_Rec;
	
	vector<Column>		m_vCol;
	
	Table(void);
	Table(void *hDb, string strName, vector<Column> vCols)
	{
		m_hDb = hDb;
		m_strName = strName;
		m_vCol = vCols;
	}
	~Table(void);
};
#endif // !__ALBERT_DATABASE_TABLE_H__
