// AlDB.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DB.h"
using namespace AlDb;

void TestDataUnit()
{
	DB_RET		Ret;
	bool		bRet;
	DataUnit	du;
	DATA_VAL	Val;
	DATA_VAL	Val2;

	Val.b = true;
	du.clearData();
	Ret = du.setDataType(DATA_T_BOOL);
	Ret = du.setData(&Val);
	Ret = du.getData(&Val2);
	bRet = du.isNull();
	du.clearData();
	bRet = du.isNull();
	Ret = du.getData(&Val2);

	Val.i = 43;
	Ret = du.setDataType(DATA_T_INTEGER);
	Ret = du.setData(&Val);
	Ret = du.getData(&Val2);
	bRet = du.isNull();
	du.clearData();
	bRet = du.isNull();
	Ret = du.getData(&Val2);

	Val.str = "This is type string";
	Ret = du.setDataType(DATA_T_STRING);
	Ret = du.setData(&Val);
	Ret = du.getData(&Val2);
	bRet = du.isNull();
	du.clearData();
	bRet = du.isNull();
	Ret = du.getData(&Val2);
}

int _tmain(int argc, _TCHAR* argv[])
{
	DataUnit	du;
	bool		pi;
	DB_RET		Ret;
	DATA_T		DataType;
	time_t		t;
	string		str = "I'm Huang";
	
	time(&t);
	du.setDataType(DATA_T_STRING);
	DataType = du.getDataType();
	TestDataUnit();
	
	return 0;
}
