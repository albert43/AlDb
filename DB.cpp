#include "Db.h"
using namespace AlDb;

/*****************************************************************************/
/****************************** Data Unit Class ******************************/
/*****************************************************************************/
DataUnit::DataUnit()
{
	cout << __FUNCTION__ << "():" << __LINE__ << endl;
	m_Data.bNull = true;
	m_DataType = DATA_T_NONE;
	m_Ret = DB_RET_SUCCESS;
}

DataUnit::DataUnit(DATA_T DataType)
{
	cout << __FUNCTION__ << "():" << __LINE__ << endl;
	m_Data.bNull = true;
	m_DataType = DATA_T_NONE;
	m_Ret = setDataType(DataType);
}

DB_RET DataUnit::setDataType(DATA_T DataType)
{
	cout << __FUNCTION__ << "():" << __LINE__ << endl;
	
	if ((DataType < DATA_T_NONE) || (DataType >= DATA_T_END))
		return DB_RET_ERR_PARAMETER;
	
	m_DataType = DataType;

	return DB_RET_SUCCESS;
}

DATA_T DataUnit::getDataType()
{
	cout << __FUNCTION__ << "():" << __LINE__ << endl;
	
	return m_DataType;
}

DB_RET DataUnit::setData(DATA_VAL *pData)
{
	cout << __FUNCTION__ << "():" << __LINE__ << endl;
	void	*p;

	if (m_DataType == DATA_T_NONE)
		return DB_RET_ERR_PROCEDURE;

	if (pData == NULL)
	{
		m_Data.bNull = true;
		return DB_RET_SUCCESS;
	}
	else
	{
		switch (m_DataType)
		{
			case DATA_T_BOOL:
			case DATA_T_DECIMAL:
			case DATA_T_INTEGER:
			case DATA_T_TIME:
				m_Data.t = pData->t;
				break;
			case DATA_T_STRING:
				m_Data.str = pData->str;
				break;
			default:
				return DB_RET_ERR_INTERNAL;
		}
		m_Data.bNull = false;
	}

	return DB_RET_SUCCESS;
}

DB_RET DataUnit::getData(DATA_VAL *pData)
{
	cout << __FUNCTION__ << "():" << __LINE__ << endl;

	if (m_DataType == DATA_T_NONE)
		m_Ret = DB_RET_ERR_PROCEDURE;

	if (pData == NULL)
		m_Ret = DB_RET_ERR_PARAMETER;

	*pData = m_Data;

	return DB_RET_SUCCESS;
}

void DataUnit::clearData()
{
	cout << __FUNCTION__ << "():" << __LINE__ << endl;

	m_Data.bNull = true;
}

bool DataUnit::isNull()
{
	return m_Data.bNull;
}

/*****************************************************************************/
/******************************* Column Class ********************************/
/*****************************************************************************/
Column::Column(){}

Column::Column(HANDLE hTable, string strColName, DATA_T DataType, bool bPriKey, bool bNullable, string strForeKey)
{
	cout << __FUNCTION__ << "():" << __LINE__ << endl;
}

DB_RET Column::open(HANDLE hTable, string strColName, DATA_T DataType, bool bPriKey, bool bNullable, string strForeKey)
{
	cout << __FUNCTION__ << "():" << __LINE__ << endl;

	//	hTable can be set when adding in table.
	if ((strColName.length() == 0) || (DataType == DATA_T_NONE))
		return DB_RET_ERR_PARAMETER;

	if ((bPriKey == true) && (bNullable == true))
		return DB_RET_ERR_DB_PRIMARY_KEY;

	m_hTable = hTable;
	m_strColName = strColName;
	m_bPriKey = bPriKey;
	m_bNullable = bNullable;
	m_strForeKey = strForeKey;
	m_vDatas.clear();

	//	Initialize search varilabe.
	m_Search.Data.clearData();
	m_Search.iLast = -1;

	return DB_RET_SUCCESS;
}
		
DB_RET Column::addData(DataUnit Data)
{
	DB_RET		Ret = DB_RET_SUCCESS;

	cout << __FUNCTION__ << "():" << __LINE__ << endl;

	if ((Ret = checkData(Data)) != DB_RET_SUCCESS)
		return Ret;

	if (m_vDatas.size() == m_vDatas.max_size())
		return DB_RET_ERR_DB_FULL;

	m_vDatas.push_back(Data);
	
	return DB_RET_SUCCESS;
}

DB_RET Column::deleteData(DataUnit Data)
{
	cout << __FUNCTION__ << "():" << __LINE__ << endl;
	return DB_RET_SUCCESS;
}

DB_RET Column::deleteData(unsigned int uiIndex)
{
	cout << __FUNCTION__ << "():" << __LINE__ << endl;
	return DB_RET_SUCCESS;
}

int Column::searchData(DataUnit *pData)
{
	cout << __FUNCTION__ << "():" << __LINE__ << endl;
	return 0;
}

DB_RET Column::getData(unsigned int uiIndex, DataUnit *pData)
{
	cout << __FUNCTION__ << "():" << __LINE__ << endl;
	return DB_RET_SUCCESS;
}

DB_RET Column::checkData(DataUnit Data)
{
	cout << __FUNCTION__ << "():" << __LINE__ << endl;

	//	Check nullable
	if (m_bNullable == false)
		if (Data.isNull() == true)
			return DB_RET_ERR_DB_ATTRIBUTE;

	//	Check primary key
	if (m_bPriKey == true)
	{
		if (Data.isNull() == true)
			return DB_RET_ERR_DB_PRIMARY_KEY;
		
		if (searchData(&Data) != -1)
			return DB_RET_ERR_DB_PRIMARY_KEY;
	}

	//	Check foreign key

	return DB_RET_SUCCESS;
}

