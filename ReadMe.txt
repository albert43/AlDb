========================================================================
    CONSOLE APPLICATION : AlDB Project Overview
========================================================================

AppWizard has created this AlDB application for you.

This file contains a summary of what you will find in each of the files that
make up your AlDB application.


AlDB.vcxproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

AlDB.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

AlDB.cpp
    This is the main application source file.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named AlDB.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////
Develop notes:
Class Data inherit Class ColumnAttr
Class Column inherit Class Data



AlDb is a database engine. It provide a uncompressed database sysytem, all of the data
are stored in text base files. Table's data store in 

AlDb is wroten in C++, it has 4 classes - Db, Talbe, Column and DataUnit.
DataUnit is a basic data unit. All the data must add in or get from database through this class.

#	Does NOT support NULL data.

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