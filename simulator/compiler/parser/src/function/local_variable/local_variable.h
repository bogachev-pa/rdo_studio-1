/*!
  \copyright (c) RDO-Team, 2011
  \file      local_variable.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      16.10.2010
  \brief     
  \indent    4T
*/

#ifndef _SIMULATOR_COMPILER_PARSER_FUNCTION_LOCAL_VARIABLE_H_
#define _SIMULATOR_COMPILER_PARSER_FUNCTION_LOCAL_VARIABLE_H_

// ----------------------------------------------------------------------- INCLUDES
#include <list>
#include <map>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdo_value.h"
#include "simulator/compiler/parser/expression.h"
#include "simulator/compiler/parser/bison_value_pair.h"
#include "simulator/compiler/parser/type/info.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- LocalVariable
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(LocalVariable);
class LocalVariable: public rdo::counter_reference
{
DECLARE_FACTORY(LocalVariable);
public:
	CREF(std::string) getName() const;
	CREF(RDOParserSrcInfo) getSrcInfo() const;
	CREF(LPExpression) getExpression() const;
	CREF(LPTypeInfo) getTypeInfo() const;
	rdo::runtime::RDOValue getDefaultValue() const;

private:
	LocalVariable(CREF(LPRDOValue) pName, CREF(LPExpression) pExpression);
	virtual ~LocalVariable();

	LPRDOValue    m_pName;
	LPExpression  m_pExpression;
};

// --------------------------------------------------------------------------------
// -------------------- LocalVariableList
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(LocalVariableList);
class LocalVariableList: public rdo::counter_reference
{
DECLARE_FACTORY(LocalVariableList);
public:
	typedef  std::map<std::string, LPLocalVariable>  VariableList;

	void append(CREF(LPLocalVariable) pVariable);
	LPLocalVariable findLocalVariable(CREF(std::string) name) const;

private:
	LocalVariableList();
	virtual ~LocalVariableList();

	VariableList  m_variableList;
};

// --------------------------------------------------------------------------------
// -------------------- LocalVariableListStack
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(LocalVariableListStack);
class LocalVariableListStack: public rdo::counter_reference
{
DECLARE_FACTORY(LocalVariableListStack);
public:
	typedef std::list<LPLocalVariableList> VariableListStack;

	void push(CREF(LPLocalVariableList) pVariableList);
	void pop ();
	LPLocalVariableList top() const;

	LPLocalVariable findLocalVariable(CREF(std::string) name) const;

private:
	LocalVariableListStack();

	VariableListStack m_pVariableListStack;
};

CLOSE_RDO_PARSER_NAMESPACE

#endif // _SIMULATOR_COMPILER_PARSER_FUNCTION_LOCAL_VARIABLE_H_
