#ifndef __ALBERT_DATABASE_DATAUNIT_H__
#define	__ALBERT_DATABASE_DATAUNIT_H__
#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include "DbBase.h"
using namespace std;

namespace AlDb
{
	typedef enum DU_TYPE
	{
		DU_TYPE_INTEGER,
		DU_TYPE_DECIMAL,
		DU_TYPE_BOOL,
		DU_TYPE_STRING,
		DU_TYPE_DATETIME
	};

	template <class T>
	class DataUnit
	{
	public:
		bool			m_bPriKey;
		bool			m_bNullable;
		vector<T>		m_vVal;

		DataUnit(){}
		DataUnit(bool bPriKey, bool bNullable);

		//	i/p:
		//		bPriKey		: 
		//	o/p:
		//		DB_RET_SUCCESS
		DB_RET open(bool bPriKey, bool bNullable);

		//	# Description:
		//		Add a data without check.
		//	i/p:
		//		The data.
		//	o/p:
		//		DB_RET_SUCCESS
		//		DB_RET_DATA_ERR_FULL
		DB_RET addData(T Data);

		//	# Description:
		//		Delete the data.
		//	i/p:
		//		The data.
		//	o/p:
		//		DB_RET_SUCCESS
		//		DB_RET_DATA_ERR_EMPTY
		//		DB_RET_DATA_ERR_NOT_FOUND
		DB_RET deleteData(T Data);

		//	# Description:
		//		Search the data. If pData is not NULL, it will search from 
		//		the begin of the list, otherwise, it will continue searching 
		//		the data.
		//	i/p:
		//		The data.
		//	o/p:
		//		The index of the searched data. If the data not be found, it 
		//		will return -1.
		int searchData(T *pData);

		//	# Description:
		//		Get the data of the index.
		//	i/p:
		//		uiIndex	: The index.
		//		pData: The data pointer to load the data.
		//	o/p:
		//		DB_RET_SUCCESS
		//		DB_RET_ERR_PARAMETER
		//		DB_RET_DATA_ERR_NOT_FOUND
		DB_RET getData(unsigned uiIndex, T *pData);

		//	# Description:
		//		Check the validity of data.
		//	i/p:
		//		The data.
		//	o/p:
		//		DB_RET_SUCCESS
		//		DB_RET_DATA_ERR_NULL
		//		DB_RET_DATA_ERR_PRIMARY_KEY
		DB_RET checkData(T Data);

	private:
		int				m_iLastSearch;
		T				m_strSearch;
	};
}
#endif // !__ALBERT_DATABASE_DATAUNIT_H__
