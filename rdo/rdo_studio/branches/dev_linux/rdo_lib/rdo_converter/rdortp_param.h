/*!
  \copyright (c) RDO-Team, 2011
  \file      rdortp_param.h
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _CONVERTOR_RDORTP_PARAM_H_
#define _CONVERTOR_RDORTP_PARAM_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_lib/rdo_converter/param.h"
#include "rdo_lib/rdo_runtime/rdo_model_i.h"
#include "rdo_common/smart_ptr/intrusive_ptr.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDORTPParam
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDORTPResType);

CLASS(RDORTPParam):
	    INSTANCE_OF      (RDOParam       )
	AND IMPLEMENTATION_OF(IModelStructure)
	AND IMPLEMENTATION_OF(IName          )
{
DECLARE_FACTORY(RDORTPParam);
public:
	DECLARE_IModelStructure;
	DECLARE_IName;

private:
	RDORTPParam(CREF(LPRDOTypeParam) pParamType, CREF(RDOValue) default, CREF(RDOParserSrcInfo) src_info);
	virtual ~RDORTPParam();
};
DECLARE_POINTER(RDORTPParam);

CLOSE_RDO_CONVERTER_NAMESPACE

#endif // _CONVERTOR_RDORTP_PARAM_H_
