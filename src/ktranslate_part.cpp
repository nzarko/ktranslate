
#include "ktranslate_part.h"
#include "addworddialog.h"
#include "editworddialog.h"
#include "ktranslateio.h"
#include "ktranslate_pref.h"

#include <kinstance.h>
#include <kaction.h>
#include <kstdaction.h>
#include <kfiledialog.h>
#include <kmessagebox.h>
#include <klistview.h>
#include <kpushbutton.h>
#include <kpopupmenu.h>
#include <kdebug.h>

#include <klocale.h>

#include <qfile.h>
#include <qtextstream.h>
#include <qclipboard.h>
#include <qapplication.h>
#include "centralview.h"

ktranslatePart::ktranslatePart( QWidget *parentWidget, const char *widgetName,
                                  QObject *parent, const char *name )
    : KParts::ReadWritePart(parent, name)
{
    // we need an instance
    setInstance( ktranslatePartFactory::instance() );
    

    // this should be your custom internal widget
    m_cView = new CentralView( parentWidget, widgetName );

    // notify the part that this is our internal widget
    setWidget(m_cView);

    // create our actions
    setupActions();
    
    //Create our popup menu.
    createLPMenu();
    

    // set our XML-UI resource file
    setXMLFile("ktranslate_part.rc");

    // we are read-write by default
    setReadWrite(true);

    // we are not modified since we haven't done anything yet
    setModified(false);
    //Connect the appropriate signals and slots to have feed back
    connect(m_cView, SIGNAL(translationRequested()), this, 
			SLOT(translationCount()) );
    connect ( m_cView->m_wordListView, SIGNAL(contextMenu (KListView *, QListViewItem *, 
    	const QPoint &)),
    	this, SLOT(listContextMenuSlot(KListView*, QListViewItem*, const QPoint& )) );
    connect(this, SIGNAL(translationsChanged()), m_cView, SLOT(translateBtnClickedSlot()));
}

ktranslatePart::~ktranslatePart()
{
}

/*!
    \fn ktranslatePart::setupActions()
 */
void ktranslatePart::setupActions()
{
	KStdAction::open(this, SLOT(fileOpen()), actionCollection());
    	KStdAction::saveAs(this, SLOT(fileSaveAs()), actionCollection());
    	KStdAction::save(this, SLOT(save()), actionCollection());
    	m_editCopyAction = KStdAction::copy(this, SLOT(stdCopy()), actionCollection());
	m_addWAction = new KAction(i18n("Add Records"), "add_records",
                                    CTRL+Key_A,
                                    this, SLOT(addWord()),
                                    actionCollection(), "add_word");
	m_editWAction = new KAction (i18n("Edit Record"), "edit",
                                      CTRL+Key_E,
                                      this, SLOT(editWord() ),
                                      actionCollection(),"edit_word");
	m_delWAction = new KAction (i18n("Delete Record"), "editdelete",
                                         Key_Delete,
                                         this, SLOT(deleteWord() ),
                                         actionCollection(), "delete_word");
	m_viewAllAction = new KAction (i18n("View All Records"), "view_text",
			KShortcut::null(), this, SLOT(viewAll()),
			actionCollection(), "view_allrecords");
	m_configAction = new KAction(i18n("Configure KTranslate"), "configure",
			KShortcut::null(), this, SLOT(configSlot()),
			actionCollection(), "options_configure");
	//TODO: Add new action to count the total translations
}

/*!
    \fn ktranslatePart::createLPMenu()
 */
void ktranslatePart::createLPMenu()
{
	pMenu = new KPopupMenu(m_cView->m_wordListView, "list_popup_menu");
        m_editCopyAction->plug(pMenu);
        pMenu->insertSeparator();
        m_editWAction->plug(pMenu);
        m_delWAction->plug(pMenu);
        pMenu->insertSeparator();
        m_addWAction->plug(pMenu);
        pMenu->insertSeparator();
        /*showPrefAction->plug(popupMenu);
        optionDataConfAction->plug(popupMenu);*/
}


void ktranslatePart::listContextMenuSlot(KListView *, QListViewItem* item, const QPoint& point)
{	
	if ( item )
        {
                m_editCopyAction->setEnabled(TRUE);
                m_editWAction->setEnabled(TRUE);
                m_delWAction->setEnabled(TRUE);
        }
        else
        {
                m_editCopyAction->setEnabled(FALSE);
                m_editWAction->setEnabled(FALSE);
                m_delWAction->setEnabled(FALSE);
        }

        pMenu->popup(point);
}

void ktranslatePart::setReadWrite(bool rw)
{
    // notify your internal widget of the read-write state
    //m_cView->setReadOnly(!rw);
    /*if (rw)
        connect(m_cView, SIGNAL(textChanged()),
                this,     SLOT(setModified()));
    else
    {
        disconnect(m_cView, SIGNAL(textChanged()),
                   this,     SLOT(setModified()));
    }*/

    ReadWritePart::setReadWrite(rw);
}

void ktranslatePart::setModified(bool modified)
{
    // get a handle on our Save action and make sure it is valid
    KAction *save = actionCollection()->action(KStdAction::stdName(KStdAction::Save));
    if (!save)
        return;

    // if so, we either enable or disable it based on the current
    // state
    if (modified)
        save->setEnabled(true);
    else
        save->setEnabled(false);

    // in any event, we want our parent to do it's thing
    ReadWritePart::setModified(modified);
}

bool ktranslatePart::openFile()
{
    // m_file is always local so we can use QFile on it
    QFile file(m_file);
    if (file.open(IO_ReadOnly) == false)
        return false;

    // our example widget is text-based, so we use QTextStream instead
    // of a raw QDataStream
    QTextStream stream(&file);
    QString str;
    while (!stream.eof())
        str += stream.readLine() + "\n";

    file.close();

    // now that we have the entire file, display it
    //m_cView->setText(str);

    // just for fun, set the status bar
    emit setStatusBarText( m_url.prettyURL() );

    return true;
}

bool ktranslatePart::saveFile()
{
    // if we aren't read-write, return immediately
    if (isReadWrite() == false)
        return false;

    // m_file is always local, so we use QFile
    QFile file(m_file);
    if (file.open(IO_WriteOnly) == false)
        return false;

    // use QTextStream to dump the text to the file
    //QTextStream stream(&file);
    //stream << m_cView->text();

    file.close();

    return true;
}

void ktranslatePart::fileOpen()
{
    // this slot is called whenever the File->Open menu is selected,
    // the Open shortcut is pressed (usually CTRL+O) or the Open toolbar
    // button is clicked
    QString file_name = KFileDialog::getOpenFileName();

    if (file_name.isEmpty() == false)
        openURL(file_name);
}

void ktranslatePart::fileSaveAs()
{
    // this slot is called whenever the File->Save As menu is selected,
    QString file_name = KFileDialog::getSaveFileName();
    if (file_name.isEmpty() == false)
        saveAs(file_name);
}

/*!
    \fn ktranslatePart::addWord()
 */
void ktranslatePart::addWord()
{
    	/*kdDebug(100) << "void ktranslate_part::addWord()" << endl;
	emit addRecRequest();*/
	m_addWDlg = new AddWordDialog();
	if ( m_addWDlg->exec() == QDialog::Accepted )
		emit translationsChanged();
	
	delete m_addWDlg;
}


/*!
    \fn ktranslatePart::deleteWord()
 */
void ktranslatePart::deleteWord()
{
	KTranslateIO trIO;
	kdDebug(101) << "void ktranslate_part::deleteWord()" << endl;
	KListViewItem * selected = (KListViewItem*)m_cView->m_wordListView->selectedItem();
	if (selected )
	{
		QString message = QString(
		"Are you sure you want to delete : \"%1 --> %2 \"record?").arg(selected->text(0)).arg(selected->text(1));
		if ( KMessageBox::warningContinueCancel(this->widget(), message) == KMessageBox::Continue )
		{
			if (trIO.deleteRec(selected->text(0), selected->text(1)))
				kdDebug() << "Ooops! The record is gone!!" << endl;
			//emit deleteRecRequest(selected);
		}
	}
	else
		kdDebug(104) << "void ktranslate_part::deleteWord() ==> No selected item." << endl;
}


/*!
    \fn ktranslatePart::editWord()
 */
void ktranslatePart::editWord()
{
	kdDebug(102) << "void ktranslate_part::editWord()" << endl;
	KListViewItem * selected = (KListViewItem*)m_cView->m_wordListView->selectedItem();
	if (selected)
	{
		m_editWDlg = new EditWordDialog(selected);
		if (m_editWDlg->exec() == QDialog::Accepted )
		{
			m_cView->m_wordListView->clear();
			emit translationsChanged();
		}
	
		//delete m_editWDlg;
		//emit editRecRequest(selected);
	}
	else
		kdDebug(103) << "void ktranslate_part::editWord() ==>No selected Item." << endl;
}


/*!
    \fn ktranslatePart::stdCopy()
 */
void ktranslatePart::stdCopy()
{
	QClipboard *cb = QApplication::clipboard();

        QString selText;
        KListViewItem *selItem =(KListViewItem*) m_cView->m_wordListView->selectedItem();

	if ( selItem != (KListViewItem*)0 ) {
		selText = selItem->text(0) + "-->" + selItem->text(1);
	
		// Copy text into the clipboard
		cb->setText ( selText );
		kdDebug() << "The clipboard contains: " << selText << endl;
	}
	else 
		kdDebug(103) << "No selected item found" << endl;
}

/*!
    \fn ktranslatePart::translationCount()
 */
void ktranslatePart::translationCount()
{
	int count = m_cView->noRecFound();
	//Inform the user how many records we have found
	QString message = QString::number(count, 10) + " translations found.";
	 emit setStatusBarText(message);
}

/*!
    \fn ktranslatePart::viewAll
 */
void ktranslatePart::viewAll()
{
	int how_many = m_cView->showRecords();	
	kdDebug() << "Found " << how_many << " total translations" << endl;
	//Inform the user how many records we have found
	emit setStatusBarText(QString("%1 translations found").arg(how_many));
}


//===============================================//
//					FACTORY CODE						  //
// It's usually safe to leave the factory code alone.. with the			  //	
// notable exception of the KAboutData data						  //
//===============================================//
#include <kaboutdata.h>
#include <klocale.h>

KInstance*  ktranslatePartFactory::s_instance = 0L;
KAboutData* ktranslatePartFactory::s_about = 0L;

ktranslatePartFactory::ktranslatePartFactory()
    : KParts::Factory()
{
}

ktranslatePartFactory::~ktranslatePartFactory()
{
    delete s_instance;
    delete s_about;

    s_instance = 0L;
}

KParts::Part* ktranslatePartFactory::createPartObject( QWidget *parentWidget, const char *widgetName,
                                                        QObject *parent, const char *name,
                                                        const char *classname, const QStringList &args )
{
    // Create an instance of our Part
    ktranslatePart* obj = new ktranslatePart( parentWidget, widgetName, parent, name );

    // See if we are to be read-write or not
    if (QCString(classname) == "KParts::ReadOnlyPart")
        obj->setReadWrite(false);

    return obj;
}

KInstance* ktranslatePartFactory::instance()
{
    if( !s_instance )
    {
        s_about = new KAboutData("ktranslatepart", I18N_NOOP("ktranslatePart"), "1.0");
        s_about->addAuthor("Nikos Zarkadas", 0, "nzarko@acn.gr");
        s_instance = new KInstance(s_about);
    }
    return s_instance;
}

extern "C"
{
    void* init_libktranslatepart()
    {
        return new ktranslatePartFactory;
    }
};

#include "ktranslate_part.moc"


/*!
    \fn ktranslatePart::onTranslationChanged()
 */
void ktranslatePart::onTranslationChanged()
{
    /// @todo implement me
}


/*!
    \fn ktranslatePart::configSlot()
 */
void ktranslatePart::configSlot()
{
	KTranslatePreferences pref_dlg;
	if ( pref_dlg.exec() )
	{
		//redo your settings
	}
}
