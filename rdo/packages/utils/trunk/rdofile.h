/*!
  \copyright (c) RDO-Team, 2011
  \file      rdofile.h
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \authors   ��������� ������� (lord.tiran@gmail.com)
  \date      10.05.2009
  \brief     ������ � �������
  \indent    4T
*/

#ifndef _UTILS_RDOFILE_H_
#define _UTILS_RDOFILE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

//! ������ ������������ ��� ��� ����������� �������, ��� �����
class File
{
public:
	static rbool create(CREF(tstring) name);
	static rbool create(CREF(tstring) name, CREF(tstring) content);

	//! ��������� ������������� �����
	//! \param name - ��� �����
	//! \result \b true, ���� ����������
	static rbool   exist          (CREF(tstring) name);

	//! ��������� ������� ������ ������
	//! \param name - ��� �����
	//! \result \b true, ���� � ����� ���������� ������� ������ ������
	static rbool   read_only      (CREF(tstring) name);

	//! ������� ����
	//! \param name - ��� �����
	//! \result \b true, ���� ���� ����� ������
	static rbool   unlink         (CREF(tstring) name);

	//! ��������� ������ ��� ����� �� ��� ������������
	//! \param[in]  name     - ������ ��� �����
	//! \param[out] fileDir  - ����
	//! \param[out] fileName - ���
	//! \param[out] fileExt  - ����������
	//! \result \b true, ���� ���������� ������� ������
	static rbool   splitpath      (CREF(tstring) name, REF(tstring) fileDir, REF(tstring) fileName, REF(tstring) fileExt);

	//! ���������� ������ ��� ���������� �����
	//! \result ������ ��� ���������� �����
	static tstring getTempFileName();

	//! ��������� ���� ����� �� ��� �����
	//! \param fileName - ������ ��� �����
	//! \result ���� � �����, ��������������� '/' ��� '\' � ����������� �� ���������
	static tstring extractFilePath(CREF(tstring) fileName);

	//! ������� ������� (�������, ���������, �������� �����) � ����� �����
	//! \param name - ��� �����
	//! \result \b true, ���� �� ������ �������
	static rbool   trimLeft       (CREF(tstring) name);
};

CLOSE_RDO_NAMESPACE

#include "utils/rdofile.inl"

#endif // _UTILS_RDOFILE_H_
