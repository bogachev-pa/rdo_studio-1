/* A Bison parser, made from .\rdodpt.y
   by GNU bison 1.34.  */

#define YYBISON 1  /* Identify Bison output.  */

#define yyparse dptparse
#define yylex dptlex
#define yyerror dpterror
#define yylval dptlval
#define yychar dptchar
#define yydebug dptdebug
#define yynerrs dptnerrs
# define	Resource_type	257
# define	permanent	258
# define	Parameters	259
# define	integer	260
# define	real	261
# define	End	262
# define	temporary	263
# define	IDENTIF	264
# define	INT_CONST	265
# define	REAL_CONST	266
# define	such_as	267
# define	dblpoint	268
# define	Resources	269
# define	trace_keyword	270
# define	no_trace	271
# define	IDENTIF_COLON	272
# define	Constant	273
# define	Body	274
# define	Function_keyword	275
# define	Type_keyword	276
# define	algorithmic	277
# define	table_keyword	278
# define	list_keyword	279
# define	Calculate_if	289
# define	Exist	281
# define	Not_Exist	282
# define	For_All	283
# define	Not_For_All	284
# define	neq	285
# define	leq	286
# define	geq	287
# define	NoCheck	288
# define	or_keyword	290
# define	and_keyword	291
# define	Sequence	292
# define	uniform	293
# define	exponential	294
# define	normal_keyword	295
# define	by_hist	296
# define	enumerative	297
# define	Pattern	298
# define	operation	299
# define	irregular_event	300
# define	rule_keyword	301
# define	keyboard	302
# define	Relevant_resources	303
# define	Keep	304
# define	Create_keyword	305
# define	Erase	306
# define	NonExist	307
# define	IDENTIF_NoChange	308
# define	Time	309
# define	Choice	310
# define	from_keyword	311
# define	first_keyword	312
# define	Convert_begin	313
# define	Convert_end	314
# define	Convert_rule	315
# define	Convert_event	316
# define	with_max	317
# define	with_min	318
# define	IDENTIF_set	319
# define	IDENTIF_NoChange_NoChange	320
# define	Operations	321
# define	Results	322
# define	watch_par	323
# define	watch_state	324
# define	watch_quant	325
# define	watch_value	326
# define	get_value	327
# define	Model_name	328
# define	Resource_file	329
# define	OprIev_file	330
# define	Frame_file	331
# define	Statistic_file	332
# define	Results_file	333
# define	Trace_file	334
# define	Show_mode	335
# define	Frame_number	336
# define	Show_rate	337
# define	Run_StartTime	338
# define	Trace_StartTime	339
# define	Trace_EndTime	340
# define	Terminate_if	341
# define	Break_point	342
# define	Seed	343
# define	NoShow	344
# define	Monitor	345
# define	Animation	346
# define	NoChange	347
# define	Decision_point	348
# define	search_keyword	349
# define	trace_stat	350
# define	trace_tops	351
# define	trace_all	352
# define	Condition_keyword	353
# define	Term_condition	354
# define	Evaluate_by	355
# define	Compare_tops	356
# define	NO	357
# define	YES	358
# define	Activities	359
# define	value_before	360
# define	value_after	361
# define	some	362
# define	Frame	400
# define	Show_if	401
# define	Back_picture	402
# define	Show	403
# define	frm_cell	404
# define	text	405
# define	transparent	406
# define	bitmap	407
# define	s_bmp	408
# define	rect_keyword	409
# define	r_rect	410
# define	line	411
# define	ellipse	412
# define	triang	413
# define	active	414
# define	QUOTED_IDENTIF	415

#line 129 ".\\rdodpt.y"

#include "pch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <FlexLexer.h>

#include "rdoparselex.h"
#include "rdofun.h"
#include "rdodpt.h"

namespace rdoParse 
{
#ifndef YYSTYPE
#define YYSTYPE int
#endif
#ifndef YYDEBUG
# define YYDEBUG 0
#endif



#define	YYFINAL		129
#define	YYFLAG		-32768
#define	YYNTBASE	137

/* YYTRANSLATE(YYLEX) -- Bison token number corresponding to YYLEX. */
#define YYTRANSLATE(x) ((unsigned)(x) <= 415 ? yytranslate[x] : 164)

/* YYTRANSLATE[YYLEX] -- Bison token number corresponding to YYLEX. */
static const short yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     133,   134,   126,   124,   136,   125,   135,   127,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     129,   128,   130,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   131,     2,   132,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
       2,    27,    28,    29,    30,    31,    32,    33,    34,    26,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123
};

#if YYDEBUG
static const short yyprhs[] =
{
       0,     0,     1,     4,     7,    10,    14,    19,    24,    29,
      34,    38,    42,    46,    50,    55,    60,    63,    65,    69,
      71,    74,    77,    80,    83,    87,    91,    94,    98,   103,
     107,   111,   114,   116,   120,   122,   125,   128,   131,   134,
     137,   139,   141,   145,   147,   150,   153,   156,   159,   162,
     166,   170,   174,   178,   182,   186,   190,   194,   198,   200,
     204,   208,   212,   216,   220,   225,   229,   231,   233,   235,
     236,   239,   243,   245,   247,   249,   251,   255,   259
};
static const short yyrhs[] =
{
      -1,   137,   147,     0,   137,   153,     0,   137,   157,     0,
      93,    18,    94,     0,    93,    18,    94,    17,     0,    93,
      18,    94,    95,     0,    93,    18,    94,    96,     0,    93,
      18,    94,    97,     0,   138,    98,   158,     0,   138,    98,
      34,     0,   139,    99,   158,     0,   140,   100,   159,     0,
     141,   101,   128,   102,     0,   141,   101,   128,   103,     0,
     142,   104,     0,   146,     0,   143,    18,    10,     0,   144,
       0,   145,    11,     0,   145,    12,     0,   145,    10,     0,
     145,   126,     0,   145,   105,   159,     0,   145,   106,   159,
       0,   143,     8,     0,    93,    18,   107,     0,    93,    18,
     107,    17,     0,   148,    98,   158,     0,   148,    98,    34,
       0,   149,   104,     0,   152,     0,   150,    18,    10,     0,
     151,     0,   152,    11,     0,   152,    12,     0,   152,    10,
       0,   152,   126,     0,   150,     8,     0,   104,     0,   156,
       0,   154,    18,    10,     0,   155,     0,   156,    11,     0,
     156,    12,     0,   156,    10,     0,   156,   126,     0,   154,
       8,     0,   159,   128,   159,     0,   159,    31,   159,     0,
     159,   129,   159,     0,   159,   130,   159,     0,   159,    32,
     159,     0,   159,    33,   159,     0,   158,    36,   158,     0,
     158,    35,   158,     0,   131,   158,   132,     0,   163,     0,
     159,   124,   159,     0,   159,   125,   159,     0,   159,   126,
     159,     0,   159,   127,   159,     0,   133,   159,   134,     0,
      10,   133,   160,   134,     0,    10,   135,    10,     0,    11,
       0,    12,     0,    10,     0,     0,   160,   159,     0,   160,
     136,   159,     0,    27,     0,    28,     0,    29,     0,    30,
       0,   161,   133,    18,     0,   162,   158,   134,     0,   162,
      34,   134,     0
};

#endif

#if YYDEBUG
/* YYRLINE[YYN] -- source line where rule number YYN was defined. */
static const short yyrline[] =
{
       0,   158,   159,   160,   161,   166,   167,   168,   169,   170,
     172,   173,   175,   177,   179,   180,   182,   183,   185,   187,
     188,   189,   190,   191,   193,   194,   196,   201,   202,   204,
     205,   207,   208,   210,   212,   213,   214,   215,   216,   218,
     223,   224,   226,   228,   229,   230,   231,   232,   234,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   263,
     264,   265,   268,   269,   270,   271,   273,   275,   276
};
#endif


#if (YYDEBUG) || defined YYERROR_VERBOSE

/* YYTNAME[TOKEN_NUM] -- String name of the token TOKEN_NUM. */
static const char *const yytname[] =
{
  "$", "error", "$undefined.", "Resource_type", "permanent", "Parameters", 
  "integer", "real", "End", "temporary", "IDENTIF", "INT_CONST", 
  "REAL_CONST", "such_as", "dblpoint", "Resources", "trace_keyword", 
  "no_trace", "IDENTIF_COLON", "Constant", "Body", "Function_keyword", 
  "Type_keyword", "algorithmic", "table_keyword", "list_keyword", 
  "Calculate_if", "Exist", "Not_Exist", "For_All", "Not_For_All", "neq", 
  "leq", "geq", "NoCheck", "or_keyword", "and_keyword", "Sequence", 
  "uniform", "exponential", "normal_keyword", "by_hist", "enumerative", 
  "Pattern", "operation", "irregular_event", "rule_keyword", "keyboard", 
  "Relevant_resources", "Keep", "Create_keyword", "Erase", "NonExist", 
  "IDENTIF_NoChange", "Time", "Choice", "from_keyword", "first_keyword", 
  "Convert_begin", "Convert_end", "Convert_rule", "Convert_event", 
  "with_max", "with_min", "IDENTIF_set", "IDENTIF_NoChange_NoChange", 
  "Operations", "Results", "watch_par", "watch_state", "watch_quant", 
  "watch_value", "get_value", "Model_name", "Resource_file", 
  "OprIev_file", "Frame_file", "Statistic_file", "Results_file", 
  "Trace_file", "Show_mode", "Frame_number", "Show_rate", "Run_StartTime", 
  "Trace_StartTime", "Trace_EndTime", "Terminate_if", "Break_point", 
  "Seed", "NoShow", "Monitor", "Animation", "NoChange", "Decision_point", 
  "search_keyword", "trace_stat", "trace_tops", "trace_all", 
  "Condition_keyword", "Term_condition", "Evaluate_by", "Compare_tops", 
  "NO", "YES", "Activities", "value_before", "value_after", "some", 
  "Frame", "Show_if", "Back_picture", "Show", "frm_cell", "text", 
  "transparent", "bitmap", "s_bmp", "rect_keyword", "r_rect", "line", 
  "ellipse", "triang", "active", "QUOTED_IDENTIF", "'+'", "'-'", "'*'", 
  "'/'", "'='", "'<'", "'>'", "'['", "']'", "'('", "')'", "'.'", "','", 
  "dpt_main", "dpt_begin_search", "dpt_condition_search", 
  "dpt_term_search", "dpt_evaluate_search", "dpt_compare_search", 
  "dpt_activ_search", "dpt_activ_search_descr", 
  "dpt_activ_search_descr_param", "dpt_activ_search_descr_value", 
  "dpt_activ_search_end", "dpt_begin_some", "dpt_condition_some", 
  "dpt_activ_some", "dpt_activ_some_descr", "dpt_activ_some_descr_param", 
  "dpt_activ_some_end", "dpt_activ_free", "dpt_activ_free_descr", 
  "dpt_activ_free_descr_param", "dpt_activ_free_end", "dpt_logic", 
  "dpt_arithm", "dpt_arithm_func_call_pars", "fun_group_keyword", 
  "fun_group_header", "fun_group", 0
};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives. */
static const short yyr1[] =
{
       0,   137,   137,   137,   137,   138,   138,   138,   138,   138,
     139,   139,   140,   141,   142,   142,   143,   143,   144,   145,
     145,   145,   145,   145,   146,   146,   147,   148,   148,   149,
     149,   150,   150,   151,   152,   152,   152,   152,   152,   153,
     154,   154,   155,   156,   156,   156,   156,   156,   157,   158,
     158,   158,   158,   158,   158,   158,   158,   158,   158,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   160,
     160,   160,   161,   161,   161,   161,   162,   163,   163
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN. */
static const short yyr2[] =
{
       0,     0,     2,     2,     2,     3,     4,     4,     4,     4,
       3,     3,     3,     3,     4,     4,     2,     1,     3,     1,
       2,     2,     2,     2,     3,     3,     2,     3,     4,     3,
       3,     2,     1,     3,     1,     2,     2,     2,     2,     2,
       1,     1,     3,     1,     2,     2,     2,     2,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     3,
       3,     3,     3,     3,     4,     3,     1,     1,     1,     0,
       2,     3,     1,     1,     1,     1,     3,     3,     3
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error. */
static const short yydefact[] =
{
       1,     0,     0,    40,     0,     0,     0,     0,     0,     0,
      19,     0,    17,     2,     0,     0,     0,    34,    32,     3,
       0,    43,    41,     4,     0,     0,     0,     0,     0,    16,
      26,     0,    22,    20,    21,     0,     0,    23,     0,    31,
      39,     0,    37,    35,    36,    38,    48,     0,    46,    44,
      45,    47,     5,    27,    68,    66,    67,    72,    73,    74,
      75,    11,     0,     0,    10,     0,     0,     0,    58,    12,
      13,     0,    18,    24,    25,    30,    29,    33,    42,     6,
       7,     8,     9,    28,    69,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    14,    15,     0,    65,    57,    63,    56,
      55,    50,    53,    54,    59,    60,    61,    62,    49,    51,
      52,    76,    78,    77,    64,     0,    70,    71,     0,     0
};

static const short yydefgoto[] =
{
       1,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    64,    65,   105,    66,    67,    68
};

static const short yypact[] =
{
  -32768,     3,    22,-32768,   -43,   -28,   -17,   -16,   -20,    54,
  -32768,     5,-32768,-32768,   -12,   -15,    55,-32768,     8,-32768,
      60,-32768,    27,-32768,   -46,    -6,    23,    -3,   -41,-32768,
  -32768,    78,-32768,-32768,-32768,    -3,    -3,-32768,     2,-32768,
  -32768,    80,-32768,-32768,-32768,-32768,-32768,    81,-32768,-32768,
  -32768,-32768,    24,    75,   -79,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,    23,    -3,    34,    33,   -40,    15,-32768,    34,
     -50,   -23,-32768,   -50,   -50,-32768,    34,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,    84,    11,   -67,    23,    23,
      -3,    -3,    -3,    -3,    -3,    -3,    -3,    -3,    -3,    -3,
      79,   -39,   -25,-32768,-32768,   -10,-32768,-32768,-32768,    62,
  -32768,   -50,   -50,   -50,   -45,   -45,-32768,-32768,   -50,   -50,
     -50,-32768,-32768,-32768,-32768,    -3,   -50,   -50,    99,-32768
};

static const short yypgoto[] =
{
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,   126,    77,-32768,-32768,-32768,-32768
};


#define	YYLAST		215


static const short yytable[] =
{
      54,    55,    56,   128,    54,    55,    56,    54,    55,    56,
      88,    89,    54,    55,    56,    32,    33,    34,    42,    43,
      44,    57,    58,    59,    60,    54,    55,    56,    61,    57,
      58,    59,    60,    54,    55,    56,    75,    48,    49,    50,
      24,    79,    57,    58,    59,    60,    88,    89,    52,   101,
      57,    58,    59,    60,    84,    25,    85,    93,    94,    95,
      96,    53,    30,    40,    90,    91,    92,   108,    46,    88,
      89,    26,    31,    41,    93,    94,    95,    96,    47,   103,
     104,    95,    96,    27,    29,    28,    38,    71,    72,    39,
      77,    78,    83,   100,   106,   122,     2,   121,    89,   129,
       0,     0,     0,     0,    70,     0,     0,     3,     0,   123,
      35,    36,    73,    74,     0,     0,     0,     0,     0,    80,
      81,    82,     0,    63,   124,    62,   125,    63,     0,     0,
      63,    37,     0,    62,    45,    63,     0,     0,     0,     0,
      87,     0,     0,   107,     0,     0,    62,     0,    63,     0,
       0,     0,    69,    51,    62,     0,    63,    93,    94,    95,
      96,    97,    98,    99,    76,     0,     0,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,     0,     0,     0,
       0,     0,   126,     0,     0,     0,     0,     0,    86,     0,
       0,     0,     0,   102,     0,     0,     0,     0,     0,     0,
       0,     0,   127,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   109,   110
};

static const short yycheck[] =
{
      10,    11,    12,     0,    10,    11,    12,    10,    11,    12,
      35,    36,    10,    11,    12,    10,    11,    12,    10,    11,
      12,    27,    28,    29,    30,    10,    11,    12,    34,    27,
      28,    29,    30,    10,    11,    12,    34,    10,    11,    12,
      18,    17,    27,    28,    29,    30,    35,    36,    94,    34,
      27,    28,    29,    30,   133,    98,   135,   124,   125,   126,
     127,   107,     8,     8,    31,    32,    33,   134,     8,    35,
      36,    99,    18,    18,   124,   125,   126,   127,    18,   102,
     103,   126,   127,   100,   104,   101,    98,   128,    10,   104,
      10,    10,    17,   133,    10,   134,    93,    18,    36,     0,
      -1,    -1,    -1,    -1,    27,    -1,    -1,   104,    -1,   134,
     105,   106,    35,    36,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    -1,   133,   134,   131,   136,   133,    -1,    -1,
     133,   126,    -1,   131,   126,   133,    -1,    -1,    -1,    -1,
      63,    -1,    -1,   132,    -1,    -1,   131,    -1,   133,    -1,
      -1,    -1,    26,   126,   131,    -1,   133,   124,   125,   126,
     127,   128,   129,   130,    38,    -1,    -1,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,    -1,    -1,    -1,
      -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,    62,    -1,
      -1,    -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    88,    89
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "..\\System\\bison.simple"

/* Skeleton output parser for bison,

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software
   Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* This is the parser code that is written into each bison parser when
   the %semantic_parser declaration is not specified in the grammar.
   It was written by Richard Stallman by simplifying the hairy parser
   used when %semantic_parser is specified.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

#if ! defined (yyoverflow) || defined (YYERROR_VERBOSE)

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
# if YYLSP_NEEDED
  YYLTYPE yyls;
# endif
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAX (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# if YYLSP_NEEDED
#  define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE) + sizeof (YYLTYPE))	\
      + 2 * YYSTACK_GAP_MAX)
# else
#  define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAX)
# endif

/* Relocate the TYPE STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Type, Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	yymemcpy ((char *) yyptr, (char *) (Stack),			\
		  yysize * (YYSIZE_T) sizeof (Type));			\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (Type) + YYSTACK_GAP_MAX;	\
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif /* ! defined (yyoverflow) || defined (YYERROR_VERBOSE) */


#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto yyacceptlab
#define YYABORT 	goto yyabortlab
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");			\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).

   When YYLLOC_DEFAULT is run, CURRENT is set the location of the
   first token.  By default, to implement support for ranges, extend
   its range to the last symbol.  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)       	\
   Current.last_line   = Rhs[N].last_line;	\
   Current.last_column = Rhs[N].last_column;
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#if YYPURE
# if YYLSP_NEEDED
#  ifdef YYLEX_PARAM
#   define YYLEX		yylex (&yylval, &yylloc, YYLEX_PARAM)
#  else
#   define YYLEX		yylex (&yylval, &yylloc)
#  endif
# else /* !YYLSP_NEEDED */
#  ifdef YYLEX_PARAM
#   define YYLEX		yylex (&yylval, YYLEX_PARAM)
#  else
#   define YYLEX		yylex (&yylval)
#  endif
# endif /* !YYLSP_NEEDED */
#else /* !YYPURE */
# define YYLEX			yylex ()
#endif /* !YYPURE */


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)
/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
#endif /* !YYDEBUG */

/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif

#if ! defined (yyoverflow) && ! defined (yymemcpy)
# if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#  define yymemcpy __builtin_memcpy
# else				/* not GNU C or C++ */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
#  if defined (__STDC__) || defined (__cplusplus)
yymemcpy (char *yyto, const char *yyfrom, YYSIZE_T yycount)
#  else
yymemcpy (yyto, yyfrom, yycount)
     char *yyto;
     const char *yyfrom;
     YYSIZE_T yycount;
#  endif
{
  register const char *yyf = yyfrom;
  register char *yyt = yyto;
  register YYSIZE_T yyi = yycount;

  while (yyi-- != 0)
    *yyt++ = *yyf++;
}
# endif
#endif

#ifdef YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif
#endif

#line 319 "..\\System\\bison.simple"


/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
#  define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL
# else
#  define YYPARSE_PARAM_ARG YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
# endif
#else /* !YYPARSE_PARAM */
# define YYPARSE_PARAM_ARG
# define YYPARSE_PARAM_DECL
#endif /* !YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
# ifdef YYPARSE_PARAM
int yyparse (void *);
# else
int yyparse (void);
# endif
#endif

/* YY_DECL_VARIABLES -- depending whether we use a pure parser,
   variables are global, or local to YYPARSE.  */

#define YY_DECL_NON_LSP_VARIABLES			\
/* The lookahead symbol.  */				\
int yychar;						\
							\
/* The semantic value of the lookahead symbol. */	\
YYSTYPE yylval;						\
							\
/* Number of parse errors so far.  */			\
int yynerrs;

#if YYLSP_NEEDED
# define YY_DECL_VARIABLES			\
YY_DECL_NON_LSP_VARIABLES			\
						\
/* Location data for the lookahead symbol.  */	\
YYLTYPE yylloc;
#else
# define YY_DECL_VARIABLES			\
YY_DECL_NON_LSP_VARIABLES
#endif


/* If nonreentrant, generate the variables here. */

#if !YYPURE
YY_DECL_VARIABLES
#endif  /* !YYPURE */

int
yyparse (YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  /* If reentrant, generate the variables here. */
#if YYPURE
  YY_DECL_VARIABLES
#endif  /* !YYPURE */

  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yychar1 = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack. */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;

#if YYLSP_NEEDED
  /* The location stack.  */
  YYLTYPE yylsa[YYINITDEPTH];
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;
#endif

#if YYLSP_NEEDED
# define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
# define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  YYSIZE_T yystacksize = YYINITDEPTH;


  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
#if YYLSP_NEEDED
  YYLTYPE yyloc;
#endif

  /* When reducing, the number of symbols on the RHS of the reduced
     rule. */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;
#if YYLSP_NEEDED
  yylsp = yyls;
#endif
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  */
# if YYLSP_NEEDED
	YYLTYPE *yyls1 = yyls;
	/* This used to be a conditional around just the two extra args,
	   but that might be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);
	yyls = yyls1;
# else
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);
# endif
	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (short, yyss);
	YYSTACK_RELOCATE (YYSTYPE, yyvs);
# if YYLSP_NEEDED
	YYSTACK_RELOCATE (YYLTYPE, yyls);
# endif
# undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
#if YYLSP_NEEDED
      yylsp = yyls + yysize - 1;
#endif

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yychar1 = YYTRANSLATE (yychar);

#if YYDEBUG
     /* We have to keep this `#if YYDEBUG', since we use variables
	which are defined only if `YYDEBUG' is set.  */
      if (yydebug)
	{
	  YYFPRINTF (stderr, "Next token is %d (%s",
		     yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise
	     meaning of a token, for further debugging info.  */
# ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
# endif
	  YYFPRINTF (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %d (%s), ",
	      yychar, yytname[yychar1]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#if YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to the semantic value of
     the lookahead token.  This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

#if YYLSP_NEEDED
  /* Similarly for the default location.  Let the user run additional
     commands if for instance locations are ranges.  */
  yyloc = yylsp[1-yylen];
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
#endif

#if YYDEBUG
  /* We have to keep this `#if YYDEBUG', since we use variables which
     are defined only if `YYDEBUG' is set.  */
  if (yydebug)
    {
      int yyi;

      YYFPRINTF (stderr, "Reducing via rule %d (line %d), ",
		 yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (yyi = yyprhs[yyn]; yyrhs[yyi] > 0; yyi++)
	YYFPRINTF (stderr, "%s ", yytname[yyrhs[yyi]]);
      YYFPRINTF (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif

  switch (yyn) {

case 5:
#line 166 ".\\rdodpt.y"
{ yyval = (int)(new RDODPTSearch((string *)yyvsp[-1])); ;
    break;}
case 6:
#line 167 ".\\rdodpt.y"
{ yyval = (int)(new RDODPTSearch((string *)yyvsp[-2], DPTnotrace)); ;
    break;}
case 7:
#line 168 ".\\rdodpt.y"
{ yyval = (int)(new RDODPTSearch((string *)yyvsp[-2], DPTtracestat)); ;
    break;}
case 8:
#line 169 ".\\rdodpt.y"
{ yyval = (int)(new RDODPTSearch((string *)yyvsp[-2], DPTtracetops)); ;
    break;}
case 9:
#line 170 ".\\rdodpt.y"
{ yyval = (int)(new RDODPTSearch((string *)yyvsp[-2], DPTtraceall)); ;
    break;}
case 10:
#line 172 ".\\rdodpt.y"
{ ((RDODPTSearch *)yyval)->setCondition((RDOFUNLogic *)yyvsp[0]); ;
    break;}
case 11:
#line 173 ".\\rdodpt.y"
{ ((RDODPTSearch *)yyval)->setCondition(); ;
    break;}
case 12:
#line 175 ".\\rdodpt.y"
{ ((RDODPTSearch *)yyval)->setTermCondition((RDOFUNLogic *)yyvsp[0]); ;
    break;}
case 13:
#line 177 ".\\rdodpt.y"
{ ((RDODPTSearch *)yyval)->setEvaluateBy((RDOFUNArithm *)yyvsp[0]); ;
    break;}
case 14:
#line 179 ".\\rdodpt.y"
{ ((RDODPTSearch *)yyval)->setCompareTops(false); ;
    break;}
case 15:
#line 180 ".\\rdodpt.y"
{ ((RDODPTSearch *)yyval)->setCompareTops(true); ;
    break;}
case 18:
#line 185 ".\\rdodpt.y"
{ ((RDODPTSearch *)yyval)->addNewActivity((string *)yyvsp[-1], (string *)yyvsp[0]); ;
    break;}
case 20:
#line 188 ".\\rdodpt.y"
{ ((RDODPTSearch *)yyval)->addActivityParam((int)yyvsp[0]); ;
    break;}
case 21:
#line 189 ".\\rdodpt.y"
{ ((RDODPTSearch *)yyval)->addActivityParam((double *)yyvsp[0]); ;
    break;}
case 22:
#line 190 ".\\rdodpt.y"
{ ((RDODPTSearch *)yyval)->addActivityParam((string *)yyvsp[0]); ;
    break;}
case 23:
#line 191 ".\\rdodpt.y"
{ ((RDODPTSearch *)yyval)->addActivityParam(); ;
    break;}
case 24:
#line 193 ".\\rdodpt.y"
{ ((RDODPTSearch *)yyval)->setActivityValue(DPT_value_before, (RDOFUNArithm *)yyvsp[0]); ;
    break;}
case 25:
#line 194 ".\\rdodpt.y"
{ ((RDODPTSearch *)yyval)->setActivityValue(DPT_value_after, (RDOFUNArithm *)yyvsp[0]); ;
    break;}
case 26:
#line 196 ".\\rdodpt.y"
{ ((RDODPTSearch *)yyval)->end();;
    break;}
case 27:
#line 201 ".\\rdodpt.y"
{ yyval = (int)(new RDODPTSome((string *)yyvsp[-1])); ;
    break;}
case 28:
#line 202 ".\\rdodpt.y"
{ yyval = (int)(new RDODPTSome((string *)yyvsp[-2])); ;
    break;}
case 29:
#line 204 ".\\rdodpt.y"
{ ((RDODPTSome *)yyval)->setCondition((RDOFUNLogic *)yyvsp[0]); ;
    break;}
case 30:
#line 205 ".\\rdodpt.y"
{ ((RDODPTSome *)yyval)->setCondition(); ;
    break;}
case 33:
#line 210 ".\\rdodpt.y"
{ ((RDODPTSome *)yyval)->addNewActivity((string *)yyvsp[-1], (string *)yyvsp[0]); ;
    break;}
case 35:
#line 213 ".\\rdodpt.y"
{ ((RDODPTSome *)yyval)->addActivityParam((int)yyvsp[0]); ;
    break;}
case 36:
#line 214 ".\\rdodpt.y"
{ ((RDODPTSome *)yyval)->addActivityParam((double *)yyvsp[0]); ;
    break;}
case 37:
#line 215 ".\\rdodpt.y"
{ ((RDODPTSome *)yyval)->addActivityParam((string *)yyvsp[0]); ;
    break;}
case 38:
#line 216 ".\\rdodpt.y"
{ ((RDODPTSome *)yyval)->addActivityParam(); ;
    break;}
case 39:
#line 218 ".\\rdodpt.y"
{ ((RDODPTSome *)yyval)->end();;
    break;}
case 42:
#line 226 ".\\rdodpt.y"
{ yyval = (int)(new RDODPTFreeActivity((string *)yyvsp[-1], (string *)yyvsp[0])); ;
    break;}
case 44:
#line 229 ".\\rdodpt.y"
{ ((RDODPTFreeActivity *)yyval)->addParam((int)yyvsp[0]); ;
    break;}
case 45:
#line 230 ".\\rdodpt.y"
{ ((RDODPTFreeActivity *)yyval)->addParam((double *)yyvsp[0]); ;
    break;}
case 46:
#line 231 ".\\rdodpt.y"
{ ((RDODPTFreeActivity *)yyval)->addParam((string *)yyvsp[0]); ;
    break;}
case 47:
#line 232 ".\\rdodpt.y"
{ ((RDODPTFreeActivity *)yyval)->addParam(); ;
    break;}
case 48:
#line 234 ".\\rdodpt.y"
{ ((RDODPTFreeActivity *)yyval)->end();;
    break;}
case 49:
#line 240 ".\\rdodpt.y"
{ yyval = (int)(*(RDOFUNArithm *)yyvsp[-2] == *(RDOFUNArithm *)yyvsp[0]); ;
    break;}
case 50:
#line 241 ".\\rdodpt.y"
{ yyval = (int)(*(RDOFUNArithm *)yyvsp[-2] != *(RDOFUNArithm *)yyvsp[0]); ;
    break;}
case 51:
#line 242 ".\\rdodpt.y"
{ yyval = (int)(*(RDOFUNArithm *)yyvsp[-2] <  *(RDOFUNArithm *)yyvsp[0]); ;
    break;}
case 52:
#line 243 ".\\rdodpt.y"
{ yyval = (int)(*(RDOFUNArithm *)yyvsp[-2] >  *(RDOFUNArithm *)yyvsp[0]); ;
    break;}
case 53:
#line 244 ".\\rdodpt.y"
{ yyval = (int)(*(RDOFUNArithm *)yyvsp[-2] <= *(RDOFUNArithm *)yyvsp[0]); ;
    break;}
case 54:
#line 245 ".\\rdodpt.y"
{ yyval = (int)(*(RDOFUNArithm *)yyvsp[-2] >= *(RDOFUNArithm *)yyvsp[0]); ;
    break;}
case 55:
#line 246 ".\\rdodpt.y"
{ yyval = (int)(*(RDOFUNLogic *)yyvsp[-2] && *(RDOFUNLogic *)yyvsp[0]); ;
    break;}
case 56:
#line 247 ".\\rdodpt.y"
{ yyval = (int)(*(RDOFUNLogic *)yyvsp[-2] || *(RDOFUNLogic *)yyvsp[0]); ;
    break;}
case 57:
#line 248 ".\\rdodpt.y"
{ yyval = yyvsp[-1]; ;
    break;}
case 58:
#line 249 ".\\rdodpt.y"
{ yyval = yyvsp[0]; ;
    break;}
case 59:
#line 252 ".\\rdodpt.y"
{ yyval = (int)(*(RDOFUNArithm *)yyvsp[-2] + *(RDOFUNArithm *)yyvsp[0]); ;
    break;}
case 60:
#line 253 ".\\rdodpt.y"
{ yyval = (int)(*(RDOFUNArithm *)yyvsp[-2] - *(RDOFUNArithm *)yyvsp[0]); ;
    break;}
case 61:
#line 254 ".\\rdodpt.y"
{ yyval = (int)(*(RDOFUNArithm *)yyvsp[-2] * *(RDOFUNArithm *)yyvsp[0]); ;
    break;}
case 62:
#line 255 ".\\rdodpt.y"
{ yyval = (int)(*(RDOFUNArithm *)yyvsp[-2] / *(RDOFUNArithm *)yyvsp[0]); ;
    break;}
case 63:
#line 256 ".\\rdodpt.y"
{ yyval = yyvsp[-1]; ;
    break;}
case 64:
#line 257 ".\\rdodpt.y"
{ yyval = (int)((RDOFUNParams *)yyvsp[-1])->createCall((string *)yyvsp[-3]) ;
    break;}
case 65:
#line 258 ".\\rdodpt.y"
{ yyval = (int)(new RDOFUNArithm((string *)yyvsp[-2], (string *)yyvsp[0])); ;
    break;}
case 66:
#line 259 ".\\rdodpt.y"
{ yyval = (int)(new RDOFUNArithm((int)yyvsp[0])); ;
    break;}
case 67:
#line 260 ".\\rdodpt.y"
{ yyval = (int)(new RDOFUNArithm((double*)yyvsp[0])); ;
    break;}
case 68:
#line 261 ".\\rdodpt.y"
{ yyval = (int)(new RDOFUNArithm((string *)yyvsp[0])); ;
    break;}
case 69:
#line 263 ".\\rdodpt.y"
{ yyval = (int)(new RDOFUNParams()); ;
    break;}
case 70:
#line 264 ".\\rdodpt.y"
{ yyval = (int)(((RDOFUNParams *)yyvsp[-1])->addParameter((RDOFUNArithm *)yyvsp[0])); ;
    break;}
case 71:
#line 265 ".\\rdodpt.y"
{ yyval = (int)(((RDOFUNParams *)yyvsp[-2])->addParameter((RDOFUNArithm *)yyvsp[0])); ;
    break;}
case 72:
#line 268 ".\\rdodpt.y"
{ yyval = 1; ;
    break;}
case 73:
#line 269 ".\\rdodpt.y"
{ yyval = 2; ;
    break;}
case 74:
#line 270 ".\\rdodpt.y"
{ yyval = 3; ;
    break;}
case 75:
#line 271 ".\\rdodpt.y"
{ yyval = 4; ;
    break;}
case 76:
#line 273 ".\\rdodpt.y"
{ yyval = (int)(new RDOFUNGroup(yyvsp[-2], (string *)yyvsp[0])); ;
    break;}
case 77:
#line 275 ".\\rdodpt.y"
{ yyval = (int)(((RDOFUNGroup *)yyvsp[-2])->createFunLogin((RDOFUNLogic *)yyvsp[-1])); ;
    break;}
case 78:
#line 276 ".\\rdodpt.y"
{ yyval = (int)(((RDOFUNGroup *)yyvsp[-2])->createFunLogin()); ;
    break;}
}

#line 705 "..\\System\\bison.simple"


  yyvsp -= yylen;
  yyssp -= yylen;
#if YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG
  if (yydebug)
    {
      short *yyssp1 = yyss - 1;
      YYFPRINTF (stderr, "state stack now");
      while (yyssp1 != yyssp)
	YYFPRINTF (stderr, " %d", *++yyssp1);
      YYFPRINTF (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;
#if YYLSP_NEEDED
  *++yylsp = yyloc;
#endif

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  char *yymsg;
	  int yyx, yycount;

	  yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (yyx = yyn < 0 ? -yyn : 0;
	       yyx < (int) (sizeof (yytname) / sizeof (char *)); yyx++)
	    if (yycheck[yyx + yyn] == yyx)
	      yysize += yystrlen (yytname[yyx]) + 15, yycount++;
	  yysize += yystrlen ("parse error, unexpected ") + 1;
	  yysize += yystrlen (yytname[YYTRANSLATE (yychar)]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "parse error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[YYTRANSLATE (yychar)]);

	      if (yycount < 5)
		{
		  yycount = 0;
		  for (yyx = yyn < 0 ? -yyn : 0;
		       yyx < (int) (sizeof (yytname) / sizeof (char *));
		       yyx++)
		    if (yycheck[yyx + yyn] == yyx)
		      {
			const char *yyq = ! yycount ? ", expecting " : " or ";
			yyp = yystpcpy (yyp, yyq);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yycount++;
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exhausted");
	}
      else
#endif /* defined (YYERROR_VERBOSE) */
	yyerror ("parse error");
    }
  goto yyerrlab1;


/*--------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action |
`--------------------------------------------------*/
yyerrlab1:
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;
      YYDPRINTF ((stderr, "Discarding token %d (%s).\n",
		  yychar, yytname[yychar1]));
      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;


/*-------------------------------------------------------------------.
| yyerrdefault -- current state does not do anything special for the |
| error token.                                                       |
`-------------------------------------------------------------------*/
yyerrdefault:
#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */

  /* If its default is to accept any token, ok.  Otherwise pop it.  */
  yyn = yydefact[yystate];
  if (yyn)
    goto yydefault;
#endif


/*---------------------------------------------------------------.
| yyerrpop -- pop the current state because it cannot handle the |
| error token                                                    |
`---------------------------------------------------------------*/
yyerrpop:
  if (yyssp == yyss)
    YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#if YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG
  if (yydebug)
    {
      short *yyssp1 = yyss - 1;
      YYFPRINTF (stderr, "Error: state stack now");
      while (yyssp1 != yyssp)
	YYFPRINTF (stderr, " %d", *++yyssp1);
      YYFPRINTF (stderr, "\n");
    }
#endif

/*--------------.
| yyerrhandle.  |
`--------------*/
yyerrhandle:
  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;
#if YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

/*---------------------------------------------.
| yyoverflowab -- parser overflow comes here.  |
`---------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}
#line 283 ".\\rdodpt.y"


}