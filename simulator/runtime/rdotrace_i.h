/*!
  \copyright (c) RDO-Team, 2011
  \file      rdotrace_i.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.07.2009
  \brief     Интерфейс ITrace
  \indent    4T
*/

#ifndef _LIB_RUNTIME_TRACE_I_H_
#define _LIB_RUNTIME_TRACE_I_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/interface/rdointerface.h"
// --------------------------------------------------------------------------------

/*!
  \interface ITrace
  \brief     Интерфейс ITrace
*/
class ITrace
{
public:
	virtual bool traceable() const = 0;
	virtual void setTrace (bool trace) = 0;

	virtual ruint getTraceID() const = 0;
	virtual void setTraceID(ruint id) = 0;
	virtual void setTraceID(ruint id, ruint str_id) = 0;

	virtual REF(std::string) traceId() const = 0;
};

#define DECLARE_ITrace                               \
	virtual bool traceable() const;                  \
	virtual void setTrace(bool trace);               \
	virtual ruint getTraceID() const;                \
	virtual void setTraceID(ruint id);               \
	virtual void setTraceID(ruint id, ruint str_id); \
	virtual REF(std::string) traceId() const ;

/*!
  \interface IResultTraceValue
  \brief     Интерфейс IResultTraceValue
*/
class IResultTraceValue
{
public:
	virtual std::string traceValue() const = 0;
};

#define DECLARE_IResultTraceValue \
	virtual std::string traceValue() const;

/*!
  \interface IResultTrace
  \brief     Интерфейс IResultTrace
*/
class IResultTrace
{
public:
	virtual void traceResult() = 0;
};

#define DECLARE_IResultTrace \
	virtual void traceResult();

#endif // _LIB_RUNTIME_TRACE_I_H_
