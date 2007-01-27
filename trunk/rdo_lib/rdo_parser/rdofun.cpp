#include "pch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdofun.h"
#include "rdoparser.h"
#include "rdortp.h"
#include "rdorss.h"
#include "rdopat.h"
#include "rdoruntime.h"
#include "rdofunc.h"
#include "rdodpt.h"
#include "rdoparser_lexer.h"

namespace rdoParse 
{

int funlex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer )
{
	reinterpret_cast<RDOLexer*>(lexer)->m_lpval = lpval;
	reinterpret_cast<RDOLexer*>(lexer)->m_lploc = llocp;
	return reinterpret_cast<RDOLexer*>(lexer)->yylex();
}
void funerror( char* mes )
{
}

void RDOParser::addConstant( RDORTPParamDesc* const _cons )
{
	if ( !findFUNConst(_cons->getName()) ) {
		RDOFUNConstant* newConst = new RDOFUNConstant( this, _cons );
		runTime->setConstValue( newConst->number, newConst->descr->getType()->getRSSDefaultValue() );
	}
}

RDOFUNConstant::RDOFUNConstant( RDOParser* _parser, RDORTPParamDesc* _descr ):
	RDOParserObject( _parser ),
	descr( _descr ),
	number( _parser->getFUNCONST_id() )
{
	parser->insertFUNConstant( this );
}

const RDOFUNConstant *RDOParser::findFUNConst(const std::string *const _cons) const
{
	std::vector<RDOFUNConstant *>::const_iterator it = 
		std::find_if(allFUNConstant.begin(), allFUNConstant.end(), compareName<RDOFUNConstant>(_cons));

	if(it == allFUNConstant.end())
		return NULL;

	return (*it);
}

const RDOFUNFunctionParam *const RDOFUNFunction::findFUNFunctionParam(const std::string *const paramName) const 
{
	std::vector<const RDOFUNFunctionParam *>::const_iterator it = 
		std::find_if(params.begin(), params.end(), compareName<RDOFUNFunctionParam>(paramName));

	if(it == params.end())
		return NULL;

	return (*it);
}

RDOFUNFunction::RDOFUNFunction( RDOParser* _parser, const std::string* const _name, const RDORTPResParam* const _retType ):
	RDOParserObject( _parser ),
	name( _name ),
	retType( _retType )
{
	parser->insertFUNFunction( this );
}

int RDOFUNFunction::findFUNFunctionParamNum(const std::string *const paramName) const
{
	std::vector<const RDOFUNFunctionParam *>::const_iterator it = 
		std::find_if(params.begin(), params.end(), compareName<RDOFUNFunctionParam>(paramName));

	if(it == params.end())
		return -1;

	return it - params.begin();
}

void RDOFUNFunction::add( const RDOFUNFunctionParam* const _param )
{ 
	if ( findFUNFunctionParam(_param->getName()) ) {
		parser->lexer_loc_set( _param->error().last_line, _param->error().last_column );
		parser->error( rdo::format("�������� ��� ����������: %s", _param->getName()->c_str()) );
//		parser->error("Second appearance of the same parameter name: " + *(_param->getName()));
	}
	params.push_back( _param ); 
}

void RDOFUNFunction::add(const RDOFUNFunctionListElement *const _param)
{
	listElems.push_back(_param); 
}

void RDOFUNFunction::add( const RDOFUNCalculateIf* const _calculateIf )
{
	calculateIf.push_back( _calculateIf );
}

void RDOFUNFunction::createListCalc()
{
	int numParams = params.size();
	int elements = listElems.size();
	int currElement = 0;

	if ( !retType->dv->exist ) {
		parser->error( rdo::format("������� '%s' ������ ����� �������� ��-���������", name->c_str()) );
//		parser->error(("list function " + *name + " must have default result value").c_str());
	}

	rdoRuntime::RDOCalcConst*   defaultValue = new rdoRuntime::RDOCalcConst( retType->getRSSDefaultValue() );
	rdoRuntime::RDOFunListCalc* funCalc      = new rdoRuntime::RDOFunListCalc( defaultValue );
	parser->lexer_loc_backup();
	try {
		for(;;)	// for all cases in list
		{
			if ( currElement >= elements )
				break;

			rdoRuntime::RDOCalc* caseCalc = new rdoRuntime::RDOCalcConst(true);
			for ( int currParam = 0; currParam < numParams; currParam++ ) {
				if ( currElement >= elements ) {
					parser->error( "������� �������� �� ���� ����������" );
				}
				const RDOFUNFunctionListElement* const arg = listElems.at( currElement++ );
				if ( arg->isEquivalence() ) {
					parser->lexer_loc_set( arg->error().first_line, arg->error().first_column );
					parser->error( "������� �������� �� ���� ����������" );
				}
				parser->lexer_loc_set( arg->error().last_line, arg->error().last_column );
				const RDOFUNFunctionParam *const param = params.at(currParam);
				rdoRuntime::RDOCalcFuncParam *funcParam = new rdoRuntime::RDOCalcFuncParam(currParam);
				rdoRuntime::RDOCalcIsEqual *compareCalc = arg->createIsEqualCalc(param, funcParam);
				rdoRuntime::RDOCalc *andCalc = new rdoRuntime::RDOCalcAnd(caseCalc, compareCalc);
				caseCalc = andCalc;
			}

			if ( currElement >= elements ) {
				parser->error( "��������� '= <��������_�������>'" );
			}
			const RDOFUNFunctionListElement* const eq = listElems.at( currElement++ );
			if ( !eq->isEquivalence() ) {
				parser->lexer_loc_set( eq->error().first_line, eq->error().first_column );
				parser->error( "��������� '= <��������_�������>'" );
			}

			if ( currElement >= elements ) {
				parser->lexer_loc_set( eq->error().last_line, eq->error().last_column );
				parser->error( "��������� '<��������_�������>'" );
			}
			const RDOFUNFunctionListElement* const res = listElems.at(currElement++);
			parser->lexer_loc_set( res->error().last_line, res->error().last_column );
			rdoRuntime::RDOCalcConst* resultCalc = res->createResultCalc(retType);

			funCalc->addCase(caseCalc, resultCalc);
		}
		functionCalc = funCalc;
		parser->lexer_loc_restore();

	} catch ( std::out_of_range ex ) {
		parser->error( rdo::format( "�������� ���������� ��������� �������: %s", name->c_str()) );
//		parser->error(("Wrong element number in list function " + *name).c_str());
	}
}

void RDOFUNFunction::createTableCalc()
{
	parser->lexer_loc_backup();
	int numParams = params.size();
	try {
		rdoRuntime::RDOCalc *calc = new rdoRuntime::RDOCalcConst(0);
		int d = 1;
		for ( int currParam = 0; currParam < numParams; currParam++ ) {
			const RDOFUNFunctionParam* const param  = params.at(currParam);
			parser->lexer_loc_set( param->error().last_line, param->error().last_column );
			rdoRuntime::RDOCalcFuncParam *funcParam = new rdoRuntime::RDOCalcFuncParam(currParam);
			rdoRuntime::RDOCalc *val2 = funcParam;
			if ( param->getType()->getType() != RDORTPResParam::pt_enum ) {
				rdoRuntime::RDOCalcConst *const1 = new rdoRuntime::RDOCalcConst(1);
				val2 = new rdoRuntime::RDOCalcMinus(val2, const1);
			}
			rdoRuntime::RDOCalcConst *const2 = new rdoRuntime::RDOCalcConst(d);
			rdoRuntime::RDOCalcMult *mult = new rdoRuntime::RDOCalcMult(const2, val2);
			rdoRuntime::RDOCalcPlus *add = new rdoRuntime::RDOCalcPlus(mult, calc);
			d *= param->getType()->getDiapTableFunc();
			calc = add;
		}

		if ( listElems.size() != d ) {
			parser->lexer_loc_restore();
			parser->error( rdo::format("�������� ���������� ��������� ��������� ������� '%s': %d, ��������� %d", name->c_str(), listElems.size(), d) );
//			parser->error(("wrong number of value in table function " + *name).c_str());
		}

		rdoRuntime::RDOFuncTableCalc *funCalc = new rdoRuntime::RDOFuncTableCalc(calc);
		for ( int currElem = 0; currElem < d; currElem++ ) {
			const RDOFUNFunctionListElement* const el = listElems.at(currElem);
			parser->lexer_loc_set( el->error().last_line, el->error().last_column );
			if ( el->isEquivalence() ) {
				parser->error( "������ '=' ���������� � ��������� �������" );
//				parser->error("\"=\" unexpected in table function");
			}
			rdoRuntime::RDOCalcConst* resultCalc = el->createResultCalc(retType);
			funCalc->addResultCalc(resultCalc);
		}
		functionCalc = funCalc;
		parser->lexer_loc_restore();

	} catch ( std::out_of_range ex ) {
		parser->error( rdo::format("�������� ���������� ��������� ��������� ������� '%s'", name->c_str()) );
//		parser->error(("Wrong element number in list function " + *name).c_str());
	}
}

rdoRuntime::RDOCalcIsEqual *RDOFUNFunctionListElement::createIsEqualCalc(const RDOFUNFunctionParam *const param, const rdoRuntime::RDOCalcFuncParam *const funcParam) const
{
	rdoRuntime::RDOCalcConst *constCalc = createResultCalc(param->getType());
	rdoRuntime::RDOCalcIsEqual *res = new rdoRuntime::RDOCalcIsEqual(constCalc, funcParam);
	return res;
}

rdoRuntime::RDOCalcConst *RDOFUNFunctionListElementIdentif::createResultCalc(const RDORTPResParam *const retType) const
{
	return new rdoRuntime::RDOCalcConst(retType->getRSSEnumValue(value));
}

rdoRuntime::RDOCalcConst *RDOFUNFunctionListElementReal::createResultCalc(const RDORTPResParam *const retType) const 
{
	return new rdoRuntime::RDOCalcConst(retType->getRSSRealValue(value));
}

rdoRuntime::RDOCalcConst *RDOFUNFunctionListElementInt::createResultCalc(const RDORTPResParam *const retType) const 
{
	return new rdoRuntime::RDOCalcConst(retType->getRSSIntValue(value));
}

rdoRuntime::RDOCalcConst* RDOFUNFunctionListElementEq::createResultCalc( const RDORTPResParam* const retType ) const
{
	parser->error( "���������� ������ �������: �������� ���������" );
//	parser->error( "Internal parser error, incorrect state" );
	return NULL;	// unreachable code
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNLogic
// ----------------------------------------------------------------------------
RDOFUNLogic::RDOFUNLogic( rdoRuntime::RDOCalc *_calc ):
	calc( _calc )
{
	if ( calc ) setErrorPos( _calc->error() );
}

RDOFUNLogic* RDOFUNLogic::operator &&( const RDOFUNLogic& second )
{
	rdoRuntime::RDOCalc* newCalc = new rdoRuntime::RDOCalcAnd( calc, second.calc );
	RDOFUNLogic* logic = new RDOFUNLogic( newCalc );
	logic->setErrorPos( error().first_line, error().first_column, second.error().last_line, second.error().last_column );
	return logic;
}

RDOFUNLogic* RDOFUNLogic::operator ||( const RDOFUNLogic& second )
{
	rdoRuntime::RDOCalc* newCalc = new rdoRuntime::RDOCalcOr( calc, second.calc );
	RDOFUNLogic* logic = new RDOFUNLogic( newCalc );
	logic->setErrorPos( error().first_line, error().first_column, second.error().last_line, second.error().last_column );
	return logic;
}

RDOFUNLogic* RDOFUNLogic::operator_not()
{
	rdoRuntime::RDOCalc* newCalc = new rdoRuntime::RDOCalcNot( calc );
	RDOFUNLogic* logic = new RDOFUNLogic( newCalc );
	logic->setErrorPos( error() );
	return logic;
}

void RDOFUNLogic::setErrorPos( const YYLTYPE& error_pos )
{
	RDOErrorPos::setErrorPos( error_pos );
	if ( calc ) calc->setErrorPos( error_pos );
}

void RDOFUNLogic::setErrorPos( int first_line, int first_column, int last_line, int last_column )
{
	YYLTYPE error;
	error.first_line   = first_line;
	error.first_column = first_column;
	error.last_line    = last_line;
	error.last_column  = last_column;
	setErrorPos( error );
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNArithm
// ----------------------------------------------------------------------------
RDOFUNArithm::RDOFUNArithm( RDOParser* _parser, RDORTPResParam::ParamType _type, rdoRuntime::RDOCalc* _calc, const YYLTYPE& error_pos ):
	RDOParserObject( _parser ),
	type( _type ),
	enu( NULL ),
	str( NULL ),
	calc( _calc )
{
	setErrorPos( error_pos );
}

RDOFUNArithm::RDOFUNArithm( const RDOParserObject* _parent, RDORTPResParam::ParamType _type, rdoRuntime::RDOCalc* _calc, const YYLTYPE& error_pos ):
	RDOParserObject( _parent ),
	type( _type ),
	enu( NULL ),
	str( NULL ),
	calc( _calc )
{
	setErrorPos( error_pos );
}

RDOFUNArithm::RDOFUNArithm( RDOParser* _parser, std::string* resName, std::string* parName, const YYLTYPE& res_name_error_pos, const YYLTYPE& par_name_error_pos ):
	RDOParserObject( _parser ),
	type( RDORTPResParam::pt_int ),
	enu( NULL ),
	str( NULL ),
	calc( NULL )
{
	init( resName, parName, res_name_error_pos, par_name_error_pos );
}

RDOFUNArithm::RDOFUNArithm( const RDOParserObject* _parent, std::string* resName, std::string* parName, const YYLTYPE& res_name_error_pos, const YYLTYPE& par_name_error_pos ):
	RDOParserObject( _parent ),
	type( RDORTPResParam::pt_int ),
	enu( NULL ),
	str( NULL ),
	calc( NULL )
{
	init( resName, parName, res_name_error_pos, par_name_error_pos );
}

void RDOFUNArithm::init( std::string* resName, std::string* parName, const YYLTYPE& res_name_error_pos, const YYLTYPE& par_name_error_pos )
{
	setErrorPos( res_name_error_pos.first_line, res_name_error_pos.first_column, par_name_error_pos.last_line, par_name_error_pos.last_column );
	const RDORSSResource* const res = parser->findRSSResource( resName ); 
	if ( res ) {
		// ��� ������ � �������� RSS
		if ( res->getType()->isTemporary() ) {
			parser->lexer_loc_set( res_name_error_pos.last_line, res_name_error_pos.last_column );
			parser->error( rdo::format("������ ������������ ��������� ������: %s", resName->c_str()) );
//			parser->error(("Cannot use temporary resource in function: " + *resName).c_str());
		}
		int resNumb = res->getNumber();
		int parNumb = res->getType()->getRTPParamNumber( parName );
		if ( parNumb == -1 ) {
			parser->lexer_loc_set( par_name_error_pos.last_line, par_name_error_pos.last_column );
			parser->error( rdo::format("����������� �������� �������: %s", parName->c_str()) );
		}
		calc = new rdoRuntime::RDOCalcGetResParam( resNumb, parNumb );
		calc->setErrorPos( error() );
		type = res->getType()->findRTPParam(parName)->getType()->getType();
		if ( type == RDORTPResParam::pt_enum ) {
			enu = ((RDORTPEnumResParam *)res->getType()->findRTPParam(parName)->getType())->enu;
		}
		return;
	} else {
		// ��� �� ������, �� �������� ����������� ������ ��� ������, ������ ��������� �������
		if ( !parser->getFUNGroupStack().empty() && *parser->getFUNGroupStack().back()->resType->getName() == *resName ) {
			// ��� ������ ������ ��������� �������
			RDOFUNGroup* currGroup = parser->getFUNGroupStack().back();
			int parNumb = currGroup->resType->getRTPParamNumber(parName);
			if ( parNumb == -1 ) {
				parser->lexer_loc_set( par_name_error_pos.last_line, par_name_error_pos.last_column );
				parser->error( rdo::format("����������� �������� �������: %s", parName->c_str()) );
			}
			calc = new rdoRuntime::RDOCalcGetGroupResParam( parNumb );
			calc->setErrorPos( error() );
			type = currGroup->resType->findRTPParam(parName)->getType()->getType();
			if ( type == RDORTPResParam::pt_enum ) {
				enu = ((RDORTPEnumResParam *)currGroup->resType->findRTPParam(parName)->getType())->enu;
			}
			return;
		} else if ( parser->getFileToParse() == rdoModelObjects::PAT && parser->getLastPATPattern() && parser->getLastPATPattern()->findRelevantResource(resName) ) {
			// ��� ����������� ������ ���-�� � �������� (with_min-common-choice, $Time, $Body)
			RDOPATPattern* pat = parser->getLastPATPattern();
			const RDORelevantResource* const rel = pat->findRelevantResource( resName );
			if ( !pat->currRelRes ) {
				// ������ with_min-common-choice ��� $Time
				if ( rel->begin == RDOResourceTrace::CS_NonExist || rel->begin == RDOResourceTrace::CS_Create ) {
					parser->lexer_loc_set( res_name_error_pos.last_line, res_name_error_pos.last_column );
					parser->error( rdo::format("����������� ������ �� ����� ���� �����������, �.�. �� ��� �� ����������: %s", rel->getName()->c_str()) );
				}
			} else {
				// ������ $Body
				// ��������� ������������� ��������������������� ���.������� � Choice from ������ ���.�������
				if ( pat->currRelRes->isChoiceFromState() ) {
					if ( !rel->alreadyHaveConverter && !rel->isDirect() ) {
						parser->lexer_loc_set( res_name_error_pos.last_line, res_name_error_pos.last_column );
						parser->error( rdo::format("����������� ������ �����������: %s. ��� ������ ������������ � �������� ������ ������ �������� �� ��� ������������ Choice from", rel->getName()->c_str()) );
					}
					if ( rel->begin == RDOResourceTrace::CS_NonExist ) {
						parser->lexer_loc_set( res_name_error_pos.last_line, res_name_error_pos.last_column );
						parser->error( rdo::format("����������� ������ � ������ �������� �� ���������� (NonExist): %s", rel->getName()->c_str()) );
					}
					if ( rel->begin == RDOResourceTrace::CS_Create ) {
						parser->lexer_loc_set( res_name_error_pos.last_line, res_name_error_pos.last_column );
						parser->error( rdo::format("����� ����� �������� (Create) ����������� ������ ����� ������������ ������ � �����������: %s", rel->getName()->c_str()) );
					}
				}
				// ��������� ������������� ���������� ���.������� ������ ���������� ������� ���.�������
				if ( rel->getType()->isTemporary() ) {
					// � ���������� ������
					if ( pat->currRelRes->currentState == RDORelevantResource::convertBegin ) {
						if ( rel->begin == RDOResourceTrace::CS_Create && !rel->alreadyHaveConverter ) {
							parser->lexer_loc_set( res_name_error_pos.last_line, res_name_error_pos.last_column );
							parser->error( rdo::format("����������� ������ ������ ������������ �� ��� �������� (Create): %s", rel->getName()->c_str()) );
						}
						if ( rel->begin == RDOResourceTrace::CS_Erase && rel->alreadyHaveConverter ) {
							parser->lexer_loc_set( res_name_error_pos.last_line, res_name_error_pos.last_column );
							parser->error( rdo::format("����������� ������ ������ ������������ ������ �������� (Erase): %s", rel->getName()->c_str()) );
						}
						if ( rel->begin == RDOResourceTrace::CS_NonExist ) {
							parser->lexer_loc_set( res_name_error_pos.last_line, res_name_error_pos.last_column );
							parser->error( rdo::format("����������� ������ �� ���������� � ���� ���������� (NonExist): %s", rel->getName()->c_str()) );
						}
					}
					// � ���������� �����
					if ( pat->currRelRes->currentState == RDORelevantResource::convertEnd ) {
						if ( rel->end == RDOResourceTrace::CS_Create && !rel->alreadyHaveConverter ) {
							parser->lexer_loc_set( res_name_error_pos.last_line, res_name_error_pos.last_column );
							parser->error( rdo::format("����������� ������ ������ ������������ �� ��� �������� (Create): %s", rel->getName()->c_str()) );
						}
						if ( rel->end == RDOResourceTrace::CS_Erase && rel->alreadyHaveConverter ) {
							parser->lexer_loc_set( res_name_error_pos.last_line, res_name_error_pos.last_column );
							parser->error( rdo::format("����������� ������ ������ ������������ ������ �������� (Erase): %s", rel->getName()->c_str()) );
						}
						if ( rel->end == RDOResourceTrace::CS_NonExist ) {
							parser->lexer_loc_set( res_name_error_pos.last_line, res_name_error_pos.last_column );
							parser->error( rdo::format("����������� ������ �� ���������� � ���� ���������� (NonExist): %s", rel->getName()->c_str()) );
						}
					}
				}
			}
			int relResNumb = pat->findRelevantResourceNum( resName );
			int parNumb    = rel->getType()->getRTPParamNumber( parName );
			if ( parNumb == -1 ) {
				parser->lexer_loc_set( par_name_error_pos.last_line, par_name_error_pos.last_column );
				parser->error( rdo::format("����������� �������� �������: %s", parName->c_str()) );
//				parser->error( "Unknown resource parameter: " + *parName );
			}
			calc = new rdoRuntime::RDOCalcGetRelevantResParam( relResNumb, parNumb );
			calc->setErrorPos( error() );
			type = rel->getType()->findRTPParam( parName )->getType()->getType();
			if ( type == RDORTPResParam::pt_enum ) {
				enu = ((RDORTPEnumResParam*)rel->getType()->findRTPParam(parName)->getType())->enu;
			}
			return;
		} else if ( parser->getFileToParse() == rdoModelObjects::DPT && parser->getLastDPTSearch() && parser->getLastDPTSearch()->lastActivity && parser->getLastDPTSearch()->lastActivity->getRule() && parser->getLastDPTSearch()->lastActivity->getRule()->findRelevantResource(resName) ) {
			// ��� ������, ������� ������������ � DPT (condition, term_condition, evaluate_by, value before, value after)
			const RDORelevantResource* const rel = parser->getLastDPTSearch()->lastActivity->getRule()->findRelevantResource( resName );
			int relResNumb = parser->getLastDPTSearch()->lastActivity->getRule()->findRelevantResourceNum( resName );
			int parNumb    = rel->getType()->getRTPParamNumber(parName);
			if ( parNumb == -1 ) {
				parser->lexer_loc_set( par_name_error_pos.last_line, par_name_error_pos.last_column );
				parser->error( rdo::format("����������� �������� �������: %s", parName->c_str()) );
			}

			calc = new rdoRuntime::RDOCalcGetRelevantResParam( relResNumb, parNumb );
			calc->setErrorPos( error() );
			type = rel->getType()->findRTPParam(parName)->getType()->getType();
			if ( type == RDORTPResParam::pt_enum ) {
				enu = ((RDORTPEnumResParam *)rel->getType()->findRTPParam(parName)->getType())->enu;
			}
			return;
		}
	}
	parser->lexer_loc_set( res_name_error_pos.last_line, res_name_error_pos.last_column );
	parser->error( rdo::format("����������� ������: %s", resName->c_str()) );
//	parser->error(("Unknown resource name: " + *resName).c_str());
}

RDOFUNArithm::RDOFUNArithm( RDOParser* _parser, int n, const YYLTYPE& error_pos ):
	RDOParserObject( _parser ),
	type( RDORTPResParam::pt_int ),
	enu( NULL ),
	str( NULL ),
	calc( NULL )
{
	calc = new rdoRuntime::RDOCalcConst( n );
	setErrorPos( error_pos );
}

RDOFUNArithm::RDOFUNArithm( const RDOParserObject* _parent, int n, const YYLTYPE& error_pos ):
	RDOParserObject( _parent ),
	type( RDORTPResParam::pt_int ),
	enu( NULL ),
	str( NULL ),
	calc( NULL )
{
	calc = new rdoRuntime::RDOCalcConst( n );
	setErrorPos( error_pos );
}

RDOFUNArithm::RDOFUNArithm( RDOParser* _parser, double* d, const YYLTYPE& error_pos ):
	RDOParserObject( _parser ),
	type( RDORTPResParam::pt_real ),
	enu( NULL ),
	str( NULL ),
	calc( NULL )
{
	calc = new rdoRuntime::RDOCalcConst( *d );
	setErrorPos( error_pos );
}

RDOFUNArithm::RDOFUNArithm( const RDOParserObject* _parent, double* d, const YYLTYPE& error_pos ):
	RDOParserObject( _parent ),
	type( RDORTPResParam::pt_real ),
	enu( NULL ),
	str( NULL ),
	calc( NULL )
{
	calc = new rdoRuntime::RDOCalcConst( *d );
	setErrorPos( error_pos );
}

RDOFUNArithm::RDOFUNArithm( RDOParser* _parser, std::string* s, const YYLTYPE& error_pos ):
	RDOParserObject( _parser ),
	type( RDORTPResParam::pt_int ),
	enu( NULL ),
	str( NULL ),
	calc( NULL )
{
	init( s, error_pos );
}

RDOFUNArithm::RDOFUNArithm( const RDOParserObject* _parent, std::string* s, const YYLTYPE& error_pos ):
	RDOParserObject( _parent ),
	type( RDORTPResParam::pt_int ),
	enu( NULL ),
	str( NULL ),
	calc( NULL )
{
	init( s, error_pos );
}

void RDOFUNArithm::init( std::string* s, const YYLTYPE& error_pos )
{
	setErrorPos( error_pos );

	if ( (*s == "Time_now") || (*s == "time_now") || (*s == "���������_�����") || (*s == "���������_�����") ) {
		type = RDORTPResParam::pt_real;
		calc = new rdoRuntime::RDOCalcGetTimeNow();
		calc->setErrorPos( error_pos );
		return;
	}

	if ( *s == "Seconds" || *s == "seconds" ) {
		type = RDORTPResParam::pt_real;
		calc = new rdoRuntime::RDOCalcGetSeconds();
		calc->setErrorPos( error_pos );
		return;
	}

	// ���� ��������� �������� ��� ������� �� �����
	const RDOFUNFunctionParam* param = NULL;
	switch ( parser->getFileToParse() ) {
		case rdoModelObjects::PAT: param = parser->getLastPATPattern()->findPATPatternParam( s ); break;
		case rdoModelObjects::FUN: param = parser->getLastFUNFunction()->findFUNFunctionParam( s ); break;
	}

	// ���� ��������� �� �����
	const RDOFUNConstant* cons = parser->findFUNConst( s );

	if ( cons && param ) {
		parser->lexer_loc_set( error_pos.last_line, error_pos.last_column );
		parser->error( rdo::format("��� ��������� ������� ��������� � ������ ���������: %s", s->c_str()) );
//		parser->error( "Ambiguity: constant or parameter usage: " + *s + " ?" );
	}

	if ( cons ) {
		type = cons->descr->getType()->getType();
		if ( type == RDORTPResParam::pt_enum ) {
			enu = ((RDORTPEnumResParam *)cons->descr->getType())->enu;
		}
		calc = new rdoRuntime::RDOCalcGetConst( cons->number );
		calc->setErrorPos( error_pos );
		return;
	}

	// ���� ������������������ �� �����
	const RDOFUNSequence* seq = parser->findSequence( s );
	if ( seq && param ) {
		parser->lexer_loc_set( error_pos.last_line, error_pos.last_column );
		parser->error( rdo::format("��� ��������� ������� ��������� � ������ ������������������: %s", s->c_str()) );
//		parser->error("Ambiguity: sequence or parameter usage: " + *s + " ?");
	}

	if ( seq ) {
		RDOFUNParams tmp( seq );
		const RDOFUNArithm* ar = tmp.createSeqCall( s );
		type = ar->getType();
		if ( type == RDORTPResParam::pt_enum ) {
			enu = ar->enu;
		}
		calc = ar->calc;
		calc->setErrorPos( error_pos );
		return;
	}

	if ( param ) {
		// ��� ��������
		type = param->getType()->getType();
		if ( type == RDORTPResParam::pt_enum ) {
			enu = ((RDORTPEnumResParam *)param->getType())->enu;
		}
		switch ( parser->getFileToParse() ) {
			case rdoModelObjects::PAT: calc = new rdoRuntime::RDOCalcPatParam( parser->getLastPATPattern()->findPATPatternParamNum( s ) ); break;
			case rdoModelObjects::FUN: calc = new rdoRuntime::RDOCalcFuncParam( parser->getLastFUNFunction()->findFUNFunctionParamNum( s ) ); break;
		}
		if ( calc ) calc->setErrorPos( error_pos );
		return;
	} else {
		// ��� �� ��������
		type = RDORTPResParam::pt_str;
		str  = s;
		return;
	}
}

RDOFUNArithm* RDOFUNArithm::operator +( RDOFUNArithm& second )
{
	RDORTPResParam::ParamType newType;

	if ( type == RDORTPResParam::pt_int && second.type == RDORTPResParam::pt_int ) {
		newType = RDORTPResParam::pt_int;
	} else if ( type == RDORTPResParam::pt_enum || second.type == RDORTPResParam::pt_enum ) {
		if ( type == RDORTPResParam::pt_enum ) {
			parser->lexer_loc_set( error().last_line, error().last_column );
		} else {
			parser->lexer_loc_set( second.error().last_line, second.error().last_column );
		}
		parser->error( "������ ���������� ������������ ������" );
//		parser->error("cannot add enumerative types");
	} else if ( type == RDORTPResParam::pt_str || second.type == RDORTPResParam::pt_str ) {
		if ( type == RDORTPResParam::pt_str ) {
			parser->lexer_loc_set( error().last_line, error().last_column );
			parser->error( rdo::format("����������� �������������: %s", str->c_str()) );
		} else {
			parser->lexer_loc_set( second.error().last_line, second.error().last_column );
			parser->error( rdo::format("����������� �������������: %s", second.str->c_str()) );
		}
	} else {
		newType = RDORTPResParam::pt_real;
	}

	RDOFUNArithm* arithm = new RDOFUNArithm( this, newType, new rdoRuntime::RDOCalcPlus( calc, second.calc ), error() );
	arithm->setErrorPos( error().first_line, error().first_column, second.error().last_line, second.error().last_column );
	return arithm;
}

RDOFUNArithm* RDOFUNArithm::operator -( RDOFUNArithm& second )
{
	RDORTPResParam::ParamType newType;

	if ( type == RDORTPResParam::pt_int && second.type == RDORTPResParam::pt_int ) {
		newType = RDORTPResParam::pt_int;
	} else if ( type == RDORTPResParam::pt_enum || second.type == RDORTPResParam::pt_enum ) {
		if ( type == RDORTPResParam::pt_enum ) {
			parser->lexer_loc_set( error().last_line, error().last_column );
		} else {
			parser->lexer_loc_set( second.error().last_line, second.error().last_column );
		}
		parser->error( "������ �������� ������������ ������" );
//		parser->error("cannot subtract enumerative types");
	} else if ( type == RDORTPResParam::pt_str || second.type == RDORTPResParam::pt_str ) {
		if ( type == RDORTPResParam::pt_str ) {
			parser->lexer_loc_set( error().last_line, error().last_column );
			parser->error( rdo::format("����������� �������������: %s", str->c_str()) );
		} else {
			parser->lexer_loc_set( second.error().last_line, second.error().last_column );
			parser->error( rdo::format("����������� �������������: %s", second.str->c_str()) );
		}
	} else {
		newType = RDORTPResParam::pt_real;
	}

	RDOFUNArithm* arithm = new RDOFUNArithm( this, newType, new rdoRuntime::RDOCalcMinus( calc, second.calc ), error() );
	arithm->setErrorPos( error().first_line, error().first_column, second.error().last_line, second.error().last_column );
	return arithm;
}

RDOFUNArithm* RDOFUNArithm::operator *( RDOFUNArithm& second )
{
	RDORTPResParam::ParamType newType;

	if ( type == RDORTPResParam::pt_int && second.type == RDORTPResParam::pt_int ) {
		newType = RDORTPResParam::pt_int;
	} else if ( type == RDORTPResParam::pt_enum || second.type == RDORTPResParam::pt_enum ) {
		if ( type == RDORTPResParam::pt_enum ) {
			parser->lexer_loc_set( error().last_line, error().last_column );
		} else {
			parser->lexer_loc_set( second.error().last_line, second.error().last_column );
		}
		parser->error( "������ ����������� ������������ ������" );
//		parser->error("cannot multiply enumerative types");
	} else if ( type == RDORTPResParam::pt_str || second.type == RDORTPResParam::pt_str ) {
		if ( type == RDORTPResParam::pt_str ) {
			parser->lexer_loc_set( error().last_line, error().last_column );
			parser->error( rdo::format("����������� �������������: %s", str->c_str()) );
		} else {
			parser->lexer_loc_set( second.error().last_line, second.error().last_column );
			parser->error( rdo::format("����������� �������������: %s", second.str->c_str()) );
		}
	} else {
		newType = RDORTPResParam::pt_real;
	}

	rdoRuntime::RDOCalc* newCalc = new rdoRuntime::RDOCalcMult( calc, second.calc );
	RDOFUNArithm* arithm = new RDOFUNArithm( this, newType, newCalc, error() );
	arithm->setErrorPos( error().first_line, error().first_column, second.error().last_line, second.error().last_column );
	newCalc->setErrorPos( arithm->error() );
	return arithm;
}

RDOFUNArithm* RDOFUNArithm::operator /( RDOFUNArithm& second )
{
	RDORTPResParam::ParamType newType;

	if ( type == RDORTPResParam::pt_int && second.type == RDORTPResParam::pt_int ) {
		newType = RDORTPResParam::pt_int;
	} else if ( type == RDORTPResParam::pt_enum || second.type == RDORTPResParam::pt_enum ) {
		if ( type == RDORTPResParam::pt_enum ) {
			parser->lexer_loc_set( error().last_line, error().last_column );
		} else {
			parser->lexer_loc_set( second.error().last_line, second.error().last_column );
		}
		parser->error( "������ ��������� ������������ ������" );
//		parser->error("cannot divide enumerative types");
	} else if ( type == RDORTPResParam::pt_str || second.type == RDORTPResParam::pt_str ) {
		if ( type == RDORTPResParam::pt_str ) {
			parser->lexer_loc_set( error().last_line, error().last_column );
			parser->error( rdo::format("����������� �������������: %s", str->c_str()) );
		} else {
			parser->lexer_loc_set( second.error().last_line, second.error().last_column );
			parser->error( rdo::format("����������� �������������: %s", second.str->c_str()) );
		}
	} else {
		newType = RDORTPResParam::pt_real;
	}

	rdoRuntime::RDOCalc* newCalc = new rdoRuntime::RDOCalcDiv( calc, second.calc );
	rdoRuntime::RDOCalc* newCalc_div = newCalc;
	if ( newType == RDORTPResParam::pt_int ) {
		newCalc = new rdoRuntime::RDOCalcDoubleToInt( newCalc );
	}
	RDOFUNArithm* arithm = new RDOFUNArithm( this, newType, newCalc, error() );
	arithm->setErrorPos( error().first_line, error().first_column, second.error().last_line, second.error().last_column );
	newCalc->setErrorPos( arithm->error() );
	newCalc_div->setErrorPos( arithm->error() );
	return arithm;
}

RDOFUNLogic* RDOFUNArithm::operator <( RDOFUNArithm& second )
{
	if ( type == RDORTPResParam::pt_enum || second.type == RDORTPResParam::pt_enum ) {
		if ( type == RDORTPResParam::pt_enum ) {
			parser->lexer_loc_set( error().last_line, error().last_column );
		} else {
			parser->lexer_loc_set( second.error().last_line, second.error().last_column );
		}
		parser->error( "������ ���������� ������������ ������" );
//		parser->error("cannot compare enumerative types");
	} else if ( type == RDORTPResParam::pt_str || second.type == RDORTPResParam::pt_str ) {
		if ( type == RDORTPResParam::pt_str ) {
			parser->lexer_loc_set( error().last_line, error().last_column );
			parser->error( rdo::format("����������� �������������: %s", str->c_str()) );
		} else {
			parser->lexer_loc_set( second.error().last_line, second.error().last_column );
			parser->error( rdo::format("����������� �������������: %s", second.str->c_str()) );
		}
	}
	rdoRuntime::RDOCalc* newCalc = new rdoRuntime::RDOCalcIsLess( calc, second.calc );
	RDOFUNLogic* logic = new RDOFUNLogic( newCalc );
	logic->setErrorPos( error().first_line, error().first_column, second.error().last_line, second.error().last_column );
	newCalc->setErrorPos( logic->error() );
	return logic;
}

RDOFUNLogic* RDOFUNArithm::operator >( RDOFUNArithm& second )
{
	if ( type == RDORTPResParam::pt_enum || second.type == RDORTPResParam::pt_enum ) {
		if ( type == RDORTPResParam::pt_enum ) {
			parser->lexer_loc_set( error().last_line, error().last_column );
		} else {
			parser->lexer_loc_set( second.error().last_line, second.error().last_column );
		}
		parser->error( "������ ���������� ������������ ������" );
	} else if ( type == RDORTPResParam::pt_str || second.type == RDORTPResParam::pt_str ) {
		if ( type == RDORTPResParam::pt_str ) {
			parser->lexer_loc_set( error().last_line, error().last_column );
			parser->error( rdo::format("����������� �������������: %s", str->c_str()) );
		} else {
			parser->lexer_loc_set( second.error().last_line, second.error().last_column );
			parser->error( rdo::format("����������� �������������: %s", second.str->c_str()) );
		}
	}
	rdoRuntime::RDOCalc* newCalc = new rdoRuntime::RDOCalcIsGreater( calc, second.calc );
	RDOFUNLogic* logic = new RDOFUNLogic( newCalc );
	logic->setErrorPos( error().first_line, error().first_column, second.error().last_line, second.error().last_column );
	newCalc->setErrorPos( logic->error() );
	return logic;
}

RDOFUNLogic* RDOFUNArithm::operator <=( RDOFUNArithm& second )
{
	if ( type == RDORTPResParam::pt_enum || second.type == RDORTPResParam::pt_enum ) {
		if ( type == RDORTPResParam::pt_enum ) {
			parser->lexer_loc_set( error().last_line, error().last_column );
		} else {
			parser->lexer_loc_set( second.error().last_line, second.error().last_column );
		}
		parser->error( "������ ���������� ������������ ������" );
	} else if ( type == RDORTPResParam::pt_str || second.type == RDORTPResParam::pt_str ) {
		if ( type == RDORTPResParam::pt_str ) {
			parser->lexer_loc_set( error().last_line, error().last_column );
			parser->error( rdo::format("����������� �������������: %s", str->c_str()) );
		} else {
			parser->lexer_loc_set( second.error().last_line, second.error().last_column );
			parser->error( rdo::format("����������� �������������: %s", second.str->c_str()) );
		}
	}
	rdoRuntime::RDOCalc* newCalc = new rdoRuntime::RDOCalcIsLEQ( calc, second.calc );
	RDOFUNLogic* logic = new RDOFUNLogic( newCalc );
	logic->setErrorPos( error().first_line, error().first_column, second.error().last_line, second.error().last_column );
	newCalc->setErrorPos( logic->error() );
	return logic;
}

RDOFUNLogic* RDOFUNArithm::operator >=( RDOFUNArithm& second )
{
	if ( type == RDORTPResParam::pt_enum || second.type == RDORTPResParam::pt_enum ) {
		if ( type == RDORTPResParam::pt_enum ) {
			parser->lexer_loc_set( error().last_line, error().last_column );
		} else {
			parser->lexer_loc_set( second.error().last_line, second.error().last_column );
		}
		parser->error( "������ ���������� ������������ ������" );
	} else if ( type == RDORTPResParam::pt_str || second.type == RDORTPResParam::pt_str ) {
		if ( type == RDORTPResParam::pt_str ) {
			parser->lexer_loc_set( error().last_line, error().last_column );
			parser->error( rdo::format("����������� �������������: %s", str->c_str()) );
		} else {
			parser->lexer_loc_set( second.error().last_line, second.error().last_column );
			parser->error( rdo::format("����������� �������������: %s", second.str->c_str()) );
		}
	}
	rdoRuntime::RDOCalc* newCalc = new rdoRuntime::RDOCalcIsGEQ( calc, second.calc );
	RDOFUNLogic* logic = new RDOFUNLogic( newCalc );
	logic->setErrorPos( error().first_line, error().first_column, second.error().last_line, second.error().last_column );
	newCalc->setErrorPos( logic->error() );
	return logic;
}

RDOFUNLogic* RDOFUNArithm::operator ==( RDOFUNArithm& second )
{
	if ( type == RDORTPResParam::pt_enum && second.type == RDORTPResParam::pt_enum ) {
		if ( enu != second.enu ) {
			parser->lexer_loc_set( second.error().last_line, second.error().last_column );
			parser->error( "������ ���������� ������ ������������ ����" );
//			parser->error("cannot compare different enumerative types");
		}
	} else if ( type == RDORTPResParam::pt_enum && second.type == RDORTPResParam::pt_str ) {
		second.calc = new rdoRuntime::RDOCalcConst( enu->findValue(second.str) );
//	} else if ( (type >= 2) || (second.type >= 2) ) {
	} else if ( (type == RDORTPResParam::pt_str && second.type == RDORTPResParam::pt_enum) || (type == RDORTPResParam::pt_str && second.type == RDORTPResParam::pt_str) ) {
		parser->lexer_loc_set( error().last_line, error().last_column );
		parser->error( rdo::format("����������� �������������: %s", str->c_str()) );
//		parser->error("cannot compare enumerative type with nonenumerative type");
	}

	rdoRuntime::RDOCalc* newCalc = new rdoRuntime::RDOCalcIsEqual( calc, second.calc );
	RDOFUNLogic* logic = new RDOFUNLogic( newCalc );
	logic->setErrorPos( error().first_line, error().first_column, second.error().last_line, second.error().last_column );
	newCalc->setErrorPos( logic->error() );
	return logic;
}

RDOFUNLogic* RDOFUNArithm::operator !=( RDOFUNArithm& second )
{
	if ( type == RDORTPResParam::pt_enum && second.type == RDORTPResParam::pt_enum ) {
		if ( enu != second.enu ) {
			parser->lexer_loc_set( second.error().last_line, second.error().last_column );
			parser->error( "������ ���������� ������ ������������ ����" );
		}
	} else if ( type == RDORTPResParam::pt_enum && second.type == RDORTPResParam::pt_str ) {
		second.calc = new rdoRuntime::RDOCalcConst( enu->findValue(second.str) );
	} else if ( (type == RDORTPResParam::pt_str && second.type == RDORTPResParam::pt_enum) || (type == RDORTPResParam::pt_str && second.type == RDORTPResParam::pt_str) ) {
		parser->lexer_loc_set( error().last_line, error().last_column );
		parser->error( rdo::format("����������� �������������: %s", str->c_str()) );
	}

	rdoRuntime::RDOCalc* newCalc = new rdoRuntime::RDOCalcIsNotEqual( calc, second.calc );
	RDOFUNLogic* logic = new RDOFUNLogic( newCalc );
	logic->setErrorPos( error().first_line, error().first_column, second.error().last_line, second.error().last_column );
	newCalc->setErrorPos( logic->error() );
	return logic;
}

rdoRuntime::RDOCalc* RDOFUNArithm::createCalc( const RDORTPResParam* const forType )
{
	if ( type != RDORTPResParam::pt_str ) {
		if ( forType == NULL ) {
			return calc;
		}
		if ( forType->getType() != RDORTPResParam::pt_int ) {
			return calc;
		}
		rdoRuntime::RDOCalc* newCalc = new rdoRuntime::RDOCalcInt( calc );
		newCalc->setErrorPos( calc->error() );
		return newCalc;
	} else if ( type == RDORTPResParam::pt_str && !forType && str ) {
		parser->lexer_loc_set( error().last_line, error().last_column );
		parser->error( rdo::format( "����������� �������������: %s", str->c_str()) );
	}

	if ( !forType ) {
		parser->lexer_loc_set( error().last_line, error().last_column );
		parser->error( "����������� ��� ���������" );
	}

	parser->lexer_loc_backup();
	parser->lexer_loc_set( error().last_line, error().last_column );
	rdoRuntime::RDOCalc* newCalc = new rdoRuntime::RDOCalcConst( forType->getRSSEnumValue(str) );
	parser->lexer_loc_restore();
	newCalc->setErrorPos( error() );
	return newCalc;
}

void RDOFUNArithm::setErrorPos( const YYLTYPE& error_pos )
{
	RDOErrorPos::setErrorPos( error_pos );
	if ( calc ) calc->setErrorPos( error_pos );
}

void RDOFUNArithm::setErrorPos( int first_line, int first_column, int last_line, int last_column )
{
	YYLTYPE error;
	error.first_line   = first_line;
	error.first_column = first_column;
	error.last_line    = last_line;
	error.last_column  = last_column;
	setErrorPos( error );
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNParams
// ----------------------------------------------------------------------------
const RDOFUNArithm* RDOFUNParams::createSeqCall( const std::string* const seqName ) const
{
	const RDOFUNSequence* const seq = parser->findSequence( seqName );
	if ( !seq ) {
		parser->lexer_loc_set( error().last_line, error().last_column );
//		parser->lexer_loc_set( name_error_pos.error().last_line, name_error_pos.error().last_column );
		parser->error( rdo::format("�������������� ������� ��� ������������������: %s", seqName->c_str()) );
//		parser->error( "Undefined function or sequence: " + *seqName );
	}
	parser->lexer_loc_backup();
	parser->lexer_loc_set( error().last_line, error().last_column );
	const RDOFUNArithm* arithm = seq->createCallCalc( this );
	parser->lexer_loc_restore();
	return arithm;
}

const RDOFUNArithm *RDOFUNSequenceUniform::createCallCalc( const RDOFUNParams* const param ) const
{
	if ( param->params.size() != 2 ) {
		parser->error( rdo::format("��� ������������ ������ ������������� '%s' ����� ������� ��� ���������: ����������� � ������������ ������� ���������", header->name->c_str()) );
//		parser->error("Wrong parameters number in uniform sequence call: " + *header->name);
	}

	rdoRuntime::RDOCalcFunctionCall *funcCall = new rdoRuntime::RDOCalcFunctionCall(next);
	RDOFUNArithm *arithm1 = param->params[0];
	rdoRuntime::RDOCalc *arg1 = arithm1->createCalc(NULL);
	RDOFUNArithm *arithm2 = param->params[1];
	rdoRuntime::RDOCalc *arg2 = arithm2->createCalc(NULL);

	funcCall->addParameter(arg1);
	funcCall->addParameter(arg2);

	RDOFUNArithm *res = new RDOFUNArithm( const_cast<RDOFUNSequenceUniform*>(this), RDORTPResParam::pt_int, funcCall, param->error() );
	res->type = header->type->getType();
	if ( res->type == RDORTPResParam::pt_enum ) {
		parser->error( "���������� ������ �������" );
//		parser->error("Internal parser error");
	}
	return res;
}

const RDOFUNArithm *RDOFUNSequenceExponential::createCallCalc( const RDOFUNParams* const param ) const
{
	if ( param->params.size() != 1 ) {
		parser->error( rdo::format("��� ����������������� ������ ������������� '%s' ����� ������� ���� ��������: �������������� ��������", header->name->c_str()) );
//		parser->error("Wrong parameters number in exponential sequence call: " + *header->name);
	}

	rdoRuntime::RDOCalcFunctionCall *funcCall = new rdoRuntime::RDOCalcFunctionCall(next);
	RDOFUNArithm *arithm1 = param->params[0];
	rdoRuntime::RDOCalc *arg1 = arithm1->createCalc(NULL);

	funcCall->addParameter(arg1);

	RDOFUNArithm* res = new RDOFUNArithm( const_cast<RDOFUNSequenceExponential*>(this), RDORTPResParam::pt_int, funcCall, param->error() );
	res->type = header->type->getType();
	if ( res->type == RDORTPResParam::pt_enum ) {
		parser->error( "���������� ������ �������" );
//		parser->error("Internal parser error");
	}
	return res;
}

const RDOFUNArithm* RDOFUNSequenceNormal::createCallCalc( const RDOFUNParams* const param ) const
{
	if ( param->params.size() != 2 ) {
		parser->error( rdo::format("��� ����������� ������ ������������� '%s' ����� ������� ��� ���������: �������������� �������� � ������� �������������� ����������", header->name->c_str()) );
//		parser->error("Wrong parameters number in normal sequence call: " + *header->name);
	}

	rdoRuntime::RDOCalcFunctionCall *funcCall = new rdoRuntime::RDOCalcFunctionCall(next);
	RDOFUNArithm *arithm1 = param->params[0];
	rdoRuntime::RDOCalc *arg1 = arithm1->createCalc(NULL);
	RDOFUNArithm *arithm2 = param->params[1];
	rdoRuntime::RDOCalc *arg2 = arithm2->createCalc(NULL);

	funcCall->addParameter(arg1);
	funcCall->addParameter(arg2);

	RDOFUNArithm* res = new RDOFUNArithm( const_cast<RDOFUNSequenceNormal*>(this), RDORTPResParam::pt_int, funcCall, param->error() );
	res->type = header->type->getType();
	if ( res->type == RDORTPResParam::pt_enum ) {
		parser->error( "���������� ������ �������" );
//		parser->error("Internal parser error");
	}
	return res;
}

const RDOFUNArithm *RDOFUNSequenceByHist::createCallCalc(const RDOFUNParams *const param) const
{
	if ( param->params.size() != 0 ) {
		parser->error( rdo::format("����������� '%s' ������ ���������� ��� ����������", header->name->c_str()) );
//		parser->error("Wrong parameters number in by_hist sequence call: " + *header->name);
	}

	rdoRuntime::RDOCalcFunctionCall *funcCall = new rdoRuntime::RDOCalcFunctionCall(next);

	RDOFUNArithm* res = new RDOFUNArithm( const_cast<RDOFUNSequenceByHist*>(this), RDORTPResParam::pt_int, funcCall, param->error() );
	res->type = header->type->getType();
	if(res->type == RDORTPResParam::pt_enum)
		res->enu = ((RDORTPEnumResParam *)header->type)->enu;

	return res;
}

const RDOFUNArithm *RDOFUNSequenceEnumerative::createCallCalc(const RDOFUNParams *const param) const
{
	if ( param->params.size() != 0 ) {
		parser->error( rdo::format("������������ '%s' ������ ���������� ��� ����������", header->name->c_str()) );
//		parser->error("Wrong parameters number in enumerative sequence call: " + *header->name);
	}

	rdoRuntime::RDOCalcFunctionCall *funcCall = new rdoRuntime::RDOCalcFunctionCall(next);

	RDOFUNArithm* res = new RDOFUNArithm( const_cast<RDOFUNSequenceEnumerative*>(this), RDORTPResParam::pt_int, funcCall, param->error() );
	res->type = header->type->getType();
	if(res->type == RDORTPResParam::pt_enum)
		res->enu = ((RDORTPEnumResParam *)header->type)->enu;

	return res;
}

const RDOFUNArithm* RDOFUNParams::createCall( const std::string* const funName ) const
{
	const RDOFUNFunction* const func = parser->findFunction( funName );
	if ( !func ) {
		return createSeqCall( funName );
	}

	int nParams = func->getParams().size();
	if ( nParams != params.size() ) {
		parser->lexer_loc_set( error().last_line, error().last_column );
		parser->error( rdo::format("�������� ���������� ���������� �������: %s", funName->c_str()) );
//		parser->error(("Wrong parameters number in function call: " + *funName).c_str());
	}

	rdoRuntime::RDOCalcFunctionCall* funcCall = new rdoRuntime::RDOCalcFunctionCall( func->getFunctionCalc() );
	funcCall->setErrorPos( error() );
	for ( int i = 0; i < nParams; i++ ) {
		const RDORTPResParam* const funcParam = func->getParams()[i]->getType();
		RDOFUNArithm* arithm = params[i];
		switch ( funcParam->getType() ) {
			case RDORTPResParam::pt_int: {
				rdoRuntime::RDOCalc* arg = arithm->createCalc( NULL );
				if ( arithm->type == RDORTPResParam::pt_real ) {
					arg = new rdoRuntime::RDOCalcDoubleToInt( arg );
				}
				if ( static_cast<const RDORTPIntResParam*>(funcParam)->diap->exist ) {
					arg = new rdoRuntime::RDOCalcCheckDiap( static_cast<const RDORTPIntResParam*>(funcParam)->diap->minVal, static_cast<const RDORTPIntResParam*>(funcParam)->diap->maxVal, arg );
				}
				funcCall->addParameter( arg );
				break;
			}
			case RDORTPResParam::pt_real: {
				rdoRuntime::RDOCalc* arg = arithm->createCalc( NULL );
				if( static_cast<const RDORTPRealResParam*>(funcParam)->diap->exist ) {
					arg = new rdoRuntime::RDOCalcCheckDiap( static_cast<const RDORTPRealResParam*>(funcParam)->diap->minVal, static_cast<const RDORTPRealResParam*>(funcParam)->diap->maxVal, arg );
				}
				funcCall->addParameter(arg);
				break;
			}
			default: {
				if ( arithm->type == RDORTPResParam::pt_int || arithm->type == RDORTPResParam::pt_real ) {
					parser->lexer_loc_set( arithm->error().last_line, arithm->error().last_column );
					parser->error( "�������� ��� ��������� �������" );
//					parser->error("wrong parameter type");
				}

				rdoRuntime::RDOCalc* arg;
				if ( arithm->type == RDORTPResParam::pt_enum ) {
					if ( arithm->enu != static_cast<const RDORTPEnumResParam*>(funcParam)->enu ) {
						parser->error( "�������� ������������ ���" );
//						parser->error("wrong enumerative parameter type");
					}
					arg = arithm->createCalc( NULL );
				} else {
					arg = new rdoRuntime::RDOCalcConst( static_cast<const RDORTPEnumResParam*>(funcParam)->enu->findValue(arithm->str) );
					arg->setErrorPos( arithm->error() );
				}
				funcCall->addParameter(arg);
				break;
			}
		}
	}

	RDOFUNArithm* res = new RDOFUNArithm( this, RDORTPResParam::pt_int, funcCall, error() );
	res->type = func->getType()->getType();
	if ( func->getType()->getType() == RDORTPResParam::pt_enum ) {
		res->enu = ((RDORTPEnumResParam *)func->getType())->enu;
	}

	return res;
}

void RDOFUNFunction::createAlgorithmicCalc()
{
	rdoRuntime::RDOFunAlgorithmicCalc *funcCalc = new rdoRuntime::RDOFunAlgorithmicCalc();
	int size = calculateIf.size();
	if ( !size ) {
		parser->warning( "����������� ���� �������" );
	}
	for ( int i = 0; i < size; i++ ) {
		funcCalc->addCalcIf(calculateIf[i]->condition->calc, calculateIf[i]->action->createCalc(getType()));
	}
	functionCalc = funcCalc;
}

RDOFUNCalculateIf::RDOFUNCalculateIf(RDOFUNLogic *_condition, std::string *_funName, RDOFUNArithm *_action):
	condition (_condition),
	funName	 (_funName),
	action	 (_action)
{
	if( *funName != *(parser->getLastFUNFunction()->getName()) ) {
		parser->error( rdo::format("��������� ��� �������: %s", parser->getLastFUNFunction()->getName()->c_str()) );
//		parser->error( "function name expected" );
	}
	parser->getLastFUNFunction()->getType()->checkParamType( action );
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNGroup
// ----------------------------------------------------------------------------
RDOFUNGroup::RDOFUNGroup( RDOParser* _parser, const std::string* const _resType ):
	RDOParserObject( _parser )
{
	init( _resType );
}

RDOFUNGroup::RDOFUNGroup( const RDOParserObject* _parent, const std::string* const _resType ):
	RDOParserObject( _parent )
{
	init( _resType );
}

void RDOFUNGroup::init( const std::string* const _resType )
{
	resType = parser->findRTPResType( _resType );
	if ( !resType ) {
		parser->error( rdo::format("����������� ��� �������: %s", _resType->c_str()) );
	}
	parser->insertFUNGroup( this );
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNGroupLogic
// ----------------------------------------------------------------------------
RDOFUNLogic* RDOFUNGroupLogic::createFunLogic()
{
	RDOFUNLogic* trueLogic = new RDOFUNLogic( new rdoRuntime::RDOCalcConst(1) );
	return createFunLogic( trueLogic );
}

RDOFUNLogic* RDOFUNGroupLogic::createFunLogic( RDOFUNLogic* cond )
{
	rdoRuntime::RDOFunCalcGroup* calc;
	switch ( funType ) {
		case 1 : calc = new rdoRuntime::RDOFunCalcExist    ( resType->getNumber(), cond->calc ); break;
		case 2 : calc = new rdoRuntime::RDOFunCalcNotExist ( resType->getNumber(), cond->calc ); break;
		case 3 : calc = new rdoRuntime::RDOFunCalcForAll   ( resType->getNumber(), cond->calc ); break;
		case 4 : calc = new rdoRuntime::RDOFunCalcNotForAll( resType->getNumber(), cond->calc ); break;
		default: parser->error( "�������������� ��� �������" );
	}
	parser->getFUNGroupStack().pop_back();
	return new RDOFUNLogic( calc );
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSelect
// ----------------------------------------------------------------------------
RDOFUNLogic* RDOFUNSelect::createFunSelect( RDOFUNLogic* cond )
{
	select = new rdoRuntime::RDOFunCalcSelect( resType->getNumber(), cond ? cond->calc : new rdoRuntime::RDOCalcConst(1) );
	return new RDOFUNLogic( select );
}

RDOFUNLogic* RDOFUNSelect::createFunSelectGroup( int funType, RDOFUNLogic* cond )
{
	rdoRuntime::RDOFunCalcSelectBase* calc;
	switch ( funType ) {
		case 1 : calc = new rdoRuntime::RDOFunCalcSelectExist    ( select, cond->calc ); break;
		case 2 : calc = new rdoRuntime::RDOFunCalcSelectNotExist ( select, cond->calc ); break;
		case 3 : calc = new rdoRuntime::RDOFunCalcSelectForAll   ( select, cond->calc ); break;
		case 4 : calc = new rdoRuntime::RDOFunCalcSelectNotForAll( select, cond->calc ); break;
		default: parser->error( "����������� ����� ��� ������ ��������" );
	}
	parser->getFUNGroupStack().pop_back();
	return new RDOFUNLogic( calc );
}

RDOFUNLogic* RDOFUNSelect::createFunSelectEmpty()
{
	parser->getFUNGroupStack().pop_back();
	return new RDOFUNLogic( select );
}

RDOFUNArithm* RDOFUNSelect::createFunSelectSize()
{
	parser->getFUNGroupStack().pop_back();
	return new RDOFUNArithm( this, RDORTPResParam::pt_int, new rdoRuntime::RDOFunCalcSelectSize( select ), error() );
}

void RDOFUNSelect::setErrorPos( const YYLTYPE& error_pos )
{
	RDOErrorPos::setErrorPos( error_pos );
	if ( select ) select->setErrorPos( error_pos );
}

void RDOFUNSelect::setErrorPos( int first_line, int first_column, int last_line, int last_column )
{
	YYLTYPE error;
	error.first_line   = first_line;
	error.first_column = first_column;
	error.last_line    = last_line;
	error.last_column  = last_column;
	setErrorPos( error );
}

// ----------------------------------------------------------------------------
// ---------- Sequences
// ----------------------------------------------------------------------------
RDOFUNSequenceUniform::RDOFUNSequenceUniform( RDOParser* _parser, RDOFUNSequenceHeader* _header, int _base ):
	RDOFUNSequence( _parser, _header, _base )
{
	if ( header->type->getType() == RDORTPResParam::pt_enum ) {
		parser->error( "������������������ ���� uniform �� ����� ����� ������������ ���" );
//		parser->error("Uniform sequence cannot be enumerative type");
	}
	createCalcs();
}

void RDOFUNSequenceUniform::createCalcs()
{
	RandGeneratorUniform* gen = new RandGeneratorUniform();
	initSeq = new rdoRuntime::RDOCalcSeqInit( base, gen );
	parser->runTime->addInitCalc( initSeq );
	next = new rdoRuntime::RDOCalcSeqNextUniform( gen );
	initResult();
}

RDOFUNSequenceExponential::RDOFUNSequenceExponential( RDOParser* _parser, RDOFUNSequenceHeader *_header, int _base ):
	RDOFUNSequence( _parser, _header, _base )
{
	if ( header->type->getType() == RDORTPResParam::pt_enum ) {
		parser->error( "������������������ ���� exponential �� ����� ����� ������������ ���" );
//		parser->error("Exponential sequence cannot be enumerative type");
	}

	createCalcs();
}

void RDOFUNSequenceExponential::createCalcs()
{
	RandGeneratorExponential* gen = new RandGeneratorExponential();
	initSeq = new rdoRuntime::RDOCalcSeqInit( base, gen );
	parser->runTime->addInitCalc( initSeq );
	next = new rdoRuntime::RDOCalcSeqNextExponential( gen );
	initResult();
}

RDOFUNSequenceNormal::RDOFUNSequenceNormal( RDOParser* _parser, RDOFUNSequenceHeader* _header, int _base ):
	RDOFUNSequence( _parser, _header, _base )
{
	if ( header->type->getType() == RDORTPResParam::pt_enum ) {
		parser->error( "������������������ ���� normal �� ����� ����� ������������ ���" );
//		parser->error("Normal sequence cannot be enumerative type");
	}
	createCalcs();
}

void RDOFUNSequenceNormal::createCalcs()
{
	RandGeneratorNormal* gen = new RandGeneratorNormal();
	initSeq = new rdoRuntime::RDOCalcSeqInit( base, gen );
	parser->runTime->addInitCalc( initSeq );
	next = new rdoRuntime::RDOCalcSeqNextNormal( gen );
	initResult();
}

void RDOFUNSequenceByHistReal::addReal( double _from, double _to, double _freq )
{
	from.push_back( _from );
	to.push_back( _to );
	freq.push_back( _freq );
}

void RDOFUNSequenceByHistReal::createCalcs()
{
	RandGeneratorByHist *gen = new RandGeneratorByHist();
	int size = from.size();
	for(int i = 0; i < size; i++)
		gen->addValues(from[i], to[i], freq[i]);

	initSeq = new rdoRuntime::RDOCalcSeqInit(base, gen);
	parser->runTime->addInitCalc(initSeq);
	next = new rdoRuntime::RDOCalcSeqNextByHist(gen);
	initResult();
}

void RDOFUNSequenceByHistEnum::addEnum( std::string* _val, double _freq )
{
	rdoRuntime::RDOValue enum_id = header->type->getRSSEnumValue( _val );
	if ( std::find( val.begin(), val.end(), enum_id ) != val.end() ) {
		parser->error( rdo::format("������������ �������� ���������� ������: %s", _val->c_str()) );
	}
	val.push_back( enum_id );
	freq.push_back( _freq );
}

void RDOFUNSequenceByHistEnum::createCalcs()
{
	RandGeneratorByHistEnum* gen = new RandGeneratorByHistEnum();
	int size = val.size();
	for ( int i = 0; i < size; i++ ) {
		gen->addValues( val[i], freq[i] );
	}
	initSeq = new rdoRuntime::RDOCalcSeqInit( base, gen );
	parser->runTime->addInitCalc( initSeq );
	next = new rdoRuntime::RDOCalcSeqNextByHist( gen );
}

void RDOFUNSequenceEnumerativeInt::addInt(int _val)
{
	val.push_back(_val);
}

void RDOFUNSequenceEnumerativeInt::createCalcs()
{
	RandGeneratorEnumerative *gen = new RandGeneratorEnumerative();
	int size = val.size();
	for(int i = 0; i < size; i++)
		gen->addValue(val[i]);

	initSeq = new rdoRuntime::RDOCalcSeqInit(base, gen);
	parser->runTime->addInitCalc(initSeq);
	next = new rdoRuntime::RDOCalcSeqNextByHist(gen);
}

void RDOFUNSequenceEnumerativeReal::addReal(double * _val)
{
	val.push_back(*_val);
}

void RDOFUNSequenceEnumerativeReal::createCalcs()
{
	RandGeneratorEnumerative *gen = new RandGeneratorEnumerative();
	int size = val.size();
	for(int i = 0; i < size; i++)
		gen->addValue(val[i]);

	initSeq = new rdoRuntime::RDOCalcSeqInit(base, gen);
	parser->runTime->addInitCalc(initSeq);
	next = new rdoRuntime::RDOCalcSeqNextByHist(gen);
}

void RDOFUNSequenceEnumerativeEnum::addEnum( std::string* _val )
{
	val.push_back( header->type->getRSSEnumValue(_val) );
}

void RDOFUNSequenceEnumerativeEnum::createCalcs()
{
	RandGeneratorEnumerative *gen = new RandGeneratorEnumerative();
	int size = val.size();
	for(int i = 0; i < size; i++)
		gen->addValue(val[i]);

	initSeq = new rdoRuntime::RDOCalcSeqInit(base, gen);
	parser->runTime->addInitCalc(initSeq);
	next = new rdoRuntime::RDOCalcSeqNextByHist(gen);
}

RDOFUNSequence::RDOFUNSequence( RDOParser* _parser, RDOFUNSequenceHeader* _header, int _base ):
	RDOParserObject( _parser ),
	header( _header ),
	base( _base )
{
	parser->insertFUNSequences( this );
}

void RDOFUNSequence::initResult()
{
	switch ( header->type->getType() ) {
		case RDORTPResParam::pt_int: {
			next->res_real = false;
			if ( static_cast<RDORTPIntResParam*>(header->type)->diap->exist ) {
				next->diap     = true;
				next->diap_min = static_cast<RDORTPIntResParam*>(header->type)->diap->minVal;
				next->diap_max = static_cast<RDORTPIntResParam*>(header->type)->diap->maxVal;
			}
			break;
		}
		case RDORTPResParam::pt_real: {
			if ( static_cast<RDORTPRealResParam*>(header->type)->diap->exist ) {
				next->diap     = true;
				next->diap_min = static_cast<RDORTPRealResParam*>(header->type)->diap->minVal;
				next->diap_max = static_cast<RDORTPRealResParam*>(header->type)->diap->maxVal;
			}
			break;
		}
	}
}

} // namespace rdoParse
