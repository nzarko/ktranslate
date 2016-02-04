/***************************************************************************
 *   Copyright (C) 2004 by Nikos Zarkadas                                  *
 *   zarko@grecom.gr                                                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "ktranenelement.h"
#include "separators.h"	//for separators intedifiers.

#include <qstringlist.h>
#include <qtextcodec.h>

KTranslateElement::KTranslateElement()
{
	m_key = "";
	m_enWord = "";
}


KTranslateElement::~KTranslateElement()
{
}

/*!
    \fn KTranslateElement::KTranslateElement(QString &k, QString &ew)
 */
 KTranslateElement::KTranslateElement(QString &k, QString &ew)
{
	m_key = k;
	m_enWord = ew;
}

/*!
    \fn KTranslateElement::KTranslateElement(const char*k, const char*ew)
 */
 KTranslateElement::KTranslateElement(const char*k, const char*ew)
{
	m_key = k;
	m_enWord = ew;
}

/*!
    \fn KTranslateElement::KTranslateElement(const KTranslateElement &elem)
    copy constructor
 */
 KTranslateElement::KTranslateElement(const KTranslateElement &elem)
{	
	QString k(elem.key());
	QString w(elem.enWord());
	
	m_key = k;
	m_enWord = w;
}

/*!
	fn\ KTranslateElement::values() const 
	@return the values of m_key and m_enWord variables in a StringList object.
*/
 QStringList KTranslateElement::values() const
{
	QStringList lst;
	lst.append(m_key);
	lst.append(m_enWord);
	
	return lst;
}

/*!
    \fn KTranslateElement::setValues(const QString &k, const QString &ew)
 */
void KTranslateElement::setValues(const QString &k, const QString &ew)
{
	m_key = k;
	m_enWord = ew;
}


/*!
    \fn KTranslateElement::setEnWord(const QString &ew)
 */
void KTranslateElement::setEnWord(const QString &ew)
{
	m_enWord = ew;
}


/*!
    \fn KTranslateElement::setKey(const QString &k)
 */
void KTranslateElement::setKey(const QString &k)
{
	m_key = k;
}


/*!
    \fn KTranslateElement::operator = (const KTranslateElement &elem)
 */
KTranslateElement &KTranslateElement::operator = (const KTranslateElement &elem)
{
	if ( this == &elem ) //object assignent to itself
		return *this;
	
	QString k(elem.key());
	QString w(elem.enWord());
	
	m_key = k;
	m_enWord = w;
	
	return *this;
}

/* *********************************************** *
 * overloaded operators << and >> for IO manipulation         *
 * *********************************************** */
 /*!
 	\fn QTextStream &operator << (QTextStream &ts, const KTranslateElement &elem)
	write a KTranslateElement object to a text file
 */
 QTextStream &operator << (QTextStream &ts, const KTranslateElement &elem)
 {
	 //ts.setCodec(QTextCodec::codecForName("ISO8859-7"));
	 //ts.setEncoding(QTextStream::Locale);
	 //Ignore the white spaces at the beginning and at the end of 
	 //the KTranslateElement variables and then write them to file.
	 QString k = elem.key().stripWhiteSpace();
	 QString ew = elem.enWord().stripWhiteSpace();
	 
	 ts << k << KEY_SEP << ew << LINE_SEP;
	 
	 return ts;
 }
 
 /*!
 	\fn QTextStream &operator >> (QTextStream &ts, KTranslateElement &elem)
	Reads a KTranslateElement object from a text file.
 */
 QTextStream &operator >> (QTextStream &ts, KTranslateElement &elem)
 {
	 //ts.setCodec(QTextCodec::codecForName("ISO8859-7"));
	QString data = ts.readLine();
	
	//Split each line in two strings : key and translation.
	QStringList fields = QStringList::split(KEY_SEP, data);
	
	//remove white spaces from begin and the end of eatch field and replace the 
	//internal white spaces with a sigle white space.
	QString key = fields[0];
	QString ew = fields[1];
	
	//set the new values
	elem.setValues(key, ew);
	
	return ts;
 }
 
