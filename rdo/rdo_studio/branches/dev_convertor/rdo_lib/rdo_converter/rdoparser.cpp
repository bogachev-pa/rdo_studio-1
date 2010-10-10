/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdoparser.cpp
 * author   : �������� ����, ������ ������
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_converter/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_converter/rdoparser.h"
#include "rdo_lib/rdo_converter/rdoparser_rdo.h"
#include "rdo_lib/rdo_converter/rdofun.h"
#include "rdo_lib/rdo_converter/rdorss.h"
#include "rdo_lib/rdo_converter/context/global.h"
#include "rdo_lib/rdo_converter/rdo_common/model_objects_convertor.h"
#include "rdo_common/rdocommon.h"
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- Converter
// ----------------------------------------------------------------------------
Converter::ParserList Converter::s_parserStack;

#define DECLARE_PARSER_OBJECT_CONTAINER_NONAME(NAME) \
void Converter::insert##NAME(LPRDO##NAME value) \
{ \
	howIsIt<LPRDO##NAME>(); \
	m_all##NAME.push_back(value); \
}

#define DECLARE_PARSER_OBJECT_CONTAINER(NAME) \
DECLARE_PARSER_OBJECT_CONTAINER_NONAME(NAME) \
const LPRDO##NAME Converter::find##NAME(CREF(tstring) name) const \
{ \
	NAME##List::const_iterator it = std::find_if(m_all##NAME.begin(), m_all##NAME.end(), compareName<RDO##NAME>(name)); \
	return it != m_all##NAME.end() ? *it : NULL; \
} \
rbool Converter::remove##NAME(const LPRDO##NAME item) \
{ \
	NAME##List::iterator it = std::find(m_all##NAME.begin(), m_all##NAME.end(), item); \
	if (it == m_all##NAME.end()) \
		return false; \
	m_all##NAME.erase(it); \
	return true; \
}

DECLARE_PARSER_OBJECT_CONTAINER(PATPattern     );
DECLARE_PARSER_OBJECT_CONTAINER(RTPResType     );
DECLARE_PARSER_OBJECT_CONTAINER(RSSResource    );
DECLARE_PARSER_OBJECT_CONTAINER(OPROperation   );
DECLARE_PARSER_OBJECT_CONTAINER(FRMFrame       );
DECLARE_PARSER_OBJECT_CONTAINER(FUNConstant    );
DECLARE_PARSER_OBJECT_CONTAINER(FUNFunction    );
DECLARE_PARSER_OBJECT_CONTAINER(FUNSequence    );
DECLARE_PARSER_OBJECT_CONTAINER(DPTSearch      );
DECLARE_PARSER_OBJECT_CONTAINER(DPTSome        );
DECLARE_PARSER_OBJECT_CONTAINER(DPTPrior       );
DECLARE_PARSER_OBJECT_CONTAINER(DPTFreeActivity);
DECLARE_PARSER_OBJECT_CONTAINER(PMDPokaz       );

DECLARE_PARSER_OBJECT_CONTAINER_NONAME(FUNGroup   );
DECLARE_PARSER_OBJECT_CONTAINER_NONAME(DPTFree    );
DECLARE_PARSER_OBJECT_CONTAINER_NONAME(PROCProcess);
DECLARE_PARSER_OBJECT_CONTAINER_NONAME(Operations );

rdoModelObjectsConvertor::RDOFileType Converter::getFileToParse()
{
	return !s_parserStack.empty() && s_parserStack.back()->m_parser_item ? s_parserStack.back()->m_parser_item->m_type : rdoModelObjectsConvertor::PAT;
}

ruint Converter::lexer_loc_line()
{
	return !s_parserStack.empty() && s_parserStack.back()->m_parser_item ? s_parserStack.back()->m_parser_item->lexer_loc_line() : ~0;
}

ruint Converter::lexer_loc_pos()
{
	return !s_parserStack.empty() && s_parserStack.back()->m_parser_item ? s_parserStack.back()->m_parser_item->lexer_loc_pos() : 0;
}

PTR(Converter) Converter::s_converter()
{
	return !s_parserStack.empty() ? s_parserStack.back() : NULL;
}

Converter::Converter()
	: m_parser_item         (NULL )
	, m_have_kw_Resources   (false)
	, m_have_kw_ResourcesEnd(false)
	, m_pattern             (false)
{
	s_parserStack.push_back(this);
	m_runtime.memory_insert(sizeof(Converter));
	m_runtime.init();
	m_contextStack.push(rdo::Factory<ContextGlobal>::create());
}

Converter::~Converter()
{
	m_runtime.deinit();
	rdo::deleteAllObjects(m_allValues);
	m_movementObjectList.clear();
	s_parserStack.remove(this);
}

REF(ContextStack) Converter::contextStack()
{
	return m_contextStack;
}

LPContext Converter::context() const
{
	return m_contextStack.top();
}

rbool Converter::isCurrentDPTSearch()
{
	return getLastDPTSearch() && !getLastDPTSearch()->closed() ? true : false;
}

rbool Converter::isCurrentDPTPrior()
{
	return getLastDPTPrior() ? true : false;
}

void Converter::insertChanges(CREF(tstring) name, CREF(tstring) value)
{
	m_changes.push_back(Changes(name, value));
}

tstring Converter::getChanges() const
{
	rdo::textstream stream;
	stream << _T("$Changes") << std::endl;
	ruint changes_max_length = 0;
	STL_FOR_ALL_CONST(ChangesList, m_changes, change_it)
	{
		if (change_it->m_name.length() > changes_max_length)
		{
			changes_max_length = change_it->m_name.length();
		}
	}
	STL_FOR_ALL_CONST(ChangesList, m_changes, change_it)
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

tstring Converter::getModelStructure()
{
	rdo::textstream modelStructure;

	// $Changes
	modelStructure << getChanges();

	// RTP
	modelStructure << std::endl << std::endl << _T("$Resource_type") << std::endl;
	STL_FOR_ALL_CONST(RTPResTypeList, m_allRTPResType, rtp_it)
	{
		(*rtp_it)->writeModelStructure(modelStructure);
	}

	// RSS
	modelStructure << std::endl << _T("$Resources") << std::endl;
	STL_FOR_ALL_CONST(RSSResourceList, m_allRSSResource, rss_it)
	{
		(*rss_it)->writeModelStructure(modelStructure);
	}

	// PAT
	modelStructure << std::endl << _T("$Pattern") << std::endl;
	STL_FOR_ALL_CONST(PATPatternList, m_allPATPattern, pat_it)
	{
		(*pat_it)->writeModelStructure(modelStructure);
	}

	// OPR/DPT
	ruint counter = 1;
	modelStructure << std::endl << _T("$Activities") << std::endl;
	modelStructure << m_runtime.writeActivitiesStructure(counter);

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
	STL_FOR_ALL_CONST(rdoRuntime::RDORuntime::LPIPokazList, m_runtime.getPokaz(), watching_it)
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
	STL_FOR_ALL_CONST(rdoRuntime::RDORuntime::LPIPokazList, m_runtime.getPokaz(), watching_it)
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

void Converter::parse()
{
	parse(rdoModelObjectsConvertor::obPRE);

	RDOParserContainer::Iterator it = begin();
	while (it != end())
	{
		m_parser_item = it->second;
		it->second->parse(this);
		m_parser_item = NULL;
		it++;
	}

	parse(rdoModelObjectsConvertor::obPOST);
}

void Converter::parse(rdoModelObjectsConvertor::RDOParseType file)
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

void RDOParserSMRInfo::parseSMR(REF(std::istream) smrStream)
{
	RDOParserContainer::Iterator it = begin();
	while (it != end())
	{
		m_parser_item = it->second;
		it->second->parse(this, smrStream);
		m_parser_item = NULL;
		it++;
	}
}

void Converter::parse(REF(std::istream) smrStream)
{
	RDOParserSMRInfo                         smrParser;
	rdoModelObjectsConvertor::RDOSMRFileInfo smrInfo;

	try
	{
		smrInfo.error = false;
		smrParser.parseSMR(smrStream);
	}
	catch (REF(rdoParse::RDOSyntaxException) ex)
	{
		tstring mess = ex.getType() + _T(" : ") + ex.message();
		smrInfo.error = true;
		int i = 1;
	}
	catch (REF(rdoRuntime::RDORuntimeException) ex)
	{
		tstring mess = ex.getType() + _T(" : ") + ex.message();
		smrInfo.error = true;
		int i = 1;
	}

	if (!smrParser.hasSMR())
		return;

	smrInfo.model_name     = smrParser.getSMR()->getFile(_T("Model_name")    );
	smrInfo.resource_file  = smrParser.getSMR()->getFile(_T("Resource_file") );
	smrInfo.frame_file     = smrParser.getSMR()->getFile(_T("Frame_file")    );
	smrInfo.statistic_file = smrParser.getSMR()->getFile(_T("Statistic_file"));
	smrInfo.results_file   = smrParser.getSMR()->getFile(_T("Results_file")  );
	smrInfo.trace_file     = smrParser.getSMR()->getFile(_T("Trace_file")    );
}

void Converter::checkFunctionName(CREF(RDOParserSrcInfo) src_info)
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

void Converter::checkActivityName(CREF(RDOParserSrcInfo) src_info)
{
	STL_FOR_ALL_CONST(DPTSearchList, getDPTSearchs(), it_search)
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
	STL_FOR_ALL_CONST(DPTSomeList, getDPTSomes(), it_some)
	{
		RDODPTSome::ActivityList::const_iterator it_some_act = std::find_if((*it_some)->getActivities().begin(), (*it_some)->getActivities().end(), compareName<RDODPTSomeActivity>(src_info.src_text()));
		if (it_some_act != (*it_some)->getActivities().end())
		{
			error().push_only(src_info, rdo::format(_T("���������� '%s' ��� ����������"), src_info.src_text().c_str()));
			error().push_only((*it_some_act)->src_info(), _T("��. ������ �����������"));
			error().push_done();
		}
	}
	STL_FOR_ALL_CONST(DPTPriorList, getDPTPriors(), it_prior)
	{
		RDODPTPrior::ActivityList::const_iterator it_prior_act = std::find_if((*it_prior)->getActivities().begin(), (*it_prior)->getActivities().end(), compareName<RDODPTPriorActivity>(src_info.src_text()));
		if (it_prior_act != (*it_prior)->getActivities().end())
		{
			error().push_only(src_info, rdo::format(_T("���������� '%s' ��� ����������"), src_info.src_text().c_str()));
			error().push_only((*it_prior_act)->src_info(), _T("��. ������ �����������"));
			error().push_done();
		}
	}
	LPRDODPTFreeActivity pFreeActivity = findDPTFreeActivity(src_info.src_text());
	if (pFreeActivity)
	{
		error().push_only(src_info, rdo::format(_T("���������� '%s' ��� ����������"), src_info.src_text().c_str()));
		error().push_only(pFreeActivity->src_info(), _T("��. ������ �����������"));
		error().push_done();
	}
	LPRDOOPROperation pOperation = findOPROperation(src_info.src_text());
	if (pOperation)
	{
		error().push_only(src_info, rdo::format(_T("�������� '%s' ��� ����������"), src_info.src_text().c_str()));
		error().push_only(pOperation->src_info(), _T("��. ������ �����������"));
		error().push_done();
	}
}

void Converter::checkDPTName(CREF(RDOParserSrcInfo) src_info)
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

CLOSE_RDO_CONVERTER_NAMESPACE
