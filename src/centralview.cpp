/***************************************************************************
 *   Copyright (C) 2004 by Nikos Zarkadas                                  *
 *   nikzar@linux                                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include <qtextcodec.h>

#include <klistview.h>
#include <kmessagebox.h>
#include <kcombobox.h>
#include <kpushbutton.h>
#include <klocale.h>

#include <kdebug.h>
 
#include "centralview.h"

CentralView::CentralView(QWidget *parent, const char *name)
        : CentralViewBase(parent, name)
{
	trIO = new KTranslateIO();
	
	if ( !trVector.isEmpty() )
		trVector.clear();
	
	m_wordListView->setHScrollBarMode(QScrollView::Auto);
	m_wordListView->setResizePolicy(QScrollView::Manual);
	setFocusPolicy(QWidget::StrongFocus);
	m_wToTranslateCbx->setFocus();
	m_translateButton->setEnabled(FALSE);
	
	 connect( m_wToTranslateCbx, SIGNAL( textChanged(const QString&) ), this,
	 	SLOT(combBoxTextChangedSlot(const QString&) ) );
}


CentralView::~CentralView()
{
	delete trIO;
}

/*! 
	\fn CentralView::showRecords()
*/
unsigned int CentralView::showRecords()
{
	m_wordListView->clear();
	
	trVector = trIO->allRecords();
	
	if ( trVector.isEmpty() )
	{
		KMessageBox::error(this, "No Records found! Check File permission and try again", "No translation found");
		return 0;
	}
	else
	{
		for (unsigned int i = 0; i < trVector.size(); i++)
		{
			KListViewItem *listItem = new KListViewItem(m_wordListView);
			listItem->setText(0, trVector[i].enWord() );
			listItem->setText(1, trVector[i].trWord() );
		}
		return trVector.size();
	}
}	

void CentralView::combBoxTextChangedSlot(const QString& str)
{
	if (str.isEmpty() )
		m_translateButton->setEnabled(FALSE);
	else
		m_translateButton->setEnabled(TRUE);
}

void CentralView::comboBoxActivatedSlot(const QString& str)
{
    	m_wordListView->clear();
	
	_rem = str;
	//Get the translations
	trVector = trIO->translate(str); //One vector with str as it gave in combobox
	trHVector = trIO->translate(ULStr(str)); //And another vector with the first letter of str in upper or 
								// lower case depends ...
	
	kdDebug(300) << "str = " << str << endl;
	kdDebug(301) << "str with ULStr filtering = " << ULStr(str) << endl;
	kdDebug() << "locale : " << QTextCodec::locale() << endl;
	
	if ( trVector.isEmpty() && trHVector.isEmpty() )
	{
		KMessageBox::error(this, "The string you entered doesn't exists", "No translation found");
		m_noRecFound = 0;
	}
	else
	{
		unsigned long int size = trVector.size();
		for (unsigned long int i = 0; i < size; i++)
		{
			KListViewItem *listItem = new KListViewItem(m_wordListView);
			listItem->setText(0, trVector[i].enWord() );
			//Convert from iso8859-7 to unicode
			//and then add it to list.

			listItem->setText(1, trVector[i].trWord() );
		}
		m_noRecFound = size;
		
		size = trHVector.size();
		if ( size != 0)
		{
			for (unsigned long int i = 0; i < size; i++)
			{
				KListViewItem *listItem = new KListViewItem(m_wordListView);
				listItem->setText(0, trHVector[i].enWord() );
				listItem->setText(1, trHVector[i].trWord() );
			}
			m_noRecFound += size;
		}
	}
	
	m_wToTranslateCbx->lineEdit()->selectAll();
	emit translationRequested();
}

void CentralView::languageChange()
{
    CentralViewBase::languageChange();
}

void CentralView::translateBtnClickedSlot()
{
	m_wordListView->clearSelection();
	comboBoxActivatedSlot(_rem);
}

/*!
    \fn CentralView::ULStr(const QString &str) const
    If the first character of str is in upper case then 
    return a string with the first character in lower case
    and vise versa.
 */
QString CentralView::ULStr(const QString &str) const
{
	int l = str.length();
	QString result;
	
	QChar first = str[0].upper();
	if ( first != str[0] ) //The str[0] is lower and the first is the equivalent upper.
	{
		QString temp = str.right(l - 1);
		temp.insert(0,first);
		result = temp;
	}
	
	first = str[0].lower();
	if ( first != str[0] ) //The str[0] is uppercase and the first is the equivalent lowercase.
	{
		QString temp = str.right(l - 1);
		temp.insert(0,first);
		result = temp;
	}
	
	return result;
}

#include "centralview.moc"

