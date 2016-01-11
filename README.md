# ORM4QT - An object relational mapping library writen in C++ for the Qt framework

## Description

This is a simple ORM library designed to be used with the Qt Framework. 
Using it, we can do tasks like insert, update, delete and search objects in the database in a much easier form.

The library is still incomplete. I started developing it as my final graduate work.
In its actual state, the library can map simple classes, without relations. All the types supported by Qt (aka QVariant supported types) are also supported here. The library is working in the RDBMSs Sqlite and PostgreSQL.

The library also support implicit ids, version control (using a version column while updating and deleting registers) and transactions.

I only tested the library with Qt 5.1 and newer versions, but I believe it will work with Qt 4.8 too (I'll test soon).

## How it works

The library has a custom reflection mechanism, inspired by the Qt Meta-Object system.
This mechanism is configured using macros that simulates the annotations environment of other languages, like Java and C#.

Behind the scenes, the macros build a special function that creates a series of lambda expressions that exposes the internal attributes of the mapped class. In that way we can access this attributes from outside of the class to build the sql expressions.

This lambda expressions are not used directly. They are wraped in a special class that stores meta information about the mapping, like column name, required or not, etc.

For communication with RDMSs we use the QtSql module.

## SQL statements Format

Insert:
	INSERT INTO TABLE ([field1], [field2], ..., [version]) VALUES ([value1], [value2], ..., [0])

Update:
	UPDATE TABLE SET [field1] = [value1], ..., [version] = [1] WHERE [pk1] = [valuePK1], ..., [autoid] = [id], [version] = [0]

Delete:
	DELETE FROM TABLE WHERE [pk1] = [valuePK1], ..., [autoid] = [id], [version] = [0]

Select:
	SELECT [field1], [field2], ..., [version], [autoid] FROM TABLE [WHERE CONDITIONS] [ORDER BY ORDERBYLIST] [LIMIT number OFFSET number]

## How to use

See the examples project.
I will add more documentation soon.

## TODOS

- Support inheritance
- Support relations one to many
- Support relations many to one
- Support relations one to one
- Support relations many to many
- Add a cache system
- Add support for MySQL
- Create a global repository of meta-informations (performance increase)

Contact:
micdoug.silva@gmail.com
