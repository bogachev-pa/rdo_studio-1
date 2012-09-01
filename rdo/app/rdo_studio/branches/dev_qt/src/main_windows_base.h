/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      main_windows_base.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      01.09.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MAIN_WINDOWS_BASE_H_
#define _RDO_STUDIO_MAIN_WINDOWS_BASE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/workspace.h"
#include "app/rdo_studio_mfc/src/output.h"
#include "app/rdo_studio_mfc/rdo_edit/rdoeditoreditstyle.h"
#include "app/rdo_studio_mfc/rdo_edit/rdoeditorresultsstyle.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdobuildeditstyle.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdobaseeditstyle.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdofindeditstyle.h"
#include "app/rdo_studio_mfc/rdo_tracer/tracer_ctrls/rdotracerlogstyle.h"
#include "app/rdo_studio_mfc/src/style.h"
#include "app/rdo_studio_mfc/src/chart/view_style.h"
#include "app/rdo_studio_mfc/src/frame/style.h"
// --------------------------------------------------------------------------------

class MainWindowBase
{
public:
	RDOStudioWorkspace workspace;
	RDOStudioOutput    output;
	PTR(CControlBar)   m_pLastDocked;

	rdoEditor::RDOEditorEditStyle    style_editor;
	rdoEditCtrl::RDOBuildEditStyle   style_build;
	rdoEditCtrl::RDOBaseEditStyle    style_debug;
	rdoTracerLog::RDOTracerLogStyle  style_trace;
	rdoEditor::RDOEditorResultsStyle style_results;
	rdoEditCtrl::RDOFindEditStyle    style_find;
	RDOStudioFrameStyle              style_frame;
	RDOStudioChartViewStyle          style_chart;

	rbool isMDIMaximazed() const { return true; }

	virtual void updateAllStyles() const = 0;
	virtual void showWorkspace  () = 0;
	virtual void showOutput     () = 0;
};

#endif // _RDO_STUDIO_MAIN_WINDOWS_BASE_H_
