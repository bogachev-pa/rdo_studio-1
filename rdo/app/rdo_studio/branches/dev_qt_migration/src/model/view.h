/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/model/view.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_MODEL_VIEW_H_
#define _RDO_STUDIO_MFC_MODEL_VIEW_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/edit/view_base.h"
#include "app/rdo_studio_mfc/src/model/model.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOStudioModelView
// --------------------------------------------------------------------------------
namespace rdoEditor {
	class RDOEditorTabCtrl;
}

class RDOStudioModelView: public RDOStudioEditBaseView
{
public:
	RDOStudioModelView(PTR(QWidget) pParent);
	virtual ~RDOStudioModelView();

	void setModel(PTR(RDOStudioModel) pModel);

	REF(rdoEditor::RDOEditorTabCtrl) getTab();

private:
	typedef  RDOStudioEditBaseView  parent_type;

	PTR(rdoEditor::RDOEditorTabCtrl) m_pTabCtrl;
	PTR(RDOStudioModel)              m_pModel;

	void closeEvent(PTR(QCloseEvent) event);

	afx_msg void OnSearchFindInModel();
	afx_msg LRESULT OnFindInModelMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnUpdateCoordStatusBar          (CCmdUI* pCmdUI);
	afx_msg void OnUpdateModifyStatusBar         (CCmdUI* pCmdUI);
	afx_msg void OnUpdateInsertOverwriteStatusBar(CCmdUI* pCmdUI);
};

#endif // RDOSTUDIOMODELVIEW_H