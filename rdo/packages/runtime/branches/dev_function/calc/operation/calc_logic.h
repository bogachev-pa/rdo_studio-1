/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_logic.h
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \authors   ����� ������� (dluschan@rk9.bmstu.ru)
  \date      13.03.2011
  \brief     ���������� ���������
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_OPERATION_LOGIC_H_
#define _LIB_RUNTIME_CALC_OPERATION_LOGIC_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/operation/calc_binary.h"
#include "simulator/runtime/calc/operation/calc_unary.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! �������� ���������� �
class RDOCalcAnd: public RDOCalcBinary<BinaryOperatorConstP1<RDOValue, &RDOValue::operator&& >, OperatorType::OT_LOGIC>
{
DECLARE_FACTORY(RDOCalcAnd)
private:
	typedef RDOCalcBinary<BinaryOperatorConstP1<RDOValue, &RDOValue::operator&& >, OperatorType::OT_LOGIC> parent_type;

	RDOCalcAnd(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight);

	RDOValue m_value_true;
	RDOValue m_value_false;

	DECLARE_ICalc;
};

//! �������� ���������� ���
class RDOCalcOr: public RDOCalcBinary<BinaryOperatorConstP1<RDOValue, &RDOValue::operator|| >, OperatorType::OT_LOGIC>
{
DECLARE_FACTORY(RDOCalcOr)
private:
	typedef RDOCalcBinary<BinaryOperatorConstP1<RDOValue, &RDOValue::operator|| >, OperatorType::OT_LOGIC> parent_type;

	RDOCalcOr(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight);

	RDOValue m_value_true;
	RDOValue m_value_false;

	DECLARE_ICalc;
};

//! �������� ���������
#ifdef COMPILER_VISUAL_STUDIO
	typedef RDOCalcUnary<rbool, (&RDOValue::operator!), OperatorType::OT_LOGIC> RDOCalcNot;         DECLARE_POINTER(RDOCalcNot);
#endif
#ifdef COMPILER_GCC
	typedef RDOCalcUnary<rbool,  &RDOValue::operator!,  OperatorType::OT_LOGIC> RDOCalcNot;         DECLARE_POINTER(RDOCalcNot);
#endif

//! �������� ���������
#ifdef COMPILER_VISUAL_STUDIO
	typedef RDOCalcBinary<BinaryOperatorConstP1<rbool, (&RDOValue::operator==) >, OperatorType::OT_LOGIC> RDOCalcIsEqual;     DECLARE_POINTER(RDOCalcIsEqual);
#endif
#ifdef COMPILER_GCC
	typedef RDOCalcBinary<BinaryOperatorConstP1<rbool,  &RDOValue::operator== >,  OperatorType::OT_LOGIC> RDOCalcIsEqual;     DECLARE_POINTER(RDOCalcIsEqual);
#endif

//! �������� �����������
#ifdef COMPILER_VISUAL_STUDIO
	typedef RDOCalcBinary<BinaryOperatorConstP1<rbool, (&RDOValue::operator!=) >, OperatorType::OT_LOGIC> RDOCalcIsNotEqual;  DECLARE_POINTER(RDOCalcIsNotEqual);
#endif
#ifdef COMPILER_GCC
	typedef RDOCalcBinary<BinaryOperatorConstP1<rbool,  &RDOValue::operator!= >,  OperatorType::OT_LOGIC> RDOCalcIsNotEqual;  DECLARE_POINTER(RDOCalcIsNotEqual);
#endif

//! �������� ������
#ifdef COMPILER_VISUAL_STUDIO
	typedef RDOCalcBinary<BinaryOperatorConstP1<rbool, (&RDOValue::operator<) >, OperatorType::OT_LOGIC> RDOCalcIsLess;      DECLARE_POINTER(RDOCalcIsLess);
#endif
#ifdef COMPILER_GCC
	typedef RDOCalcBinary<BinaryOperatorConstP1<rbool,  &RDOValue::operator< >,  OperatorType::OT_LOGIC> RDOCalcIsLess;      DECLARE_POINTER(RDOCalcIsLess);
#endif

//! �������� ������
#ifdef COMPILER_VISUAL_STUDIO
	typedef RDOCalcBinary<BinaryOperatorConstP1<rbool, (&RDOValue::operator>) >, OperatorType::OT_LOGIC> RDOCalcIsGreater;   DECLARE_POINTER(RDOCalcIsGreater);
#endif
#ifdef COMPILER_GCC
	typedef RDOCalcBinary<BinaryOperatorConstP1<rbool,  &RDOValue::operator> >,  OperatorType::OT_LOGIC> RDOCalcIsGreater;   DECLARE_POINTER(RDOCalcIsGreater);
#endif

//! �������� ������ ��� �����
#ifdef COMPILER_VISUAL_STUDIO
	typedef RDOCalcBinary<BinaryOperatorConstP1<rbool, (&RDOValue::operator<=) >, OperatorType::OT_LOGIC> RDOCalcIsLEQ;       DECLARE_POINTER(RDOCalcIsLEQ);
#endif
#ifdef COMPILER_GCC
	typedef RDOCalcBinary<BinaryOperatorConstP1<rbool,  &RDOValue::operator<= >,  OperatorType::OT_LOGIC> RDOCalcIsLEQ;       DECLARE_POINTER(RDOCalcIsLEQ);
#endif

//! �������� ������ ��� �����
#ifdef COMPILER_VISUAL_STUDIO
	typedef RDOCalcBinary<BinaryOperatorConstP1<rbool, (&RDOValue::operator>=) >, OperatorType::OT_LOGIC> RDOCalcIsGEQ;       DECLARE_POINTER(RDOCalcIsGEQ);
#endif
#ifdef COMPILER_GCC
	typedef RDOCalcBinary<BinaryOperatorConstP1<rbool,  &RDOValue::operator>= >,  OperatorType::OT_LOGIC> RDOCalcIsGEQ;       DECLARE_POINTER(RDOCalcIsGEQ);
#endif

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/calc/operation/calc_logic.inl"

#endif // _LIB_RUNTIME_CALC_OPERATION_LOGIC_H_