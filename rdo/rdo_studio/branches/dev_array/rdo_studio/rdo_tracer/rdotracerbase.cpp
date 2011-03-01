#include "rdo_studio/stdafx.h"
#include "rdo_studio/rdo_tracer/rdotracerbase.h"
#include "rdo_studio/rdo_tracer/rdotracerrestype.h"
#include "rdo_studio/rdo_tracer/rdotracerresource.h"
#include "rdo_studio/rdo_tracer/rdotracerpattern.h"
#include "rdo_studio/rdo_tracer/rdotraceroperation.h"
#include "rdo_studio/rdo_tracer/rdotracerresult.h"
#include "rdo_studio/rdo_tracer/rdotracervalues.h"
#include "rdo_studio/rdo_tracer/tracer_ctrls/rdotracerlogctrl.h"
#include "rdo_studio/rdo_tracer/tracer_ctrls/rdotracertreectrl.h"
#include "rdo_studio/rdostudiochartdoc.h"
#include "rdo_studio/rdostudiochartview.h"
#include "rdo_studio/rdostudioapp.h"
#include "rdo_studio/rdostudiochildfrm.h"
#include "rdo_studio/rdostudiomainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoTracer;
using namespace rdoTracerLog;

// ----------------------------------------------------------------------------
// ---------- RDOTracerBase
// ----------------------------------------------------------------------------
RDOTracerBase::RDOTracerBase( const std::string& _thread_name, RDOKernelGUI* _kernel_gui ):
	RDOThreadGUI( _thread_name, _kernel_gui ),
	log( NULL ),
	tree( NULL ),
	clipboardFormat( 0 ),
	chartDocTemplate( NULL ),
	eventIndex( 0 ),
	drawTrace( true ),
	action( RUA_NONE )
{
}

RDOTracerBase::~RDOTracerBase()
{
	deleteTrace();
}

RDOTracerResParamInfo* RDOTracerBase::getParamType( rdo::textstream& stream )
{
	std::string            par_type;
	RDOTracerResParamType  parType;
	ruint                  enum_count = 0;
	stream >> par_type;
	if ( par_type == "E" )
	{
		parType = RDOPT_ENUMERATIVE;
		stream >> enum_count;
	}
	if ( par_type == "I" )
	{
		parType = RDOPT_INTEGER;
	}
	else if ( par_type == "R" )
	{
		parType = RDOPT_REAL;
	}
	else if ( par_type == "A" )
	{
		parType = RDOPT_ARRAY;
	}
	RDOTracerResParamInfo* param = new RDOTracerResParamInfo( parType );
	if ( parType == RDOPT_ENUMERATIVE )
	{
		std::string en_name;
		for ( ruint j = 0; j < enum_count; j++ )
		{
			stream >> en_name;
			stream >> en_name;
			param->addEnumValue( en_name );
		}
	}
	else if ( parType == RDOPT_ARRAY )
	{
		RDOTracerResParamInfo* arrayItem = getParamType(stream);
		int i = 1;
	}
	return param;
}

RDOTracerResParamInfo* RDOTracerBase::getParam( rdo::textstream& stream )
{
	std::string par_type;
	std::string par_name;
	stream >> par_type;
	stream >> par_name;
	RDOTracerResParamInfo* param = getParamType(stream);
	param->Name = par_name;
	return param;
}

void RDOTracerBase::addResourceType( std::string& s, rdo::textstream& stream )
{
	RDOTracerResType* type = new RDOTracerResType( RDOTK_PERMANENT );
	stream >> type->Name;
	int paramcount;
	stream >> paramcount;
	for ( int i = 0; i < paramcount; i++ )
	{
		type->addParamInfo( getParam(stream) );
	}
	resTypes.push_back( type );
	tree->addResourceType( type );
}

void RDOTracerBase::addResource( std::string& s, rdo::textstream& stream )
{
	int rtp;
	std::string res_name;
	stream >> res_name;
	stream >> rtp;
	RDOTracerResource* res = new RDOTracerResource( resTypes.at( rtp - 1 ), res_name );
	res->id = atoi( s.c_str() );

	/*int pos = s.find( ' ' );
	int endpos = s.rfind( ' ' );
	string rtpstr = s.substr( endpos );
	trim( rtpstr );
	string name = s.substr( pos, endpos - pos );
	trim( name );
	RDOTracerResource* res = new RDOTracerResource( resTypes.at( atoi( rtpstr.c_str() ) - 1 ), name );
	rtpstr = getNextValue( s );
	res->id = atoi( rtpstr.c_str() );*/
	
	resources.push_back( res );
	tree->addResource( res );
}

void RDOTracerBase::addPattern( std::string& s, rdo::textstream& stream )
{
	std::string pat_name;
	stream >> pat_name;
	std::string pat_type;
	stream >> pat_type;
	RDOTracerPatternKind kind;
	if ( pat_type == "A" )
		kind = RDOPK_OPERATION;
	else if ( pat_type == "I" )
		kind = RDOPK_IRREGULAREVENT;
	else if ( pat_type == "R" )
		kind = RDOPK_RULE;
	else if ( pat_type == "K" )
		kind = RDOPK_KEYBOARD;
	RDOTracerPattern* pat = new RDOTracerPattern( kind );
	pat->Name = pat_name;

	/*string str = s;
	string part = getNextValue( str );
	string name = getNextValue( str );
	part = getNextValue( str );
	RDOTracerPatternKind kind;
	if ( part == "A" )
		kind = RDOPK_OPERATION;
	else if ( part == "I" )
		kind = RDOPK_IRREGULAREVENT;
	else if ( part == "R" )
		kind = RDOPK_RULE;
	else if ( part == "K" )
		kind = RDOPK_KEYBOARD;
	RDOTracerPattern* pat = new RDOTracerPattern( kind );
	pat->Name = name;*/

	patterns.push_back( pat );
	tree->addPattern( pat );
	int rel_res_count;
	std::string dummy;
	stream >> rel_res_count;
	for( int i = 0; i < rel_res_count; i++ )
		stream >> dummy;
}

void RDOTracerBase::addOperation( std::string& s, rdo::textstream& stream )
{
	std::string opr_name;
	stream >> opr_name;
	int pat_id;
	stream >> pat_id;
	RDOTracerPattern* pattern = patterns.at( pat_id - 1 );
	
	RDOTracerOperationBase* opr = NULL;
	
	if ( pattern->getPatternKind() != RDOPK_RULE && pattern->getPatternKind() != RDOPK_IRREGULAREVENT )
		opr = new RDOTracerOperation( pattern );
	else
		opr = new RDOTracerEvent( pattern );
	
	opr->setName( opr_name );

	if ( pattern->getPatternKind() != RDOPK_IRREGULAREVENT ) {
		operations.push_back( opr );
	} else {
		irregularEvents.push_back( static_cast<RDOTracerEvent*>(opr) );
	}
	
	tree->addOperation( opr );

	/*int pos = s.find( ' ' );
	int endpos = s.rfind( ' ' );
	string patstr = s.substr( endpos );
	trim( patstr );
	RDOTracerPattern* pattern = patterns.at( atoi( patstr.c_str() ) -1 );
	
	RDOTracerOperationBase* opr = NULL;

	if ( pattern->getPatternKind() != RDOPK_RULE )
		opr = new RDOTracerOperation( pattern );
	else
		opr = new RDOTracerEvent( pattern );

	patstr = s.substr( pos, endpos - pos );
	trim( patstr );
	opr->setName( patstr );
	
	operations.push_back( opr );
	tree->addOperation( opr );*/
}

/*void RDOTracerBase::addIrregularEvent( std::string& s, rdo::textstream& stream )
{
	int pos = s.find( ' ' );
	int endpos = s.rfind( ' ' );
	string patstr = s.substr( endpos );
	trim( patstr );
	RDOTracerEvent* event = new RDOTracerEvent( patterns.at( atoi( patstr.c_str() ) -1 ) );

	patstr = s.substr( pos, endpos - pos );
	trim( patstr );
	event->setName( patstr );
	
	irregularEvents.push_back( event );
	tree->addOperation( event );
}*/

void RDOTracerBase::addResult( std::string& s, rdo::textstream& stream )
{
	int resid;
	stream >> resid;
	std::string res_kind;
	stream >> res_kind;
	RDOTracerResultKind resKind;
	if ( res_kind == "watch_par" )
		resKind = RDORK_WATCHPAR;
	else if ( res_kind == "watch_state" )
		resKind = RDORK_WATCHSTATE;
	else if ( res_kind == "watch_quant" )
		resKind = RDORK_WATCHQUANT;
	else if ( res_kind == "watch_value" )
		resKind = RDORK_WATCHVALUE;
	RDOTracerResult* res = new RDOTracerResult( resKind );
	res->setName( s );
	res->id = resid;
	/*string str = s;
	string name = getNextValue( str );
	string part = getNextValue( str );
	int resid = atoi( part.c_str() );
	part = getNextValue( str );
	RDOTracerResultKind resKind;
	if ( part == "watch_par" )
		resKind = RDORK_WATCHPAR;
	else if ( part == "watch_state" )
		resKind = RDORK_WATCHSTATE;
	else if ( part == "watch_quant" )
		resKind = RDORK_WATCHQUANT;
	else if ( part == "watch_value" )
		resKind = RDORK_WATCHVALUE;
	RDOTracerResult* res = new RDOTracerResult( resKind );
	res->setName( name );
	res->id = resid;*/
	results.push_back( res );
	tree->addResult( res );
}

void RDOTracerBase::dispatchNextString( std::string& line )
{
	if ( line.empty() )
		return;

	std::string key = getNextValue( line );
	RDOTracerTimeNow* timeNow;
	if ( key != "SO" && key.find( "ST" ) == std::string::npos && key != "SD" && key.find( "SE" ) == std::string::npos )
		timeNow = addTime( getNextValue( line ) );
	else
		timeNow = timeList.back();

	if ( key == "ES" ) {
	} else if ( key == "EB" ) {
		startAction( line, timeNow );
	} else if ( key == "EF" ) {
		accomplishAction( line, timeNow );
	} else if ( key == "EI" ) {
		irregularEvent( line, timeNow );
	} else if ( key == "ER" ) {
		productionRule( line, timeNow );
	} else if ( key == "RC" || key == "SRC" ) {
		resource = resourceCreation( line, timeNow );
		action = RUA_ADD;
#ifdef RDOSIM_COMPATIBLE
	} else if ( key == "RE" || key == "SRE" ) {
		std::string copy1 = line;
		resource = resourceElimination( line, timeNow );
		if ( !resource ) {
			std::string copy2 = copy1;
			resource = resourceCreation( copy1, timeNow );
			tree->addResource( resource );
			resource = resourceElimination( copy2, timeNow );
		}
		action = RUA_UPDATE;
	} else if ( key == "RK" || key == "SRK" ) {
		std::string copy = line;
		RDOTracerResource* res = resourceChanging( line, timeNow );
		if ( !res ) {
			resource = resourceCreation( copy, timeNow );
			action = RUA_ADD;
		}
#else
	} else if ( key == "RK" || key == "SRK" || key == "RE" || key == "SRE" ) {
		bool re = key == "RE" || key == "SRE";
		std::string copy1 = line;
		RDOTracerResource* res = resourceChanging( line, timeNow );
		if ( !res ) {
			std::string copy2 = copy1;
			resource = resourceCreation( copy2, timeNow );
			tree->addResource( resource );
		}
		if ( re ) {
			resource = resourceElimination( copy1, timeNow );
			action = RUA_UPDATE;
		}
#endif
	} else if ( key == "V" ) {
		resultChanging( line, timeNow );
	}/* else if ( key == "$Status" ) {
	} else if ( key.Find( "DPS", 0) != -1 ) {
	} else if ( key == "SB" ) {
	} else if ( key == "SO" ) {
	} else if ( key == "STN" ) {
	} else if ( key == "STD" ) {
	} else if ( key == "STR" ) {
	} else if ( key == "SD" ) {
	} else if ( key == "SES" ) {
	} else if ( key == "SEN" ) {
	} else if ( key == "SEM" ) {
	} else if ( key == "SEF" ) {
	} else if ( key == "SEU" ) {
	}*/
}

std::string RDOTracerBase::getNextValue( std::string& line )
{
	int posstart = line.find_first_not_of( ' ' );
	int posend = line.find_first_of( ' ', posstart );
	std::string res = line.substr( posstart, posend - posstart );
	line.erase( 0, posend + 1 );
	rdo::trim( res );
	return res;
}

RDOTracerTimeNow* RDOTracerBase::addTime( std::string& time )
{
	double val = atof( time.c_str() );
	bool empty = timeList.empty();
	RDOTracerTimeNow* last = NULL;
	if ( !empty ) last = timeList.back();
	if ( empty || last->time != val ) {
		RDOTracerTimeNow* timeNow = new RDOTracerTimeNow( val );
		timeList.push_back( timeNow );
		eventIndex = 0;
		for ( std::vector <RDOTracerOperationBase*>::iterator it = operations.begin(); it != operations.end(); it++ ) {
			(*it)->monitorTime( timeNow, eventIndex );
		}
		for ( std::vector <RDOTracerEvent*>::iterator it_ie = irregularEvents.begin(); it_ie != irregularEvents.end(); it_ie++ ) {
			(*it_ie)->monitorTime( timeNow, eventIndex );
		}
	} else {
		last->eventCount ++;
		eventIndex ++;
		for ( std::vector< RDOStudioChartDoc* >::iterator it = charts.begin(); it != charts.end(); it++ ) {
			(*it)->lock();
			(*it)->incTimeEventsCount( last );
			(*it)->unlock();
		}
	}
	return timeList.back();
}

RDOTracerOperationBase* RDOTracerBase::getOperation( std::string& line )
{
	getNextValue( line );
	return operations.at( atoi( getNextValue( line ).c_str() ) - 1 );
}

void RDOTracerBase::startAction( std::string& line, RDOTracerTimeNow* const time  )
{
	static_cast<RDOTracerOperation*>(getOperation( line ))->start( time, eventIndex );
}

void RDOTracerBase::accomplishAction( std::string& line, RDOTracerTimeNow* const time  )
{
	static_cast<RDOTracerOperation*>(getOperation( line ))->accomplish( time, eventIndex );
}

void RDOTracerBase::irregularEvent( std::string& line, RDOTracerTimeNow* const time  )
{
#ifdef RDOSIM_COMPATIBLE
	irregularEvents.at( atoi( getNextValue( line ).c_str() ) - 1 )->occurs( time, eventIndex );
#else
	irregularEvents.at( atoi( getNextValue( line ).c_str() ) - 1 )->occurs( time, eventIndex );
#endif
}

void RDOTracerBase::productionRule( std::string& line, RDOTracerTimeNow* const time  )
{
	static_cast<RDOTracerEvent*>(getOperation( line ))->occurs( time, eventIndex );
}

RDOTracerResource* RDOTracerBase::getResource( std::string& line )
{
	getNextValue( line );
	RDOTracerResource* res = NULL;
	int findid = atoi( getNextValue( line ).c_str() );
	int i = 0;
	for ( std::vector< RDOTracerResource* >::iterator it = resources.begin(); it != resources.end(); it++ ) {
		if ( (*it)->id == findid )
		if ( (*it)->id == findid && !(*it)->isErased() ) {
			res = *it;
			break;
		}
		i++;
	}
	return res;
}

RDOTracerResource* RDOTracerBase::resourceCreation( std::string& line, RDOTracerTimeNow* const time  )
{
	RDOTracerResType* type = resTypes.at( atoi( getNextValue( line ).c_str() ) - 1 );
	int id = atoi( getNextValue( line ).c_str() );
	RDOTracerResource* res = new RDOTracerResource( type, rdo::format( "%s #%d", type->Name.c_str(), id ) );
	res->id = id;
	res->setParams( line, time, eventIndex );

	resources.push_back( res );
	//tree->addResource( res );
	return res;
}

RDOTracerResource* RDOTracerBase::resourceElimination( std::string& line, RDOTracerTimeNow* const time  )
{
	RDOTracerResource* res = getResource( line );
	if ( !res ) return NULL;
#ifdef RDOSIM_COMPATIBLE
	res->setParams( line, time, eventIndex, true );
#else
	res->setParams( line, time, eventIndex, false );
#endif
	res->setErased( true );
	//tree->updateResource( res );
	return res;
}

RDOTracerResource* RDOTracerBase::resourceChanging( std::string& line, RDOTracerTimeNow* const time  )
{
	RDOTracerResource* res = getResource( line );
	if ( res ) {
		res->setParams( line, time, eventIndex );
	}
	return res;
}

RDOTracerResult* RDOTracerBase::getResult( std::string& line )
{
	RDOTracerResult* res = NULL;
	int findid = atoi( getNextValue( line ).c_str() );
	for ( std::vector< RDOTracerResult* >::iterator it = results.begin(); it != results.end(); it++ ) {
		if ( (*it)->id == findid ) {
			res = *it;
			break;
		}
	}
	return res;
}

void RDOTracerBase::resultChanging( std::string& line, RDOTracerTimeNow* const time  )
{
	getResult( line )->setValue( line, time, eventIndex );
}

void RDOTracerBase::deleteTrace()
{
	mutex.Lock();
	
	int count = resources.size();
	int i;
	for ( i = 0; i < count; i++ ) {
		delete resources.at( i );
	}
	resources.clear();
	count = resTypes.size();
	for ( i = 0; i < count; i++ ) {
		delete resTypes.at( i );
	}
	resTypes.clear();
	count = operations.size();
	for ( i = 0; i < count; i++ ) {
		delete operations.at( i );
	}
	operations.clear();
	count = irregularEvents.size();
	for ( i = 0; i < count; i++ ) {
		delete irregularEvents.at( i );
	}
	irregularEvents.clear();
	count = patterns.size();
	for ( i = 0; i < count; i++ ) {
		delete patterns.at( i );
	}
	patterns.clear();
	count = results.size();
	for ( i = 0; i < count; i++ ) {
		delete results.at( i );
	}
	results.clear();
	std::list< RDOTracerTimeNow* >::iterator it = timeList.begin();
	while ( it != timeList.end() ) {
		delete *it;
		it++;
	}
	timeList.clear();

	mutex.Unlock();
}

void RDOTracerBase::clear()
{
	mutex.Lock();

	clearCharts();
	deleteTrace();
	if( tree )
		tree->clear();
	if ( log )
		log->clear();

	mutex.Unlock();
}

void RDOTracerBase::clearCharts()
{
	mutex.Lock();

	while ( !charts.empty() ) {
		charts.front()->OnCloseDocument();
	}
	charts.clear();

	mutex.Unlock();
}

CMultiDocTemplate* RDOTracerBase::createDocTemplate()
{
	chartDocTemplate = new CMultiDocTemplate( IDR_CHARTTYPE, RUNTIME_CLASS(RDOStudioChartDoc), RUNTIME_CLASS(RDOStudioChildFrame), RUNTIME_CLASS(RDOStudioChartView) );
	return chartDocTemplate;
}

RDOTracerLogCtrl* RDOTracerBase::createLog()
{
	log = new RDOTracerLogCtrl();
	return log;
}

RDOTracerTreeCtrl* RDOTracerBase::createTree()
{
	tree = new RDOTracerTreeCtrl();
	return tree;
}

void RDOTracerBase::getModelStructure( rdo::textstream& stream )
{
	mutex.Lock();

	std::string s;

	while( !stream.eof() ) {
		stream >> s;
		if ( !s.empty() ) {
			int pos = std::string::npos;
			if ( s.find( "$Resource_type" ) != std::string::npos ) {
				do {
					stream >> s;
					pos = s.find( "$Resources" );

					// AB if model has no $Resources at all,
					// we get endless cycle here
					if(pos == std::string::npos)
						pos = s.find( "$Pattern" );

					if ( !s.empty() && pos == std::string::npos )
						addResourceType( s, stream );
				} while ( pos == std::string::npos && !stream.eof() );
			}
			if ( s.find( "$Resources" ) != std::string::npos ) {
				do {
					stream >> s;
					pos = s.find( "$Pattern" );
					if ( !s.empty() && pos == std::string::npos )
						addResource( s, stream );
				} while ( pos == std::string::npos && !stream.eof() );
			}
			if ( s.find( "$Pattern" ) != std::string::npos ) {
				do {
					stream >> s;
					pos = s.find( "$Activities" );
					if ( !s.empty() && pos == std::string::npos )
						addPattern( s, stream );
				} while ( pos == std::string::npos && !stream.eof() );
			}
			if ( s.find( "$Activities" ) != std::string::npos ) {
				do {
					stream >> s;
					pos = s.find( "$Watching" );
					if ( !s.empty() && pos == std::string::npos )
						addOperation( s, stream );
				} while ( pos == std::string::npos && !stream.eof() );
			}
			if ( s.find( "$Watching" ) != std::string::npos && !stream.eof() ) {
				do {
					s = "";
					stream >> s;
					if ( !s.empty() )
						addResult( s, stream );
				} while ( !stream.eof() );
			}
		}
	}

	mutex.Unlock();

	/*stream >> s;

	if ( s.find( "$Resource_type" ) != string::npos ) {
		do {
			stream >> s;
			if ( !s.empty() )
				addResourceType( s, stream );
		} while ( s != "" );
	}
	
	if ( s.find( "$Resources" ) != string::npos ) {
		do {
			stream >> s;
			if ( !s.empty() )
				addResource( s, stream );
		} while ( s != "" );
	}

	if ( s.find( "$Pattern" ) != string::npos ) {
		do {
			stream >> s;
			if ( !s.empty() )
				addPattern( s, stream );
		} while ( s != "" );
	}

	if ( s.find( "$Activities" ) != string::npos ) {
		do {
			stream >> s;
			if ( !s.empty() )
				addOperation( s, stream );
		} while ( s != "" );
		do {
			stream >> s;
			if ( !s.empty() )
				addIrregularEvent( s, stream );
		} while ( s != "" );
	}

	if ( s.find( "$Watching" ) != string::npos ) {
		do {
			stream >> s;
			if ( !s.empty() )
				addResult( s, stream );
		} while ( s != "" );
	}*/
}

void RDOTracerBase::getTraceString( std::string trace_string )
{
	mutex.Lock();

	if ( log ) {
		log->addStringToLog( trace_string );
	}

	action = RUA_NONE;
	resource = NULL;

	dispatchNextString( trace_string );

	mutex.Unlock();

	switch( action ) {
		case RUA_ADD: {
			tree->addResource( resource );
			break;
		}
		case RUA_UPDATE: {
			tree->updateResource( resource );
			break;
		}
		default: {
			break;
		}
	}
}

RDOStudioChartDoc* RDOTracerBase::createNewChart()
{
	RDOStudioChartDoc* doc = NULL;
	if ( chartDocTemplate ) {
		doc = (RDOStudioChartDoc*)chartDocTemplate->OpenDocumentFile( NULL );
	}
	return doc;
}

RDOStudioChartDoc* RDOTracerBase::addSerieToChart( RDOTracerSerie* const serie, RDOStudioChartDoc* chart )
{
	mutex.Lock();

	if ( !chart )
		chart = createNewChart();
	if( chart ) {
		chart->addSerie( serie );
	}
	
	mutex.Unlock();

	return chart;
}

void RDOTracerBase::addChart( RDOStudioChartDoc* const chart )
{
	mutex.Lock();
	
	charts.push_back( chart );

	mutex.Unlock();
}

void RDOTracerBase::removeChart( RDOStudioChartDoc* chart )
{
	mutex.Lock();
	
	for ( std::vector< RDOStudioChartDoc* >::iterator it = charts.begin(); it != charts.end(); it++ ) {
		if ( (*it) == chart ) {
			charts.erase( it );
			break;
		}
	}

	mutex.Unlock();
}

void RDOTracerBase::updateChartsStyles() const
{
	const_cast<CMutex&>(mutex).Lock();
	
	for ( std::vector< RDOStudioChartDoc* >::const_iterator it = charts.begin(); it != charts.end(); it++ ) {
		POSITION pos = (*it)->GetFirstViewPosition();
		while ( pos != NULL ) {
			CView* pView = (*it)->GetNextView( pos );
			if ( pView && pView->IsKindOf(RUNTIME_CLASS(RDOStudioChartView)) ) {
				((RDOStudioChartView*)pView)->setStyle( &studioApp.mainFrame->style_chart );
			}
		}
	}

	const_cast<CMutex&>(mutex).Unlock();
}

void RDOTracerBase::setModelName( std::string name ) const
{
	if ( tree ) tree->setModelName( name );
}

void RDOTracerBase::setDrawTrace( const bool value )
{
	if ( drawTrace != value ) {
		drawTrace = value;
		if ( !drawTrace )
			clearCharts();
		log->setDrawLog( value );
	}
}