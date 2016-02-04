
#include "ktranslate.h"
#include "ktranslateio.h"

#include <qregexp.h>

#include <kkeydialog.h>
#include <kfiledialog.h>
#include <kconfig.h>
#include <kurl.h>

#include <kedittoolbar.h>

#include <kaction.h>
#include <kstdaction.h>
#include <kstddirs.h>

#include <klibloader.h>
#include <kmessagebox.h>
#include <kstatusbar.h>
#include <klocale.h>

#if KDE_VERSION >= 290 // KDE 3
#include <kparts/componentfactory.h>
#endif

#include <ktrader.h>
#include <kservice.h>
#include <kate/document.h>
#include <kstddirs.h>

QString get_file_name()
{
	KLocale *cur_locale = KGlobal::locale();
	QString loc_name = QString(cur_locale->encoding());
	QRegExp rxp = QRegExp("utf*8", FALSE, TRUE);	
	int isUTF8 = loc_name.contains(rxp);
	QString dFile = isUTF8 >= 1 ? QString("dictionary_utf8.zdf") : QString("dictionary_elGR.zdf");
	KStandardDirs *dirs = KGlobal::dirs();
	dFile = dirs->findResource("data", dFile);
	//QString dFile = locate( "appdata", "dictionary.zdf" );
	return dFile;
}

ktranslate::ktranslate()
    : KParts::MainWindow( 0L, "ktranslate" )
{
	partChanged = false;
    // set the shell's ui resource file
    setXMLFile("ktranslate_shell.rc");

    // then, setup our actions
    setupActions();

    // and a status bar
    statusBar()->show();
    

    //m_manager manipulate the active part.
    m_manager = new KParts::PartManager( this );
    // When the manager says the active part changes,
   // the builder updates (recreates) the GUI
   connect( m_manager, SIGNAL( activePartChanged( KParts::Part * ) ),
            this, SLOT( createGUI( KParts::Part * ) ) );
   
    // this routine will find and load our Part.  it finds the Part by
    // name which is a bad idea usually.. but it's alright in this
    // case since our Part is made for this Shell
    KLibFactory *factory = KLibLoader::self()->factory("libktranslatepart");
    if (factory)
    {
        // now that the Part is loaded, we cast it to a Part to get
        // our hands on it
        m_part = static_cast<KParts::ReadWritePart *>(factory->create(this,
                                "ktranslate_part", "KParts::ReadWritePart" ));

        if (m_part)
        {
            // tell the KParts::MainWindow that this is indeed the main widget
            //setCentralWidget(m_part->widget());
		//Add the Central View part as a main widget
	    	m_manager->addPart(m_part, true);
		m_manager->activeWidget()->setFocusPolicy(QWidget::ClickFocus);
    		setCentralWidget(m_manager->activeWidget());

            // and integrate the part's GUI with the shell's
            //createGUI(m_part);
        }
    }
    else
    {
        // if we couldn't find our Part, we exit since the Shell by
        // itself can't do anything useful
        KMessageBox::error(this, i18n("Could not find our part."));
        kapp->quit();
        // we return here, cause kapp->quit() only means "exit the
        // next time we enter the event loop...
        return;
    }
    
    // apply the saved mainwindow settings, if any, and ask the mainwindow
    // to automatically save settings if changed: window size, toolbar
    // position, icon size, etc.
    setAutoSaveSettings();
    
    //Now connect some usefull slots to our app.
    /*connect (m_part, SIGNAL(addRecRequest()), this, SLOT(addRec()) );
    connect(m_part, SIGNAL(editRecRequest(KListViewItem *)), this, SLOT(editRec(KListViewItem* )) );*/
}

ktranslate::~ktranslate()
{
}

void ktranslate::load(const KURL& url)
{
    m_part->openURL( url );
}

void ktranslate::setupActions()
{
    KStdAction::openNew(this, SLOT(fileNew()), actionCollection());
    KStdAction::open(this, SLOT(fileOpen()), actionCollection());

    KStdAction::quit(kapp, SLOT(quit()), actionCollection());

    m_toolbarAction = KStdAction::showToolbar(this, SLOT(optionsShowToolbar()), actionCollection());
    m_statusbarAction = KStdAction::showStatusbar(this, SLOT(optionsShowStatusbar()), actionCollection());

    KStdAction::keyBindings(this, SLOT(optionsConfigureKeys()), actionCollection());
    KStdAction::configureToolbars(this, SLOT(optionsConfigureToolbars()), actionCollection());
    
    m_changePartAction = new KAction ( i18n("New Translations File"), "pencil",
		    CTRL+Key_E,
		    this, SLOT(changePart()),
		    actionCollection(), "change_part");
    m_changePartAction->setToolTip(i18n("write a new translation file with a text editor."));
    
}

void ktranslate::saveProperties(KConfig* /*config*/)
{
    // the 'config' object points to the session managed
    // config file.  anything you write here will be available
    // later when this app is restored
}

void ktranslate::readProperties(KConfig* /*config*/)
{
    // the 'config' object points to the session managed
    // config file.  this function is automatically called whenever
    // the app is being restored.  read in here whatever you wrote
    // in 'saveProperties'
}

void ktranslate::fileNew()
{
    // this slot is called whenever the File->New menu is selected,
    // the New shortcut is pressed (usually CTRL+N) or the New toolbar
    // button is clicked

    // About this function, the style guide (
    // http://developer.kde.org/documentation/standards/kde/style/basics/index.html )
    // says that it should open a new window if the document is _not_
    // in its initial state.  This is what we do here..
    if ( ! m_part->url().isEmpty() || m_part->isModified() )
    {
        (new ktranslate)->show();
    };
}


void ktranslate::optionsShowToolbar()
{
    // this is all very cut and paste code for showing/hiding the
    // toolbar
    if (m_toolbarAction->isChecked())
        toolBar()->show();
    else
        toolBar()->hide();
}

void ktranslate::optionsShowStatusbar()
{
    // this is all very cut and paste code for showing/hiding the
    // statusbar
    if (m_statusbarAction->isChecked())
        statusBar()->show();
    else
        statusBar()->hide();
}

void ktranslate::optionsConfigureKeys()
{
    KKeyDialog::configureKeys(actionCollection(), "ktranslate_shell.rc");
}

void ktranslate::optionsConfigureToolbars()
{
#if defined(KDE_MAKE_VERSION)
# if KDE_VERSION >= KDE_MAKE_VERSION(3,1,0)
    saveMainWindowSettings(KGlobal::config(), autoSaveGroup());
# else
    saveMainWindowSettings(KGlobal::config() );
# endif
#else
    saveMainWindowSettings(KGlobal::config() );
#endif

    // use the standard toolbar editor
    KEditToolbar dlg(factory());
    connect(&dlg, SIGNAL(newToolbarConfig()),
            this, SLOT(applyNewToolbarConfig()));
    dlg.exec();
}

void ktranslate::applyNewToolbarConfig()
{
#if defined(KDE_MAKE_VERSION)
# if KDE_VERSION >= KDE_MAKE_VERSION(3,1,0)
    applyMainWindowSettings(KGlobal::config(), autoSaveGroup());
# else
    applyMainWindowSettings(KGlobal::config());
# endif
#else
    applyMainWindowSettings(KGlobal::config());
#endif
}

void ktranslate::fileOpen()
{
    // this slot is called whenever the File->Open menu is selected,
    // the Open shortcut is pressed (usually CTRL+O) or the Open toolbar
    // button is clicked
    KURL url =
        KFileDialog::getOpenURL( QString::null, QString::null, this );

    if (url.isEmpty() == false)
    {
        // About this function, the style guide (
        // http://developer.kde.org/documentation/standards/kde/style/basics/index.html )
        // says that it should open a new window if the document is _not_
        // in its initial state.  This is what we do here..
        if ( m_part->url().isEmpty() && ! m_part->isModified() )
        {
            // we open the file in this window...
            load( url );
        }
        else
        {
            // we open the file in a new window...
            ktranslate* newWin = new ktranslate;
            newWin->load( url );
            newWin->show();
        }
    }
}

/*!
    \fn ktranslate::changePart()
 */
void ktranslate::changePart()
{	
	KTranslateIO trIO;
	if ( !partChanged ) 
	{
		loadEditorPart();
		//Allow us to edit the dictionary file.
		//m_editorPart->openURL ( KURL(trIO.dictFileName()) );
		m_editorPart->setReadWrite(true);
		
		//Change the icon of m_changePartAction.
		m_changePartAction->setIcon("fileclose");
		m_changePartAction->setText("Close Editor");
		m_changePartAction->setToolTip(i18n("close editor and return to dictionary interface"));
	}
	else
	{
		//Prompt before save file.
		m_editorPart->closeURL(true);
		if ( m_editorPart->waitSaveComplete() )
		{
			trIO.sort();
			KMessageBox::information(this, QString("Save Button Pressed"), QString("Information"));
		}
		loadPart();
		m_changePartAction->setIcon("pencil");
		m_changePartAction->setText("New Translations File");
		m_changePartAction->setToolTip(i18n("write a new translation file with a text editor."));
		 connect (m_part, SIGNAL(addRecRequest()), this, SLOT(addRec()) );
	}
	partChanged ^= 1;
}

bool ktranslate::loadPart()
{
	KLibFactory *factory = KLibLoader::self()->factory("libktranslatepart");
	if (factory)
	{
		// now that the Part is loaded, we cast it to a Part to get
		// our hands on it
		m_part = static_cast<KParts::ReadWritePart *>(factory->create(this,
					"ktranslate_part", "KParts::ReadWritePart" ));
		 if (m_part)
		{
		// tell the KParts::MainWindow that this is indeed the main widget
		//setCentralWidget(m_part->widget());
			//Add the Central View part as a main widget
			m_manager->replacePart(m_editorPart, m_part, true);
			m_manager->activeWidget()->setFocusPolicy(QWidget::ClickFocus);
			setCentralWidget(m_manager->activeWidget());
			if( m_editorPart )
				delete m_editorPart;
			
			return true;
		}
	}
	else
	{
		// if we couldn't find our Part, we exit since the Shell by
		// itself can't do anything useful
		KMessageBox::error(this, i18n("Could not find our part."));
		kapp->quit();
		// we return here, cause kapp->quit() only means "exit the
		// next time we enter the event loop...
		return false;
	}
}

bool ktranslate::loadEditorPart()
{
	/*KTrader::OfferList offers  = KTrader::self()->query("text/plain");
	ASSERT(offers.count() >= 1);
	KService::Ptr service = *offers.begin();
	KLibFactory *factory = KLibLoader::self()->factory(service->library());
	//ASSERT(factory);
	
	if (factory)
	{
		// now that the Part is loaded, we cast it to a Part to get
		// our hands on it
		//m_editorPart = static_cast<KParts::ReadWritePart *>(factory->create(this,
		//			"kate_part", "KParts::ReadWritePart" ));
		m_editorPart = static_cast<Kate::Document*>(factory->create(this,
					0, "Kate::Document" ));
		 if (m_editorPart)
		{
		// tell the KParts::MainWindow that this is indeed the main widget
		//setCentralWidget(m_part->widget());
			//Add the Central View part as a main widget
			m_manager->replacePart(m_part, m_editorPart, true);
			//m_manager->activeWidget()->setFocusPolicy(QWidget::ClickFocus);
			setCentralWidget(m_manager->activeWidget());
	
			return true;
		}
	}
	else
	{
		// if we couldn't find our Part, we exit since the Shell by
		// itself can't do anything useful
		KMessageBox::error(this, i18n("Could not find our part."));
		kapp->quit();
		// we return here, cause kapp->quit() only means "exit the
		// next time we enter the event loop...
		return false;
	}
	*/
	KTranslateIO trIO;
	m_editorPart = 0;
	QString mimetype = KMimeType::findByURL(KURL(trIO.dictFileName()))->name();
	QString mimetype2;
	if (mimetype.startsWith("text/"))
		mimetype2 = "text/plain";
	
#if KDE_VERSION >= 290 // KDE 3
	m_editorPart = KParts::ComponentFactory::createPartInstanceFromQuery<KParts::ReadWritePart>
			( mimetype2, QString::null, this, 0L, this, 0L );
#else

	KTrader::OfferList offers = KTrader::self()->query( mimetype2,
		 "('KParts/ReadOnlyPart' in ServiceTypes) or ('Browser/View' in ServiceTypes)");
	KLibFactory *factory = 0;
	KTrader::OfferList::Iterator it(offers.begin());
	for( ; it != offers.end(); ++it)
	{
		KService::Ptr service = (*it);
		factory = KLibLoader::self()->factory( QFile::encodeName(service->library()) );
		if (factory)
		{
			m_editorPart=static_cast<KParts::ReadWritePart*>
				(factory->create(this, service->name().latin1(), "KParts::ReadWritePart"));
			break;
		}
	}
#endif

	if ( !m_editorPart )
	{
		KMessageBox::error( this, i18n("No handler found for %1!").arg(mimetype2) );
		return false;
	}
	else
	{
		m_manager->replacePart(m_part, m_editorPart, true);
		setCentralWidget(m_manager->activeWidget());
		if(m_part)
			delete m_part;
		return true;
	}
}

/*!
    \fn ktranslate::addRec()
 */
/*void ktranslate::addRec()
{
	m_addWDlg = new AddWordDialog();
	m_addWDlg->exec();
	
	delete m_addWDlg;
}
*/
/*!
    \fn ktranslate::editRec(KListViewItem *item)
 */
			/*
void ktranslate::editRec(KListViewItem *item)
{
	m_editWDlg = new EditWordDialog(item);
	m_editWDlg->exec();
	
	delete m_editWDlg;
}
			*/
#include "ktranslate.moc"
