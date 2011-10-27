// rdoprocess_shape_resource.h: interface for the RPShapeResource_MJ class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RDOPROCESS_SHAPE_RESOURCE_H__108AB1F9_8742_446F_9CB2_ABD461A09C12__INCLUDED_)
#define AFX_RDOPROCESS_SHAPE_RESOURCE_H__108AB1F9_8742_446F_9CB2_ABD461A09C12__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape.h"

class RPShapeResource_MJ : public RPShape_MJ
{
friend class RPMethodProc2RDO_MJ;

private:
	static RPObject* newObject( RPObject* parent );

public:
	RPShapeResource_MJ( RPObject* parent );
	virtual ~RPShapeResource_MJ();

	virtual rp::string getClassName() const { return "RPShapeResource_MJ"; }

	virtual void onLButtonDblClk( UINT nFlags, CPoint global_chart_pos );
	virtual void generate();

	// ���������������� ����������� ������� save_To_XML ��� RPShapeResource_MJ:
	void save_To_XML(pugi::xml_document &doc, std::list< RPObject* >& all_child);

	int gamount;
};

#endif // !defined(AFX_RDOPROCESS_SHAPE_RESOURCE_H__108AB1F9_8742_446F_9CB2_ABD461A09C12__INCLUDED_)
