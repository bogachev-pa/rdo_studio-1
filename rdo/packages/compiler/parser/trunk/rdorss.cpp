/*!
  \copyright (c) RDO-Team, 2011
  \file      rdorss.cpp
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdorss.h"
#include "simulator/compiler/parser/rdortp.h"
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdoparser_lexer.h"
#include "simulator/runtime/calc/resource/calc_resource.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

int rsslex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void rsserror(PTR(char) message)
{
	UNUSED(message);
}

// --------------------------------------------------------------------------------
// -------------------- RDORSSResource
// --------------------------------------------------------------------------------
RDORSSResource::RDORSSResource(CREF(LPRDOParser) pParser, CREF(RDOParserSrcInfo) src_info, CREF(LPRDORTPResType) pResType, ruint id)
	: RDOParserSrcInfo(src_info                                      )
	, m_pResType      (pResType                                      )
	, m_id            (id == UNDEFINED_ID ? pParser->getRSS_id() : id)
	, trace           (false                                         )
{
	ASSERT(m_pResType);
	pParser->insertRSSResource(LPRDORSSResource(this));
	m_currParam = m_pResType->getParams().begin();
	RDOParser::s_parser()->contextStack()->push(this);
}

RDORSSResource::~RDORSSResource()
{}

void RDORSSResource::end()
{
	RDOParser::s_parser()->contextStack()->pop();
}

IContextFind::Result RDORSSResource::onSwitchContext(CREF(LPExpression) pSwitchExpression, CREF(LPRDOValue) pValue) const
{
	ASSERT(pSwitchExpression);
	ASSERT(pValue           );

	ruint parNumb = getType()->getRTPParamNumber(pValue->value().getIdentificator());
	if (parNumb == RDORTPResType::UNDEFINED_PARAM)
	{
		RDOParser::s_parser()->error().error(pValue->src_info(), rdo::format(_T("����������� �������� �������: %s"), pValue->value().getIdentificator().c_str()));
	}

	LPRDORTPParam pParam = getType()->findRTPParam(pValue->value().getIdentificator());
	ASSERT(pParam);

	LPExpression pExpression = rdo::Factory<Expression>::create(
		pParam->getTypeInfo(),
		rdo::Factory<rdoRuntime::RDOCalcGetResParamByCalc>::create(pSwitchExpression->calc(), parNumb),
		pValue->src_info()
	);
	ASSERT(pExpression);

	return IContextFind::Result(const_cast<PTR(RDORSSResource)>(this), pExpression, pValue);
}

void RDORSSResource::writeModelStructure(REF(std::ostream) stream) const
{
	stream << (getID() + 1) << _T(" ") << name() << _T(" ") << getType()->getNumber() << std::endl;
}

void RDORSSResource::addParam(CREF(LPRDOValue) pParam)
{
	ASSERT(pParam);

	if (m_currParam == getType()->getParams().end())
	{
		RDOParser::s_parser()->error().push_only(pParam->src_info(), _T("������� ����� ����������"));
		RDOParser::s_parser()->error().push_only(getType()->src_info(), _T("��. ��� �������"));
		RDOParser::s_parser()->error().push_done();
	}
	try
	{
		if (pParam->value().getAsString() == _T("*"))
		{
			if (!(*m_currParam)->getDefault()->defined())
			{
				RDOParser::s_parser()->error().push_only(pParam->src_info(), _T("���������� ������������ '*', �.�. ����������� �������� ��-���������"));
				/// @todo src_info() ��� ��������� RDOParserSrcInfo()
				RDOParser::s_parser()->error().push_only((*m_currParam)->getTypeInfo()->src_info(RDOParserSrcInfo()), _T("��. �������� ���������"));
				RDOParser::s_parser()->error().push_done();
			}
			m_paramList.push_back(Param((*m_currParam)->getDefault()));
			m_currParam++;
		}
		else
		{
			m_paramList.push_back(Param((*m_currParam)->getTypeInfo()->value_cast(pParam)));
			m_currParam++;
		}
	}
	catch(REF(RDOSyntaxException))
	{
		RDOParser::s_parser()->error().modify(rdo::format(_T("��� ��������� '%s': "), (*m_currParam)->name().c_str()));
	}
}

bool RDORSSResource::defined() const
{
	return m_currParam == getType()->getParams().end();
}

rdoRuntime::LPRDOCalc RDORSSResource::createCalc() const
{
	std::vector<rdoRuntime::RDOValue> paramList;
	STL_FOR_ALL_CONST(params(), it)
	{
		paramList.push_back(it->param()->value());
	}

	rdoRuntime::LPRDOCalc calc = rdo::Factory<rdoRuntime::RDOCalcCreateResource>::create(getType()->getRuntimeResType(), paramList, getTrace(), getType()->isPermanent());
	calc->setSrcInfo(src_info());
	calc->setSrcText(_T("�������� ������� ") + calc->src_text());
	return calc;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCResource
// --------------------------------------------------------------------------------
RDOPROCResource::RDOPROCResource(CREF(LPRDOParser) pParser, CREF(RDOParserSrcInfo) src_info, CREF(LPRDORTPResType) pResType, int id)
	: RDORSSResource(pParser, src_info, pResType, id)
{}

RDOPROCResource::~RDOPROCResource()
{}

rdoRuntime::LPRDOCalc RDOPROCResource::createCalc() const
{
	std::vector<rdoRuntime::RDOValue> paramList;
	STL_FOR_ALL_CONST(params(), it)
	{
		paramList.push_back(it->param()->value());
	}

	rdoRuntime::LPRDOCalc calc = rdo::Factory<rdoRuntime::RDOCalcCreateResource>::create(getType()->getRuntimeResType(), paramList, getTrace(), getType()->isPermanent());
	calc->setSrcInfo(src_info());
	calc->setSrcText(_T("�������� ������� ") + calc->src_text());
	return calc;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCTransact
// --------------------------------------------------------------------------------
RDOPROCTransact::RDOPROCTransact(CREF(LPRDOParser) pParser, CREF(RDOParserSrcInfo) src_info, CREF(LPRDORTPResType) pResType, int id)
: RDORSSResource(pParser, src_info, pResType, id)
{}

RDOPROCTransact::~RDOPROCTransact()
{}

rdoRuntime::LPRDOCalc RDOPROCTransact::createCalc() const
{
	std::vector<rdoRuntime::RDOValue> paramList;
	STL_FOR_ALL_CONST(params(), it)
	{
		paramList.push_back(it->param()->value());
	}

	rdoRuntime::LPRDOCalc calc = rdo::Factory<rdoRuntime::RDOCalcCreateResource>::create(getType()->getRuntimeResType(), paramList, getTrace(), getType()->isPermanent());
	calc->setSrcInfo(src_info());
	calc->setSrcText(_T("�������� ������� ") + calc->src_text());
	return calc;
}

CLOSE_RDO_PARSER_NAMESPACE
