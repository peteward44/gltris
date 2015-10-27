// Exception.h: interface for the CException class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXCEPTION_H__2F896FF7_0959_4DFF_87CD_645D7C3A9408__INCLUDED_)
#define AFX_EXCEPTION_H__2F896FF7_0959_4DFF_87CD_645D7C3A9408__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CException  
{
public:
	CException(string error);
	CException();
	virtual ~CException();

	void SetErrorMessage(string error);
	virtual void HandleError();

protected:
	string errorMessage;
};


class CFatalException : public CException  
{
public:
	CFatalException(string error) : CException(error) {}
	virtual ~CFatalException() {}

};


class CTextureException : public CException  
{
public:
	CTextureException(string error) : CException(error) {}
	virtual ~CTextureException() {}

};

#endif // !defined(AFX_EXCEPTION_H__2F896FF7_0959_4DFF_87CD_645D7C3A9408__INCLUDED_)
