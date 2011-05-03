/*
 * copyright: (c) RDO-Team, 2010
 * filename : array.h
 * author   : ������ ������
 * date     : 03.12.10
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOCALC_ARRAY_H_
#define _RDOCALC_ARRAY_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOCalcArraySize
// ----------------------------------------------------------------------------
CALC(RDOCalcArraySize)
{
DECLARE_FACTORY(RDOCalcArraySize)
private:
	RDOCalcArraySize(CREF(LPRDOCalc) pArray);

	LPRDOCalc m_pArray;

	DECALRE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDOCALC_ARRAY_H_
