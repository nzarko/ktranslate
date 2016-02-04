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
#ifndef CENTRALVIEW_H
#define CENTRALVIEW_H

#include <centralviewbase.h>

#include "ktranslateio.h"
#include "ktranslatedata.h"

/**
@author Nikos Zarkadas
*/
class CentralView : public CentralViewBase
{
    Q_OBJECT
private :
	KTranslateIO *trIO;		/**< Object to send-retrieve data to-form dictionary file. */
	KTranslateVector trVector;   /**< Basic vector containing translations. */
	KTranslateVector trHVector; /**< Helper vector. */
	unsigned int m_noRecFound;/**< How many records have been found every time a translation happed.*/
	QString _rem;   	 		/**< The string in the combobox. */
public:
    CentralView(QWidget *parent = 0, const char *name = 0);

    ~CentralView();
    
    unsigned int showRecords();
    unsigned int noRecFound() const { return m_noRecFound; }
    QString ULStr(const QString &str) const;
    
public slots:
    void combBoxTextChangedSlot(const QString&);
    void comboBoxActivatedSlot(const QString& );
    void languageChange();
    void translateBtnClickedSlot();

signals:
//Will be emmited when a translation is requested.
	void translationRequested();
};

#endif
