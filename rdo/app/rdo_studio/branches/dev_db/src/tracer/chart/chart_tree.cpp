/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      chart_tree.cpp
  \author    ������� �����
  \date      12.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include <QtCore/qprocess.h>
#include <QtCore/qtextstream.h>
#include <QtWidgets/qfiledialog.h>
#include <QtGui/qdrag.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/main_frm.h"
#include "app/rdo_studio/src/model/model.h"
#include "app/rdo_studio/src/tracer/tracer.h"
#include "app/rdo_studio/src/tracer/chart/chart_tree.h"
#include "app/rdo_studio/src/tracer/chart/chart_serie.h"
#include "app/rdo_studio/src/tracer/tracer_resource_type.h"
#include "app/rdo_studio/src/tracer/tracer_resource.h"
#include "app/rdo_studio/src/tracer/tracer_pattern.h"
#include "app/rdo_studio/src/tracer/tracer_operation.h"
#include "app/rdo_studio/src/tracer/tracer_result.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::tracer;

//! @todo qt
//BEGIN_MESSAGE_MAP(ChartTree, RDOTreeCtrl)
//	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnDragDrop)
//END_MESSAGE_MAP()

Q_DECLARE_METATYPE(const ChartTreeItem*);

ChartTree::ChartTree(PTR(QWidget) pParent)
	: parent_type(pParent)
{
	setColumnCount    (1);
	setHeaderHidden   (true);
	setRootIsDecorated(false);

	m_iconList.reserve(IT_COUNT);
	m_iconList.push_back(QIcon(QString::fromUtf8(":/images/images/tree_chart_root.png")));
	m_iconList.push_back(QIcon(QString::fromUtf8(":/images/images/tree_chart_sub_root_1.png")));
	m_iconList.push_back(QIcon(QString::fromUtf8(":/images/images/tree_chart_sub_root_2.png")));
	m_iconList.push_back(QIcon(QString::fromUtf8(":/images/images/tree_chart_sub_root_3.png")));
	m_iconList.push_back(QIcon(QString::fromUtf8(":/images/images/tree_chart_value.png")));
	m_iconList.push_back(QIcon(QString::fromUtf8(":/images/images/tree_chart_erased.png")));

	m_root    = rdo::Factory<ChartTreeItem>::create();
	m_rootRTP = rdo::Factory<ChartTreeItem>::create();
	m_rootPAT = rdo::Factory<ChartTreeItem>::create();
	m_rootPMV = rdo::Factory<ChartTreeItem>::create();

	m_root->setCtrlItem(new QTreeWidgetItem(this));
	m_root->getCtrlItem().setText(0, QString::fromStdWString(L"������"));
	m_root->getCtrlItem().setIcon(0, m_iconList[IT_ROOT]);

	createItem(m_root, m_rootRTP, QString::fromStdWString(L"���� ��������"), IT_SUB_ROOT_1);
	createItem(m_root, m_rootPAT, QString::fromStdWString(L"�������"),       IT_SUB_ROOT_1);
	createItem(m_root, m_rootPMV, QString::fromStdWString(L"����������"),    IT_SUB_ROOT_1);

	m_root->getCtrlItem().setExpanded(true);

	connect(
		this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),
		this, SLOT(onTreeWidgetItemDoubleClicked(QTreeWidgetItem*, int))
	);

	Ui::MainWindow* pMainWindow = g_pApp->getMainWndUI();
	ASSERT(pMainWindow);

	m_pPopupMenu = new QMenu(pParent);
	m_pPopupMenu->addAction(pMainWindow->actChartFindInChart);
	m_pPopupMenu->addAction(pMainWindow->actChartAddToNewChart);
	m_pPopupMenu->addAction(pMainWindow->actChartExport);
}

ChartTree::~ChartTree()
{}

LPChartTreeItem ChartTree::getIfItemIsDrawable(CPTR(QTreeWidgetItem) pCtrlItem) const
{
	LPChartTreeItem pRes;
	if (pCtrlItem)
	{
		PTR(ChartTreeItem) pItem = const_cast<PTR(ChartTreeItem)>(pCtrlItem->data(0, Qt::UserRole).value<CPTR(ChartTreeItem)>());
		pRes = pItem && pItem->isDrawable()
			? pItem
			: NULL;
	}
	return pRes;
}

void ChartTree::doDragDrop(CREF(LPSerie) pSerie)
{
	quintptr address=(quintptr)pSerie.get();
	QByteArray serieData(QString::number(address).toLatin1());

	QDrag *drag = new QDrag(this);
	QMimeData *mimeData = new QMimeData;

	mimeData->setData("ChartSerie", serieData);
	drag->setMimeData(mimeData);
	drag->exec();
}

void ChartTree::setModelName(CREF(QString) modelName)
{
	m_root->getCtrlItem().setText(0, QString::fromLocal8Bit("������ : %1").arg(modelName));
}

void ChartTree::createItem(CREF(LPChartTreeItem) parent, CREF(LPChartTreeItem) item, CREF(QString) name, IconType iconType)
{
	PTR(QTreeWidgetItem) pCtrlItem = new QTreeWidgetItem(&parent->getCtrlItem());
	pCtrlItem->setText(0, name);
	pCtrlItem->setIcon(0, m_iconList[iconType]);
	//! @todo smart_ptr
	const ChartTreeItem* pRawItem = item.get();
	pCtrlItem->setData(0, Qt::UserRole, QVariant::fromValue(pRawItem));
	item->setCtrlItem(pCtrlItem);
}

void ChartTree::addResourceType(CREF(LPResourceType) pRTP)
{
	ASSERT(pRTP);
	createItem(m_rootRTP, pRTP, pRTP->getName(), IT_SUB_ROOT_2);
}

void ChartTree::addResource(CREF(LPResource) pRSS)
{
	LPResourceType pRTP = pRSS->getType();
	ASSERT(pRTP);
	ASSERT(pRSS);
	createItem(pRTP, pRSS, pRSS->getName(), IT_SUB_ROOT_3);

	int count = pRTP->getParamsCount();
	for (int i = 0; i < count; i++)
	{
		LPChartTreeItem pParam = pRSS->getParam(i);
		ASSERT(pParam);
		createItem(pRSS, pParam, pRTP->getParamInfo(i)->getName(), IT_VALUE);
	}
	updateResource(pRSS);
}

void ChartTree::updateResource(CREF(LPResource) pRSS)
{
	if (pRSS->isErased())
	{
		pRSS->getCtrlItem().setIcon(0, m_iconList[IT_ERASED]);
	}
	else
	{
		pRSS->getCtrlItem().setIcon(0, m_iconList[IT_SUB_ROOT_3]);
	}
}

void ChartTree::addPattern(CREF(LPPattern) pPAT)
{
	ASSERT(pPAT);
	createItem(m_rootPAT, pPAT, pPAT->getName(), IT_SUB_ROOT_2);
}

void ChartTree::addOperation(CREF(LPOperationBase) pOPR)
{
	createItem(pOPR->getPattern(), pOPR, pOPR->getName(), IT_VALUE);
}

void ChartTree::addResult(CREF(LPResult) pPMV)
{
	createItem(m_rootPMV, pPMV, pPMV->getName(), IT_VALUE);
}

void ChartTree::deleteChildren(CREF(LPChartTreeItem) pParent)
{
	QList<PTR(QTreeWidgetItem)> children = pParent->getCtrlItem().takeChildren();
	BOOST_FOREACH(PTR(QTreeWidgetItem) item, children)
	{
		pParent->getCtrlItem().removeChild(item);
	}
}

void ChartTree::clear()
{
	deleteChildren(m_rootRTP);
	deleteChildren(m_rootPAT);
	deleteChildren(m_rootPMV);
	m_root->getCtrlItem().setText(0, QString::fromStdWString(L"������"));
}

void ChartTree::addToNewChart(PTR(QTreeWidgetItem) pCtrlItem) const
{
	LPSerie pSerie = getIfItemIsDrawable(pCtrlItem).object_dynamic_cast<Serie>();
	if (pSerie)
	{
		g_pTracer->addSerieToChart(pSerie);
	}
}

rbool ChartTree::findInCharts(PTR(QTreeWidgetItem) pCtrlItem) const
{
	LPSerie pSerie = getIfItemIsDrawable(pCtrlItem).object_dynamic_cast<Serie>();
	if (pSerie)
	{
		return pSerie->activateFirstDoc();
	}
	return false;
}

PTR(QTreeWidgetItem) ChartTree::getSelected() const
{
	QList<PTR(QTreeWidgetItem)> selected = selectedItems();
	return selected.size() == 1
		? selected.front()
		: NULL;
}

void ChartTree::onAddToNewChart()
{
	addToNewChart(getSelected());
}

void ChartTree::onTreeWidgetItemDoubleClicked(QTreeWidgetItem* pCtrlItem, int)
{
	if (!g_pTracer->getDrawTrace())
		return;

	if (!findInCharts(pCtrlItem))
	{
		addToNewChart(pCtrlItem);
	}
}

void ChartTree::onChartExport()
{
	if (!g_pTracer->getDrawTrace())
		return;

	LPChartTreeItem pItem = getIfItemIsDrawable(getSelected());
	if (!pItem)
		return;

	LPSerie pSerie = pItem.object_dynamic_cast<Serie>();
	ASSERT(pSerie);
	Serie::ExportData exportData = pSerie->exportData();
	if (exportData.empty())
		return;

	boost::filesystem::path path =
		boost::filesystem::path(g_pModel->getFullName().toLocal8Bit().constData()).parent_path() /
		QString("%1.csv").arg(pSerie->getTitle()).toLocal8Bit().constData();

	QString fileName = QFileDialog::getSaveFileName(
		this,
		QString::fromStdWString(L"���������"),
		QString::fromLocal8Bit (path.string().c_str()),
		QString::fromStdWString(L"csv-���� (*.csv);;��� ����� (*.*)")
	);
	if (fileName.isEmpty())
		return;

	QFile data(fileName);
	if (data.open(QIODevice::Text | QFile::WriteOnly | QFile::Truncate)) 
	{
		QTextStream stream(&data);
		BOOST_FOREACH(CREF(Serie::ExportData::value_type) exportItem, exportData)
		{
			stream << exportItem << endl;
		}
		data.close();
	}
}

rbool ChartTree::onUpdateChartFindInCharts()
{
	rbool enable = false;
	if (g_pTracer->getDrawTrace())
	{
		LPSerie pSerie = getIfItemIsDrawable(getSelected()).object_dynamic_cast<Serie>();
		if (pSerie)
		{
			enable = pSerie->isInOneOrMoreDocs();
		}
	}
	return enable;
}

void ChartTree::onChartFindInCharts()
{
	findInCharts(getSelected());
}

void ChartTree::focusInEvent(QFocusEvent* pEvent)
{
	parent_type::focusInEvent(pEvent);
	activate(pEvent);
}

void ChartTree::focusOutEvent(QFocusEvent* pEvent)
{
	parent_type::focusOutEvent(pEvent);
	deactivate(pEvent);
}

void ChartTree::onUpdateActions(rbool activated)
{
	RDOStudioMainFrame* pMainWindow = g_pApp->getMainWndUI();
	ASSERT(pMainWindow);

	updateAction(
		pMainWindow->actHelpContext,
		activated,
		this, &ChartTree::onHelpContext
	);

	updateAction(
		pMainWindow->actChartAddToNewChart,
		activated && g_pTracer->getDrawTrace() && getIfItemIsDrawable(getSelected()),
		this, &ChartTree::onAddToNewChart
	);

	updateAction(
		pMainWindow->actChartFindInChart,
		activated && onUpdateChartFindInCharts(),
		this, &ChartTree::onChartFindInCharts
	);

	updateAction(
		pMainWindow->actChartExport,
		activated && g_pTracer->getDrawTrace() && (getIfItemIsDrawable(getSelected()) != NULL),
		this, &ChartTree::onChartExport
	);
}

void ChartTree::onHelpContext()
{
	QByteArray ba;
	ba.append("setSource qthelp://studio/doc/rdo_studio_rus/html/work_model/work_model_chart.htm\n");
	g_pApp->callQtAssistant(ba);
}

void ChartTree::mousePressEvent(QMouseEvent* pEvent)
{
	QTreeWidgetItem* item = itemAt(pEvent->pos());
	this->setCurrentItem(item);
	onUpdateActions(isActivated());
	if (pEvent->button() == Qt::LeftButton)
	{ 
		LPSerie pSerie = getIfItemIsDrawable(getSelected()).object_dynamic_cast<Serie>();
		if(pSerie)
		{
			doDragDrop(pSerie);
		}
		parent_type::mousePressEvent(pEvent);
	}
	else if (pEvent->button() == Qt::RightButton)
	{
		m_pPopupMenu->exec(pEvent->globalPos());
	}
}