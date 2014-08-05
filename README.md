###########################################################################
##			                    Description					                         ##
###########################################################################

ORM4QT - An object relational mapping library writen in C++ for the Qt framework.

It is capable of automate the tasks of insert, update, delete and search 
of registers related with the objects of the object oriented context of the C++
language.

In its actual state the library is only capable of mapping simple classes, without
relations.

There isn't support for mapping of:
- Inheritance
- Relations one to many
- Relations many to one
- Relations one to one
- Relations many to many

The library uses an own reflection mechanism, that is configured through the use
of macros that simulate the annotations environment of other languages, for example
Java and C#.

To realize the communication with the RDBMSs it is used the QtSQL module of the 
Qt framework. 

The annotations system permits the use of implicit ids and version control during 
the update and delete of registers. It also permits the use of transactions.

The library is testing in the RDBMS Sqlite and PostgreSQL.

###########################################################################
##			                Restrictions of use                     				 ##
###########################################################################

For the correct use of the library the mapped classes must have a default
constructor and a copy constructor. If the default copy constructor can't be 
used, so you have to use the COPYTAGS() in its implementation.

ExampleClass::ExampleClass(const ExampleClass &other) : COPYTAGS(other) {}

##############################################################################
##			            Format of the SQL Instructions      	            	    ##
##############################################################################

Insert:
	INSERT INTO TABLE ([field1], [field2], ..., [version]) VALUES ([value1], [value2], ..., [0])

Update:
	UPDATE TABLE SET [field1] = [value1], ..., [version] = [1] WHERE [pk1] = [valuePK1], ..., [autoid] = [id], [version] = [0]

Delete:
	DELETE FROM TABLE WHERE [pk1] = [valuePK1], ..., [autoid] = [id], [version] = [0]

Select:
	SELECT [field1], [field2], ..., [version], [autoid] FROM TABLE [WHERE CONDITIONS] [ORDER BY ORDERBYLIST] [LIMIT number OFFSET number]

###############################################################################
##			                        More information                				     ##
###############################################################################

See the examples project.

Contact:
micdoug.silva@gmail.com
