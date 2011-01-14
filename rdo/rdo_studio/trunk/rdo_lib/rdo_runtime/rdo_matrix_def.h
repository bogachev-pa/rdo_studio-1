/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdo_matrix_def.h
 * author   : ������ ������
 * date     : 01.10.2010
 * bref     : 
 * indent   : 4T
 */
#ifndef _RDO_MATRIX_DEF_H_
#define _RDO_MATRIX_DEF_H_

#ifndef _RDO_VALUE_H_
#error include "rdo_value.h" first (������ "rdo_matrix_def.h" ���� ��������� "rdo_matrix.h")
#endif

//
// �������� ���� ���� ��������� �� ����, ������
// #include "rdo_matrix.h"
//

// ====================================================================== INCLUDES
#include <string>
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo_type.h"
#include "rdo_lib/rdo_runtime/rdo_object.h"
#include "rdo_common/rdocommon.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOMatrixValue
// ----------------------------------------------------------------------------
class RDOMatrixType;

class RDOMatrixValue
{
public:
	typedef std::vector<RDOValue> Container;

	RDOMatrixValue(CREF(RDOMatrixType)  type );
	RDOMatrixValue(CREF(RDOMatrixValue) value);
	~RDOMatrixValue();

	CREF(RDOMatrixType) type() const;

	void insertItem(CREF(RDOValue) pMatrix);
	Container::iterator containerBegin();
	Container::iterator containerEnd();
	void insertItems(Container::iterator itr, Container::iterator itrFst, Container::iterator itrLst);
	void  eraseItems(Container::iterator itrFst, Container::iterator itrLst                         );
	CREF(RDOValue) operator[] (CREF(RDOValue) ind);

	tstring getAsString() const;

private:
	Container          m_container;
	CPTR(RDOMatrixType) m_matrixType;
};

// ----------------------------------------------------------------------------
// ---------- RDOMatrixIterator
// ----------------------------------------------------------------------------
class RDOMatrixIterator
{
public:
	typedef RDOMatrixValue::Container::iterator Iterator;

	RDOMatrixIterator(CREF(RDOMatrixIterator) iterator);
	RDOMatrixIterator(CREF(Iterator)         iterator);

	Iterator getIterator() const;
	Iterator operator+  (rsint num);
	Iterator operator-  (rsint num);
	rbool    operator== (CREF(RDOMatrixIterator) iterator) const;

	RDOValue getValue() const;

private:
	Iterator m_iterator;
};

// ----------------------------------------------------------------------------
// ---------- RDOMatrixType
// ----------------------------------------------------------------------------
class RDOMatrixType: public RDOType, public RDORuntimeObject
{
public:
	typedef CREF(RDOType) MatrixType;

	RDOMatrixType(PTR(RDORuntimeParent) parent, MatrixType pMatrixType);

	MatrixType getMatrixType() const;

private:
	MatrixType  m_pMatrixType;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDO_MATRIX_DEF_H_
