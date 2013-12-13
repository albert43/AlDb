Release Note:
	#	V-1.0.0.3
		#	Limit name length to 30 characters.
		#	Column::save()
		#	Table::save()
		#	Db::save()
		#	Table::commit()
		#	Db::commit()
	#	V-1.0.0.4
		#	Change to record Table index for foreign key
		#	Set max column number to 32 and don't use dynamic array for search.
	#	V-1.0.0.5
		#	Change vector type from "Table" to "void *" for DB::m_vTables
		#	Debug mode will crash when the program finish.
			Release mode is fine.
/////////////////////////////////////////////////////////////////////////////
Develop notes:
Class Data inherit Class ColumnAttr
Class Column inherit Class Data



AlDb is a database engine. It provide a uncompressed database sysytem, all of the data
are stored in text base files. Table's data store in 

AlDb is wroten in C++, it has 3 classes - Db, Talbe and Column.
DataUnit is a basic data unit. All the data must add in or get from database through this class.

#	Does NOT support NULL data.
#	save()	: Memory = Table.temp and Table_new.tbl not exist.
	commit(): Memory = Table_new.tbl and Table.temp not exist.
	revert(): Memory = Table_old.tbl and Table_new not exist.
	load()	: Memory = Table_old.tbl and Table_new not exist.

#	Known issue.
	#	The lower layer data can not be accessed by upper layer class.
		AlDb::Column::m_vDatas can not be accessed by AlDb::Table
	#	The allocated array pointer should initilize to NULL in Constructor to avoid
		crash becaseu of freeing a non-exist memory.
		If the initial value isn't NULL the code will free it.
		(done)
	#	m_Search.Item.past can not be freed in deconstructor.

#	To-do...
	#	Add function to get column attributes and change column attributes member variable
		to private.	(done)
	#	Using char * to instead of string
	#	Change the name m_Search.Item to m_Search.Column
	#	Check class name for handle parameters