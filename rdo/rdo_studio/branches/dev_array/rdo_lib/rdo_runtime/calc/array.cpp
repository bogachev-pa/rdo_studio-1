/*
 * copyright: (c) RDO-Team, 2011
 * filename : array.cpp
 * author   : ������ ������
 * date     : 02.05.11
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_runtime/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/calc/array.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOCalcArraySize
// ----------------------------------------------------------------------------
RDOCalcArraySize::RDOCalcArraySize(CREF(LPRDOCalc) pArray)
	: m_pArray(pArray)
{}

REF(RDOValue) RDOCalcArraySize::doCalc(PTR(RDORuntime) pRuntime)
{
	RDOValue m_value = m_pArray->calcValue(pRuntime);
	//ASSERT(m_value);
	RDOArrayValue arrayValue = m_value.getArray();
	rsint aSize = arrayValue.arraySize();
	m_value = RDOValue(aSize);
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE
