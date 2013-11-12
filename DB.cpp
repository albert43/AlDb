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

bool DataUnit::compareData(DataUnit *pDataUnit)
{
	DATA_T	DataType;

	DataType = pDataUnit->getDataType();
	if (m_DataType != DataType)
		return false;

	if (m_Data.bNull != pDataUnit->m_Data.bNull)
		return false;

	switch(DataType)
	{
		case DATA_T_BOOL:
		case DATA_T_DECIMAL:
		case DATA_T_INTEGER:
		case DATA_T_TIME:
			if (m_Data.t != pDataUnit->m_Data.t)
				return false;
			break;
		case DATA_T_STRING:
			if (m_Data.str.compare(pDataUnit->m_Data.str) != 0)
				return false;
			break;
	}

	return true;
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
Column::Column()
{
	m_hTable = NULL;
	m_strColName.clear();
	m_DataType = DATA_T_NONE;
	m_bPriKey = false;
	m_bNullable = false;
	m_strForeKey.clear();
}

Column::Column(HANDLE hTable, string strColName, DATA_T DataType, bool bPriKey, bool bNullable, string strForeKey)
{
	cout << __FUNCTION__ << "():" << __LINE__ << endl;

	open(hTable, strColName, DataType, bPriKey, bNullable, strForeKey);
}

DB_RET Column::open(HANDLE hTable, string strColName, DATA_T DataType, bool bPriKey, bool bNullable, string strForeKey)
{
	cout << __FUNCTION__ << "():" << __LINE__ << endl;

	DB_RET	Ret;

	//	hTable can be set when adding in table.
	if (strColName.length() == 0)
		return DB_RET_ERR_PARAMETER;

	//	Check primary key attribute.
	if (bPriKey == true)
	{
		if (bNullable == true)
			return DB_RET_ERR_DB_PRIMARY_KEY;
		if (DataType == DATA_T_BOOL)
			return DB_RET_ERR_DB_PRIMARY_KEY;
		if (strForeKey.length() > 0)
			return DB_RET_ERR_DB_PRIMARY_KEY;
	}
	
	if ((Ret = this->setDataType(DataType)) != DB_RET_SUCCESS)
		return Ret;

	m_hTable = hTable;
	m_strColName = strColName;
	m_DataType = DataType;
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

DB_RET Column::deleteData(DataUnit *pData)
{
	cout << __FUNCTION__ << "():" << __LINE__ << endl;

	int		iIndex;

	if (pData == NULL)
		return DB_RET_ERR_PARAMETER;
	
	iIndex = searchData(pData);
	if (iIndex < 0)
		return DB_RET_ERR_DB_NOT_FOUND;

	return deleteData((unsigned int)iIndex);
}

DB_RET Column::deleteData(unsigned int uiIndex)
{
	cout << __FUNCTION__ << "():" << __LINE__ << endl;

	if (uiIndex >= m_vDatas.size())
		return DB_RET_ERR_DB_NOT_FOUND;

	m_vDatas.erase(m_vDatas.begin() + uiIndex);

	return DB_RET_SUCCESS;
}

int Column::searchData(DataUnit *pData)
{
	cout << __FUNCTION__ << "():" << __LINE__ << endl;
	
	unsigned int	ui;

	if (m_DataType != pData->getDataType())
		return (int)DB_RET_ERR_PARAMETER;

	if (pData)
	{
		m_Search.Data = *pData;
		m_Search.iLast = -1;
	}

	for (ui = 0; ui < m_vDatas.size(); ui++)
		if (m_vDatas[ui].compareData(&m_Search.Data) == true)
			return ui;

	//	Can not find the data, reset!
	m_Search.Data.clearData();
	m_Search.iLast = -1;

	return (int)DB_RET_ERR_DB_NOT_FOUND;
}

DB_RET Column::getData(unsigned int uiIndex, DataUnit *pData)
{
	cout << __FUNCTION__ << "():" << __LINE__ << endl;

	if (pData == NULL)
		return DB_RET_ERR_PARAMETER;

	if (uiIndex >= m_vDatas.size())
		return DB_RET_ERR_DB_NOT_FOUND;

	*pData = m_vDatas[uiIndex];

	return DB_RET_SUCCESS;
}

unsigned int Column::getDataNumber()
{
	cout << __FUNCTION__ << "():" << __LINE__ << endl;

	return m_vDatas.size();
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

	return DB_RET_SUCCESS;
}

