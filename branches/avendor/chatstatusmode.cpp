#include "stdafx.h"
#include "chatstatusmode.h"
#include "resource.h"

// ----------------------------------------------------------------------------
// ---------- CChatStatusMode
// ----------------------------------------------------------------------------
CChatStatusMode::CChatStatusMode():
	type( CSMT_Online ),
	info( "" )
{
	name.LoadString( IDS_STATUSMODE_ONLINE );
}

CChatStatusMode::CChatStatusMode( const CChatStatusMode& statusMode )
{
	*this = statusMode;
}

CChatStatusMode::~CChatStatusMode()
{
}

CChatStatusMode& CChatStatusMode::operator= ( const CChatStatusMode& statusMode )
{
	type = statusMode.type;
	name = statusMode.name;
	info = statusMode.info;
	return *this;
}

bool CChatStatusMode::operator== ( const CChatStatusMode& statusMode )
{
	return type == statusMode.type &&
	       name == statusMode.name &&
	       info == statusMode.info;
}

bool CChatStatusMode::operator!= ( const CChatStatusMode& statusMode )
{
	return !( *this == statusMode );
}

// ----------------------------------------------------------------------------
// ---------- CChatStatusModeList
// ----------------------------------------------------------------------------
CChatStatusModeList::CChatStatusModeList(): CList< CChatStatusMode*, CChatStatusMode* >()
{
}

CChatStatusModeList::~CChatStatusModeList()
{
	while ( POSITION pos = GetTailPosition() ) {
		delete GetAt( pos );
		RemoveTail();
	}
}

void CChatStatusModeList::init()
{
	CWinApp* app = AfxGetApp();
	CChatStatusMode* statusMode;
	statusMode = new CChatStatusMode();
	statusMode->type = CSMT_Online;
	statusMode->name = app->GetProfileString( "StatusMode\\Online", "name", getDefaultName( CSMT_Online ) );
	statusMode->info = app->GetProfileString( "StatusMode\\Online", "info", "" );
	AddTail( statusMode );
	statusMode = new CChatStatusMode();
	statusMode->type = CSMT_Away;
	statusMode->name = app->GetProfileString( "StatusMode\\Away", "name", getDefaultName( CSMT_Away ) );
	statusMode->info = app->GetProfileString( "StatusMode\\Away", "info", "" );
	AddTail( statusMode );
	statusMode = new CChatStatusMode();
	statusMode->type = CSMT_NotAvailible;
	statusMode->name = app->GetProfileString( "StatusMode\\NotAvailible", "name", getDefaultName( CSMT_NotAvailible ) );
	statusMode->info = app->GetProfileString( "StatusMode\\NotAvailible", "info", "" );
	AddTail( statusMode );
}

void CChatStatusModeList::saveSetting() const
{
	CWinApp* app = AfxGetApp();
	CChatStatusMode* statusMode;
	statusMode = getStatusMode( CSMT_Online );
	app->WriteProfileString( "StatusMode\\Online", "name", statusMode->name );
	app->WriteProfileString( "StatusMode\\Online", "info", statusMode->info );
	statusMode = getStatusMode( CSMT_Away );
	app->WriteProfileString( "StatusMode\\Away", "name", statusMode->name );
	app->WriteProfileString( "StatusMode\\Away", "info", statusMode->info );
	statusMode = getStatusMode( CSMT_NotAvailible );
	app->WriteProfileString( "StatusMode\\NotAvailible", "name", statusMode->name );
	app->WriteProfileString( "StatusMode\\NotAvailible", "info", statusMode->info );
}

CChatStatusMode* CChatStatusModeList::getStatusMode( const CChatStatusModeType statusModeType ) const
{
	POSITION pos = GetHeadPosition();
	for ( int i = 0; i < GetCount(); i++ ) {
		if ( GetAt( pos )->type == statusModeType ) {
			return GetAt( pos );
		}
		GetNext( pos );
	}
	return NULL;
}

CString CChatStatusModeList::getDefaultName( const CChatStatusModeType statusModeType ) const
{
	CString s;
	int res;
	switch ( statusModeType ) {
		case CSMT_Online       : res = IDS_STATUSMODE_ONLINE; break;
		case CSMT_Away         : res = IDS_STATUSMODE_AWAY; break;
		case CSMT_NotAvailible : res = IDS_STATUSMODE_NOTAVAILIBLE; break;
		default                : res = -1;
	}
	if ( res != -1 ) {
		s.LoadString( res );
	} else {
		s = "";
	}
	return s;
}

// ----------------------------------------------------------------------------
// ---------- CChatStatusModeDialog
// ----------------------------------------------------------------------------
CChatStatusModeDialog::CChatStatusModeDialog( UINT nIDTemplate, CWnd* pParentWnd ):
	CDialog( nIDTemplate, pParentWnd ),
	info( "" ),
	useAsDefault( 0 )
{
}

CChatStatusModeDialog::~CChatStatusModeDialog()
{
}

void CChatStatusModeDialog::DoDataExchange( CDataExchange* pDX )
{
	CDialog::DoDataExchange( pDX );

	DDX_Text( pDX, IDC_INFO_EDIT, info );
	DDX_Check( pDX, IDC_INFO_CHECK, useAsDefault );
}

int CChatStatusModeDialog::DoModal()
{
	return CDialog::DoModal();
}
