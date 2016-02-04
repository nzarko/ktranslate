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
#include "ktranslateio.h"

#include <qregexp.h>

#include <kdebug.h>
#include <kstddirs.h>
#include <kglobal.h>
#include <kmessagebox.h>
#include <kurl.h>
#include <kxmlguiclient.h>
#include <kio/job.h>
#include <kio/jobclasses.h>
#include <klocale.h>
#include "../src/ktranslate.h"

#include <algorithm>

KTranslateIO::KTranslateIO()
{
	createLocalResources();
	
	dictFile.setName(localFileName);
	if ( !dictFile.exists() )
		kdDebug() << "The " << dictFile.name() << " doesn't exists" << endl;
	
	if ( !trVector.isEmpty() )
		trVector.clear();
	
	isModified = false;
}

/*!
    \fn KTranslateIO::createLocalResources()
 */
void KTranslateIO::createLocalResources()
{
	KLocale *cur_locale = KGlobal::locale();
	QString loc_name = QString(cur_locale->encoding());
	QRegExp rxp = QRegExp("utf*8", FALSE, TRUE);	
	int isUTF8 = loc_name.contains(rxp);
	if ( isUTF8 > 0 )
	{
		kdDebug() << "KTranslateIO locale : " << loc_name << endl;
		kdDebug() << "KTranslateIO isUTF8 --> " << isUTF8 << endl;
	}
	else
		kdDebug() <<"KTranslateIO locale : Not a UTF8" << endl;
	QString dFile = isUTF8 >= 1 ? QString("ktranslatepart/dictionary_utf8.zdf") : QString("ktranslatepart/dictionary_elGR.zdf");
	//Find global resources (usually there are where the globa kde dir is.
	KStandardDirs *dirs = KGlobal::dirs();
	dFile = dirs->findResource("data", dFile );
	if (dFile.isEmpty() )
		dFile = get_file_name();
	
	//Debug info.
    	kdDebug() << "Global Dictionary file -->" << dFile << endl;
	
	//Check for the local dictionary file name. 
	//If the file doesn't exist then we will create it.
	localFileName = locateLocal ("appdata", "dictionary.zdf");
	
	if ( QFile::exists(localFileName) )
		kdDebug() << "Dictionary file already exists in " << locateLocal("data","ktranslate/") << endl;
	else
	{
		kdDebug() << "Create local files." << endl;
        	KURL srcURL (dFile );
		KIO::file_copy ( dFile, localFileName );
		kdDebug() << "Local files created succesfull." << endl;
	}
	
	//Create the strings that represents the temporary and the backup file names.
	tmpFileName = locateLocal("appdata", "dict_tmp.tmp");
	backupFileName = locateLocal( "appdata", "backup.txt");
}

KTranslateIO::~KTranslateIO()
{
	trVector.clear();
	if ( isModified )
	{
		kdDebug() << "Create backup file!!" << endl;
		KIO::file_delete((KURL)backupFileName);
		KIO::file_copy((KURL)localFileName, (KURL)backupFileName);
	}
}


/*!
    \fn KTranslateIO::translate(const QString &s)
 */
KTranslateVector KTranslateIO::translate(const QString &s)
{
	trVector.clear();
	
	int len = s.length();
	
	if ( !dictFile.open(IO_ReadOnly) )
	{
		kdDebug() << "Can't open " << dictFile.name() << " for reading" << endl;
		return trVector;
	}
	
	QTextStream ts(&dictFile);
	QCString tmp;
	while( !ts.atEnd() )
	{
		ts >> trData;
		QString substr = trData.enWord().left(len);
		if ( substr == s)
			trVector.push_back(trData);
		if ( substr[0] > s[0] )
			break;
	}
	
	dictFile.close();
	return trVector;
}

/*!
    \fn KTranslateIO::revTranslate(const QString &tr_str)
 */
KTranslateVector KTranslateIO::revTranslate(const QString &tr_str)
{
    /// @todo implement me
}

/*!
    \fn KTranslateIO::allRecords()
 */
KTranslateVector KTranslateIO::allRecords()
{
	trVector.clear();
	//check if the file is arleady open.
	if ( dictFile.isOpen() )
		dictFile.close();
	
	if ( !dictFile.open(IO_ReadOnly) )
	{
		kdDebug() << "Can't open " << dictFile.name() << " for reading" << endl;
		return trVector;
	}
	
	QTextStream ts(&dictFile);
	
	while( !ts.atEnd() )
	{
		ts >> trData;
		trVector.push_back(trData);
	}
	
	dictFile.close();
	return trVector;
}

/*!
    \fn KTranslateIO::join(const QStringList filenames)
 */
void KTranslateIO::join(const QStringList filenames)
{
    /// @todo implement me
}


/*!
    \fn KTranslateIO::split(const QString lang=QString::null)
 */
void KTranslateIO::split(QString lang)
{
    /// @todo implement me
}

/*!
    \fn KTranslateIO::sort()
 */
void KTranslateIO::sort()
{
	sort(localFileName);
}


/*!
    \fn KTranslateIO::sort(const QString &filename)
 */
void KTranslateIO::sort(const QString &filename)
{
	dictFile.setName(filename);
	    
	//check if the file is arleady open.
	if ( dictFile.isOpen() )
		dictFile.close();
	    //Fill the vector;
	if ( !dictFile.open(IO_ReadOnly) )
	{
		kdFatal(200)<< "Can't open " << filename << " file for reading" << endl;
		return;
	}
	QTextStream ts(&dictFile);
	
	while(!ts.atEnd())
	{
		ts >> trData;
		trVector.push_back(trData);
	}
	dictFile.close();
	
	//Sort the vector.
	std::stable_sort(trVector.begin(), trVector.end());
	
	//remove the same translations
	removeSameTranslations(trVector);
	//Regenerate the keys from beggining
	recount(trVector);
	
	//Write back the sorted data to file.
	if( !dictFile.open(IO_WriteOnly))
	{
		kdFatal(201) << "Can't open " << filename << " file for writing" << endl;
	}
	QTextStream wts(&dictFile);
	KTVCiter iter;
	for( iter = trVector.begin(); iter != trVector.end(); iter++)
		wts << *iter;
	
	isModified = true;
	dictFile.close();
}


/*!
    \fn KTranslateIO::deleteRec(const KTranslateData &data)
 */
bool KTranslateIO::deleteRec(const KTranslateData &data)
{
	kdDebug() << "Trying to delete " << data.enWord() << "-->" << data.trWord() << endl;
	
	KTranslateData t_data;
	QFile tmp_file(tmpFileName);
	
	if ( !dictFile.open(IO_ReadOnly) || !tmp_file.open(IO_WriteOnly) )
	{
		kdDebug() << "KTranslateIO::deleteRec --> Can't open files to delete the record." << endl;
		return false;
	}
	
	QTextStream ts(&dictFile);
	QTextStream tmp_ts (&tmp_file);
	
	while( !ts.atEnd() )
	{
		ts >> t_data;
		if ( t_data != data )
			tmp_ts << t_data;
		else
			kdDebug() << "Find the deleted " << endl;
	}
	
	dictFile.close();
	tmp_file.close();
	
	//Now delete the old file and rename the tmp file as the new dictionary file.
	KIO::file_delete(localFileName);
	KIO::file_move(tmpFileName, localFileName);
	
	kdDebug() << "Delete completed succesfull" << endl;
	//Inform that the dictionary file has modified.
	isModified = true;
	return true;
}


/*!
    \fn KTranslateIO::deleteRec(const QString &enW, const QString &trW)
 */
bool KTranslateIO::deleteRec(const QString &enW, const QString &trW)
{
    KTranslateData t_data ("0", enW, trW);
    return deleteRec(t_data);
}

/*!
    \fn KTranslateIO::editRec(const KTranslateData &data)
 */
bool KTranslateIO::editRec(const KTranslateData &old_data, const KTranslateData &new_data)
{
	return (insert(new_data) && deleteRec(old_data) );
}


/*!
    \fn KTranslateIO::editRec(const QString &enW, const QString &trW,
    	const QString &new_enW, const QString &new_trW)
 */
bool KTranslateIO::editRec(const QString &old_enW, const QString &old_trW,
	const QString &new_enW, const QString &new_trW)
{
	KTranslateData old_data("0", old_enW, old_trW);
	KTranslateData new_data("1", new_enW, new_trW);
	
	return editRec(old_data, new_data);
}

/*!
    \fn KTranslateIO::findRec(const QString &enW, const QString &trW)
    Finds the record with english_word=enW and hellenic_word=trW.
    Return the key of this record. We use this method when we edit or
    delete a record from the file. we need the key so the new record 
    has the same key as the old one.
    @return the key of this record.
 */
QString KTranslateIO::findRec(const QString &enW, const QString &trW)
{
	//Find all the records with the english_word=enW.
	KTranslateVector vec = translate(enW);
	
	unsigned int sz = vec.size();
	//Form these records find exaclty that one with hellenic_word=trW.
	for( unsigned i = 0; i < sz; i++)
	{
		if ( trW == vec[i].trWord() )
		{
			_oldkey = vec[i].key();
			kdDebug() << "KTranslateIO::findRec return : " << _oldkey << endl;
		}
	}
	
	return _oldkey;
}

/*!
    \fn KTranslateIO::insert(const KTranslateData &data)
 */
bool KTranslateIO::insert(const KTranslateData &data)
{
	if ( dictFile.isOpen() )
		dictFile.close();
	
	if ( !dictFile.open(IO_WriteOnly | IO_Append ) )
	{
		kdDebug() << "Can't Open " << localFileName << " for append!" << endl;
		return false;
	}
	
	QTextStream ts(&dictFile);
	ts << data;
	dictFile.close();
	
	sort();
	return true;
}

/*!
    \fn KTranslateIO::Insert(const KTranslateVector &)
    insert new records at hte end of the dictionary file 
    and sort the file.
    @param newTrVector The vector containing the new records.
    @return How many records are inserted or -1 if something goes wrong.
 */
int KTranslateIO::insert(const KTranslateVector &newTrVector, bool overwrite)
{
	KTVCiter iter;
	int count = 0;
	int open_mode;
	
	//check if the file is arleady open.
	if ( dictFile.isOpen() )
		dictFile.close();
	
	if (overwrite == false)
		open_mode =  IO_WriteOnly | IO_Append;
	else
		open_mode = IO_WriteOnly;
	
	if( !dictFile.open(open_mode))
	{
		kdFatal(202) << "Can't open " << dictFile.name() << " file for writing" << endl;
		return -1;
	}
	kdDebug(203) << "File open succesfull. Now inserting the new records..." << endl;
	QTextStream wts(&dictFile);

	for( iter = newTrVector.begin(); iter != newTrVector.end(); iter++)
	{
		count++;
		wts << *iter;
	}
	
	kdDebug(80) << "OK!" << endl;
	sort();
	
	dictFile.close();
	return count;
}


/*!
    \fn KTranslateIO::count()
    @return How meny records are in the dictionary file.
 */
unsigned int KTranslateIO::count()
{
	uint result = allRecords().size();
	return result;
}


/*!
    \fn KTranslateIO::removeSameTranslations(KTranslateVector &tvec)
    Remove the same translations from dictionary file.
    Be carefull : This method must called after a call to KTranslateIO::sort
    method.
    @param tvec The vector containing the ordered translations of the 
    dictionary file.
 */
void KTranslateIO::removeSameTranslations(KTranslateVector &tvec)
{
	/* ******************************************************* *
	 * 	ATTENTION DO NOT CALL SORT METHOD INSIDE THIS METHOD. *
	 *    DICTIONARY FILE ARE ALREADY SORTED AND THIS METHOD DO    *
	 *    NOT CHANGE ANYTHING IN DICTIONARY FILE ORDER, IT JUST  	*
	 *    REMOVES THE SAME RECORDS FROM THE FILE.				*
	 * ******************************************************* */ 
	KTVCiter it;
	//Remove double records.
	KTranslateData curItem, nextItem;
	KTranslateVector newVector;
	
	unsigned int itemsNo = tvec.size();
	for (unsigned int index = 0; index < itemsNo; index++)
	{
		if (index == 0)
		{
			newVector.push_back(tvec[0]);
			curItem = tvec[0];
		}
		else
		{
			nextItem = tvec[index];
			if (curItem == nextItem)
			{
				//kdDebug() << "Ignore " << nextItem.enWord() << "-->"<<nextItem.trWord() << endl;
				continue;
			}
			else
			{
				newVector.push_back(tvec[index]);
				//kdDebug() << "Added " << nextItem.enWord << "-->"<<nextItem.trWord << endl;
				curItem = nextItem;
			}
		}
	}
	
	//Clear the tvec
	tvec.clear();
	//Copy the new elements from newVector
	for (it = newVector.begin(); it != newVector.end(); it++)
        	tvec.push_back ( *it );
}


/*!
    \fn KTranslateIO::discount(QString key)
    Create keys from beggining starting with 1.
    This function must be called after the last call \n
    to removeSameTranslations method.
 */
void KTranslateIO::recount(KTranslateVector &tvec)
{
	unsigned count = 1;
	QValueVector<KTranslateData>::iterator iter;
	
	for( iter = tvec.begin(); iter != tvec.end(); iter++ )
	{
		iter->setKey(QString::number(count, 10));
		count ++;
	}
}
