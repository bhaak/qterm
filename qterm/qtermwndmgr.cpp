/*******************************************************************************
FILENAME:      qtermwndmgr.cpp
REVISION:      2001.8.12 first created.
         
AUTHOR:        kingson fiasco
*******************************************************************************/
/*******************************************************************************
                                    NOTE
 This file may be used, distributed and modified without limitation.
 *******************************************************************************/
#include "qtermwndmgr.h"
#include "qtermwindow.h"
#include "qtermframe.h"

#include <qapplication.h>
#include <qtabbar.h>
#include <qiconset.h>
#include <qtimer.h>

#if (QT_VERSION>=300)
#include <qptrlist.h>
#else
#include <qlist.h>
#endif

#include <stdio.h>

extern QString pathLib;


//constructor
QTermWndMgr::QTermWndMgr( QObject * parent, const char * name )
	:QObject(parent,name)
{
	pFrame = (QTermFrame *) parent;
	
	nActive = -1;
	pWin.setAutoDelete( false );
	pTab.setAutoDelete( false );
	pIcon.setAutoDelete( false );
}

//destructor
QTermWndMgr::~QTermWndMgr()
{
}

//add window-tab-iconset 
int   QTermWndMgr::addWindow(QTermWindow * mw,QTab * qtab, QIconSet * icon)
{
  
	ASSERT(mw!=NULL);
	pWin.append(mw);
	ASSERT(qtab!=NULL);
	pTab.append(qtab);
	pIcon.append(icon);

	if( pWin.count()==1 )
		pFrame->enableMenuToolBar( true );

	return pTab.count();
}

//remove window-tab-iconset
void QTermWndMgr::removeWindow(QTermWindow * mw)
{
	//find where it is
	int n=pWin.find(mw);
	QTab * qtab=pTab.at(n);
	QIconSet *qicon=pIcon.at(n);
	//remove them from list
	pTab.remove(qtab);
	pWin.remove(mw);
	pIcon.remove(qicon);

	if( pWin.count()==0 )
	{
		nActive = -1;
		pFrame->enableMenuToolBar( false );
	}
	
	//remove from the Tabbar
	pFrame->tabBar->removeTab(qtab);	
}
//avtive the tab when switch the window
void QTermWndMgr::activateTheTab(QTermWindow * mw)
{
	//find where it is
	int n=pWin.find(mw);

	nActive = n;

	QTab * qtab=pTab.at(n);
	//set it seleted
	
	pFrame->tabBar->setCurrentTab(qtab);

	pFrame->updateMenuToolBar();
}
//active the window when switch the tab
void QTermWndMgr::activateTheWindow(QTab *qtab)
{
	//find where it is
	int n=pTab.find(qtab);

	nActive = n;

	QTermWindow * mw=pWin.at(n);
	//set focus to it
	mw->setFocus();	
	
	pFrame->updateMenuToolBar();
}
//blink the tab when message come in
void QTermWndMgr::blinkTheTab(QTermWindow * mw,bool bVisible)
{
	//find where it is
	int n=pWin.find(mw);
	QIconSet* icon=pIcon.at(n);
		
	if(bVisible)
		icon->setPixmap(pathLib+"./pic/tabpic.xpm",QIconSet::Automatic);
	else
		icon->setPixmap(pathLib+"./pic/blink.xpm",QIconSet::Automatic);

	pFrame->tabBar->update();		
}
//return the number of connected window
int QTermWndMgr::count()
{
	return pWin.count();
}

QTermWindow * QTermWndMgr::activeWindow()
{
	if( nActive == -1 )
		return NULL;
	else
		return pWin.at( nActive );
}

void QTermWndMgr::activeNextPrev(bool next)
{
	int n = nActive;
	
	if(n==-1)
		return;
	if(next)
		n = (n==pWin.count()-1)?0:n+1;
	else
		n = (n==0)?pWin.count()-1:n-1;
	
	nActive = n;
	pWin.at(n)->setFocus();
}