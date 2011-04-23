/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdopat.cpp
 * author   : �������� ����, ������ ������
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdopat.h"
#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_parser/rdortp.h"
#include "rdo_lib/rdo_parser/rdoparser_lexer.h"
#include "rdo_lib/rdo_parser/type/range.h"
#include "rdo_lib/rdo_parser/local_variable.h"
#include "rdo_lib/rdo_runtime/rdo_pattern.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

int patlex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void paterror(PTR(char) mes)
{}

int evnlex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void evnerror(PTR(char) mes)
{}

int evn_preparse_lex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}
void evn_preparse_error(PTR(char) mes)
{}

// ----------------------------------------------------------------------------
// ---------- RDOPATPattern
// ----------------------------------------------------------------------------
RDOPATPattern::RDOPATPattern(CREF(RDOParserSrcInfo) name_src_info)
	: RDOParserSrcInfo    (name_src_info)
	, m_pCommonChoice     (NULL         )
	, m_pPatRuntime       (NULL         )
	, m_pCurrRelRes       (NULL         )
	, m_currentRelResIndex(0            )
	, m_useCommonChoice   (false        )
{
	LPRDOPATPattern pPatternExist = RDOParser::s_parser()->findPATPattern(src_info().src_text());
	if (pPatternExist)
	{
		rdoParse::g_error().push_only(src_info(), rdo::format(_T("������� '%s' ��� ����������"), name().c_str()));
		rdoParse::g_error().push_only(pPatternExist->src_info(), _T("��. ������ �����������"));
		rdoParse::g_error().push_done();
	}
	RDOParser::s_parser()->insertPATPattern(this);
	RDOParser::s_parser()->contextStack()->push(this);

	m_pContextMemory = rdo::Factory<ContextMemory>::create();
	ASSERT(m_pContextMemory);
	RDOParser::s_parser()->contextStack()->push(m_pContextMemory);

	LPLocalVariableListStack pLocalVariableListStack = m_pContextMemory->getLocalMemory();
	ASSERT(pLocalVariableListStack);

	LPLocalVariableList pLocalVariableList = rdo::Factory<LocalVariableList>::create();
	ASSERT(pLocalVariableList);

	pLocalVariableListStack->push(pLocalVariableList);
}

LPContext RDOPATPattern::onFindContext(CREF(RDOValue) value) const
{
	if (m_pContextMemory->onFindContext(value))
	{
		return m_pContextMemory;
	}

	//! ����������� �������
	LPRDORelevantResource pRelevantResource = findRelevantResource(value->getIdentificator());
	if (pRelevantResource)
	{
		if (!m_pCurrRelRes)
		{
			//! ������ with_min-common-choice ��� $Time
			if (pRelevantResource->m_statusBegin == rdoRuntime::RDOResource::CS_NonExist || pRelevantResource->m_statusBegin == rdoRuntime::RDOResource::CS_Create)
			{
				RDOParser::s_parser()->error().error(value.src_info(), rdo::format(_T("����������� ������ �� ����� ���� �����������, �.�. �� ��� �� ����������: %s"), pRelevantResource->name().c_str()));
			}
		}
		else
		{
			//! ������ $Body
			//! ��������� ������������� ��������������������� ���.������� (pRelevantResource) � Choice from ������ ���.������� (m_pCurrRelRes)
			if (m_pCurrRelRes->isChoiceFromState())
			{
				if (!pRelevantResource->m_alreadyHaveConverter && !pRelevantResource->isDirect())
				{
					RDOParser::s_parser()->error().error(value.src_info(), rdo::format(_T("����������� ������ �����������: %s. ��� ������ ������������ � �������� ������ ������ �������� �� ��� ������������ Choice from"), pRelevantResource->name().c_str()));
				}
				if (pRelevantResource->m_statusBegin == rdoRuntime::RDOResource::CS_NonExist)
				{
					RDOParser::s_parser()->error().error(value.src_info(), rdo::format(_T("����������� ������ � ������ �������� �� ���������� (NonExist): %s"), pRelevantResource->name().c_str()));
				}
				if (pRelevantResource->m_statusBegin == rdoRuntime::RDOResource::CS_Create)
				{
					RDOParser::s_parser()->error().error(value.src_info(), rdo::format(_T("����� ����� �������� (Create) ����������� ������ '%s' ����� ������������ ������ � �����������, �� �� � ������� ������"), pRelevantResource->name().c_str()));
				}
			}
			//! ��������� ������������� ���������� ���.������� ������ ���������� ������� ���.�������
			if (pRelevantResource->getType()->isTemporary())
			{
				//! � ���������� ������
				if (m_pCurrRelRes->m_currentState == RDORelevantResource::convertBegin)
				{
					if (pRelevantResource->m_statusBegin == rdoRuntime::RDOResource::CS_Create && !pRelevantResource->m_alreadyHaveConverter)
					{
						RDOParser::s_parser()->error().error(value.src_info(), rdo::format(_T("����������� ������ ������ ������������ �� ��� �������� (Create): %s"), pRelevantResource->name().c_str()));
					}
					if (pRelevantResource->m_statusBegin == rdoRuntime::RDOResource::CS_Erase && pRelevantResource->m_alreadyHaveConverter)
					{
						RDOParser::s_parser()->error().error(value.src_info(), rdo::format(_T("����������� ������ ������ ������������ ����� �������� (Erase): %s"), pRelevantResource->name().c_str()));
					}
					if (pRelevantResource->m_statusBegin == rdoRuntime::RDOResource::CS_NonExist)
					{
						RDOParser::s_parser()->error().error(value.src_info(), rdo::format(_T("����������� ������ �� ���������� � ���� ���������� (NonExist): %s"), pRelevantResource->name().c_str()));
					}
				}
				//! � ���������� �����
				if (m_pCurrRelRes->m_currentState == RDORelevantResource::convertEnd)
				{
					if (pRelevantResource->m_statusEnd == rdoRuntime::RDOResource::CS_Create && !pRelevantResource->m_alreadyHaveConverter)
					{
						RDOParser::s_parser()->error().error(value.src_info(), rdo::format(_T("����������� ������ ������ ������������ �� ��� �������� (Create): %s"), pRelevantResource->name().c_str()));
					}
					if (pRelevantResource->m_statusEnd == rdoRuntime::RDOResource::CS_Erase && pRelevantResource->m_alreadyHaveConverter)
					{
						RDOParser::s_parser()->error().error(value.src_info(), rdo::format(_T("����������� ������ ������ ������������ ����� �������� (Erase): %s"), pRelevantResource->name().c_str()));
					}
					if (pRelevantResource->m_statusEnd == rdoRuntime::RDOResource::CS_NonExist)
					{
						RDOParser::s_parser()->error().error(value.src_info(), rdo::format(_T("����������� ������ �� ���������� � ���� ���������� (NonExist): %s"), pRelevantResource->name().c_str()));
					}
				}
			}
		}
		return pRelevantResource.object_parent_cast<Context>();
	}

	//! ���������
	LPRDOParam pParam = findPATPatternParam(value->getIdentificator());
	if (pParam)
	{
		return const_cast<PTR(RDOPATPattern)>(this);
	}

	return NULL;
}

LPExpression RDOPATPattern::onCreateExpression(CREF(RDOValue) value)
{
	//! ���������
	LPRDOParam pParam = findPATPatternParam(value->getIdentificator());
	if (pParam)
	{
		LPExpression pExpression = rdo::Factory<Expression>::create(
			pParam->getTypeInfo(),
			rdo::Factory<rdoRuntime::RDOCalcPatParam>::create(findPATPatternParamNum(value->getIdentificator())),
			value.src_info()
		);
		ASSERT(pExpression);
		return pExpression;
	}

	NEVER_REACH_HERE;
	return NULL;
}

tstring RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::ConvertStatus value)
{
	switch (value)
	{
	case rdoRuntime::RDOResource::CS_Keep    : return _T("Keep");
	case rdoRuntime::RDOResource::CS_Create  : return _T("Create");
	case rdoRuntime::RDOResource::CS_Erase   : return _T("Erase");
	case rdoRuntime::RDOResource::CS_NonExist: return _T("NonExist");
	case rdoRuntime::RDOResource::CS_NoChange: return _T("NoChange");
	default                                  : NEVER_REACH_HERE;
	}
	return tstring();
}

rdoRuntime::RDOResource::ConvertStatus RDOPATPattern::StrToStatus(CREF(tstring) value, CREF(YYLTYPE) convertor_pos)
{
	if (value == _T("Keep") || value == _T("keep"))
	{
		return rdoRuntime::RDOResource::CS_Keep;
	}
	else if (value == _T("Create") || value == _T("create"))
	{
		return rdoRuntime::RDOResource::CS_Create;
	}
	else if (value == _T("Erase") || value == _T("erase"))
	{
		return rdoRuntime::RDOResource::CS_Erase;
	}
	else if (value == _T("NonExist") || value == _T("nonexist"))
	{
		return rdoRuntime::RDOResource::CS_NonExist;
	}
	else if (value == _T("NoChange") || value == _T("nochange"))
	{
		return rdoRuntime::RDOResource::CS_NoChange;
	}
	rdoParse::g_error().error(convertor_pos, rdo::format(_T("�������� ������ ����������: %s"), value.c_str()));
	return rdoRuntime::RDOResource::CS_Keep;
}

void RDOPATPattern::beforeRelRensert(CREF(RDOParserSrcInfo) rel_info)
{
	if (findRelevantResource(rel_info.src_text()))
	{
		rdoParse::g_error().error(rel_info, rdo::format(_T("����������� ������ '%s' ��� ���������"), rel_info.src_text().c_str()));
	}
}

void RDOPATPattern::rel_res_insert(CREF(LPRDORelevantResource) pRelevantResource)
{
	ASSERT(pRelevantResource);
	switch (getType())
	{
	case PT_Event    : static_cast<PTR(rdoRuntime::RDOPatternEvent)>     (getPatRuntime())->addConvertorStatus     (pRelevantResource->m_statusBegin); break;
	case PT_Rule     : static_cast<PTR(rdoRuntime::RDOPatternRule)>      (getPatRuntime())->addConvertorStatus     (pRelevantResource->m_statusBegin); break;
	case PT_Operation: static_cast<PTR(rdoRuntime::RDOPatternOperation)> (getPatRuntime())->addConvertorBeginStatus(pRelevantResource->m_statusBegin); break;
	case PT_Keyboard : static_cast<PTR(rdoRuntime::RDOPatternKeyboard)>  (getPatRuntime())->addConvertorBeginStatus(pRelevantResource->m_statusBegin); break;
	default          : rdoParse::g_error().error(src_info(), _T("����������� ��� �������"));
	}
	m_relResList.push_back(pRelevantResource);
}

void RDOPATPattern::addRelResConvert(rbool trace, CREF(LPConvertCmdList) commands, CREF(YYLTYPE) convertor_pos, CREF(YYLTYPE) trace_pos, rdoRuntime::RDOResource::ConvertStatus status)
{
	if (status == rdoRuntime::RDOResource::CS_NoChange || status == rdoRuntime::RDOResource::CS_NonExist)
	{
		rdoParse::g_error().error(convertor_pos, getErrorMessage_NotNeedConvertor(m_pCurrRelRes->name(), status));
	}

	if (status == rdoRuntime::RDOResource::CS_Create)
	{
		addParamSetCalc(createRelRes(trace));
	}
	else
	{
		if (trace)
		{
			rdoParse::g_error().error(trace_pos, _T("������� ����������� � ������ ����� �������� ������ ��� ������������ �������"));
		}
	}

	if (commands->commands().empty() && status == rdoRuntime::RDOResource::CS_Keep)
	{
		rdoParse::g_error().warning(convertor_pos, getWarningMessage_EmptyConvertor(m_pCurrRelRes->name(), status));
	}

	STL_FOR_ALL_CONST(commands->commands(), cmdIt)
		addParamSetCalc(*cmdIt);

	ASSERT(m_pCurrRelRes);
}

void RDOPATPattern::addParamSetCalc(CREF(rdoRuntime::LPRDOCalc) pCalc)
{
	switch (getType())
	{
	case PT_Event    : static_cast<PTR(rdoRuntime::RDOPatternEvent)>     (getPatRuntime())->addConvertorCalc(pCalc); break;
	case PT_Rule     : static_cast<PTR(rdoRuntime::RDOPatternRule)>      (getPatRuntime())->addConvertorCalc(pCalc); break;
	case PT_Operation: NEVER_REACH_HERE;
	case PT_Keyboard : NEVER_REACH_HERE;
	default          : rdoParse::g_error().error(src_info(), _T("����������� ��� �������"));
	}
}

tstring RDOPATPattern::getPatternId() const
{ 
	return m_pPatRuntime->traceId(); 
}

void RDOPATPattern::writeModelStructure(REF(std::ostream) stream) const
{
	stream << getPatternId() << " " << name() << " " << getModelStructureLetter() << " " << m_relResList.size();
	STL_FOR_ALL_CONST(m_relResList, it)
		stream << " " << (*it)->getType()->getNumber();

	stream << std::endl;
}

LPRDOParam RDOPATPattern::findPATPatternParam(CREF(tstring) paramName) const
{
	ParamList::const_iterator it = std::find_if(m_paramList.begin(), m_paramList.end(), compareName<RDOParam>(paramName));
	return it != m_paramList.end() ? (*it) : NULL;
}

LPRDORelevantResource RDOPATPattern::findRelevantResource(CREF(tstring) resName) const
{
	RelResList::const_iterator it = std::find_if(m_relResList.begin(), m_relResList.end(), compareName<RDORelevantResource>(resName));
	return it != m_relResList.end() ? (*it) : NULL;
}

int RDOPATPattern::findPATPatternParamNum(CREF(tstring) paramName) const
{
	ParamList::const_iterator it = std::find_if(m_paramList.begin(), m_paramList.end(), compareName<RDOParam>(paramName));
	return it != m_paramList.end() ? it - m_paramList.begin() : -1;
}

int RDOPATPattern::findRelevantResourceNum(CREF(tstring) resName) const
{
	RelResList::const_iterator it = std::find_if(m_relResList.begin(), m_relResList.end(), compareName<RDORelevantResource>(resName));
	return it != m_relResList.end() ? it - m_relResList.begin() : -1;
}

void RDOPATPattern::add(CREF(LPRDOParam) pParam)
{
	ASSERT(pParam);

	LPRDOParam pParamExist = findPATPatternParam(pParam->name());
	if (pParamExist)
	{
		rdoParse::g_error().push_only(pParam->src_info(), rdo::format(_T("�������� '%s' ��� ���������"), pParam->src_text().c_str()));
		rdoParse::g_error().push_only(pParamExist->src_info(), _T("��. ������ �����������"));
		rdoParse::g_error().push_done();
	}
	m_paramList.push_back(pParam);
}

void RDOPATPattern::setCommonChoiceFirst()
{
	m_useCommonChoice = true;
	m_pCommonChoice   = NULL;
//	rdoParse::g_error().error(_T("�������� ������, �.�. � ���������� �� ������� ������������ m_pCommonChoice"));
}

void RDOPATPattern::setCommonChoiceWithMin(CREF(LPRDOFUNArithm) arithm)
{
	m_useCommonChoice  = true;
	m_useCommonWithMax = false;
	m_pCommonChoice    = arithm;
}

void RDOPATPattern::setCommonChoiceWithMax(CREF(LPRDOFUNArithm) arithm)
{
	m_useCommonChoice  = true;
	m_useCommonWithMax = true;
	m_pCommonChoice    = arithm;
}

void RDOPATPattern::setTime(REF(LPRDOFUNArithm) arithm)
{ 
	switch (getType())
	{
	case PT_Operation: static_cast<PTR(rdoRuntime::RDOPatternOperation)> (getPatRuntime())->setTime(arithm->createCalc(NULL)); break;
	case PT_Keyboard : static_cast<PTR(rdoRuntime::RDOPatternKeyboard)>  (getPatRuntime())->setTime(arithm->createCalc(NULL)); break;
	default          : rdoParse::g_error().error(src_info(), rdo::format(_T("��� ������� ���� %s ����������� ������������� ��������� �������"), typeToString(getType()).c_str()));
	}
}

void RDOPATPattern::addChoiceFromCalc(CREF(rdoRuntime::LPRDOCalc) pCalc)
{
	switch (getType())
	{
	case PT_Event    : static_cast<PTR(rdoRuntime::RDOPatternEvent)>     (getPatRuntime())->addPreSelectRelRes(pCalc); break;
	case PT_Rule     : static_cast<PTR(rdoRuntime::RDOPatternRule)>      (getPatRuntime())->addChoiceFromCalc (pCalc); break;
	case PT_Operation: static_cast<PTR(rdoRuntime::RDOPatternOperation)> (getPatRuntime())->addChoiceFromCalc (pCalc); break;
	case PT_Keyboard : static_cast<PTR(rdoRuntime::RDOPatternKeyboard)>  (getPatRuntime())->addChoiceFromCalc (pCalc); break;
	default          : rdoParse::g_error().error(src_info(), rdo::format(_T("��� ������� ���� %s ����������� ������������� �������� ������"), typeToString(getType()).c_str()));
	}
}

void RDOPATPattern::addRelResBody(CREF(RDOParserSrcInfo) body_name)
{ 
	RelResList::const_iterator it = std::find_if(m_relResList.begin(), m_relResList.end(), compareName<RDORelevantResource>(body_name.src_text()));
	if (it == m_relResList.end())
	{
		rdoParse::g_error().error(body_name.src_info(), rdo::format(_T("����������� ����������� ������: %s"), body_name.src_text().c_str()));
	}
	if (findRelevantResourceNum(body_name.src_text()) != m_currentRelResIndex)
	{
		tstring rel_res_waiting = m_currentRelResIndex < m_relResList.size() ? m_relResList[m_currentRelResIndex]->name().c_str() : _T("");
		rdoParse::g_error().error(body_name.src_info(), rdo::format(_T("��������� �������� ������������ ������� '%s', ������ ���� �������: %s"), rel_res_waiting.c_str(), body_name.src_text().c_str()));
	}
	if ((*it)->m_alreadyHaveConverter)
	{
		rdoParse::g_error().error(body_name.src_info(), rdo::format(_T("����������� ������ ��� ������������: %s"), body_name.src_text().c_str()));
	}
	m_pCurrRelRes = (*it);
	m_pCurrRelRes->m_bodySrcInfo = body_name;
	m_pCurrRelRes->m_alreadyHaveConverter = true;
	m_currentRelResIndex++;
}

void RDOPATPattern::addRelResUsage(CREF(LPRDOPATChoiceFrom) pChoiceFrom, CREF(LPRDOPATChoiceOrder) pChoiceOrder)
{
	if (!m_useCommonChoice)
	{
		if (pChoiceOrder->m_type == rdoRuntime::RDOSelectResourceCalc::order_empty)
		{
			if ((m_pCurrRelRes->m_statusBegin != rdoRuntime::RDOResource::CS_Create) && (m_pCurrRelRes->m_statusEnd != rdoRuntime::RDOResource::CS_Create))
			{
//				pChoiceOrder->m_type = rdoRuntime::RDOSelectResourceCalc::order_first;
			}
		}
		else if (m_pCurrRelRes->isDirect())
		{
			rdoParse::g_error().warning(pChoiceOrder->src_info(), rdo::format(_T("������� ������ '%s' ������������ ������� '%s' �� ����� ������, �.�. ����������� ������ ��������� ����� ���, � �� ���, � �� ����� ���� ������ � �����-���� ������ ��������"), pChoiceOrder->src_text().c_str(), m_pCurrRelRes->name().c_str()));
		}
	}
	else
	{
		if (pChoiceOrder->m_type != rdoRuntime::RDOSelectResourceCalc::order_empty)
		{
			rdoParse::g_error().push_only(pChoiceOrder->src_info(), _T("������ ������� ������ ������ ������������ �������, �.�. ������������ ������ ��� ���� ����������� �������� ������, ��������� �� ��������� ����� $Body"));
			if (m_pCommonChoice)
			{
				rdoParse::g_error().push_only(m_pCommonChoice->src_info(), rdo::format(_T("��. '%s'"), m_pCommonChoice->src_text().c_str()));
			}
			rdoParse::g_error().push_done();
		}
	}

	if ((m_pCurrRelRes->m_statusBegin == rdoRuntime::RDOResource::CS_Create) || (m_pCurrRelRes->m_statusEnd == rdoRuntime::RDOResource::CS_Create))
	{
		if (pChoiceFrom->m_type != RDOPATChoiceFrom::ch_empty)
		{
			rdoParse::g_error().error(pChoiceFrom->src_info(), _T("����������� ������ ���������, ��� ���� ������ ������������ Choice from ��� Choice NoCheck"));
		}
		if (pChoiceOrder->m_type != rdoRuntime::RDOSelectResourceCalc::order_empty)
		{
			rdoParse::g_error().error(pChoiceOrder->src_info(), rdo::format(_T("����������� ������ ���������, ��� ���� ������ ������������ ������� ������ '%s'"), pChoiceOrder->asString().c_str()));
		}
	}

	m_pCurrRelRes->m_pChoiceFrom  = pChoiceFrom;
	m_pCurrRelRes->m_pChoiceOrder = pChoiceOrder;
}

void RDOPATPattern::end()
{
	int size = m_relResList.size();
	for (int i = 0; i < size; i++)
	{
		LPRDORelevantResource pCurrRelRes = m_relResList.at(i);
		if (!pCurrRelRes->m_alreadyHaveConverter)
		{
			//! TODO: � ������ ������ ������� warning ? ��������, ���� ������� ���������� ����������� ������� ���. �������.
			rdoParse::g_error().error(pCurrRelRes->src_info(), rdo::format(_T("����������� ������ '%s' �� ������������ � ������� '%s'"), pCurrRelRes->name().c_str(), name().c_str()));
		}
		m_pPatRuntime->addPreSelectRelRes(pCurrRelRes->createPreSelectRelResCalc());
	}

	if (m_useCommonChoice)
	{
		//! first
		//! �������� �����������, � ��� ������������ first
		if (m_pCommonChoice == NULL)
		{
			//! first
			std::vector<rdoRuntime::LPIRDOSelectResourceCommon> resSelectors;
			for (int i = 0; i < size; i++)
			{
				if (m_relResList.at(i)->m_statusBegin == rdoRuntime::RDOResource::CS_Keep || m_relResList.at(i)->m_statusBegin == rdoRuntime::RDOResource::CS_Erase || m_relResList.at(i)->m_statusBegin == rdoRuntime::RDOResource::CS_NoChange)
				{
					resSelectors.push_back(m_relResList.at(i)->createSelectResourceCommonChoiceCalc());
				}
			}
			rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOSelectResourceCommonCalc>::create(resSelectors, m_useCommonWithMax, rdoRuntime::LPRDOCalc(NULL));
			pCalc->setSrcInfo(src_info() );
			pCalc->setSrcText(_T("first"));
			addChoiceFromCalc(pCalc      );
		}
		else
		{
			//! with_min/with_max
			std::vector<rdoRuntime::LPIRDOSelectResourceCommon> resSelectors;
			for (int i = 0; i < size; i++)
			{
				if (m_relResList.at(i)->m_statusBegin == rdoRuntime::RDOResource::CS_Keep || m_relResList.at(i)->m_statusBegin == rdoRuntime::RDOResource::CS_Erase || m_relResList.at(i)->m_statusBegin == rdoRuntime::RDOResource::CS_NoChange)
				{
					resSelectors.push_back(m_relResList.at(i)->createSelectResourceCommonChoiceCalc());
				}
			}
			rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOSelectResourceCommonCalc>::create(resSelectors, m_useCommonWithMax, m_pCommonChoice->createCalc());
			pCalc->setSrcInfo(m_pCommonChoice->src_info());
			addChoiceFromCalc(pCalc);
		}
	}
	else
	{
		for (int i = 0; i < size; i++)
		{
			rdoRuntime::LPRDOCalc pCalc = m_relResList.at(i)->createSelectResourceChoiceCalc();
			addChoiceFromCalc(pCalc);
		}
	}
	RDOParser::s_parser()->contextStack()->pop();
	RDOParser::s_parser()->contextStack()->pop();
}

// ----------------------------------------------------------------------------
// ---------- RDOPatternEvent
// ----------------------------------------------------------------------------
RDOPatternEvent::RDOPatternEvent(CREF(RDOParserSrcInfo) name_src_info, rbool trace)
	: RDOPATPattern(name_src_info)
{ 
	m_pPatRuntime = new rdoRuntime::RDOPatternEvent(RDOParser::s_parser()->runtime(), trace); 
	m_pPatRuntime->setTraceID(RDOParser::s_parser()->getPAT_id());
}

void RDOPatternEvent::addRelRes(CREF(RDOParserSrcInfo) rel_info, CREF(RDOParserSrcInfo) type_info, rdoRuntime::RDOResource::ConvertStatus beg, CREF(YYLTYPE) convertor_pos)
{
	beforeRelRensert(rel_info);
	if (beg == rdoRuntime::RDOResource::CS_NonExist || beg == rdoRuntime::RDOResource::CS_NoChange)
	{
		rdoParse::g_error().error(convertor_pos, _T("������� ����������� NonExist � NoChange �� ����� ���� ������������ � �������"));
	}

	LPRDORelevantResource pRelevantResource;
	LPRDORSSResource res = RDOParser::s_parser()->findRSSResource(type_info.src_text());
	if (res)
	{
		switch (beg)
		{
		case rdoRuntime::RDOResource::CS_Create: rdoParse::g_error().error(type_info, rdo::format(_T("��� �������� ������� '%s' ��������� ������� ��� ���, �� ������ ������ ������ (%s)"), rel_info.src_text().c_str(), type_info.src_text().c_str())); break;
		case rdoRuntime::RDOResource::CS_Erase : rdoParse::g_error().error(convertor_pos, _T("������� ������� � ������� ������")); break;
		}
		pRelevantResource = rdo::Factory<RDORelevantResourceDirect>::create(rel_info, rel_res_count(), res, beg);
		ASSERT(pRelevantResource);
		rel_res_insert(pRelevantResource);
	}
	else
	{
		LPRDORTPResType pResType = RDOParser::s_parser()->findRTPResType(type_info.src_text());
		if (!pResType)
		{
			rdoParse::g_error().error(type_info, rdo::format(_T("����������� ��� �������: %s"), type_info.src_text().c_str()));
		}
		switch (beg)
		{
		case rdoRuntime::RDOResource::CS_Create: if (!pResType->isTemporary()) rdoParse::g_error().error(type_info, rdo::format(_T("��� ������� '%s' ����������. ����������� ��������� �� ���� ������� ������"), type_info.src_text().c_str())); break;
		case rdoRuntime::RDOResource::CS_Keep  :
		case rdoRuntime::RDOResource::CS_Erase : rdoParse::g_error().error(convertor_pos, _T("������� ����������� Keep � Erase ����� ���� ������������ � ������� � ���������� � ���� �������, �� �� ���� �������")); break;
		}
		pRelevantResource = rdo::Factory<RDORelevantResourceByType>::create(rel_info, rel_res_count(), pResType, beg);
		ASSERT(pRelevantResource);
		rel_res_insert(pRelevantResource);
	}
	if (pRelevantResource->m_statusBegin == rdoRuntime::RDOResource::CS_Erase)
	{
		rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcEraseRes>::create(pRelevantResource->m_relResID, pRelevantResource->name());
		pCalc->setSrcInfo(rel_info);
		pCalc->setSrcText(rdo::format(_T("�������� ���������� ������� %s"), rel_info.src_text().c_str()));
		static_cast<PTR(rdoRuntime::RDOPatternEvent)>(getPatRuntime())->addEraseCalc(pCalc);
	}
}

void RDOPatternEvent::addRelResUsage(CREF(LPRDOPATChoiceFrom) pChoiceFrom, CREF(LPRDOPATChoiceOrder) pChoiceOrder)
{
	if (pChoiceFrom->m_type != RDOPATChoiceFrom::ch_empty)
	{
		rdoParse::g_error().error(pChoiceFrom->src_info(), _T("����������� ������� ������� ������ �������� � ������� Choice from ��� Choice NoCheck"));
	}
	if (pChoiceOrder->m_type != rdoRuntime::RDOSelectResourceCalc::order_empty)
	{
		rdoParse::g_error().error(pChoiceFrom->src_info(), rdo::format(_T("��� ����������� �������� ������� ������ ������������ ������� ������ '%s'"), pChoiceOrder->asString().c_str()));
	}
	m_pCurrRelRes->m_pChoiceFrom  = pChoiceFrom;
	m_pCurrRelRes->m_pChoiceOrder = pChoiceOrder;
}

rdoRuntime::LPRDOCalc RDOPATPattern::createRelRes(rbool trace) const
{
	std::vector<rdoRuntime::RDOValue> params_default;
	STL_FOR_ALL_CONST(m_pCurrRelRes->getType()->getParams(), it)
	{
		if (!(*it)->getDefault().defined())
		{
			params_default.push_back(rdoRuntime::RDOValue(0));
			if (!m_pCurrRelRes->getParamSetList().find((*it)->name()))
			{
				rdoParse::g_error().error(m_pCurrRelRes->src_info(), rdo::format(_T("��� �������� ������� ���������� ���������� ��� ��� ���������. �� ������� ����������� ���������: %s"), (*it)->name().c_str()));
			}
		}
		else
		{
			params_default.push_back((*it)->getDefault().value());
		}
	}
	rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcCreateEmptyResource>::create(m_pCurrRelRes->getType()->getNumber(), trace, params_default, m_pCurrRelRes->m_relResID);
	pCalc->setSrcInfo(m_pCurrRelRes->src_info());
	pCalc->setSrcText(rdo::format(_T("�������� ���������� ������� %s"), m_pCurrRelRes->name().c_str()));
	return pCalc;
}

tstring RDOPatternEvent::getErrorMessage_NotNeedConvertor(CREF(tstring) name, rdoRuntime::RDOResource::ConvertStatus status)
{
	return rdo::format(_T("��� ������������ ������� '%s' �� ��������� ��������� (Convert_event), �.�. ��� ������: %s"), name.c_str(), RDOPATPattern::StatusToStr(status).c_str());
}

tstring RDOPatternEvent::getWarningMessage_EmptyConvertor(CREF(tstring) name, rdoRuntime::RDOResource::ConvertStatus status)
{
	return rdo::format(_T("��� ������������ ������� '%s' ������ ������ ��������� (Convert_event), ���� ��� ������: %s"), name.c_str(), RDOPATPattern::StatusToStr(status).c_str());
}

// ----------------------------------------------------------------------------
// ---------- RDOPatternRule
// ----------------------------------------------------------------------------
RDOPatternRule::RDOPatternRule(CREF(RDOParserSrcInfo) name_src_info, rbool trace)
	: RDOPATPattern(name_src_info)
{ 
//	RDOParser::s_parser()->runtime()->addRuntimeRule((RDOPatternRule *)(m_pPatRuntime = new RDOPatternRule(RDOParser::s_parser()->runtime(), _trace))); 
	m_pPatRuntime = new rdoRuntime::RDOPatternRule(RDOParser::s_parser()->runtime(), trace);
	m_pPatRuntime->setTraceID(RDOParser::s_parser()->getPAT_id());
}

void RDOPatternRule::addRelRes(CREF(RDOParserSrcInfo) rel_info, CREF(RDOParserSrcInfo) type_info, rdoRuntime::RDOResource::ConvertStatus beg, CREF(YYLTYPE) convertor_pos)
{
	beforeRelRensert(rel_info);
	if (beg == rdoRuntime::RDOResource::CS_NonExist)
	{
		rdoParse::g_error().error(convertor_pos, rdo::format(_T("������ ������������ ������ ���������� '%s' � ������������� �������"), RDOPATPattern::StatusToStr(beg).c_str()));
	}

	LPRDORelevantResource pRelevantResource;
	LPRDORSSResource res = RDOParser::s_parser()->findRSSResource(type_info.src_text());
	if (res)
	{
		if (beg == rdoRuntime::RDOResource::CS_Create)
		{
			rdoParse::g_error().error(type_info, rdo::format(_T("��� �������� ������� '%s' ��������� ������� ��� ���, �� ������ ������ ������ (%s)"), rel_info.src_text().c_str(), type_info.src_text().c_str()));
		}
		if (beg == rdoRuntime::RDOResource::CS_Erase)
		{
			rdoParse::g_error().error(convertor_pos, rdo::format(_T("������������ ������ ���������� ��� �������: %s"), RDOPATPattern::StatusToStr(beg).c_str()));
		}
		pRelevantResource = rdo::Factory<RDORelevantResourceDirect>::create(rel_info, rel_res_count(), res, beg);
		ASSERT(pRelevantResource);
		rel_res_insert(pRelevantResource);
	}
	else
	{
		LPRDORTPResType pResType = RDOParser::s_parser()->findRTPResType(type_info.src_text());
		if (!pResType)
		{
			rdoParse::g_error().error(type_info, rdo::format(_T("����������� ��� �������: %s"), type_info.src_text().c_str()));
		}
		if (!pResType->isTemporary() && (beg == rdoRuntime::RDOResource::CS_Create || beg == rdoRuntime::RDOResource::CS_Erase))
		{
			rdoParse::g_error().error(type_info, rdo::format(_T("��� �������� ��� �������� ������� ��� ��� ������ ���� ��������� (temporary), � �� ���������� (permanent), ��� � '%s'"), type_info.src_text().c_str()));
		}
		pRelevantResource = rdo::Factory<RDORelevantResourceByType>::create(rel_info, rel_res_count(), pResType, beg);
		ASSERT(pRelevantResource);
		rel_res_insert(pRelevantResource);
	}
	if (pRelevantResource->m_statusBegin == rdoRuntime::RDOResource::CS_Erase)
	{
		rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcEraseRes>::create(pRelevantResource->m_relResID, pRelevantResource->name());
		pCalc->setSrcInfo(rel_info);
		pCalc->setSrcText(rdo::format(_T("�������� ���������� ������� %s"), rel_info.src_text().c_str()));
		static_cast<PTR(rdoRuntime::RDOPatternRule)>(getPatRuntime())->addEraseCalc(pCalc);
	}
}

tstring RDOPatternRule::getErrorMessage_NotNeedConvertor(CREF(tstring) name, rdoRuntime::RDOResource::ConvertStatus status)
{
	return rdo::format(_T("��� ������������ ������� '%s' �� ��������� ��������� (Convert_rule), �.�. ��� ������: %s"), name.c_str(), RDOPATPattern::StatusToStr(status).c_str());
}

tstring RDOPatternRule::getWarningMessage_EmptyConvertor(CREF(tstring) name, rdoRuntime::RDOResource::ConvertStatus status)
{
	return rdo::format(_T("��� ������������ ������� '%s' ������ ������ ��������� (Convert_rule), ���� ��� ������: %s"), name.c_str(), RDOPATPattern::StatusToStr(status).c_str());
}

// ----------------------------------------------------------------------------
// ---------- RDOPatternOperation
// ----------------------------------------------------------------------------
RDOPatternOperation::RDOPatternOperation(CREF(RDOParserSrcInfo) name_src_info, rbool trace)
	: RDOPATPattern  (name_src_info )
	, m_convertorType(convert_unknow)
{ 
	m_pPatRuntime = new rdoRuntime::RDOPatternOperation(RDOParser::s_parser()->runtime(), trace);
	m_pPatRuntime->setTraceID(RDOParser::s_parser()->getPAT_id());
}

RDOPatternOperation::RDOPatternOperation(rbool trace, CREF(RDOParserSrcInfo) name_src_info)
	: RDOPATPattern  (name_src_info )
	, m_convertorType(convert_unknow)
{}

void RDOPatternOperation::rel_res_insert(CREF(LPRDORelevantResource) pRelevantResource)
{
	ASSERT(pRelevantResource);
	RDOPATPattern::rel_res_insert(pRelevantResource);
	static_cast<PTR(rdoRuntime::RDOPatternOperation)>(getPatRuntime())->addConvertorEndStatus(pRelevantResource->m_statusEnd);
}

void RDOPatternOperation::addRelRes(CREF(RDOParserSrcInfo) rel_info, CREF(RDOParserSrcInfo) type_info, rdoRuntime::RDOResource::ConvertStatus beg, CREF(YYLTYPE) convertor_pos)
{
	rdoParse::g_error().error(convertor_pos, _T("���������� ������ �������"));
}

void RDOPatternOperation::addRelRes(CREF(RDOParserSrcInfo) rel_info, CREF(RDOParserSrcInfo) type_info, rdoRuntime::RDOResource::ConvertStatus beg, rdoRuntime::RDOResource::ConvertStatus end, CREF(YYLTYPE) convertor_begin_pos, CREF(YYLTYPE) convertor_end_pos)
{
	beforeRelRensert(rel_info);
	switch (beg)
	{
	case rdoRuntime::RDOResource::CS_Keep:
		if (end != rdoRuntime::RDOResource::CS_Keep && end != rdoRuntime::RDOResource::CS_Erase && end != rdoRuntime::RDOResource::CS_NoChange)
		{
			rdoParse::g_error().error(convertor_end_pos, rdo::format(_T("������ ���������� ����� ����������� �� �������� ���������� ������: %s � %s"), RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()));
		}
		break;
	case rdoRuntime::RDOResource::CS_Create:
		if (end != rdoRuntime::RDOResource::CS_Keep && end != rdoRuntime::RDOResource::CS_Erase && end != rdoRuntime::RDOResource::CS_NoChange)
		{
			rdoParse::g_error().error(convertor_end_pos, rdo::format(_T("������ ���������� ����� ����������� �� �������� ���������� ������: %s � %s"), RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()));
		}
		break;
	case rdoRuntime::RDOResource::CS_Erase:
		if (end != rdoRuntime::RDOResource::CS_NonExist)
		{
			rdoParse::g_error().error(convertor_end_pos, rdo::format(_T("������ ���������� ����� ����������� �� �������� ���������� ������: %s � %s"), RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()));
		}
		break;
	case rdoRuntime::RDOResource::CS_NonExist:
		if (end != rdoRuntime::RDOResource::CS_Create)
		{
			rdoParse::g_error().error(convertor_end_pos, rdo::format(_T("������ ���������� ����� ����������� �� �������� ���������� ������: %s � %s"), RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()));
		}
		break;
	case rdoRuntime::RDOResource::CS_NoChange:
		if (end != rdoRuntime::RDOResource::CS_Keep && end != rdoRuntime::RDOResource::CS_Erase && end != rdoRuntime::RDOResource::CS_NoChange)
		{
			rdoParse::g_error().error(convertor_end_pos, rdo::format(_T("������ ���������� ����� ����������� �� �������� ���������� ������: %s � %s"), RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()));
		}
		break;
	};

	LPRDORelevantResource pRelevantResource;
	LPRDORSSResource res = RDOParser::s_parser()->findRSSResource(type_info.src_text());
	if (res)
	{
		if (beg == rdoRuntime::RDOResource::CS_Create)
		{
			rdoParse::g_error().error(convertor_begin_pos, rdo::format(_T("��� �������� ������� '%s' ��������� ������� ��� ���, �� ������ ������ ������ (%s)"), rel_info.src_text().c_str(), type_info.src_text().c_str()));
		}
		if (end == rdoRuntime::RDOResource::CS_Create)
		{
			rdoParse::g_error().error(convertor_end_pos, rdo::format(_T("��� �������� ������� '%s' ��������� ������� ��� ���, �� ������ ������ ������ (%s)"), rel_info.src_text().c_str(), type_info.src_text().c_str()));
		}
		if (beg == rdoRuntime::RDOResource::CS_Erase || beg == rdoRuntime::RDOResource::CS_NonExist)
		{
			rdoParse::g_error().error(convertor_begin_pos, rdo::format(_T("������������ ������ ���������� ������ ��� �������: %s"), RDOPATPattern::StatusToStr(beg).c_str()));
		}
		if (end == rdoRuntime::RDOResource::CS_Erase || end == rdoRuntime::RDOResource::CS_NonExist)
		{
			rdoParse::g_error().error(convertor_end_pos, rdo::format(_T("������������ ������ ���������� ����� ��� �������: %s"), RDOPATPattern::StatusToStr(end).c_str()));
		}
		pRelevantResource = rdo::Factory<RDORelevantResourceDirect>::create(rel_info, rel_res_count(), res, beg, end);
		ASSERT(pRelevantResource);
		rel_res_insert(pRelevantResource);
	}
	else
	{
		LPRDORTPResType pResType = RDOParser::s_parser()->findRTPResType(type_info.src_text());
		if (!pResType)
		{
			rdoParse::g_error().error(type_info, rdo::format(_T("����������� ��� �������: %s"), type_info.src_text().c_str()));
		}
		if (pResType->isPermanent())
		{
			if (beg == rdoRuntime::RDOResource::CS_Create || beg == rdoRuntime::RDOResource::CS_Erase || beg == rdoRuntime::RDOResource::CS_NonExist)
			{
				rdoParse::g_error().error(convertor_begin_pos, rdo::format(_T("������������ ������ ���������� ������ ��� ����������� ����: %s"), RDOPATPattern::StatusToStr(beg).c_str()));
			}
			if (end == rdoRuntime::RDOResource::CS_Create || end == rdoRuntime::RDOResource::CS_Erase || end == rdoRuntime::RDOResource::CS_NonExist)
			{
				rdoParse::g_error().error(convertor_end_pos, rdo::format(_T("������������ ������ ���������� ����� ��� ����������� ����: %s"), RDOPATPattern::StatusToStr(end).c_str()));
			}
		}
		pRelevantResource = rdo::Factory<RDORelevantResourceByType>::create(rel_info, rel_res_count(), pResType, beg, end);
		ASSERT(pRelevantResource);
		rel_res_insert(pRelevantResource);
	}
	if (pRelevantResource->m_statusBegin == rdoRuntime::RDOResource::CS_Erase)
	{
		rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcEraseRes>::create(pRelevantResource->m_relResID, pRelevantResource->name());
		pCalc->setSrcInfo(rel_info);
		pCalc->setSrcText(rdo::format(_T("�������� ���������� ������� %s"), rel_info.src_text().c_str()));
		static_cast<PTR(rdoRuntime::RDOPatternOperation)>(getPatRuntime())->addEraseBeginCalc(pCalc);
	}
	if (pRelevantResource->m_statusEnd == rdoRuntime::RDOResource::CS_Erase)
	{
		rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcEraseRes>::create(pRelevantResource->m_relResID, pRelevantResource->name());
		pCalc->setSrcInfo(rel_info);
		pCalc->setSrcText(rdo::format(_T("�������� ���������� ������� %s"), rel_info.src_text().c_str()));
		static_cast<PTR(rdoRuntime::RDOPatternOperation)>(getPatRuntime())->addEraseEndCalc(pCalc);
	}
}

void RDOPatternOperation::addRelResConvertBeginEnd(rbool trace_begin, CREF(LPConvertCmdList) cmd_begin, rbool trace_end, CREF(LPConvertCmdList) cmd_end, CREF(YYLTYPE) convertor_begin_pos, CREF(YYLTYPE) convertor_end_pos, CREF(YYLTYPE) trace_begin_pos, CREF(YYLTYPE) trace_end_pos)
{
	if (cmd_begin)
	{
		m_convertorType = convert_begin;
		ASSERT(m_pCurrRelRes);
		addRelResConvert(trace_begin, cmd_begin, convertor_begin_pos, trace_begin_pos, m_pCurrRelRes->m_statusBegin);
		m_convertorType = convert_unknow;
	}
	if (cmd_end)
	{
		m_convertorType = convert_end;
		ASSERT(m_pCurrRelRes);
		addRelResConvert(trace_end, cmd_end, convertor_end_pos, trace_end_pos, m_pCurrRelRes->m_statusEnd);
		m_convertorType = convert_unknow;
	}
}

void RDOPatternOperation::addParamSetCalc(CREF(rdoRuntime::LPRDOCalc) pCalc)
{
	switch (m_convertorType)
	{
	case convert_begin: static_cast<PTR(rdoRuntime::RDOPatternOperation)>(getPatRuntime())->addConvertorBeginCalc(pCalc); break;
	case convert_end  : static_cast<PTR(rdoRuntime::RDOPatternOperation)>(getPatRuntime())->addConvertorEndCalc  (pCalc); break;
	default           : NEVER_REACH_HERE;
	}
}

tstring RDOPatternOperation::getErrorMessage_NotNeedConvertor(CREF(tstring) name, rdoRuntime::RDOResource::ConvertStatus status)
{
	switch (m_convertorType)
	{
	case convert_begin: return rdo::format(_T("��� ������������ ������� '%s' �� ��������� ��������� ������ (Convert_begin), �.�. ��� ������: %s"), name.c_str(), RDOPATPattern::StatusToStr(status).c_str()); break;
	case convert_end  : return rdo::format(_T("��� ������������ ������� '%s' �� ��������� ��������� ����� (Convert_end), �.�. ��� ������: %s"), name.c_str(), RDOPATPattern::StatusToStr(status).c_str()); break;
	default           : NEVER_REACH_HERE;
	}
	return tstring();
}

tstring RDOPatternOperation::getWarningMessage_EmptyConvertor(CREF(tstring) name, rdoRuntime::RDOResource::ConvertStatus status)
{
	switch (m_convertorType)
	{
	case convert_begin: return rdo::format(_T("��� ������������ ������� '%s' ������ ������ ��������� ������ (Convert_begin), ���� ��� ������: %s"), name.c_str(), RDOPATPattern::StatusToStr(status).c_str()); break;
	case convert_end  : return rdo::format(_T("��� ������������ ������� '%s' ������ ������ ��������� ����� (Convert_end), ���� ��� ������: %s"), name.c_str(), RDOPATPattern::StatusToStr(status).c_str()); break;
	default           : NEVER_REACH_HERE;
	}
	return tstring();
}

// ----------------------------------------------------------------------------
// ---------- RDOPatternKeyboard
// ----------------------------------------------------------------------------
RDOPatternKeyboard::RDOPatternKeyboard(CREF(RDOParserSrcInfo) name_src_info, rbool trace)
	: RDOPatternOperation(trace, name_src_info)
{
	m_pPatRuntime = new rdoRuntime::RDOPatternKeyboard(RDOParser::s_parser()->runtime(), trace); 
	m_pPatRuntime->setTraceID(RDOParser::s_parser()->getPAT_id());
}

// ----------------------------------------------------------------------------
// ---------- RDORelevantResource
// ----------------------------------------------------------------------------
LPExpression RDORelevantResource::onCreateExpression(CREF(RDOValue) value)
{
	ruint parNumb = getType()->getRTPParamNumber(value->getIdentificator());
	if (parNumb == RDORTPResType::UNDEFINED_PARAM)
	{
		RDOParser::s_parser()->error().error(value.src_info(), rdo::format(_T("����������� �������� �������: %s"), value->getIdentificator().c_str()));
	}

	//! ��������� ������������� ��� �� ������������������� (������ ��� Create) ��������� ���. ������� � ��� �� ����������
	LPRDORTPParam pParam = getType()->findRTPParam(value->getIdentificator());
	ASSERT(pParam);
	//! � ���������� ������
	if (m_currentState == RDORelevantResource::convertBegin && m_statusBegin == rdoRuntime::RDOResource::CS_Create)
	{
		if (!getParamSetList().find(value->getIdentificator()))
		{
			if (!pParam->getDefault().defined())
			{
				RDOParser::s_parser()->error().error(value.src_info(), rdo::format(_T("�������� '%s' ��� �� ���������, ��� ���������� ��������� �������� � ������� ���������� ��� ������� �������� ��-��������� � ���� �������"), value->getIdentificator().c_str()));
			}
		}
	}
	//! � ���������� �����
	if (m_currentState == RDORelevantResource::convertEnd && m_statusEnd == rdoRuntime::RDOResource::CS_Create)
	{
		if (!getParamSetList().find(value->getIdentificator()))
		{
			if (!getParamSetList().find(value->getIdentificator()))
			{
				if (!pParam->getDefault().defined())
				{
					RDOParser::s_parser()->error().error(value.src_info(), rdo::format(_T("�������� '%s' ��� �� ���������, ��� ���������� ��������� �������� � ������� ���������� ��� ������� �������� ��-��������� � ���� �������"), value->getIdentificator().c_str()));
				}
			}
		}
	}

	LPExpression pExpression = rdo::Factory<Expression>::create(
		pParam->getTypeInfo(),
		rdo::Factory<rdoRuntime::RDOCalcGetRelevantResParam>::create(m_relResID, parNumb),
		value.src_info()
	);
	ASSERT(pExpression);
	return pExpression;
}

rdoRuntime::LPRDOCalc RDORelevantResource::getChoiceCalc()
{
	if (m_pChoiceFrom && m_pChoiceFrom->m_type == rdoParse::RDOPATChoiceFrom::ch_from)
	{
		return m_pChoiceFrom->m_pLogic->getCalc(rdoRuntime::RDOType::t_int);
	}
	return NULL;
}

rdoRuntime::LPRDOCalc RDORelevantResource::getSelectCalc()
{
	if (m_pChoiceOrder && m_pChoiceOrder->m_pArithm)
	{
		return m_pChoiceOrder->m_pArithm->createCalc(NULL);
	}
	return NULL;
}

rdoRuntime::RDOSelectResourceCalc::Type RDORelevantResource::getSelectType() const
{
	return m_pChoiceOrder ? m_pChoiceOrder->m_type : rdoRuntime::RDOSelectResourceCalc::order_empty;
}

// ----------------------------------------------------------------------------
// ---------- RDORelevantResourceDirect - �� ����� �������
// ----------------------------------------------------------------------------
rdoRuntime::LPRDOCalc RDORelevantResourceDirect::createPreSelectRelResCalc()
{
	rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOSelectResourceDirectCalc>::create(m_relResID, m_pResource->getID());
	pCalc->setSrcInfo(src_info());
	pCalc->setSrcText(rdo::format(_T("��������������� ����� ���. ������� %s"), pCalc->src_text().c_str()));
	return pCalc;
}

rdoRuntime::LPRDOCalc RDORelevantResourceDirect::createSelectResourceChoiceCalc()
{
	rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOSelectResourceDirectCalc>::create(m_relResID, m_pResource->getID(), getChoiceCalc(), getSelectCalc(), getSelectType());
	pCalc->setSrcInfo(m_pChoiceFrom->src_info());
	return pCalc;
}

rdoRuntime::LPRDOCalc RDORelevantResourceDirect::createSelectFirstResourceChoiceCalc()
{
	return rdo::Factory<rdoRuntime::RDOSelectResourceDirectCalc>::create(m_relResID, m_pResource->getID(), getChoiceCalc());
}

rdoRuntime::LPIRDOSelectResourceCommon RDORelevantResourceDirect::createSelectResourceCommonChoiceCalc()
{
	rdoRuntime::LPRDOSelectResourceDirectCommonCalc pDirectCommonCalc = rdo::Factory<rdoRuntime::RDOSelectResourceDirectCommonCalc>::create(m_relResID, m_pResource->getID(), getChoiceCalc());
	rdoRuntime::LPIRDOSelectResourceCommon pSelectResourceCommon = pDirectCommonCalc.interface_cast<rdoRuntime::IRDOSelectResourceCommon>();
	ASSERT(pSelectResourceCommon);
	return pSelectResourceCommon;
}

LPRDORTPResType RDORelevantResourceDirect::getType() const 
{ 
	return m_pResource->getType(); 
}

// ----------------------------------------------------------------------------
// ---------- RDORelevantResourceByType - �� ����� ����
// ----------------------------------------------------------------------------
rdoRuntime::LPRDOCalc RDORelevantResourceByType::createPreSelectRelResCalc()
{
	if ((m_statusBegin != rdoRuntime::RDOResource::CS_Create) && (m_statusEnd != rdoRuntime::RDOResource::CS_Create))
	{
		rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOSelectResourceByTypeCalc>::create(m_relResID, m_pResType->getNumber());
		pCalc->setSrcInfo(src_info());
		pCalc->setSrcText(rdo::format(_T("��������������� ����� ���. ������� %s"), pCalc->src_text().c_str()));
		return pCalc;
	}
	else
	{
		if (m_statusBegin == rdoRuntime::RDOResource::CS_NonExist)
		{
			return rdo::Factory<rdoRuntime::RDOSelectResourceNonExistCalc>::create(m_relResID);
		}
		else
		{
			rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcConst>::create(1);
			pCalc->setSrcInfo(src_info());
			pCalc->setSrcText(rdo::format(_T("��������������� ����� ���. ������� ����� ��������� %s"), pCalc->src_text().c_str()));
			return pCalc;
		}
	}
}

rdoRuntime::LPRDOCalc RDORelevantResourceByType::createSelectResourceChoiceCalc()
{
	if ((m_statusBegin != rdoRuntime::RDOResource::CS_Create) && (m_statusEnd != rdoRuntime::RDOResource::CS_Create))
	{
		rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOSelectResourceByTypeCalc>::create(m_relResID, m_pResType->getNumber(), getChoiceCalc(), getSelectCalc(), getSelectType());
		pCalc->setSrcInfo(m_pChoiceFrom->src_info());
		return pCalc;
	}
	else
	{
		if (m_statusBegin == rdoRuntime::RDOResource::CS_NonExist)
		{
			return rdo::Factory<rdoRuntime::RDOSelectResourceNonExistCalc>::create(m_relResID);
		}
		else
		{
			rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcConst>::create(1);
			pCalc->setSrcInfo(src_info());
			pCalc->setSrcText(rdo::format(_T("����� ��������� ���. ������� %s"), pCalc->src_text().c_str()));
			return pCalc;
		}
	}
}

rdoRuntime::LPRDOCalc RDORelevantResourceByType::createSelectFirstResourceChoiceCalc()
{
	if ((m_statusBegin != rdoRuntime::RDOResource::CS_Create) && (m_statusEnd != rdoRuntime::RDOResource::CS_Create))
	{
		return rdo::Factory<rdoRuntime::RDOSelectResourceByTypeCalc>::create(m_relResID, m_pResType->getNumber(), getChoiceCalc());
	}
	else
	{
		if (m_statusBegin == rdoRuntime::RDOResource::CS_NonExist)
		{
			return rdo::Factory<rdoRuntime::RDOSelectResourceNonExistCalc>::create(m_relResID);
		}
		else
		{
			return rdo::Factory<rdoRuntime::RDOCalcConst>::create(1);
		}
	}
}

rdoRuntime::LPIRDOSelectResourceCommon RDORelevantResourceByType::createSelectResourceCommonChoiceCalc()
{
	rdoRuntime::LPRDOSelectResourceByTypeCommonCalc pByTypeCommonCalc = rdo::Factory<rdoRuntime::RDOSelectResourceByTypeCommonCalc>::create(m_relResID, m_pResType->getNumber(), getChoiceCalc());
	rdoRuntime::LPIRDOSelectResourceCommon pSelectResourceCommon = pByTypeCommonCalc.interface_cast<rdoRuntime::IRDOSelectResourceCommon>();
	ASSERT(pSelectResourceCommon);
	return pSelectResourceCommon;
}

CLOSE_RDO_PARSER_NAMESPACE
