#ifndef RDODPT_DPT
#define RDODPT_DPT

#include "rdo_lib/rdo_parser/rdo_object.h"
#include "rdo_lib/rdo_parser/rdo_logic.h"
#include "rdo_lib/rdo_parser/rdofun.h"
#include "rdo_lib/rdo_parser/rdopat.h"
#include "rdo_lib/rdo_runtime/rdoprocess.h"
#include "rdo_lib/rdo_runtime/rdo_activity.h"
#include "rdo_lib/rdo_runtime/searchtrace.h"
#include "rdo_lib/rdo_runtime/rdo_dptsearch_activity_interface.h"
#include "rdo_lib/rdo_mbuilder/rdo_resources.h"

namespace rdoRuntime
{
class RDOCalc;
class RDODPTFree;
class RDODPTSome;
class RDODPTPrior;
class RDODPTSearchRuntime;
class RDOPROCProcess;
class RDOPROCGenerate;
class RDOPROCSeize;
class RDOPROCTerminate;
class RDOPROCAdvance;
class RDOPROCRelease;
struct parser_for_Seize;
struct parser_for_Queue;
}

namespace rdoParse 
{

int  dptparse      (PTR(void) lexer);
int  dptlex        (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void dpterror      (PTR(char) mes);

int  proc_rtp_parse(PTR(void) lexer);
int  proc_rtp_lex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void proc_rtp_error(PTR(char) mes);

int  proc_rss_parse(PTR(void) lexer);
int  proc_rss_lex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void proc_rss_error(PTR(char) mes);

int  proc_opr_parse(PTR(void) lexer);
int  proc_opr_lex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void proc_opr_error(PTR(char) mes);

// ----------------------------------------------------------------------------
// ---------- RDODPTActivity
// ----------------------------------------------------------------------------
class RDODPTActivity: public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDODPTActivity( const RDOParserObject* _parent, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info );

	const std::string&       name() const     { return src_info().src_text(); }
	CREF(LPIActivity)        activity() const { return m_activity;            }
	const RDOPATPattern*     pattern() const  { return m_pattern;             }

	void addParam( const RDOValue& param  );
	void endParam( const YYLTYPE& _param_pos );

	bool setPrior( RDOFUNArithm* prior );

protected:
	LPIActivity           m_activity;

private:
	unsigned int          m_currParam;
	const RDOPATPattern*  m_pattern;
};

// ----------------------------------------------------------------------------
// ---------- RDODPTActivityHotKey
// ----------------------------------------------------------------------------
class RDODPTActivityHotKey: public RDODPTActivity
{
public:
	void addHotKey( const std::string& hotKey, const YYLTYPE& hotkey_pos );

protected:
	RDODPTActivityHotKey( LPIBaseOperationContainer dpt, const RDOParserObject* parent, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info );
};

// ----------------------------------------------------------------------------
// ---------- RDODPTFreeActivity
// ----------------------------------------------------------------------------
class RDODPTFreeActivity: public RDODPTActivityHotKey
{
friend class RDOLogicActivity<rdoRuntime::RDODPTFree, RDODPTFreeActivity>;
private:
	RDODPTFreeActivity( LPIBaseOperationContainer dpt, const RDOParserObject* parent, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info );
};

// ----------------------------------------------------------------------------
// ---------- RDODPTFree
// ----------------------------------------------------------------------------
class RDODPTFree: public RDOLogicActivity<rdoRuntime::RDODPTFree, RDODPTFreeActivity>
{
public:
	RDODPTFree( RDOParser* _parser, const RDOParserSrcInfo& _src_info );
};

// ----------------------------------------------------------------------------
// ---------- RDODPTSomeActivity
// ----------------------------------------------------------------------------
class RDODPTSomeActivity: public RDODPTActivityHotKey
{
friend class RDOLogicActivity<rdoRuntime::RDODPTSome, RDODPTSomeActivity>;
private:
	RDODPTSomeActivity( LPIBaseOperationContainer dpt, const RDOParserObject* _parent, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info );
};

// ----------------------------------------------------------------------------
// ---------- RDODPTPriorActivity
// ----------------------------------------------------------------------------
class RDODPTPriorActivity: public RDODPTActivityHotKey
{
friend class RDOLogicActivity<rdoRuntime::RDODPTPrior, RDODPTPriorActivity>;
private:
	RDODPTPriorActivity( LPIBaseOperationContainer dpt, const RDOParserObject* _parent, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info );
};

// ----------------------------------------------------------------------------
// ---------- RDODPTSome
// ----------------------------------------------------------------------------
class RDODPTSome: public RDOLogicActivity<rdoRuntime::RDODPTSome, RDODPTSomeActivity>
{
public:
	RDODPTSome( RDOParser* _parser, const RDOParserSrcInfo& _src_info, LPILogic _parent = NULL );

	LPILogic getLogic() const                         { return m_rt_logic;     }
	RDOFUNLogic* getConditon() const                  { return m_conditon;     }
	void setCondition( RDOFUNLogic* conditon = NULL ) { m_conditon = conditon; }

	void end();

private:
	RDOFUNLogic* m_conditon;
};

// ----------------------------------------------------------------------------
// ---------- RDODPTPrior
// ----------------------------------------------------------------------------
class RDODPTPrior: public RDOLogicActivity<rdoRuntime::RDODPTPrior, RDODPTPriorActivity>
{
public:
	RDODPTPrior( RDOParser* _parser, const RDOParserSrcInfo& _src_info, LPILogic _parent = NULL  );

	LPILogic getLogic() const                         { return m_rt_logic;     }
	RDOFUNLogic* getConditon() const                  { return m_conditon;     }
	void setCondition( RDOFUNLogic* conditon = NULL ) { m_conditon = conditon; }

	void end();

private:
	RDOFUNLogic* m_conditon;
};

// ----------------------------------------------------------------------------
// ---------- RDODPTSearchActivity
// ----------------------------------------------------------------------------
class RDODPTSearchActivity: public RDODPTActivity
{
friend class RDOLogicActivity<rdoRuntime::RDODPTSearchRuntime, RDODPTSearchActivity>;
public:
	IDPTSearchActivity::ValueTime getValue() const { return m_value; }
	void setValue( IDPTSearchActivity::ValueTime value, RDOFUNArithm* ruleCost, const YYLTYPE& _param_pos );

	RDOFUNArithm* getRuleCost() const { return m_ruleCost; }

private:
	RDODPTSearchActivity( LPIBaseOperationContainer dpt, const RDOParserObject* _parent, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info );

	IDPTSearchActivity::ValueTime  m_value;
	RDOFUNArithm*                  m_ruleCost;
};

// ----------------------------------------------------------------------------
// ---------- RDODPTSearch
// ----------------------------------------------------------------------------
class RDODPTSearch: public RDOLogicActivity<rdoRuntime::RDODPTSearchRuntime, RDODPTSearchActivity>
{
public:
	RDODPTSearch( RDOParser* _parser, const RDOParserSrcInfo& _src_info, rdoRuntime::RDODPTSearchTrace::DPT_TraceFlag trace = rdoRuntime::RDODPTSearchTrace::DPT_no_trace, LPILogic _parent = NULL );

	void setCondition( RDOFUNLogic* conditon = NULL )         { m_conditon     = conditon;     }
	void setTermCondition( RDOFUNLogic* termConditon = NULL ) { m_termConditon = termConditon; }
	void setEvaluateBy( RDOFUNArithm* evalBy )                { m_evalBy       = evalBy;       }
	void setCompareTops( bool compTops )                      { m_compTops     = compTops;     }

	void end();
	bool closed() const { return m_closed; }

private:
	RDOFUNLogic*                                  m_conditon;
	RDOFUNLogic*                                  m_termConditon;
	RDOFUNArithm*                                 m_evalBy;
	LPILogic                                      m_parent;
	bool                                          m_compTops;
	bool                                          m_closed;
	rdoRuntime::RDODPTSearchTrace::DPT_TraceFlag  m_trace;
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCProcess
// ----------------------------------------------------------------------------
class RDOPROCProcess: public RDOParserObject, public RDOParserSrcInfo
{
friend class RDOPROCOperator;

public:
	static std::string s_name_prefix;
	static std::string s_name_sufix;

	RDOPROCProcess(PTR(RDOParser) parser, CREF(RDOParserSrcInfo) info);

	void end();
	bool closed() const { return m_closed; }

	void setCondition( RDOFUNLogic* conditon = NULL ) { m_conditon = conditon; }
	RDOFUNLogic* getConditon() const                  { return m_conditon;     }

	bool setPrior(RDOFUNArithm* prior);

	void insertChild( RDOPROCProcess* value );

	LPILogic getRunTime() const { return m_runtime; }

protected:
	bool                          m_closed;
	RDOPROCProcess*               m_parent;
	std::list< RDOPROCProcess* >  m_child;
	std::list< RDOPROCOperator* > m_operations;
	LPILogic                      m_runtime;

private:
	RDOFUNLogic*                  m_conditon;
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCOperator
// ----------------------------------------------------------------------------
class RDOPROCOperator: public RDOParserObject
{
protected:
	std::string     name;
	RDOPROCProcess* process;

	RDOPROCOperator( RDOPROCProcess* _process, const std::string& _name );
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCGenerate
// ----------------------------------------------------------------------------
class RDOPROCGenerate: public RDOPROCOperator
{
protected:
	LPIPROCBlock runtime;

public:
	RDOPROCGenerate( RDOPROCProcess* _process, const std::string& _name, CREF(rdoRuntime::LPRDOCalc) time );
};
// ----------------------------------------------------------------------------
// ---------- RDOPROCBlockForQueue
// ----------------------------------------------------------------------------
class RDOPROCBlockForQueue: public RDOPROCOperator
{
protected:
	// ������� parser_for_runtime ������ ��� �������� ���������� � ��������� "���������" �������  
	rdoRuntime::parser_for_Queue parser_for_runtime;
public:
	RDOPROCBlockForQueue ( RDOPROCProcess* _process, const std::string& _name ): RDOPROCOperator( _process, _name ){	}
	static bool checkType( RDOParser *parser, rdoMBuilder::RDOResType rtp, const RDOParserSrcInfo& info );
	static void createRes( RDOParser *parser, rdoMBuilder::RDOResType rtp, const std::string& res_name );
	static rdoMBuilder::RDOResType createType( RDOParser *parser, const std::string& rtp_name, const RDOParserSrcInfo& info );
};
// ----------------------------------------------------------------------------
// ---------- RDOPROCQueue
// ----------------------------------------------------------------------------
class RDOPROCQueue: public RDOPROCBlockForQueue
{
protected:
	std::string Res;
	LPIPROCBlock runtime;
public:
	RDOPROCQueue( RDOPROCProcess* _process, const std::string& _name) : RDOPROCBlockForQueue( _process, _name ){}
	void create_runtime_Queue( RDOParser *parser );
	void add_Queue_Resource( std::string res_name)	{Res = res_name;}
};
// ----------------------------------------------------------------------------
// ---------- RDOPROCDepart
// ----------------------------------------------------------------------------
class RDOPROCDepart: public RDOPROCBlockForQueue
{
protected:
	std::string Res;
	LPIPROCBlock runtime;
public:
	RDOPROCDepart( RDOPROCProcess* _process, const std::string& _name) : RDOPROCBlockForQueue( _process, _name ){}
	void create_runtime_Depart( RDOParser *parser );
	void add_Depart_Resource( std::string res_name)	{Res = res_name;}
};
// ----------------------------------------------------------------------------
// ---------- RDOPROCBlockForSeize
// ----------------------------------------------------------------------------
class RDOPROCBlockForSeize: public RDOPROCOperator
{
protected:
	// ������� parser_for_runtime ������ ��� �������� ���������� � ��������� "���������" �������  
	rdoRuntime::parser_for_Seize parser_for_runtime;
public:
	RDOPROCBlockForSeize( RDOPROCProcess* _process, const std::string& _name ): RDOPROCOperator( _process, _name ){	}
	static bool checkType( RDOParser *parser, rdoMBuilder::RDOResType rtp, const RDOParserSrcInfo& info );
	static void createRes( RDOParser *parser, rdoMBuilder::RDOResType rtp, const std::string& res_name );
	static void reobjectRes( RDOParser *parser, rdoMBuilder::RDOResType rtp, const std::string& res_name );
	static rdoMBuilder::RDOResType createType( RDOParser *parser, const std::string& rtp_name, const RDOParserSrcInfo& info );
};
/*
// ----------------------------------------------------------------------------
// ---------- RDOPROCSeize
// ----------------------------------------------------------------------------
class RDOPROCSeize: public RDOPROCBlockForSeize
{
protected:
	std::string Res;
	rdoRuntime::RDOPROCSeize* runtime;
public:
	RDOPROCSeize( RDOPROCProcess* _process, const std::string& _name ): RDOPROCBlockForSeize( _process, _name ){}
	void create_runtime_Seize( RDOParser *parser );
	void add_Seize_Resource	( std::string res_name)		{Res = res_name;}
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCRelease
// ----------------------------------------------------------------------------
class RDOPROCRelease: public RDOPROCBlockForSeize
{
protected:
	std::string Res;
	rdoRuntime::RDOPROCRelease* runtime;
public:
	RDOPROCRelease( RDOPROCProcess* _process, const std::string& _name) : RDOPROCBlockForSeize( _process, _name ){}
	void create_runtime_Release( RDOParser *parser );
	void add_Release_Resource	( std::string res_name)		{Res = res_name;}
};
*/
// ----------------------------------------------------------------------------
// ---------- RDOPROCSeize
// ----------------------------------------------------------------------------
class RDOPROCSeize: public RDOPROCBlockForSeize
{
protected:
	std::list< std::string > Resources;
	std::vector< rdoRuntime::parser_for_Seize > parser_for_runtime;
	LPIPROCBlock runtime;
public:
	RDOPROCSeize              ( RDOPROCProcess* _process, const std::string& _name ): RDOPROCBlockForSeize( _process, _name ){}
	void create_runtime_Seize ( RDOParser *parser );
	void add_Seize_Resourse   ( std::string res_name){Resources.push_back( res_name );}	
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCRelease
// ----------------------------------------------------------------------------
class RDOPROCRelease: public RDOPROCBlockForSeize
{
protected:
	std::list< std::string > Resources;
	std::vector< rdoRuntime::parser_for_Seize > parser_for_runtime;
	LPIPROCBlock runtime;
public:
	RDOPROCRelease              ( RDOPROCProcess* _process, const std::string& _name ): RDOPROCBlockForSeize( _process, _name ){}
	void create_runtime_Release ( RDOParser *parser );
	void add_Release_Resourse   ( std::string res_name){Resources.push_back( res_name );}	
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCAdvance
// ----------------------------------------------------------------------------
class RDOPROCAdvance: public RDOPROCOperator
{
protected:
	LPIPROCBlock runtime;

public:
	RDOPROCAdvance( RDOPROCProcess* _process, const std::string& _name, CREF(rdoRuntime::LPRDOCalc) time );
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCTerminate
// ----------------------------------------------------------------------------
class RDOPROCTerminate: public RDOPROCOperator
{
protected:
	LPIPROCBlock runtime;

public:
	RDOPROCTerminate( RDOPROCProcess* _process, const std::string& _name, const unsigned int& _term);
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCAssign
// ----------------------------------------------------------------------------
class RDOPROCAssign: public RDOPROCOperator
{
protected:
	LPIPROCBlock runtime;

public:
	RDOPROCAssign( RDOPROCProcess* _process, const std::string& _name, CREF(rdoRuntime::LPRDOCalc) value, int Id_res, int Id_param );
};

} // namespace rdoParse

#endif // RDODPT_DPT
