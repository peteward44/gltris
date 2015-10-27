// SingletonDestroyer.h: interface for the CSingletonDestroyer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINGLETONDESTROYER_H__9D55338C_01E8_4CDC_96ED_CA971C6F927F__INCLUDED_)
#define AFX_SINGLETONDESTROYER_H__9D55338C_01E8_4CDC_96ED_CA971C6F927F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// CSingletonDestroyer
// makes sure Singleton classes are destroyed correctly


template <class myclass>
class CSingletonDestroyer  
{
public:
	// Constructor, sets the singleton class
	CSingletonDestroyer(myclass* t = 0) { SetSingleton(t); };

	// destructor, deletes the singleton class
	virtual ~CSingletonDestroyer() { if (_singleton) delete _singleton; };

	// SetSingleton, set the singleton class later on.
	void SetSingleton(myclass* t) { _singleton = t; };

private:
	myclass* _singleton;
};


#endif // !defined(AFX_SINGLETONDESTROYER_H__9D55338C_01E8_4CDC_96ED_CA971C6F927F__INCLUDED_)
