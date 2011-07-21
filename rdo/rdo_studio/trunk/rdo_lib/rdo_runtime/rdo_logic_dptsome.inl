/******************************************************************************//**
 * @copyright (c) RDO-Team, 2009
 * @file      rdo_logic_dptsome.inl
 * @authors   ����� �������
 * @date      04.11.09
 * @brief     unknown
 * @indent    4T
 *********************************************************************************/

// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

// ********************************************************************************
// ******************** RDODPTSome
// ********************************************************************************
inline RDODPTSome::RDODPTSome(CREF(LPRDORuntime) pRuntime, LPIBaseOperationContainer parent)
	: RDOLogicSimple(pRuntime, parent)
{
	pRuntime->getFreeDPTId();
}

inline RDODPTSome::~RDODPTSome()
{}

CLOSE_RDO_RUNTIME_NAMESPACE
