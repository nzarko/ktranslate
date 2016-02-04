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

#ifndef ADDWORDDIALOG_H
#define ADDWORDDIALOG_H

#include "addworddialogbase.h"
#include "ktranslateio.h"

class QString;

class AddWordDialog : public AddWordDialogBase
{
  Q_OBJECT

public:
  AddWordDialog(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
  ~AddWordDialog();
  /*$PUBLIC_FUNCTIONS$*/

public slots:
  /*$PUBLIC_SLOTS$*/
  virtual void          advanceEditButton_clicked();
  virtual void          finishButtonClickedSlot();
  virtual void          progressOkButtonClickedSlot();
  virtual void          progressTryAgainButtonClickedSlot();
  virtual void          progressEditButtonClickedSlot();
  virtual void          trWordLETextChanged( const QString & );
  virtual void          enWordLETextChanged( const QString & );
  virtual void          findRecordsFileSlot();
  virtual void          checkBoxStateChanged( int );
  virtual void          clearAllButtonSlot();
  virtual void          addWordToLVSlot();

protected:
  /*$PROTECTED_FUNCTIONS$*/

protected slots:
  /*$PROTECTED_SLOTS$*/
  virtual void          accept();
  
private:
	QString fileName;
	KTranslateData trData;
	KTranslateVector trVector;
	KTVCiter iter;
	void load ( const QString & filename );
    	void lvInsertTranslations(KTranslateVector &trVec);

};

#endif

