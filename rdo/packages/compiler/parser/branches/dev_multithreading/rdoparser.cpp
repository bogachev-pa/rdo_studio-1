/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoparser.cpp
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
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdoparser_rdo.h"
#include "simulator/compiler/parser/rdofun.h"
#include "simulator/compiler/parser/rdorss.h"
#include "simulator/runtime/calc/calc_process.h"
#include "simulator/runtime/calc/resource/calc_resource.h"
#include "simulator/runtime/calc/function/calc_function_system.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSE_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOParser
// --------------------------------------------------------------------------------
RDOParser::ParserList RDOParser::s_parserStack;

#define DECLARE_PARSER_OBJECT_CONTAINER_NONAME(NAME) \
void RDOParser::insert##NAME(LPRDO##NAME value) \
{ \
	howIsIt<LPRDO##NAME>(); \
	m_all##NAME.push_back(value); \
}

#define DECLARE_PARSER_OBJECT_CONTAINER(NAME) \
DECLARE_PARSER_OBJECT_CONTAINER_NONAME(NAME) \
const LPRDO##NAME RDOParser::find##NAME(CREF(tstring) name) const \
{ \
	NAME##List::const_iterator it = std::find_if(m_all##NAME.begin(), m_all##NAME.end(), compareName<RDO##NAME>(name)); \
	return it != m_all##NAME.end() ? *it : LPRDO##NAME(NULL); \
} \
rbool RDOParser::remove##NAME(const LPRDO##NAME item) \
{ \
	NAME##List::iterator it = std::find(m_all##NAME.begin(), m_all##NAME.end(), item); \
	if (it == m_all##NAME.end()) \
		return false; \
	m_all##NAME.erase(it); \
	return true; \
}

DECLARE_PARSER_OBJECT_CONTAINER(PATPattern );
DECLARE_PARSER_OBJECT_CONTAINER(RTPResType );
DECLARE_PARSER_OBJECT_CONTAINER(RSSResource);
DECLARE_PARSER_OBJECT_CONTAINER(FRMFrame   );
DECLARE_PARSER_OBJECT_CONTAINER(FUNConstant);
DECLARE_PARSER_OBJECT_CONTAINER(FUNFunction);
DECLARE_PARSER_OBJECT_CONTAINER(FUNSequence);
DECLARE_PARSER_OBJECT_CONTAINER(DPTSearch  );
DECLARE_PARSER_OBJECT_CONTAINER(DPTSome    );
DECLARE_PARSER_OBJECT_CONTAINER(DPTPrior   );
DECLARE_PARSER_OBJECT_CONTAINER(DPTActivity);
DECLARE_PARSER_OBJECT_CONTAINER(Event      );
DECLARE_PARSER_OBJECT_CONTAINER(ResultGroup);
DECLARE_PARSER_OBJECT_CONTAINER(PROCProcess);

DECLARE_PARSER_OBJECT_CONTAINER_NONAME(FUNGroup);

rdoModelObjects::RDOFileType RDOParser::getFileToParse()
{
	return !s_parserStack.empty() && s_parserStack.back()->m_parser_item ? s_parserStack.back()->m_parser_item->m_type : rdoModelObjects::PAT;
}

ruint RDOParser::lexer_loc_line()
{
	return !s_parserStack.empty() && s_parserStack.back()->m_parser_item ? s_parserStack.back()->m_parser_item->lexer_loc_line() : ~0;
}

ruint RDOParser::lexer_loc_pos()
{
	return !s_parserStack.empty() && s_parserStack.back()->m_parser_item ? s_parserStack.back()->m_parser_item->lexer_loc_pos() : 0;
}

LPRDOParser RDOParser::s_parser()
{
	return !s_parserStack.empty() ? s_parserStack.back() : LPRDOParser(NULL);
}

RDOParser::RDOParser()
	: m_parser_item         (NULL )
	, m_have_kw_Resources   (false)
	, m_have_kw_ResourcesEnd(false)
	, m_pattern             (false)
{}

RDOParser::~RDOParser()
{}

void RDOParser::init()
{
	s_parserStack.push_back(this);

	m_pRuntime = rdo::Factory<rdoRuntime::RDORuntime>::create();
	ASSERT(m_pRuntime);
	m_pRuntime->memory_insert(sizeof(RDOParser));
	m_pRuntime->init();

	m_pContextStack = rdo::Factory<ContextStack>::create();
	ASSERT(m_pContextStack);

	m_pContextStack->push(this);

	LPRDOSMR pSMR = rdo::Factory<RDOSMR>::create();
	ASSERT(pSMR);
	setSMR(pSMR);

	m_resultGeneratorID.get(); //! ��� PMD ��������� � 1
}

void RDOParser::deinit()
{
	m_pContextStack->pop();

	Context::deinit();

	m_allPATPattern .clear();
	m_allRTPResType .clear();
	m_allRSSResource.clear();
	m_allFRMFrame   .clear();
	m_allFUNConstant.clear();
	m_allFUNFunction.clear();
	m_allFUNSequence.clear();
	m_allDPTSearch  .clear();
	m_allDPTSome    .clear();
	m_allDPTPrior   .clear();
	m_allDPTActivity.clear();
	m_allEvent      .clear();
	m_allResultGroup.clear();
	m_allPROCProcess.clear();
	m_allFUNGroup   .clear();

	m_parser_item   = NULL;
	m_pSMR          = NULL;
	m_pContextStack = NULL;

	m_preCastTypeList   .clear();
	m_movementObjectList.clear();

	m_pRuntime->deinit();
	m_pRuntime = NULL;

	s_parserStack.remove(this);
}

LPContextStack RDOParser::contextStack()
{
	return m_pContextStack;
}

LPContext RDOParser::context() const
{
	return m_pContextStack->top();
}

Context::FindResult RDOParser::onFindContext(CREF(LPRDOValue) pValue) const
{
	ASSERT(pValue);

	if (pValue->value().getIdentificator() == _T("Time_now") || pValue->value().getIdentificator() == _T("time_now") || pValue->value().getIdentificator() == _T("���������_�����") || pValue->value().getIdentificator() == _T("���������_�����"))
	{
		LPExpression pExpression = rdo::Factory<Expression>::create(
			rdo::Factory<TypeInfo>::create(
				rdo::Factory<RDOType__real>::create(),
				pValue->src_info()
			),
			rdo::Factory<rdoRuntime::RDOCalcGetTimeNow>::create(),
			pValue->src_info()
		);
		ASSERT(pExpression);
		return Context::FindResult(const_cast<PTR(RDOParser)>(this), pExpression, pValue);
	}
	else if (pValue->value().getIdentificator() == _T("Seconds") || pValue->value().getIdentificator() == _T("seconds"))
	{
		LPExpression pExpression = rdo::Factory<Expression>::create(
			rdo::Factory<TypeInfo>::create(
				rdo::Factory<RDOType__real>::create(),
				pValue->src_info()
			),
			rdo::Factory<rdoRuntime::RDOCalcGetSeconds>::create(),
			pValue->src_info()
		);
		ASSERT(pExpression);
		return Context::FindResult(const_cast<PTR(RDOParser)>(this), pExpression, pValue);
	}
	else if (pValue->value().getIdentificator() == _T("Terminate_counter") || pValue->value().getIdentificator() == _T("terminate_counter"))
	{
		LPExpression pExpression = rdo::Factory<Expression>::create(
			rdo::Factory<TypeInfo>::create(
				rdo::Factory<RDOType__int>::create(),
				pValue->src_info()
			),
			rdo::Factory<rdoRuntime::RDOCalcGetTermNow>::create(),
			pValue->src_info()
		);
		ASSERT(pExpression);
		return Context::FindResult(const_cast<PTR(RDOParser)>(this), pExpression, pValue);
	}

	//! ���� ��������
	LPRDORTPResType pResType = findRTPResType(pValue->value().getIdentificator());
	if (pResType)
	{
		//! ��� ��� ������� � �������� RTP
		LPExpression pExpression = rdo::Factory<Expression>::create(
			rdo::Factory<TypeInfo>::create(
				pResType,
				pValue->src_info()
			),
			rdoRuntime::LPRDOCalc(NULL),
			pValue->src_info()
		);
		ASSERT(pExpression);
		return Context::FindResult(const_cast<PTR(RDOParser)>(this), pExpression, pValue, pResType.object_static_cast<Context>());
	}

	//! ��� ������ � �������� RSS
	LPRDORSSResource pResource = findRSSResource(pValue->value().getIdentificator());
	if (pResource)
	{
		LPExpression pExpression = rdo::Factory<Expression>::create(
			rdo::Factory<TypeInfo>::create(
				pResource->getType(),
				pValue->src_info()
			),
			rdo::Factory<rdoRuntime::RDOCalcGetResourceByID>::create(pResource->getID()),
			pValue->src_info()
		);
		ASSERT(pExpression);
		return Context::FindResult(const_cast<PTR(RDOParser)>(this), pExpression, pValue, pResource.object_static_cast<Context>());
	}

	//! ���������
	LPRDOFUNConstant pConstant = findFUNConstant(pValue->value().getIdentificator());
	if (pConstant)
	{
		LPExpression pExpression = rdo::Factory<Expression>::create(
			pConstant->getTypeInfo(),
			rdo::Factory<rdoRuntime::RDOCalcGetConst>::create(pConstant->getNumber()),
			pValue->src_info()
		);
		ASSERT(pExpression);
		return Context::FindResult(const_cast<PTR(RDOParser)>(this), pExpression, pValue);
	}

	//! ������������������
	LPRDOFUNSequence pSequence = findFUNSequence(pValue->value().getIdentificator());
	if (pSequence)
	{
		LPRDOFUNParams pParams = rdo::Factory<RDOFUNParams>::create(
			rdo::Factory<ArithmContainer>::create()
		);
		ASSERT(pParams);
		LPRDOFUNArithm pArithm = pParams->createSeqCall(pValue->value().getIdentificator());
		ASSERT(pArithm);
		pArithm->setSrcInfo(pValue->src_info());
		LPExpression pExpression = rdo::Factory<Expression>::create(
			pArithm->typeInfo(),
			pArithm->calc(),
			pValue->src_info()
		);
		ASSERT(pExpression);
		return Context::FindResult(const_cast<PTR(RDOParser)>(this), pExpression, pValue);
	}

	//! ��������, ��� ��� �������� ������������� ����, ������ ���� � ���� �������� ����� ����������� � ������
	//! ������������ �����, ������� ����� ������ �� ��� ������� - ������
	{ErrorBlockMonicker errorBlockMonicker;
		CREF(PreCastTypeList) typeList = getPreCastTypeList();
		STL_FOR_ALL_CONST(typeList, it)
		{
			LPRDOValue pTryCastValue = (*it)->value_cast(pValue);
			if (pTryCastValue && pTryCastValue->defined())
			{
				LPExpression pExpression = rdo::Factory<Expression>::create(
					rdo::Factory<TypeInfo>::create(
						rdo::Factory<RDOType__identificator>::create(),
						pValue->src_info()
					),
					rdo::Factory<rdoRuntime::RDOCalcConst>::create(pValue->value()),
					pValue->src_info()
				);
				ASSERT(pExpression);
				return Context::FindResult(const_cast<PTR(RDOParser)>(this), pExpression, pValue);
			}
		}
	}

	const_cast<PTR(RDOParser)>(this)->error().error(pValue->src_info(), rdo::format(_T("����������� �������������: %s"), pValue->value().getIdentificator().c_str()));
	return Context::FindResult();
}

rbool RDOParser::isCurrentDPTSearch()
{
	return getLastDPTSearch() && !getLastDPTSearch()->closed() ? true : false;
}

rbool RDOParser::isCurrentDPTPrior()
{
	return getLastDPTPrior() ? true : false;
}

void RDOParser::insertChanges(CREF(tstring) name, CREF(tstring) value)
{
	m_changes.push_back(Changes(name, value));
}

tstring RDOParser::getChanges() const
{
	rdo::textstream stream;
	stream << _T("$Changes") << std::endl;
	ruint changes_max_length = 0;
	STL_FOR_ALL_CONST(m_changes, change_it)
	{
		if (change_it->m_name.length() > changes_max_length)
		{
			changes_max_length = change_it->m_name.length();
		}
	}
	STL_FOR_ALL_CONST(m_changes, change_it)
	{
		stream << _T("  ") << change_it->m_name;
		for (ruint i = change_it->m_name.length(); i < changes_max_length; i++)
		{
			stream << _T(" ");
		}
		stream << _T("  = ") << change_it->m_value << std::endl;
	}
	return stream.str();
}

tstring RDOParser::getModelStructure()
{
	rdo::textstream modelStructure;

	// $Changes
	modelStructure << getChanges();

	// RTP
	modelStructure << std::endl << std::endl << _T("$Resource_type") << std::endl;
	STL_FOR_ALL_CONST(m_allRTPResType, rtp_it)
	{
		(*rtp_it)->writeModelStructure(modelStructure);
	}

	// RSS
	modelStructure << std::endl << _T("$Resources") << std::endl;
	STL_FOR_ALL_CONST(m_allRSSResource, rss_it)
	{
		(*rss_it)->writeModelStructure(modelStructure);
	}

	// PAT
	modelStructure << std::endl << _T("$Pattern") << std::endl;
	STL_FOR_ALL_CONST(m_allPATPattern, pat_it)
	{
		(*pat_it)->writeModelStructure(modelStructure);
	}

	// OPR/DPT
	ruint counter = 1;
	modelStructure << std::endl << _T("$Activities") << std::endl;
	modelStructure << m_pRuntime->writeActivitiesStructure(counter);

	// DPT only
	for (ruint i = 0; i < m_allDPTSearch.size(); i++)
	{
		for (ruint j = 0; j < m_allDPTSearch.at(i)->getActivities().size(); j++)
		{
			LPRDODPTSearchActivity pSearchActivity = m_allDPTSearch.at(i)->getActivities().at(j);
			ASSERT(pSearchActivity);
			modelStructure << counter++ << _T(" ") << pSearchActivity->name() << _T(" ") << pSearchActivity->pattern()->getPatternId() << std::endl;
		}
	}

	// PMD
	modelStructure << std::endl << _T("$Watching") << std::endl;
	ruint watching_max_length = 0;
	STL_FOR_ALL_CONST(m_pRuntime->getResult(), watching_it)
	{
		LPITrace          trace     = *watching_it;
		LPIName           name      = trace;
		LPIModelStructure structure = trace;
		if (trace && name && structure)
		{
			if (trace->traceable() && name->name().length() > watching_max_length)
			{
				watching_max_length = name->name().length();
			}
		}
	}
	STL_FOR_ALL_CONST(m_pRuntime->getResult(), watching_it)
	{
		LPITrace          trace     = *watching_it;
		LPIName           name      = trace;
		LPIModelStructure structure = trace;
		if (trace && name && structure)
		{
			if (trace->traceable())
			{
				modelStructure << _T("  ") << name->name();
				for (ruint i = name->name().length(); i < watching_max_length + 2; i++)
					modelStructure << _T(" ");

				structure->writeModelStructure(modelStructure);
			}
		}
	}

	return modelStructure.str();
}

void RDOParser::parse()
{
	parse(rdoModelObjects::obPRE);

	RDOParserContainer::Iterator it = begin();
	while (it != end())
	{
		m_parser_item = it->second;
		it->second->parse(this);
		m_parser_item = NULL;
		it++;
	}

	parse(rdoModelObjects::obPOST);
}

void RDOParser::parse(rdoModelObjects::RDOParseType file)
{
	ruint min, max;
	RDOParserContainer::getMinMax(file, min, max);
	if (min == RDOParserContainer::UNDEFINED_ID || max == RDOParserContainer::UNDEFINED_ID)
		return;

	RDOParserContainer::Iterator it = find(min);
	while (it != end())
	{
		if (it->first <= max)
		{
			m_parser_item = it->second;
			it->second->parse(this);
			m_parser_item = NULL;
		}
		else
		{
			break;
		}
		it++;
	}
}

void RDOParser::parse(REF(std::istream) stream)
{
	RDOParserContainer::Iterator it = begin();
	while (it != end())
	{
		m_parser_item = it->second;
		it->second->parse(this, stream);
		m_parser_item = NULL;
		it++;
	}
}

void RDOParser::checkFunctionName(CREF(RDOParserSrcInfo) src_info)
{
	LPRDOFUNConstant pConstant = findFUNConstant(src_info.src_text());
	if (pConstant)
	{
		error().push_only(src_info, rdo::format(_T("��������� '%s' ��� ����������"), src_info.src_text().c_str()));
//		parser->error(_T("Second appearance of the same constant name: ") + *(_cons->getName()));
		error().push_only(pConstant->src_info(), _T("��. ������ �����������"));
		error().push_done();
	}
	LPRDOFUNSequence pSequence = findFUNSequence(src_info.src_text());
	if (pSequence)
	{
		error().push_only(src_info, rdo::format(_T("������������������ '%s' ��� ����������"), src_info.src_text().c_str()));
		error().push_only(pSequence->src_info(), _T("��. ������ �����������"));
		error().push_done();
	}
	LPRDOFUNFunction pFunction = findFUNFunction(src_info.src_text());
	if (pFunction)
	{
		error().push_only(src_info, rdo::format(_T("������� '%s' ��� ����������"), src_info.src_text().c_str()));
		error().push_only(pFunction->src_info(), _T("��. ������ �����������"));
		error().push_done();
	}
}

void RDOParser::checkActivityName(CREF(RDOParserSrcInfo) src_info)
{
	STL_FOR_ALL_CONST(getDPTSearchs(), it_search)
	{
		RDODPTSearch::ActivityList::const_iterator it_search_act = std::find_if((*it_search)->getActivities().begin(), (*it_search)->getActivities().end(), compareName<RDODPTSearchActivity>(src_info.src_text()));
		if (it_search_act != (*it_search)->getActivities().end())
		{
			error().push_only(src_info, rdo::format(_T("���������� '%s' ��� ����������"), src_info.src_text().c_str()));
			error().push_only((*it_search_act)->src_info(), _T("��. ������ �����������"));
			error().push_done();
//			error(_T("Activity name: ") + *_name + _T(" already defined"));
		}
	}
	STL_FOR_ALL_CONST(getDPTSomes(), it_some)
	{
		RDODPTSome::ActivityList::const_iterator it_some_act = std::find_if((*it_some)->getActivities().begin(), (*it_some)->getActivities().end(), compareName<RDODPTSomeActivity>(src_info.src_text()));
		if (it_some_act != (*it_some)->getActivities().end())
		{
			error().push_only(src_info, rdo::format(_T("���������� '%s' ��� ����������"), src_info.src_text().c_str()));
			error().push_only((*it_some_act)->src_info(), _T("��. ������ �����������"));
			error().push_done();
		}
	}
	STL_FOR_ALL_CONST(getDPTPriors(), it_prior)
	{
		RDODPTPrior::ActivityList::const_iterator it_prior_act = std::find_if((*it_prior)->getActivities().begin(), (*it_prior)->getActivities().end(), compareName<RDODPTPriorActivity>(src_info.src_text()));
		if (it_prior_act != (*it_prior)->getActivities().end())
		{
			error().push_only(src_info, rdo::format(_T("���������� '%s' ��� ����������"), src_info.src_text().c_str()));
			error().push_only((*it_prior_act)->src_info(), _T("��. ������ �����������"));
			error().push_done();
		}
	}
}

void RDOParser::checkDPTName(CREF(RDOParserSrcInfo) src_info)
{
	if (src_info.src_text().empty())
	{
		// ��������� ��� �������� � ��������� ������ �����������
		return;
	}
	DPTSearchList::const_iterator search_it = std::find_if(getDPTSearchs().begin(), getDPTSearchs().end(), compareName<RDODPTSearch>(src_info.src_text()));
	if (search_it != getDPTSearchs().end())
	{
		error().push_only(src_info, rdo::format(_T("����� '%s' ��� ����������"), src_info.src_text().c_str()));
		error().push_only((*search_it)->src_info(), _T("��. ������ �����������"));
		error().push_done();
//		error(src_info, _T("DPT name: ") + src_info.src_text() + _T(" already defined"));
	}
	DPTSomeList::const_iterator some_it = std::find_if(getDPTSomes().begin(), getDPTSomes().end(), compareName<RDODPTSome>(src_info.src_text()));
	if (some_it != getDPTSomes().end())
	{
		error().push_only(src_info, rdo::format(_T("����� '%s' ��� ����������"), src_info.src_text().c_str()));
		error().push_only((*some_it)->src_info(), _T("��. ������ �����������"));
		error().push_done();
	}
}

CLOSE_RDO_PARSE_NAMESPACE
