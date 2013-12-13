// AlDB.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DB.h"
using namespace AlDb;

void TestTable(Db dbEngLab, Table *ptblTeacher)
{
	DB_RET	Ret;
	Column	col;

	//	Create table - teacher
	Ret = ptblTeacher->open(&dbEngLab, "teacher");
	Ret = ptblTeacher->addColumn("id", DATA_T_INTEGER, true, -1);
	Ret = ptblTeacher->addColumn("name_ch", DATA_T_STRING, false, -1);
	Ret = ptblTeacher->addColumn("on_abroad", DATA_T_TIME, false, -1);
	
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
		Ret = ptblTeacher->addRecord(aRec);
		if (Ret != DB_RET_SUCCESS)
			cout << "Failure" << endl;
	}

	//	Test getRecord()
	Ret = ptblTeacher->getRecord(3, aRec);
	Ret = ptblTeacher->getRecord(7, aRec);

	//	Test searchRecord()
	memset (aRec, 0, sizeof(DATA_VAL) * 3);
	aRec[1].t = 1385793344;
	i = ptblTeacher->searchRecord(1, 2, aRec[1]);

	//	Test deleteRecord()
	Ret = ptblTeacher->deleteRecord(3);
	i = ptblTeacher->searchRecord(1, 0, aRec[1]);
}

void testDb(Db dbEngLab, Table *ptblTeacher)
{
	DB_RET		Ret;

	Ret = dbEngLab.open("EngLab", "D:\\Dropbox\\develop");
	Ret = dbEngLab.addTable(ptblTeacher);
//	Ret = dbEngLab.commit();
}

int _tmain(int argc, _TCHAR* argv[])
{
	time_t		t;
	string		str = "I'm Huang";
	struct stat	stfStat;

	int			iRet;
	bool		bPk = true, bFk = true;
	Db			dbEngLab;
	Table       tblTeacher;

	cout << "Table class size:" << sizeof(Table) << endl;
	str.at(0) = DATA_T_INTEGER + '0';
	str.at(1) = bPk + '0';
	
	string		strOld = "D:\\Dropbox\\develop\\";
	string		strNew = "D:\\Dropbox\\develop\\temp\\ii.tbl";

	iRet = rename(strOld.c_str(), strNew.c_str());
	perror("rename:");
	printf ("errno: %d\n", errno);

	cout << "Format: " << str << endl;
	iRet = _mkdir("D:\\Dropbox\\develop\\temp");
	if (iRet == EEXIST);
	perror("mkdir:");
	printf ("errno: %d\n", errno);
	iRet = stat("E:\\From ¥@°a\\2009¦~09¤ë\\2009-09-05 ³¢´I«°-mp3", &stfStat);
	if ((iRet = stfStat.st_mode & S_IFMT) == S_IFDIR)
		cout << "Folder" << endl;

	time(&t);
	TestTable(dbEngLab, &tblTeacher);
	testDb(dbEngLab, &tblTeacher);

	return 0;
}
