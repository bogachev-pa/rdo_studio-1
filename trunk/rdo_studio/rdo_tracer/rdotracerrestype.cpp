#include "stdafx.h"
#include "rdotracerrestype.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

// ----------------------------------------------------------------------------
// ---------- RDOTracerResParamInfo
// ----------------------------------------------------------------------------
RDOTracerResParamInfo::RDOTracerResParamInfo( const RDOTracerResParamType type ) :
	paramType( type ),
	enumValues( NULL )
{
	if ( paramType == RDOPT_ENUMERATIVE )
		enumValues = new RDOStringVector();
}

RDOTracerResParamInfo::~RDOTracerResParamInfo()
{
	if ( enumValues )
		delete enumValues;
};

int RDOTracerResParamInfo::addEnumValue( const string& value )
{
	if ( !enumValues )
		return -1;
	enumValues->push_back( value );
	return enumValues->size() - 1;
}

static string nullStr = "";

string RDOTracerResParamInfo::getEnumValue( const int index ) const
{
	if ( !enumValues )
		return nullStr;
	if ( index >= enumValues->size() || index < 0 )
		return nullStr;
	return enumValues->at( index );
}

// ----------------------------------------------------------------------------
// ---------- RDOTracerResType
// ----------------------------------------------------------------------------
RDOTracerResType::RDOTracerResType( const RDOTracerResTypeKind kind ) :
	RDOTracerTreeItem(),
	restypeKind( kind )
{
}

RDOTracerResType::~RDOTracerResType()
{
	int count = paramsInfo.size();
	for (int i = 0; i < count; i++ ) {
		delete paramsInfo.at( i );
	}
};

int RDOTracerResType::addParamInfo( RDOTracerResParamInfo* const value )
{
	paramsInfo.push_back( value );
	return paramsInfo.size() - 1;
}

RDOTracerResParamInfo* RDOTracerResType::getParamInfo( const int index ) const
{
	if ( index >= paramsInfo.size() || index < 0 )
		return NULL;
	return paramsInfo.at( index );
}