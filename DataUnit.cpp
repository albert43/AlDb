#include "DataUnit.h"
using namespace AlDb;

template <class T>
DataUnit<T>::DataUnit(bool bPriKey, bool bNullable)
{
	open(bPriKey, bNullable);
}

template <class T>
DB_RET DataUnit<T>::open(bool bPriKey, bool bNullable)
{
	m_bPriKey = bPriKey;
	m_bNullable = bNullable;
	m_vVal.clear();
	m_strSearch.clear();
	m_iLastSearch = -1;

	return DB_RET_SUCCESS;
}

template <class T>
DB_RET DataUnit<T>::addData(T Data)
{
	if (m_vVal.size() == m_vVal.max_size())
		return DB_RET_DATA_ERR_FULL;

	m_vVal.push_back(Data);
	
	return DB_RET_SUCCESS;
}

template <class T>
DB_RET DataUnit<T>::deleteData(T Data)
{
	int		iIndex;

	if (m_vVal.size() == 0)
		return DB_RET_DATA_ERR_EMPTY;

	iIndex = searchData(&Data);
	if (iIndex == -1)
		return DB_RET_DATA_ERR_NOT_FOUND;

	m_vVal.erase(m_vVal.begin() + iIndex);
	
	return DB_RET_SUCCESS;
}

template <class T>
int DataUnit<T>::searchData(T *pData)
{
	unsigned int		ui;

	if (pData)
	{
		m_strSearch = *pData;
		m_iLastSearch = -1;
	}
	for (ui = m_iLastSearch + 1; ui < m_vVal.size(); ui++)
	{
		if (m_strSearch.compare(m_vVal[ui]) == 0)
		{
			//	Found!
			m_iLastSearch = ui;
			return m_iLastSearch;
		}
	}

	//	Can not find the data, reset!
	m_iLastSearch = -1;
	m_strSearch.clear();

	return -1;
}

template <class T>
DB_RET DataUnit<T>::getData(unsigned int uiIndex, T *pData)
{
	if (pData == NULL)
		return DB_RET_ERR_PARAMETER;

	if (uiIndex >= m_vVal.size())
		return DB_RET_DATA_ERR_NOT_FOUND;

	*pData = m_vVal[uiIndex];
	
	return DB_RET_SUCCESS;
}

template <class T>
DB_RET DataUnit<T>::checkData(T Data)
{
	//	Check nullable
	if (m_bNullable == false)
		if (Data.length() == 0)
			return DB_RET_DATA_ERR_NULL;

	//	Check primary key
	if (m_bPriKey == true)
	{
		if (Data.length() == 0)
			return DB_RET_DATA_ERR_PRIMARY_KEY;
		
		if (searchData(&Data) != -1)
			return DB_RET_DATA_ERR_PRIMARY_KEY;
	}

	return DB_RET_SUCCESS;
}
