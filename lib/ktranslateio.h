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
#ifndef KTRANSLATEIO_H
#define KTRANSLATEIO_H

#include <ktranslatedata.h>
#include <qvaluevector.h>
#include <qstring.h>
#include <qfile.h>
#include <qstringlist.h>

typedef QValueVector<KTranslateData> KTranslateVector;
typedef QValueVector<KTranslateData>::const_iterator KTVCiter;
/**
Manipulate files with KTranslateData records.

@author Nikos Zarkadas
*/

/**@todo: change the way of this class loading the data from file.
 * Read all data in a vector at the begining of the program
 * and the read and write actions take place at the vector,
 * instead of reading and write every time the program demand
*/
class KTranslateIO
{
	private:
		QFile dictFile;
		KTranslateData trData;
		KTranslateVector trVector;
		KTVCiter trVIter;
		QString localFileName;
		QString tmpFileName;
		QString backupFileName;
		QString _oldkey;
		bool isModified;
	
		void createLocalResources();
    		void removeSameTranslations(KTranslateVector &tvec);
    		void recount(KTranslateVector &tvec);
	public:
		KTranslateIO();
		
		~KTranslateIO();
		KTranslateVector translate(const QString &s);
		KTranslateVector revTranslate(const QString &tr_str);
		KTranslateVector allRecords();
		void join(const QStringList filenames);
		void split(QString lang=QString::null);
		//local file names.
		const QString dictFileName() { return localFileName; }
		const QString tempFileName() { return tmpFileName; }
		const QString backupFName() { return backupFileName; }
		
		//Add , edit,  delete, sort methods.
		bool deleteRec(const KTranslateData &data);
		bool deleteRec(const QString &enW, const QString &trW);
		void sort();
		void sort(const QString &filename);
    		unsigned int count();
		bool editRec(const KTranslateData &old_data, const KTranslateData &new_data);
		bool editRec(const QString &old_enW, const QString &old_trW, 
			const QString &new_enW, const QString &new_trW);
		bool insert(const KTranslateData &data);
		int insert(const KTranslateVector &, bool overwrite = false);
		QString findRec(const QString &enW, const QString &trW);
};

#endif
