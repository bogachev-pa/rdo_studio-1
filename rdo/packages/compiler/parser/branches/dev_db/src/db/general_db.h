﻿/*!
  \copyright (c) RDO-Team, 2012
  \file      general_db.h
  \authors   Клеванец Игорь (cerevra@gmail.com)
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      15.11.2012
  \brief     Базовая работа с базой данных
  \indent    4T
*/

#ifndef _SIMULATOR_RUNTIME_SQL_GENERAL_DB_H_
#define _SIMULATOR_RUNTIME_SQL_GENERAL_DB_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtSql\QtSql>
#include <boost\optional\optional.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator\compiler\parser\headers\db\interface_db.h"
// --------------------------------------------------------------------------------

class GeneralDB: public IDB
{
public:
	typedef boost::optional<int> boint;

	GeneralDB();
	GeneralDB(
		const QString& hostName,
		const QString& databaseName,
		const QString& userName,
		const QString& password,
		const int& port
	);
	~GeneralDB();

	virtual void insertRow     (const QString& tableName, const QString& qRow);
	virtual void queryExec     (const QString&   query                       );
	virtual void queryExec     (const QueryList& query                       );

	virtual int  queryExecIndex(const QString& table                         );
	virtual void setContext    (int context                                  );
	virtual int  getContext    (                                             );

private:
	void initDB();
	void setDefParams();

	QString      m_hostName;
	QString      m_databaseName;
	QString      m_userName;
	QString      m_password;
	int          m_port;
	QSqlDatabase m_db;

	boint m_contextParrentID;
};

#endif // _SIMULATOR_RUNTIME_SQL_GENERAL_DB_H_