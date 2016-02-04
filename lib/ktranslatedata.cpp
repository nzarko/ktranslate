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
#include <qtextcodec.h>

#include "ktranslatedata.h"
#include "separators.h"

#include <kdebug.h>

/*!
	\fn KTranslateData::KTranslateData()
	Default constructor.
*/
KTranslateData::KTranslateData()
{
	m_key = "";
	m_enWord = "";
	if( !m_translationLst.isEmpty() )
		m_translationLst.clear();
}

/*!
	\fn KTranslateData::KTranslateData(QString &k, QString &ew, QStringList &lst)
	Construct a KTranslateData object with initial values.
*/
KTranslateData::KTranslateData(QString &k, QString &ew, QStringList &lst)
{
	m_key = k;
	m_enWord = ew;
	m_translationLst = lst;
}

/*!
    \fn KTranslateData::KTranslateData(const QString &, const Qstring &, const QString &, int index = 0)
 */
 KTranslateData::KTranslateData(const QString &k, const QString &ew, const QString &tw, int index)
{
	m_key = k;
	m_enWord = ew;
	m_translationLst[index] = tw;
	m_trWord = tw;
}

/*!
	\fn KTranslateData::KTranslateData(const KTranslateElement&ee, const KTranTrElement &te, int pos)
*/
KTranslateData::KTranslateData(const KTranslateElement &ee1, const KTranslateElement &ee2, int pos)
{
	if ( ee1.key() == ee2.key() )
		m_key = ee1.key();
	
	m_enWord = ee1.enWord();
	m_translationLst[pos] = ee2.enWord();
}

/*!
	\fn KTranslateData::~ KTranslateData()
	Destructor.
*/
KTranslateData::~KTranslateData()
{
}

/*!
	\fn void KTranslateData::setValues(const QString &k, const QString &ew, const QStringList &lst)
*/
void KTranslateData::setValues(const QString &k, const QString &ew, const QStringList &lst)
{
	setKey(k);
	setEnWord(ew);
	m_translationLst = lst;
}

/*!
    \fn KTranslateData::setValues(const QString &k, const QString &ew, const QString &tw, int index = 0)
 */
void KTranslateData::setValues(const QString &k, const QString &ew, const QString &tw, int index)
{
	m_key = k;
	m_enWord = ew;
	m_trWord = tw;
	m_translationLst[index] = tw;
}

/*!
	\fn operator <(const KTranslateData &trData1, const KTranslateData &trData2)
*/
bool operator < (const KTranslateData &trData1, const KTranslateData &trData2)
{
	if ( trData1.m_enWord == trData2.m_enWord )
		return (trData1.m_translationLst[0] < trData2.m_translationLst[0]);
	return (trData1.m_enWord < trData2.m_enWord);
}

/*!
	\fn KTranslateData::operator >(const KTranslateData &trData)
*/
bool operator >(const KTranslateData &trData1, const KTranslateData &trData2)
{
	return !(trData1 < trData2 );
}

/*!
	\fn KTranslateData::operator ==(const KTranslateData &trData)
*/
bool operator ==(const KTranslateData &trData1, const KTranslateData &trData2)
{
	return ((trData1.m_enWord == trData2.m_enWord) && 
	(trData1.m_trWord == trData2.m_trWord)) ;
}

/*!
	\fn KTranslateData::operator !=(const KTranslateData &trData)
*/
bool operator != (const KTranslateData &trData1, const KTranslateData &trData2)
{
	return !(trData1 == trData2 );
}

/*!
	\fnQTextStream &operator << (QTextStream &ts, const KTranslateData &data)
	Writes translation data to file.
*/
QTextStream &operator << (QTextStream &ts, const KTranslateData &data)
{
	QStringList::ConstIterator ct;
	QStringList temp;
	
	//ts.setCodec(QTextCodec::codecForName("ISO8859-7"));
	
	ts << data.key() << KEY_SEP << data.enWord() << EN_SEP;
	
	//The following snipet is necessery because the are problems 
	//if the m_translationLst has only one string
	//in case that m_translationLst contains more than one translations
	if ( data.m_translationLst.size() > 1) 
	{	
		//remove white spaces from begin and the end of the translations.
		for ( ct = data.translationLst().begin(); ct != data.translationLst().end(); ++ct )
			temp += (*ct).stripWhiteSpace();
	
	 	for( ct = temp.begin(); ct != temp.end() ; ++ct )
			ts << *ct << TRAN_SEP;
 	}
	else //only the greek translation is going to be changed.
	{
		QString  twp = data.m_translationLst[0];
		ts << twp;
	}	
	 
	 ts << LINE_SEP;
	 
	 return ts;
}

/*!
	\fn QTextStream &operator >> (QTextStream &ts, KTranslateData &data)
*/
QTextStream &operator >> (QTextStream &ts, KTranslateData &data)
{
	QStringList keyList, fullRecLst, tranLst;
	
	//ts.setCodec(QTextCodec::codecForName("ISO8859-7"));
	
	QString line = ts.readLine();
	//kdDebug(10) << "line : " << line << endl;
	
	keyList = QStringList::split(KEY_SEP, line);
	//Now keyList[0] = key.
	//Show debug info.
	//kdDebug(10) << "keyList[0] : " << keyList[0] << endl;
	//kdDebug(10) << "keyList[1] : " << keyList[1] << endl;
	//Split the keyList[1] to english words and the translation.
	fullRecLst = QStringList::split(EN_SEP, keyList[1]);
	//kdDebug(10) << "fullRecLst[0] : " << fullRecLst[0] << endl;
	//kdDebug(10) << "fullRecLst[1] : " << fullRecLst[1] << endl;
	//Now fullRecLst[0] = m_enWord.
	//Split the fullRecLst[1] and fill the m_translationLst
	tranLst = QStringList::split(TRAN_SEP, fullRecLst[1]);
	//kdDebug(10) << "tranLst[0] : " << tranLst[0] << endl;
	//kdDebug(10) << "tranLst[1] : " << tranLst[1] << endl;
	
	if ( tranLst.size() == 1)
		data.setValues(keyList[0], fullRecLst[0], tranLst[0] );
	else
		data.setValues(keyList[0], fullRecLst[0], tranLst);	
	
	return ts;
}

/*!
    \fn KTranslateData::operator[] (unsigned int index)
 */
KTranslateData & KTranslateData::operator[] (unsigned int index)
{
	m_trWord = this->m_translationLst[index];
	return *this;
}

