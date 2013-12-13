#include "Db.h"
using namespace AlDb;

/*
DB_RET Pvoid2DataVal(DATA_T DataType, void *pData, DATA_VAL *pstData)
{
	cout << __FUNCTION__ << "():" << __LINE__ << endl;

	if ((pstData == NULL) || (pData == NULL))
		return DB_RET_ERR_INTERNAL;

	//	Assign pData to DATA_VAL
	switch (DataType)
	{
		case DATA_T_BOOL:
		case DATA_T_DECIMAL:
		case DATA_T_INTEGER:
		case DATA_T_TIME:
			pstData->t = *(int *)pData;
			break;
		case DATA_T_STRING:
			pstData->str = *(string *)pData;
			break;
		default:
			return DB_RET_ERR_INTERNAL;
	}

	return DB_RET_SUCCESS;
}

DB_RET DataVal2Pvoid(DATA_T DataType, DATA_VAL *pstData, void *pData)
{
	cout << __FUNCTION__ << "():" << __LINE__ << endl;

	if ((pstData == NULL) || (pData == NULL))
		return DB_RET_ERR_PARAMETER;

	switch (DataType)
	{
		case DATA_T_BOOL:
			*(bool *)pData = pstData->b;
			break;
		case DATA_T_DECIMAL:
			*(double *)pData = pstData->d;
			break;
		case DATA_T_INTEGER:
			*(int *)pData = pstData->i;
			break;
		case DATA_T_TIME:
			*(time_t *)pData = pstData->t;
			break;
		case DATA_T_STRING:
			*(string *)pData = pstData->str;
			break;
		default:
			return DB_RET_ERR_INTERNAL;
	}

	return DB_RET_SUCCESS;
}
*/

/*****************************************************************************/
/******************************* Column Class ********************************/
/*****************************************************************************/
Column::Column()
{
	cout << __FUNCTION__ << "():" << __LINE__ << endl;
	
	reset();
}

Column::Column(HANDLE hTable, string strColName, DATA_T DataType, bool bPriKey, int strForeKey)
{
	reset();
	open(hTable, strColName, DataType, bPriKey, strForeKey);
}

DB_RET Column::open(HANDLE hTable, string strColName, DATA_T DataType, bool bPriKey, int strForeKey)
{
	//	hTable can NOT be set when adding in table.
	//	Because there's no public function to set the handle.
	if (hTable == NULL)
		return DB_RET_ERR_PARAMETER;

	if (strColName.length() == 0)
		return DB_RET_ERR_PARAMETER;

	if (strColName.length() > ALDB_NAME_LENGTH_MAX)
		return DB_RET_ERR_DB_ATTRIBUTE;

	//	Check primary key attribute.
	if (bPriKey == true)
	{
		if (DataType == DATA_T_BOOL)
			return DB_RET_ERR_DB_PRIMARY_KEY;
		if (strForeKey >= 0)
			return DB_RET_ERR_DB_PRIMARY_KEY;
	}

	if ((DataType < DATA_T_NONE) || (DataType >= DATA_T_END))
		return DB_RET_ERR_PARAMETER;

	if (strForeKey < 0)
		return DB_RET_ERR_DB_ATTRIBUTE;

	m_hTable = hTable;
	m_Attr.strColName = strColName;
	m_Attr.DataType = DataType;
	m_Attr.bPriKey = bPriKey;
	m_Attr.strForeKey = strForeKey;

	return DB_RET_SUCCESS;
}
		
void Column::reset()
{
	m_hTable = NULL;
	m_Attr.strColName.clear();
	m_Attr.DataType = DATA_T_NONE;
	m_Attr.bPriKey = false;
	m_Attr.strForeKey = -1;
	m_vstDatas.clear();

	//	Initialize search varilabe.
	memset (&m_Search.stData, 0, sizeof(DATA_VAL));
	m_Search.iLast = (int)DB_RET_ERR_DB_NOT_FOUND;
}

DB_RET Column::addData(DATA_VAL *pData)
{
	DB_RET			Ret = DB_RET_SUCCESS;
	unsigned int	uiData;
	int				iIndex;

	if (m_Attr.DataType == DATA_T_NONE)
		return DB_RET_ERR_PROCEDURE;
	
	if (pData == NULL)
		return DB_RET_ERR_PARAMETER;

	//	Check primary key
	uiData = m_vstDatas.size();
	if (uiData > 0)
	{
		//	Check whether it's full or not.
		if (uiData == m_vstDatas.max_size())
			return DB_RET_ERR_DB_FULL;

		if (m_Attr.bPriKey == true)
		{
			iIndex = searchData(pData, false);
			if (iIndex >= 0)
				return DB_RET_ERR_DB_PRIMARY_KEY;
			else
			{
				if (iIndex != DB_RET_ERR_DB_NOT_FOUND)
					return (DB_RET)iIndex;
			}
		}
	}

	//	Add DATA_VAL to vector
	m_vstDatas.push_back(*pData);
	
	return DB_RET_SUCCESS;
}

int Column::searchData(DATA_VAL *pData, bool bContiune)
{
	unsigned int	ui;

	if (bContiune == false)
	{
		m_Search.stData = *pData;
		m_Search.iLast = -1;
	}

	for (ui = m_Search.iLast + 1; ui < m_vstDatas.size(); ui++)
	{
		if (compareData(ui, pData) == true)
		{
			m_Search.iLast = ui;
			return ui;
		}
	}

	//	Can not find the data, reset!
	memset(&m_Search.stData, 0, sizeof(DATA_VAL));
	m_Search.iLast = -1;

	return DB_RET_ERR_DB_NOT_FOUND;
}

DB_RET Column::deleteData(DATA_VAL *pData)
{
	int		iIndex;

	iIndex = searchData(pData, false);
	if (iIndex < 0)
		return (DB_RET)iIndex;

	if (deleteData((unsigned int)iIndex) != DB_RET_SUCCESS)
		return DB_RET_ERR_INTERNAL;

	return DB_RET_SUCCESS;
}

DB_RET Column::deleteData(unsigned int uiIndex)
{
	if (uiIndex >= m_vstDatas.size())
		return DB_RET_ERR_DB_NOT_FOUND;

	m_vstDatas.erase(m_vstDatas.begin() + uiIndex);

	return DB_RET_SUCCESS;
}

DB_RET Column::getData(unsigned int uiIndex, DATA_VAL *pData)
{
	if (pData == NULL)
		return DB_RET_ERR_PARAMETER;

	if (uiIndex >= m_vstDatas.size())
		return DB_RET_ERR_DB_NOT_FOUND;

	*pData = m_vstDatas.at(uiIndex);

	return DB_RET_SUCCESS;
}

bool Column::compareData(unsigned int uiIndex, DATA_VAL *pData)
{
	if (uiIndex > m_vstDatas.size())
		return false;

	if (pData == NULL)
		return false;

	switch(m_Attr.DataType)
	{
		case DATA_T_BOOL:
		case DATA_T_INTEGER:
			if (m_vstDatas.at(uiIndex).i != pData->i)
				return false;
			break;
		case DATA_T_DECIMAL:
			if (m_vstDatas.at(uiIndex).d != pData->d)
				return false;
			break;
		case DATA_T_TIME:
			if (m_vstDatas.at(uiIndex).t != pData->t)
				return false;
			break;
		case DATA_T_STRING:
			if (m_vstDatas.at(uiIndex).str.compare(pData->str) != 0)
				return false;
			break;
	}

	return true;
}

unsigned int Column::getDataNumber()
{
	return m_vstDatas.size();
}

DB_RET Column::getAttribute(COLUMN_ATTR *pAttr)
{
	if (pAttr == NULL)
		return DB_RET_ERR_PARAMETER;

	memcpy (pAttr, &m_Attr, sizeof(COLUMN_ATTR));

	return DB_RET_SUCCESS;
}

//	Column Attribute Format:
//		|Type|PK|FK-Name_Length|FK-Name|Column-Name|
DB_RET Column::save(FILE *fp)
{
	DB_RET			Ret;
	string			str = "";
	unsigned int	uiData;
	DATA_VAL		stData;
	char			szNumeric[32];

	//	Character 1: DataType
	str.at(0) = m_Attr.DataType + '0';
	
	//	Character 2: Primary key
	str.at(1) = m_Attr.bPriKey + '0';

	//	Character 3-4: Foreign key name length.
//	str.at(2) = (m_Attr.strForeKey.length() / 10) + '0';
//	str.at(3) = (m_Attr.strForeKey.length() % 10) + '0';

	//	Character 5-Length+5: Foreign key name
//	str.append(m_Attr.strForeKey);

	//	Character X-TAB: Column name
	str.append(m_Attr.strColName);

	for (uiData = 0; uiData < m_vstDatas.size(); uiData++)
	{
		if ((Ret = getData(uiData, &stData)) != DB_RET_SUCCESS)
			return Ret;
		
		switch (m_Attr.DataType)
		{
			case DATA_T_BOOL:
			case DATA_T_INTEGER:
				str.append("\t");
				sprintf(szNumeric, "%d", stData.i);
				str.append(szNumeric);
				
				break;
			case DATA_T_DECIMAL:
				str.append("\t");
				sprintf(szNumeric, "%lf", stData.d);
				str.append(szNumeric);
				
				break;
			case DATA_T_TIME:
				str.append("\t");
				sprintf(szNumeric, "%lld", stData.t);
				str.append(szNumeric);
				
				break;
			case DATA_T_STRING:
				str.append("\t");
				str.append(stData.str);
				
				break;
			default:
				return DB_RET_ERR_INTERNAL;
		}
	}
	str.append(NEW_LINE);

	if (fwrite(str.c_str(), 1, str.length(), fp) != str.length())
		return DB_RET_ERR_SYS_IO;

	return DB_RET_SUCCESS;
}
//	End of Column class

/*****************************************************************************/
/******************************** Table Class ********************************/
/*****************************************************************************/
Table::Table()
{
	reset();
	cout << "1.name:" << m_Attr.strTableName << "Addr:" << this << endl;
}

Table::Table(HANDLE hDb, string strTableName)
{
	reset();
	
	open(hDb, strTableName);
}

Table::~Table()
{
	cout << "name:" << m_Attr.strTableName << "Addr:" << this << endl;
	//	Release memory.
//	if (m_vCols.size() > 0)
/*	if (m_Search.Item.pai != NULL)
	{
		delete [] m_Search.Item.pai;
		m_Search.Item.pai = NULL;
	}

	if (m_Search.Item.past != NULL)
	{
		delete [] m_Search.Item.past;
		m_Search.Item.past = NULL;
	}
*/
}

DB_RET Table::open(HANDLE hDb, string strTableName)
{
	if (strTableName.length() <= 0)
		return DB_RET_ERR_PARAMETER;

	if (strTableName.length() > ALDB_NAME_LENGTH_MAX)
		return DB_RET_ERR_DB_ATTRIBUTE;

	reset();
	m_hDb = hDb;
	m_Attr.strTableName = strTableName;

	return DB_RET_SUCCESS;
}

DB_RET Table::setDbHandle(HANDLE hDb)
{
	if (hDb == NULL)
		return DB_RET_ERR_PARAMETER;

	m_hDb = hDb;

	return DB_RET_SUCCESS;
}

DB_RET Table::getAttribute(TABLE_ATTR *pAttr)
{
	if (pAttr == NULL)
		return DB_RET_ERR_PARAMETER;

	memcpy (pAttr, &m_Attr, sizeof(TABLE_ATTR));

	return DB_RET_SUCCESS;
}

DB_RET Table::addColumn(string strColName, DATA_T DataType, bool bPrimaryKey, int strForeignKey)
{
	DB_RET	Ret = DB_RET_SUCCESS;
	Column	col;

	if ((m_Attr.iPrimary != -1) && (bPrimaryKey == true))
		return DB_RET_ERR_DB_ATTRIBUTE;

	Ret = col.open(this, strColName, DataType, bPrimaryKey, strForeignKey);
	if (Ret != DB_RET_SUCCESS)
		return Ret;
/*
	//	Change the size of the arrays.
	if (m_Search.Item.pai != NULL)
		delete [] m_Search.Item.pai;
	m_Search.Item.pai = new int[m_vCols.size() + 1];
	if (m_Search.Item.pai == NULL)
		return DB_RET_ERR_SYS_MEMORY;

	//	string in DATA_VAL can not be new by realloc() function.
	if (m_Search.Item.past != NULL)
		delete [] m_Search.Item.past;
	m_Search.Item.past = new DATA_VAL[m_vCols.size() + 1];
	if (m_Search.Item.past == NULL)
		return DB_RET_ERR_SYS_MEMORY;
*/
	m_vCols.push_back(col);
	if (bPrimaryKey == true)
		m_Attr.iPrimary = m_vCols.size() - 1;
	if (strForeignKey >= 0)
		m_Attr.vstrForeign.push_back(strForeignKey);

	return DB_RET_SUCCESS;
}

int Table::searchColumn(string strColName)
{
	COLUMN_ATTR		Attr;
	int				iIndex = -1;

	if (strColName.length() <= 0)
		return DB_RET_ERR_DB_NOT_FOUND;
	
	for (iIndex = 0; iIndex < (int)m_vCols.size(); iIndex++)
	{
		m_vCols[iIndex].getAttribute(&Attr);
		if (Attr.strColName.compare(strColName) == 0)
			return iIndex;
	}

	return DB_RET_ERR_DB_NOT_FOUND;
}

const Column* Table::getColumn(string strColName)
{
	int	iIndex;

	if ((iIndex = searchColumn(strColName)) < 0)
		return NULL;
	
	return (const Column *)&m_vCols[iIndex];
}

DB_RET Table::addRecord(DATA_VAL *paRecord)
{
	DB_RET			Ret = DB_RET_SUCCESS;
	COLUMN_ATTR		ColAttr;
	unsigned int	uiParamNum = 0, ui;

	//	Check the table is valid.
	if (valid() == false)
		return DB_RET_ERR_PROCEDURE;

	//	Add data in each column.
	for (ui = 0; ui < m_vCols.size(); ui++)
	{
		Ret = m_vCols.at(ui).addData(&paRecord[ui]);
		if (Ret != DB_RET_SUCCESS)
			break;
	}

	//	Add fail, delete the added data.
	if (Ret != DB_RET_SUCCESS)
	{
		if (ui != 0)
		{
			for (ui = ui - 1; ui >= 0; ui--)
				m_vCols[ui].deleteData(ui);
		}
	}

	return Ret;
}

int Table::searchRecord(unsigned int uiCompareItemNum, ...)
{
	va_list			list;
	int				iIndex;

	va_start(list, uiCompareItemNum);
	
	iIndex = searchRecord(uiCompareItemNum, list);
	va_end(list);

	return iIndex;
}

int Table::searchRecord(unsigned int uiCompareItemNum, va_list List)
{
	int				nDataNum;
	bool			bFound = true;
	int				i, iItem;

	//	Check the table is valid.
	if (valid() == false)
		return DB_RET_ERR_PROCEDURE;
		
	//	Allocate a array to load each record.
	if (uiCompareItemNum != 0)
	{
		//	Check the table is valid.
		if (valid() == false)
			return DB_RET_ERR_PROCEDURE;

		if (uiCompareItemNum > m_vCols.size())
			return DB_RET_ERR_PARAMETER;

		//	Get input parameters.
//		memset(m_Search.Item.past, 1, sizeof(DATA_VAL) * m_vCols.size());
		for (i = 0; i < (int)uiCompareItemNum; i++)
		{
			//	Get compared column index.
			m_Search.Item.pai[i] = va_arg(List, int);
			if ((m_Search.Item.pai[i] < 0) || (m_Search.Item.pai[i] > (int)m_vCols.size()))
				return DB_RET_ERR_PARAMETER;

			m_Search.Item.past[i] = va_arg(List, DATA_VAL);
		}

		m_Search.iLast = -1;
		m_Search.uiComparedNum = uiCompareItemNum;
	}

	nDataNum = m_vCols.at(m_Attr.iPrimary).getDataNumber();
	for (i = m_Search.iLast + 1; i < nDataNum; i++)
	{
		for (iItem = 0; iItem < (int)m_Search.uiComparedNum; iItem++)
		{
			bFound = m_vCols.at(m_Search.Item.pai[iItem]).compareData(i, &m_Search.Item.past[iItem]);
			if (bFound == false)
				break;
		}
		if (bFound == true)
		{
			m_Search.iLast = i;
			return i;
		}
	}
	
	m_Search.iLast = -1;

	return DB_RET_ERR_DB_NOT_FOUND;
}

DB_RET Table::deleteRecord(int uiIndex)
{
	DB_RET			Ret = DB_RET_SUCCESS;
	unsigned int	ui;

	//	Check the table is valid.
	if (valid() == false)
		return DB_RET_ERR_PROCEDURE;
		
	//	Don't worry the unsynchronous problem.
	//	Because if the failure occur it must happens in the first delete procedure.
	for (ui = 0; ui < m_vCols.size(); ui++)
	{
		if ((Ret = m_vCols.at(ui).deleteData(uiIndex)) != DB_RET_SUCCESS)
			return Ret;
	}

	return DB_RET_SUCCESS;
}

DB_RET Table::getRecord(unsigned int uiIndex, DATA_VAL *paRecord)
{
	DB_RET			Ret = DB_RET_SUCCESS;
	COLUMN_ATTR		Attr;
	unsigned int	ui;

	//	Check the table is valid.
	if (valid() == false)
		return DB_RET_ERR_PROCEDURE;
		
	if (paRecord == NULL)
		return DB_RET_ERR_PARAMETER;

	for (ui = 0; ui < m_vCols.size(); ui++)
	{
		Ret = m_vCols.at(ui).getData(uiIndex, &paRecord[ui]);
		if (Ret != DB_RET_SUCCESS)
			return Ret;
	}

	return DB_RET_SUCCESS;
}

unsigned int Table::getRecordNum()
{
	//	Check the table is valid.
	if (valid() == false)
		return 0;

	return m_vCols.at(0).getDataNumber();
}

bool Table::valid()
{
	if (m_Attr.strTableName.length() <= 0)
		return false;

	if ((m_vCols.size() <= 1) || (m_Attr.iPrimary == -1))
		return false;

	return true;
}

DB_RET Table::save(string strDbPath)
{
	DB_RET			Ret;
	string			strTablePath;
	FILE			*fp;
	unsigned int	uiCol;

	if (valid() == false)
		return DB_RET_ERR_PROCEDURE;

	strTablePath = strDbPath;
	strTablePath.append("\\");
	strTablePath.append(m_Attr.strTableName);
	strTablePath.append(".temp");

	fp = fopen(strTablePath.c_str(), "w");
	if (fp == NULL)
		return DB_RET_ERR_SYS_IO;

	for (uiCol = 0; uiCol < m_vCols.size(); uiCol++)
	{
		if ((Ret = m_vCols.at(uiCol).save(fp)) != DB_RET_SUCCESS)
		{
			fclose(fp);
			return Ret;
		}
	}

	return DB_RET_SUCCESS;
}

DB_RET Table::commit(string strDbPath)
{
	DB_RET		Ret;
	int			iRet;
	string		strPathTemp, strPath;
	struct stat	stfStat;

	strPathTemp = strPath = strDbPath;

	strPathTemp.append("\\");
	strPathTemp.append(m_Attr.strTableName);
	strPathTemp.append(".temp");

	strPath.append("\\");
	strPath.append(m_Attr.strTableName);
	strPath.append(".tbl");

	if ((iRet = stat(strPath.c_str(), &stfStat)) != 0)
	{
		cout << "errno:" << errno << endl;
		if (errno == ENOENT)
			return DB_RET_ERR_DB_NOT_FOUND;
		else
			return DB_RET_ERR_SYS_IO;
	}

	if ((iRet = rename(strPathTemp.c_str(), strPath.c_str())) != 0)
	{
		if (errno != ENOENT)
			return DB_RET_ERR_SYS_IO;
		else
			return DB_RET_ERR_PROCEDURE;
	}
	
	return DB_RET_SUCCESS;
}

//
//	Private functions
//
void Table::reset()
{
	m_hDb = NULL;
	m_Attr.strTableName = "";
	m_Attr.iPrimary = -1;
	m_Attr.vstrForeign.clear();
	m_vCols.clear();
	//	Reset search variable arraies.
	m_Search.iLast = -1;
//	m_Search.Item.pai = NULL;
//	m_Search.Item.past = NULL;
}
/*****************************************************************************/
/********************************* Db Class **********************************/
/*****************************************************************************/
Db::Db()
{
}

Db::Db(string strDbName, string strPath)
{
	open(strDbName, strPath);
}

Db::~Db()
{
	m_vTables.clear();
}

DB_RET Db::open(string strDbName, string strPath)
{
	int			iRet;
	struct stat	stfStat;

	if (strDbName.length() <= 0)
		return DB_RET_ERR_PARAMETER;

	if (strDbName.length() > ALDB_NAME_LENGTH_MAX)
		return DB_RET_ERR_DB_ATTRIBUTE;

	//	Check the path is a valid file path.
	iRet = stat(strPath.c_str(), &stfStat);
	if (iRet != 0)
		return DB_RET_ERR_PARAMETER;

	//	Check the file path is a directory.
	if ((stfStat.st_mode & S_IFMT) != S_IFDIR)
		return DB_RET_ERR_PARAMETER;

	m_strDbName = strDbName;
	m_strPath = strPath;
	m_vTables.clear();
	
	return DB_RET_SUCCESS;
}

DB_RET Db::addTable(Table *pTable)
{
	DB_RET			Ret = DB_RET_SUCCESS;
	TABLE_ATTR		Attr;
	unsigned int	ui;

	if ((m_strDbName.length() <= 0) || (m_strPath.length() <= 0))
		return DB_RET_ERR_PROCEDURE;

	if (pTable == NULL)
		return DB_RET_ERR_PARAMETER;

	if (pTable->valid() == false)
		return DB_RET_ERR_PARAMETER;

	//	Check foreign key.
	if ((Ret = pTable->getAttribute(&Attr)) != DB_RET_SUCCESS)
		return Ret;

	for (ui = 0; ui < Attr.vstrForeign.size(); ui++)
	{
//		if (searchTable(Attr.vstrForeign.at(ui)) < 0)
			return DB_RET_ERR_PARAMETER;
	}

	m_vTables.push_back((void *)pTable);

	return DB_RET_SUCCESS;
}

const Table* Db::getTable(string strTableName)
{
	int		iIndex;

	if ((iIndex = searchTable(strTableName)) < 0)
		return NULL;

	return (const Table *)&m_vTables.at(iIndex);
}

int Db::searchTable(string strTableName)
{
	DB_RET			Ret = DB_RET_SUCCESS;
	TABLE_ATTR		Attr;
	unsigned int	ui;

	if ((m_strDbName.length() <= 0) || (m_strPath.length() <= 0))
		return (int)DB_RET_ERR_PROCEDURE;

	for (ui = 0; ui < m_vTables.size(); ui++)
	{
//		if ((Ret = m_vTables.at(ui).getAttribute(&Attr)) != DB_RET_SUCCESS)
			return (int)Ret;
		if (Attr.strTableName.compare(strTableName) == 0)
			return (int)ui;
	}

	return (int)DB_RET_ERR_DB_NOT_FOUND;
}

DB_RET Db::addRecord(string strTableName, DATA_VAL *paRecord)
{
	int		iTable;

	if ((m_strDbName.length() <= 0) || (m_strPath.length() <= 0))
		return DB_RET_ERR_PROCEDURE;

	if ((iTable = searchTable(strTableName)) < 0)
		return (DB_RET)iTable;

	return ((Table *)(m_vTables.at(iTable)))->addRecord(paRecord);
}

DB_RET Db::deleteRecord(string strTableName, unsigned int uiIndex)
{
	int		iTable;

	if ((m_strDbName.length() <= 0) || (m_strPath.length() <= 0))
		return DB_RET_ERR_PROCEDURE;

	if ((iTable = searchTable(strTableName)) < 0)
		return (DB_RET)iTable;

	return ((Table *)(m_vTables.at(iTable)))->deleteRecord(uiIndex);
}

int Db::searchRecord(string strTableName, unsigned int uiCompareItemNum, ...)
{
	va_list	List;
	int		iTable, iRecord;

	if ((m_strDbName.length() <= 0) || (m_strPath.length() <= 0))
		return (int)DB_RET_ERR_PROCEDURE;

	if ((iTable = searchTable(strTableName)) < 0)
		return iTable;

	va_start(List, uiCompareItemNum);
	iRecord = ((Table *)(m_vTables.at(iTable)))->searchRecord(uiCompareItemNum, List);
	va_end(List);

	return iRecord;
}

DB_RET Db::getRecord(string strTableName, unsigned int uiIndex, DATA_VAL *paRecord)
{
	int		iTable;

	if ((m_strDbName.length() <= 0) || (m_strPath.length() <= 0))
		return DB_RET_ERR_PROCEDURE;

	if ((iTable = searchTable(strTableName)) < 0)
		return (DB_RET)iTable;

	return ((Table *)(m_vTables.at(iTable)))->getRecord(uiIndex, paRecord);
}

bool Db::valid()
{
	TABLE_ATTR		Attr;
	unsigned int	uiTable, uiForeignKey;

	if ((m_strDbName.length() <= 0) || (m_strPath.length() <= 0))
		return false;
	
	if (m_vTables.size() <= 0)
		return false;

	for (uiTable = 0; uiTable < m_vTables.size(); uiTable++)
	{
		if (((Table *)(m_vTables.at(uiTable)))->valid() == false)
			return false;
		if (((Table *)(m_vTables.at(uiTable)))->getAttribute(&Attr) != DB_RET_SUCCESS)
			return false;
		
		//	Check the foreign table is exist.
		for (uiForeignKey = 0; uiForeignKey < Attr.vstrForeign.size(); uiForeignKey++)
		{
			((Table *)(m_vTables.at(Attr.vstrForeign.at(uiForeignKey))))->getAttribute(&Attr);
			if (searchTable(Attr.strTableName) < 0)
				return false;
		}
	}

	return true;
}

DB_RET Db::save()
{
	DB_RET			Ret;
	int				iRet;
	string			strDbPath;
	unsigned int	uiTable;

	if (valid() == false)
		return DB_RET_ERR_PROCEDURE;

	//	Check db folder
	if ((iRet = _mkdir(m_strPath.c_str())) != 0)
	{
		//	EEXIST: File exist.
		if (errno != EEXIST)
			return DB_RET_ERR_SYS_IO;
	}

	strDbPath = m_strPath;
	strDbPath.append("\\");
	strDbPath.append(m_strPath);
	for (uiTable = 0; uiTable < m_vTables.size(); uiTable++)
	{
		if ((Ret = ((Table *)(m_vTables.at(uiTable)))->save(strDbPath)) != DB_RET_SUCCESS)
			return Ret;
	}

	return DB_RET_SUCCESS;
}

DB_RET Db::commit()
{
	DB_RET			Ret;
	int				iRet;
	string			strDbPath;
	unsigned int	uiTable;

	if (valid() == false)
		return DB_RET_ERR_PROCEDURE;

	strDbPath = m_strPath;
	strDbPath.append("\\");
	strDbPath.append(m_strPath);
	for (uiTable = 0; uiTable < m_vTables.size(); uiTable++)
	{
		if ((Ret = ((Table *)(m_vTables.at(uiTable)))->commit(strDbPath)) != DB_RET_SUCCESS)
			return Ret;
	}

	return DB_RET_SUCCESS;
}
