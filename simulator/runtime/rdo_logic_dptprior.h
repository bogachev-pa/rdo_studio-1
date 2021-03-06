/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_logic_dptprior.h
  \author    Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      04.11.2009
  \brief     DPTPrior
  \indent    4T
*/

#ifndef _LIB_RUNTIME_LOGIC_DPTPRIOR_H_
#define _LIB_RUNTIME_LOGIC_DPTPRIOR_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_logic.h"
#include "simulator/runtime/rdo_priority.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class   RDOOrderDPTPrior
  \brief   Дисциплина очереди для приоритетных логик БЗ
*/
class RDOOrderDPTPrior
{
public:
	static LPIBaseOperation sort(const LPRDORuntime& pRuntime, BaseOperationList& container);
};

/*!
  \class   RDOLogicDPTPrior
  \brief   Логика БЗ для DPTPrior
*/
class RDOLogicDPTPrior: public RDOLogic<RDOOrderDPTPrior>
{
protected:
	RDOLogicDPTPrior(const LPRDORuntime& pRuntime, LPIBaseOperationContainer parent)
		: RDOLogic<RDOOrderDPTPrior>(pRuntime, parent)
	{}
	virtual ~RDOLogicDPTPrior()
	{}
};

/*!
  \class   RDODPTPrior
  \brief   Точка принятия решений DPTPrior
*/
class RDODPTPrior: public RDOLogicDPTPrior, public RDOPatternPrior
{
DECLARE_FACTORY(RDODPTPrior);
private:
	RDODPTPrior(const LPRDORuntime& pRuntime, LPIBaseOperationContainer pParent = NULL);
	virtual ~RDODPTPrior();
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_LOGIC_DPTPRIOR_H_
