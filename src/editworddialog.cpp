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


#include <klineedit.h>
#include <kdebug.h>

#include "editworddialog.h"

EditWordDialog::EditWordDialog(KListViewItem *item, QWidget* parent, const char* name, bool modal, WFlags fl)
: EditWordDialogBase(parent,name, modal,fl)
{
	m_newEnWordLEdit->setText(item->text(0));
	m_newTrWordLEdit->setText(item->text(1));
	
	QString k = trIO.findRec(m_newEnWordLEdit->text(), m_newTrWordLEdit->text());
	
	old_data = new KTranslateData(k, item->text(0), item->text(1) );
	kdDebug() << "EditWordDialog::old_data --> " << old_data->key() << ":" 
			<<old_data->enWord() << "=" << old_data->trWord() << endl;
}

EditWordDialog::~EditWordDialog()
{
	delete old_data;
	delete new_data;
}

/*$SPECIALIZATION$*/
void EditWordDialog::accept()
{
	new_data = new KTranslateData("0", m_newEnWordLEdit->text(), m_newTrWordLEdit->text());
	if (*old_data != *new_data)
	{
		kdDebug() << "EditWordDialog::new_data --> " << new_data->key() << ":" 
				<<new_data->enWord() << "=" << new_data->trWord() << endl;
		//now we can proceed to edit.
		trIO.editRec(*old_data, *new_data);
		emit translationChanged();
	}
	else
		kdDebug() <<"EditWordDialog::accept -->  No change happed" << endl;
	QDialog::accept();
}



#include "editworddialog.moc"

