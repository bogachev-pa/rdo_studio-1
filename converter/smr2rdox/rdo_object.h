/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_object.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _CONVERTOR_RDOCONVERTER_OBJECT_H_
#define _CONVERTOR_RDOCONVERTER_OBJECT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/namespace.h"
#include "converter/smr2rdox/grammar/rdobison.h"
#include "simulator/runtime/rdo_object.h"
#include "utils/src/common/rdomacros.h"
#include "utils/src/common/rdotypes.h"
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOParserSrcInfo
// --------------------------------------------------------------------------------
class RDOParserSrcInfo: public rdo::runtime::RDOSrcInfo
{
public:
	RDOParserSrcInfo();
	RDOParserSrcInfo(CREF(YYLTYPE) pos);
	RDOParserSrcInfo(CREF(rdo::runtime::RDOSrcInfo) info);
	RDOParserSrcInfo(CREF(rdo::runtime::RDOSrcInfo::Position) pos);
	explicit RDOParserSrcInfo(CREF(std::string) text);
	RDOParserSrcInfo(CREF(YYLTYPE) pos, CREF(std::string) text);
	RDOParserSrcInfo(CREF(YYLTYPE) pos_begin, CREF(YYLTYPE) pos_end, bool first_align = false);
	RDOParserSrcInfo(CREF(YYLTYPE) pos_begin, CREF(YYLTYPE) pos_end, CREF(std::string) text);

	virtual void setSrcInfo(CREF(RDOParserSrcInfo) info)
	{
		setSrcPos     (info.src_pos()     );
		setSrcText    (info.src_text()    );
		setSrcFileType(info.src_filetype());
	}
	void setSrcInfo(CREF(RDOParserSrcInfo) begin, CREF(std::string) delim, CREF(RDOParserSrcInfo) end)
	{
		RDOParserSrcInfo src_info;
		src_info.setSrcPos     (begin.src_pos().m_first_line, begin.src_pos().m_first_pos, end.src_pos().m_last_line, end.src_pos().m_last_pos);
		src_info.setSrcText    (begin.src_text() + delim + end.src_text());
		src_info.setSrcFileType(begin.src_filetype());
		setSrcInfo(src_info);
	}
	virtual void setSrcPos(CREF(RDOSrcInfo::Position) pos)
	{
		RDOSrcInfo::setSrcPos(pos);
	}
	void setSrcPos(CREF(YYLTYPE) pos)
	{
		setSrcPos(Position(pos.m_first_line, pos.m_first_pos, pos.m_last_line, pos.m_last_pos));
	}
	void setSrcPos(CREF(YYLTYPE) pos_begin, CREF(YYLTYPE) pos_end)
	{
		setSrcPos(Position(pos_begin.m_first_line, pos_begin.m_first_pos, pos_end.m_last_line, pos_end.m_last_pos));
	}
	void setSrcPos(ruint first_line, ruint first_pos, ruint last_line, ruint last_pos)
	{
		setSrcPos(Position(first_line, first_pos, last_line, last_pos));
	}
	YYLTYPE getPosAsYY() const
	{
		YYLTYPE  pos1;
		Position pos2 = src_pos();
		pos1.m_first_line = pos2.m_first_line;
		pos1.m_first_pos  = pos2.m_first_pos;
		pos1.m_last_line  = pos2.m_last_line;
		pos1.m_last_pos   = pos2.m_last_pos;
		pos1.m_first_seek = ruint(Position::UNDEFINE_POS);
		pos1.m_last_seek  = ruint(Position::UNDEFINE_POS);
		return pos1;
	}
	static ruint getPosByLength(ruint pos, CREF(std::string) text)
	{
		return pos + text.length();
	}

private:
	void init();
};

template <class T>
class compareName
{
public:
	compareName(CREF(compareName<T>) obj)
		: m_name(obj.m_name)
	{}
	compareName(CREF(std::string) name)
		: m_name(name)
	{}
	REF(compareName<T>) operator= (CREF(compareName<T>) obj)
	{
		m_name = obj.m_name;
		return *this;
	}
	bool operator() (CPTR(T) pObj)
	{
		return pObj->name() == m_name;
	}
	bool operator() (CREF(rdo::intrusive_ptr<T>) pObj)
	{
		return pObj->name() == m_name;
	}

private:
	CREF(std::string) m_name;
};

template <class T>
class compareNameRef
{
public:
	compareNameRef(CREF(compareNameRef<T>) obj)
		: m_name(obj.m_name)
	{}
	compareNameRef(CREF(std::string) name)
		: m_name(name)
	{}
	REF(compareNameRef<T>) operator= (CREF(compareNameRef<T>) obj)
	{
		m_name = obj.m_name;
		return *this;
	}
	bool operator() (CREF(T) obj)
	{
		return obj.name() == m_name;
	}

private:
	CREF(std::string) m_name;
};

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE

#endif // _CONVERTOR_RDOCONVERTER_OBJECT_H_
