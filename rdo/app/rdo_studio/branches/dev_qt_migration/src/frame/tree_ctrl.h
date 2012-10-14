/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/frame/tree_ctrl.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      28.03.2003
  \brief     ������ ������ ��������
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_FRAME_TREE_CTRL_H_
#define _RDO_STUDIO_MFC_FRAME_TREE_CTRL_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtGui/qtreewidget.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/help_context_i.h"
// --------------------------------------------------------------------------------

class RDOStudioFrameTreeCtrl
	: public QTreeWidget
	, public IHelpContext
{
public:
	RDOStudioFrameTreeCtrl(PTR(QWidget) pParent);
	virtual ~RDOStudioFrameTreeCtrl();

	PTR(QTreeWidgetItem) insertFrame(CREF(QString) name);

	void clear();

private:
	typedef  QTreeWidget  parent_type;

	PTR(QTreeWidgetItem) m_pRootItem;

	DECLARE_IHelpContext;
};

#endif // _RDO_STUDIO_MFC_FRAME_TREE_CTRL_H_