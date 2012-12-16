/*!
  \copyright (c) RDO-Team, 2011
  \file      local_variable.cpp
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      16.10.2010
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/src/function/local_variable/local_variable.h"
#include "simulator/compiler/parser/rdoparser_error.h"
#include "simulator/runtime/rdo_resource.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- LocalVariable
// --------------------------------------------------------------------------------
LocalVariable::LocalVariable(CREF(LPRDOValue) pName, CREF(LPExpression) pExpression)
	: m_pName      (pName      )
	, m_pExpression(pExpression)
{
	ASSERT(m_pName      );
	ASSERT(m_pExpression);

	if (m_pExpression->typeInfo()->type()->typeID() == rdo::runtime::RDOType::t_unknow)
	{
		parser::g_error().error(m_pExpression->typeInfo()->src_info(), _T("� ������� ���� ��� �������� �����������"));
	}
}

LocalVariable::~LocalVariable()
{}

CREF(tstring) LocalVariable::getName() const
{
	return m_pName->value().getIdentificator();
}

CREF(RDOParserSrcInfo) LocalVariable::getSrcInfo() const
{
	return *m_pName;
}

CREF(LPExpression) LocalVariable::getExpression() const
{
	return m_pExpression;
}

CREF(LPTypeInfo) LocalVariable::getTypeInfo() const
{
	return m_pExpression->typeInfo();
}

rdo::runtime::RDOValue LocalVariable::getDefaultValue() const
{
	return m_pExpression->typeInfo()->type()->get_default();
}

// --------------------------------------------------------------------------------
// -------------------- LocalVariableList
// --------------------------------------------------------------------------------
LocalVariableList::LocalVariableList()
{}

LocalVariableList::~LocalVariableList()
{}

void LocalVariableList::append(CREF(LPLocalVariable) pVariable)
{
	ASSERT(pVariable);

	LPLocalVariable pExistItem = findLocalVariable(pVariable->getName());

	if (pExistItem)
	{
		parser::g_error().push_only(pVariable->getSrcInfo(),  rdo::format(_T("���������� %s ��� ���������"), pVariable->getName().c_str()));
		parser::g_error().push_only(pExistItem->getSrcInfo(), _T("��. ������ ��������"));
		parser::g_error().push_done();
	}

	m_variableList.insert(VariableList::value_type(pVariable->getName(), pVariable));
}

LPLocalVariable LocalVariableList::findLocalVariable(CREF(tstring) name) const
{
	VariableList::const_iterator it = m_variableList.find(name);
	return it != m_variableList.end() ? it->second : LPLocalVariable(NULL);
}

// --------------------------------------------------------------------------------
// -------------------- LocalVariableListStack
// --------------------------------------------------------------------------------
LocalVariableListStack::LocalVariableListStack()
{}

void LocalVariableListStack::push(CREF(LPLocalVariableList) pVariableList)
{
	m_pVariableListStack.push_back(pVariableList);
}

void LocalVariableListStack::pop()
{
	ASSERT(!m_pVariableListStack.empty());
	m_pVariableListStack.pop_back();
}

LPLocalVariableList LocalVariableListStack::top() const
{
	ASSERT(!m_pVariableListStack.empty());
	return m_pVariableListStack.back();
}

LPLocalVariable LocalVariableListStack::findLocalVariable(CREF(tstring) name) const
{
	STL_FOR_ALL_CONST(m_pVariableListStack, it)
	{
		if ((*it)->findLocalVariable(name))
		{
			return (*it)->findLocalVariable(name);
		}
	}
	return NULL;
}

CLOSE_RDO_PARSER_NAMESPACE