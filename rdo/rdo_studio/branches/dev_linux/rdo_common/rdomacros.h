/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdomacros.h
 * author   : ������ ������
 * date     : 13.06.2009
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOMACROS_H_
#define _RDOMACROS_H_

// ====================================================================== INCLUDES
#include <boost/typeof/typeof.hpp>
// ====================================================================== SYNOPSIS
// ===============================================================================

#define   REF(A)       A&
#define  CREF(A) const A&
#define   PTR(A)       A*
#define  CPTR(A) const A*
#define CPTRC(A) const A* const

#define  CLASS(A)            class A
#define STRUCT(A)            struct A
#define S_INTERFACE(A)       struct A
#define IS  ,
#define AND ,
#define IMPLEMENTATION_OF(A) public A
#define INSTANCE_OF(A)       public A

#define  CLASS_PARENT_OF(A, B)  CLASS(A): IMPLEMENTATION_OF(B)
#define STRUCT_PARENT_OF(A, B) STRUCT(A): IMPLEMENTATION_OF(B)

#define STL_FOR_ALL(CONTAINER, IT)       for (BOOST_TYPEOF(CONTAINER)::iterator IT = CONTAINER.begin(); IT != CONTAINER.end(); ++IT)
#define STL_FOR_ALL_CONST(CONTAINER, IT) for (BOOST_TYPEOF(CONTAINER)::const_iterator IT = CONTAINER.begin(); IT != CONTAINER.end(); ++IT)

#define AUTO BOOST_AUTO

#endif //! _RDOMACROS_H_
