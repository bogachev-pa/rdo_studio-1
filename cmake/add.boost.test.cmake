#==============================================================================
# Copyright (c) 2011-2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MACRO(ADD_BOOST_TEST args)

MESSAGE(STATUS "CREATE  ${ARGV0}")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${RDO_BASE_DIRECTORY})

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

ADD_EXECUTABLE(${ARGV0} ${ARGN})
SET_TARGET_PROPERTIES(${ARGV0} PROPERTIES LINKER_LANGUAGE CXX)
SET_TARGET_PROPERTIES(${ARGV0} PROPERTIES FOLDER ${TESTS_FOLDERS})
ADD_TEST(NAME ${ARGV0} COMMAND ${EXECUTABLE_OUTPUT_PATH}/${ARGV0})

IF(MSVC)
    SET_TARGET_PROPERTIES(${ARGV0} PROPERTIES COMPILE_FLAGS "/EHa")
ENDIF(MSVC)

TARGET_LINK_LIBRARIES(${ARGV0} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

SOURCE_GROUP(".test" FILES 
    ${ARGN}
    )

ENDMACRO(ADD_BOOST_TEST)
