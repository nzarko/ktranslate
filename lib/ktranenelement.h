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
#ifndef KTRANENELEMENT_H
#define KTRANENELEMENT_H

#include <qstring.h>

class QStringList;
class QTextStream;

/**
Represent the base (unkown) part of the KTranslateData.
The file with KTranslateElement records must have the following
format : "key:english_word\n"
@author Nikos Zarkadas
*/
class KTranslateElement
{
	private:
		QString m_key;
		QString m_enWord;
	public:
		KTranslateElement();
		~KTranslateElement();
     		KTranslateElement(QString &k, QString &ew);
     		KTranslateElement(const char*k, const char*ew);
		KTranslateElement(const KTranslateElement &elem);
		
		QString key() const {return m_key;}
		QString enWord() const {return m_enWord; }
		QStringList values() const ;
		void setValues(const QString &k, const QString &ew);
		void setEnWord(const QString &ew);
		void setKey(const QString &k);
		
		KTranslateElement &operator = (const KTranslateElement &elem);
		friend QTextStream &operator << (QTextStream &ts, const KTranslateElement &elem);
		friend QTextStream &operator >> (QTextStream &ts, KTranslateElement &elem);

};

#endif
