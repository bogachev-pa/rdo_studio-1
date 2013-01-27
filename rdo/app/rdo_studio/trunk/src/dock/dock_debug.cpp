/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      dock_debug.cpp
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      03.10.2012
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <QtGui/qaction.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/dock/dock_debug.h"
// --------------------------------------------------------------------------------

DockDebug::DockDebug(PTR(QWidget) pParent)
	: DockFocusable("�����", pParent)
{
	PTR(context_type) pWidget = new context_type(this);
	pWidget->setMinimumSize(QSize(300, 150));

	setWidget(pWidget);

	toggleViewAction()->setIcon(QIcon(QString::fromUtf8(":/images/images/dock_debug.png")));
}

DockDebug::~DockDebug()
{}

void DockDebug::appendString(CREF(tstring) str)
{
	getContext().appendLine(str);
}

void DockDebug::clear()
{
	getContext().clearAll();
}

REF(DockDebug::context_type) DockDebug::getContext()
{
	return *static_cast<PTR(context_type)>(widget());
}