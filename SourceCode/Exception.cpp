// Exception.cpp: implementation of the CException class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Exception.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CException::CException(string error)
{
	SetErrorMessage(error);
}

CException::CException()
{
	SetErrorMessage("");
}

CException::~CException()
{

}

//////////////////////////////////////////////////////////////////////

void CException::SetErrorMessage(string error)
{
	errorMessage = error;
}

void CException::HandleError()
{
	MessageBox(NULL, errorMessage.c_str(), "Error", MB_OK);
}
