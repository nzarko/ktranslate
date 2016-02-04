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
#include "ktranslateio.h"
#include "ktranslatedata.h"

#include <qlabel.h>
#include <qtextbrowser.h>
#include <qstring.h>
#include <qwidgetstack.h>
#include <qcheckbox.h>
 
#include <kpushbutton.h>
#include <klineedit.h>
#include <kprogress.h>
#include <klistview.h>
#include <kdebug.h>
#include <kfiledialog.h>
#include <krun.h>
#include <kstandarddirs.h>
#include <kmessagebox.h>
#include <klocale.h>

#include "addworddialog.h"

AddWordDialog::AddWordDialog(QWidget* parent, const char* name, bool modal, WFlags fl)
: AddWordDialogBase(parent,name, modal,fl)
{
	m_widgetStack->raiseWidget(defaultPage);
        m_findRecordsFileBtn->setEnabled(FALSE);
        m_addWordBtn->setEnabled(FALSE);
        //m_applyBtn->setEnabled(FALSE);
	
	if ( !trVector.isEmpty() )
		trVector.clear();
	
	//Debug info
        kdDebug(80) << "AddWordDialog created!" << endl;
}

AddWordDialog::~AddWordDialog()
{
	kdDebug(80) << "AddWordDialog deleted!" << endl;
}

/*$SPECIALIZATION$*/
void AddWordDialog::advanceEditButton_clicked()
{
}

void AddWordDialog::finishButtonClickedSlot()
{	
	if ( !trVector.isEmpty() )
		trVector.clear();
	
	QString line;
	QStringList tranLst;
	
	QFile file(fileName);
	
	if ( !file.open(IO_ReadOnly) )
	{
		kdFatal(90) << "Can't open " << fileName << " for read" << endl;
	}
	
	QTextStream ts(&file);
	while ( !ts.atEnd() )
	{
		//Read the file and split it to records.
		line = ts.readLine();
		tranLst = QStringList::split("=", line);
		//We cast this list to KTranslateData with m_key = 0 only for conviniens
		trData.setValues("0", tranLst[0], tranLst[1]);
		trVector.push_back(trData);
	}
	file.close();
	
	m_widgetStack->raiseWidget(defaultPage);
	lvInsertTranslations(trVector);
	//m_addDlgListView->update();
}

/*!
    \fn AddWordDialog::lvInsertTranslations(KTranslateVector &trVec)
 */
void AddWordDialog::lvInsertTranslations(KTranslateVector &trVec)
{
	KTVCiter iter;
	
	for( iter = trVec.begin(); iter != trVec.end(); iter++ )
	{
		KListViewItem *newItem = new KListViewItem (m_addDlgListView);
        	newItem->setText(0, iter->enWord());
        	newItem->setText(1, iter->trWord());
	}
}

void AddWordDialog::progressOkButtonClickedSlot()
{
	m_progressEditButton->setEnabled(TRUE);
	m_errorTextBrowser->setBold(true);
	m_errorTextBrowser->insert(QString("parsing %1 file\n").arg(fileName));
	m_errorTextBrowser->insert("Starting now...\n");
	m_errorTextBrowser->setBold(false);
	
	QFile file(fileName);
	
	if ( !file.open(IO_ReadOnly) )
	{
		kdFatal(90) << "Can't open " << fileName << " for read" << endl;
	}
	
	QTextStream ts(&file);
	QString doc = ts.read();
	m_errorTextBrowser->insert(doc);
	
	file.close();
	
	QString endMessage = "Check the above records and if you find no problem with these then\n"
			"press the \"Finish\" button, else press the \"Edit\" button!!\n";
	m_errorTextBrowser->setBold(TRUE);
	m_errorTextBrowser->insert(endMessage);
}

void AddWordDialog::progressTryAgainButtonClickedSlot()
{
}

void AddWordDialog::progressEditButtonClickedSlot()
{
	m_progressTryAgainButton->setEnabled ( TRUE );
        KRun::run("kwrite", QStringList(fileName) ,
                "kwrite", locate ("icon", "hicolor/SuSE-16x16/apps/kwrite.png") );
}

void AddWordDialog::trWordLETextChanged( const QString &curText )
{
	//kdDebug() << "AddWordDialog::trWordLETextChanged() Message: I'm in" << endl;
        if (curText.isEmpty() && m_enWordLineEdit->text().isEmpty())
                m_addWordBtn->setEnabled(FALSE);
        else if(!curText.isEmpty() && !m_enWordLineEdit->text().isEmpty())
                m_addWordBtn->setEnabled(TRUE);
}

void AddWordDialog::enWordLETextChanged( const QString & )
{
	//TODO : Ignore me for the moment.
}

void AddWordDialog::findRecordsFileSlot()
{
	QString filters;
        filters.sprintf( "*.zdu *.txt|Update files for ktranslate (*.zdu *.txt)\n"
                "*.*|All files");
        QString Caption = "Select a File";
        QString filename = KFileDialog::getOpenFileName(QString::null, filters,
                                                                this,Caption );
        load (filename);
}

/*!
    \fn AddWordDialog::load ( const QString & filename )
 */
void AddWordDialog::load ( const QString & filename )
{
        if (!filename.isEmpty() )
        {
                kdDebug() <<"Form AddWordDialog::findRecordsFileSlot Message :" << filename << " file selected" << endl;
                fileName = filename;
                m_widgetStack->raiseWidget(progressPage);
                QString mes;
                mes = QString("Ready to parsing %1 file .").arg( fileName );
                m_taskLabel->setText (mes);

                m_progressOkButton->setEnabled (TRUE);
                m_progressEditButton->setEnabled ( FALSE );
                m_progressTryAgainButton->setEnabled ( FALSE );

                m_errorTextBrowser->clear();
                m_parsingProgress->reset();
        }
        else
                kdDebug() << "No file selected" << endl;
}


void AddWordDialog::checkBoxStateChanged( int )
{
	if ( m_addFromFileCheckBox->isChecked() )
        {
                m_enWordLineEdit->setEnabled(FALSE);
                m_trWordLineEdit->setEnabled(FALSE);
                m_addWordBtn->setEnabled(FALSE);
                m_clearAllBtn->setEnabled(FALSE);
                m_findRecordsFileBtn->setEnabled(TRUE);

        }

        else
        {
                m_enWordLineEdit->setEnabled(TRUE);
                m_trWordLineEdit->setEnabled(TRUE);
                m_addWordBtn->setEnabled(TRUE);
                m_clearAllBtn->setEnabled(TRUE);
                m_findRecordsFileBtn->setEnabled(FALSE);
        }
}

void AddWordDialog::clearAllButtonSlot()
{
	m_enWordLineEdit->clear();
        m_trWordLineEdit->clear();
        m_enWordLineEdit->setFocus();
}

void AddWordDialog::addWordToLVSlot()
{
	//First we add the text from edit lines
        KListViewItem *newItem = new KListViewItem (m_addDlgListView);
        newItem->setText(0,m_enWordLineEdit->text());
        newItem->setText(1, m_trWordLineEdit->text());
        
        //m_applyBtn->setEnabled(TRUE);
        //Then clear the Edit Lines and set focus to m_enWordLineEdit
        clearAllButtonSlot();
        m_enWordLineEdit->setFocus();

}

void AddWordDialog::accept()
{
	//KTranslateData trData;
	KTranslateIO *trIO = new KTranslateIO();
	//KTranslateVector trVector;
	//Get the number of the records in the dictionary file ,
	//to keep in touch with the key of the records.
	unsigned int lastkey = trIO->count();
	
	if ( !trVector.isEmpty() )
		trVector.clear();
	
	KListViewItem *item = dynamic_cast<KListViewItem*>(m_addDlgListView->firstChild());
	//Fill the trVector
	kdDebug(80) << "Trying to fill the vector ..." << endl;
	while( item )
	{
		++lastkey;
		trData.setValues(QString::number(lastkey), item->text(0), item->text(1));
		trVector.push_back(trData);
		item = dynamic_cast<KListViewItem*>(item->nextSibling());
	}
	kdDebug(80) << "OK!" << endl;
	//Add the new records to dictionary file.
	kdDebug(80) << "Trying to insert the new records to dictionary file ..." << endl;
	if ( trVector.size() > 0)
	{
		if (trIO->insert(trVector) >= 0)
			KMessageBox::information(this, i18n("You have inserted succesfull %1 records.").arg(trVector.size()),
				"Information");
	}
	else
		kdDebug(80) << "No records inserted!" << endl;
	
	QDialog::accept();
}



#include "addworddialog.moc"

