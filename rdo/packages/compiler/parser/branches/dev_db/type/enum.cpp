/*!
  \copyright (c) RDO-Team, 2011
  \file      enum.cpp
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/type/enum.h"
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdoparser_error.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOEnumType
// --------------------------------------------------------------------------------
RDOEnumType::RDOEnumType()
	: RuntimeWrapperType(rdo::Factory<rdo::runtime::RDOEnumType>::create())
{
	ASSERT(m_pType);
}

RDOEnumType::RDOEnumType(CREF(rdo::runtime::LPRDOEnumType) pEnumType)
	: RuntimeWrapperType(rdo::runtime::g_unknow)
{
	rdo::runtime::LPRDOEnumType pType = rdo::Factory<rdo::runtime::RDOEnumType>::create();
	m_pType = pType;
	ASSERT(m_pType);

	rdo::runtime::RDOEnumType::Enums::const_iterator it = pEnumType->begin();
	while (it != pEnumType->end())
	{
		pType->add(*it);
	}
}

RDOEnumType::~RDOEnumType()
{}

tstring RDOEnumType::name() const
{
	tstring str = _T("(");
	rdo::runtime::RDOEnumType::const_iterator it = getEnums()->begin();
	while (it != getEnums()->end())
	{
		str += *it;
		++it;
		if (it != getEnums()->end())
		{
			str += _T(", ");
		}
	}
	str += _T(")");
	return str;
}

LPRDOType RDOEnumType::type_cast(CREF(LPRDOType) from, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	switch (from->type()->typeID())
	{
		case rdo::runtime::RDOType__int::t_enum:
		{
			LPRDOEnumType pEnum(const_cast<PTR(RDOEnumType)>(this));
			//! ��� ���� � ��� �� ���
			if (pEnum == from)
				return pEnum;

			//! ���� ������, �������� ������
			if (pEnum.compare(from.object_static_cast<RDOEnumType>()))
			{
				parser::g_error().push_only(src_info,     _T("������������ ��������� ������������ ���� � ����������� ����������"));
				parser::g_error().push_only(to_src_info,   to_src_info.src_text());
				parser::g_error().push_only(src_info,     _T("�"));
				parser::g_error().push_only(from_src_info, from_src_info.src_text());
				parser::g_error().push_only(src_info,     _T("��������, ������� ������������ ������ �� ��� ��� ������������, � ������ ��� such_as �� ����, ����� ��������� ����� ����� ���������� � �����������"));
			}
			else
			{
				parser::g_error().push_only(src_info,     _T("�������������� ������������ �����"));
				parser::g_error().push_only(to_src_info,   to_src_info.src_text());
				parser::g_error().push_only(src_info,     _T("�"));
				parser::g_error().push_only(from_src_info, from_src_info.src_text());
			}
			parser::g_error().push_done();
			break;
		}
		case rdo::runtime::RDOType::t_string       :
		case rdo::runtime::RDOType::t_identificator:
		{
			if (getEnums()->exist(from_src_info.src_text()))
			{
				LPRDOEnumType pEnum(const_cast<PTR(RDOEnumType)>(this));
				return pEnum;
			}
			parser::g_error().error(src_info, rdo::format(_T("�������� '%s' �� �������� ��������� ������������� ���� %s"), from_src_info.src_text().c_str(), to_src_info.src_text().c_str()));
			break;
		}
		default:
			parser::g_error().push_only(src_info,    rdo::format(_T("��������� �������� ������������� ����, �������: %s"), from_src_info.src_text().c_str()));
			parser::g_error().push_only(to_src_info, rdo::format(_T("��. ���: %s"), to_src_info.src_text().c_str()));
			parser::g_error().push_done();
			break;
	}
	return NULL;
}

LPRDOValue RDOEnumType::value_cast(CREF(LPRDOValue) pFrom, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	ASSERT(pFrom);

	LPRDOValue pToValue;
	LPRDOEnumType pEnum(const_cast<PTR(RDOEnumType)>(this));
	try
	{
		switch (pFrom->typeID())
		{
		case rdo::runtime::RDOType::t_identificator:
			if (getEnums()->findEnum(pFrom->value().getIdentificator()) != rdo::runtime::RDOEnumType::END)
			{
				LPTypeInfo pType = rdo::Factory<TypeInfo>::create(pEnum, to_src_info);
				ASSERT(pType);
				pToValue = rdo::Factory<RDOValue>::create(rdo::runtime::RDOValue(getEnums(), pFrom->value().getIdentificator()), pFrom->src_info(), pType);
				ASSERT(pToValue);
			}
			else
			{
				pToValue = RDOValue::getUnknow(pFrom->src_info());
				ASSERT(pToValue);
			}
			break;

		case rdo::runtime::RDOType::t_string:
			if (getEnums()->findEnum(pFrom->value().getAsString()) != rdo::runtime::RDOEnumType::END)
			{
				LPTypeInfo pType = rdo::Factory<TypeInfo>::create(pEnum, to_src_info);
				ASSERT(pType);
				pToValue = rdo::Factory<RDOValue>::create(rdo::runtime::RDOValue(getEnums(), pFrom->value().getAsString()), pFrom->src_info(), pType);
				ASSERT(pToValue);
			}
			else
			{
				pToValue = RDOValue::getUnknow(pFrom->src_info());
				ASSERT(pToValue);
			}
			break;

		case rdo::runtime::RDOType::t_enum:
			if (m_pType == pFrom->typeInfo()->type()->type())
				pToValue = rdo::Factory<RDOValue>::create(pFrom);
			break;

		default:
			break;
		}
	}
	catch (CREF(rdo::runtime::RDOValueException))
	{}

	if (!pToValue || pToValue->typeID() == rdo::runtime::RDOType::t_unknow)
	{
		parser::g_error().push_only(src_info,    rdo::format(_T("�������� �������� ��������� ������������� ����: %s"), pFrom->src_info().src_text().c_str()));
		parser::g_error().push_only(to_src_info, rdo::format(_T("��������� ��������: %s"), name().c_str()));
		parser::g_error().push_done();
	}
	return pToValue;
}

rdo::runtime::LPRDOCalc RDOEnumType::calc_cast(CREF(rdo::runtime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return RuntimeWrapperType::calc_cast(pCalc, pType);
}

rdo::runtime::RDOValue RDOEnumType::get_default() const
{
	return rdo::runtime::RDOValue(getEnums(), 0);
}

void RDOEnumType::writeModelStructure(REF(rdo::ostream) stream) const
{
	stream << "E " << getEnums()->getValues().size() << std::endl;
	for (ruint i = 0; i < getEnums()->getValues().size(); i++)
	{
		stream << "    " << i << " " << getEnums()->getValues().at(i) << std::endl;
	}
}

void RDOEnumType::add(CREF(LPRDOValue) pNext)
{
	ASSERT(pNext);

	if (getEnums()->findEnum(pNext->value().getAsString()) != rdo::runtime::RDOEnumType::END)
	{
		parser::g_error().error(pNext->src_info(), rdo::format(_T("�������� ������������� ���� ��� ����������: %s"), pNext->src_text().c_str()));
	}
	getEnums()->add(pNext->value().getAsString());
}

rdo::runtime::LPRDOEnumType RDOEnumType::getEnums() const
{
	return m_pType.object_static_cast<rdo::runtime::RDOEnumType>();
}

rbool RDOEnumType::operator== (CREF(RDOEnumType) pEnumType) const
{
	return getEnums()->getValues() == pEnumType.getEnums()->getValues();
}

rbool RDOEnumType::operator!= (CREF(RDOEnumType) pEnumType) const
{
	return !operator==(pEnumType);
}

void RDOEnumType::serializeInDB(REF(IDB) db) const
{
	get_default().serializeInDB(db);
	db.insertRow("enum",QString("DEFAULT,%1")
		.arg(boost::any_cast<int>(db.popContext())));
	int enum_id = db.queryExecIndex("enum");

	for (ruint i = 0; i < getEnums()->getValues().size(); i++)
	{
		db.insertRow("enum_valid_value",QString("%1,DEFAULT,'%2',%3")
			.arg(enum_id)
			.arg(QString::fromLocal8Bit(getEnums()->getValues().at(i).c_str()))
			.arg(i));
	}
}

CLOSE_RDO_PARSER_NAMESPACE
