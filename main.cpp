// AlDB.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DB.h"
using namespace AlDb;

void TestTable()
{
	DB_RET	Ret;
	Db		dbEngLab;
	Table	tblTeacher;
	Column	col;

	//	Create table - teacher
	Ret = tblTeacher.open(&dbEngLab, "teacher");
	Ret = tblTeacher.addColumn("id", DATA_T_INTEGER, true, "");
	Ret = tblTeacher.addColumn("name_ch", DATA_T_STRING, false, "");
	Ret = tblTeacher.addColumn("on_abroad", DATA_T_TIME, false, "");
	
	//	Add records
	DATA_VAL	aRec[3];
	int			iKey[10] = {1, 3, 8, 9, 11, 23, 33, 44, 12, 43};
	string		strName[10] = {"Albert", "Mico", "Josie", "Heidy", "Travies", "Harry", "Tim", "Tian", "Joicy", "Albee"};
	time_t		tmOnAbroad[10] = {1385780344, 1385190344, 1385793344, 1385760344, 1385792344, 1385792044, 1385787344, 1385793344, 1385799344, 1385490344};
	int			i;
	
	//	Test addRecord()
	for (i = 0; i < 10; i++)
	{
		aRec[0].i = iKey[i];
		aRec[1].str = strName[i];
		aRec[2].t = tmOnAbroad[i];
		Ret = tblTeacher.addRecord(aRec);
		if (Ret != DB_RET_SUCCESS)
			cout << "Failure" << endl;
	}

	//	Test getRecord()
	Ret = tblTeacher.getRecord(3, aRec);
	Ret = tblTeacher.getRecord(7, aRec);

	//	Test searchRecord()
	memset (aRec, 0, sizeof(DATA_VAL) * 3);
	aRec[1].t = 1385793344;
	i = tblTeacher.searchRecord(1, 2, aRec[1]);

	//	Test deleteRecord()
	Ret = tblTeacher.deleteRecord(3);
	i = tblTeacher.searchRecord(1, 0, aRec[1]);
	
}

int _tmain(int argc, _TCHAR* argv[])
{
	time_t		t;
	string		str = "I'm Huang";
	struct stat	stfStat;

	int			iRet;
	
	iRet = _mkdir("D:\\Dropbox\\develop\\temp");
	if (iRet == EEXIST);
	perror("mkdir:");
	printf ("errno: %d\n", errno);
	iRet = stat("E:\\From ¥@°a\\2009¦~09¤ë\\2009-09-05 ³¢´I«°-mp3", &stfStat);
	if ((iRet = stfStat.st_mode & S_IFMT) == S_IFDIR)
		cout << "Folder" << endl;

	time(&t);
	TestTable();
	
	return 0;
}
