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
#ifndef KTRANSLATEDATA_H
#define KTRANSLATEDATA_H

#include <qstring.h>
#include <qstringlist.h>

#include "ktranenelement.h"
//#include "ktrantrelement.h"

/**
This class represent the data format of the 
ktranslate application

@author Nikos Zarkadas
*/

class KTranslateData
{
	private:
		QString m_key;
		QString m_enWord;
		QString m_trWord;
		QStringList m_translationLst;
		KTranslateElement enElem;
		//KTranTrElement trElem;
	public:
		KTranslateData();
		KTranslateData(QString &k, QString &ew, QStringList &lst);
		KTranslateData(const QString &, const QString &, const QString &, int index = 0);
		KTranslateData(const KTranslateElement &ee, const KTranslateElement &te, int pos);
		~KTranslateData();
		
		//getter methods :
		QString key() const { return m_key; }
		QString enWord() const { return m_enWord; }
		QString trWord() const { return m_trWord; }
		QStringList translationLst() const  { return m_translationLst; }
		
		//setter methods :
		void setKey(const QString &k) { m_key = k; }
		void setEnWord(const QString &ew) { m_enWord = ew; }
		void setValues(const QString &k, const QString &ew, const QStringList &lst);
		void setValues(const QString &k, const QString &ew, const QString &tw, int index = 0);
		void setTranslationLst ( const QStringList &lst) { m_translationLst = lst; }
		void setTranslation( const QString &translation, int pos) { m_translationLst[pos] = translation; }
		
		KTranslateData & operator[] (unsigned int index);
		
		friend bool operator < (const KTranslateData &, const KTranslateData &);
		friend bool operator > (const KTranslateData &, const KTranslateData &);
		friend bool operator == (const KTranslateData &, const KTranslateData &);
		friend bool operator != (const KTranslateData &, const KTranslateData &);
		
		friend QTextStream &operator << (QTextStream &ts, const KTranslateData &data);
		friend QTextStream &operator >> (QTextStream &ts, KTranslateData &data);
};

#endif
