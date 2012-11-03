/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      dock_find.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      04.10.2012
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_DOCK_FIND_H_
#define _RDO_STUDIO_DOCK_FIND_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtGui/qdockwidget.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/dock/dock_base.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdofindedit.h"
// --------------------------------------------------------------------------------

class DockFind: public QDockWidget
{
public:
	typedef rdoEditCtrl::RDOFindEdit context_type;

	DockFind(PTR(QWidget) pParent);
	virtual ~DockFind();

	void appendString(
		CREF(tstring)                str,
		rdoModelObjects::RDOFileType fileType = rdoModelObjects::PAT,
		int lineNumber = -1, int posInLine = 0);
	void clear();

	REF(context_type) getContext();
};

#endif // _RDO_STUDIO_DOCK_FIND_H_
