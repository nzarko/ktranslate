
#ifndef _KTRANSLATE_H_
#define _KTRANSLATE_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <kxmlguiclient.h>
#include <kapplication.h>
#include <kparts/mainwindow.h>
#include <kparts/partmanager.h>

class KToggleAction;
/*class AddWordDialog;
class EditWordDialog;*/
class KListViewItem;

QString get_file_name();

/**
 * This is the application "Shell".  It has a menubar, toolbar, and
 * statusbar but relies on the "Part" to do all the real work.
 *
 * @short Application Shell
 * @author Nikos Zarkadas <nzarko@acn.gr>
 * @version 0.1
 */
class ktranslate : public KParts::MainWindow
{
    Q_OBJECT
public:
	/**
	* Default Constructor
	*/
	ktranslate();
	
	/**
	* Default Destructor
	*/
	virtual ~ktranslate();
	
	/**
	* Use this method to load whatever file/URL you have
	*/
	void load(const KURL& url);
	
	protected:
	/**
	* This method is called when it is time for the app to save its
	* properties for session management purposes.
	*/
	void saveProperties(KConfig *);
	
	/**
	* This method is called when this app is restored.  The KConfig
	* object points to the session management config file that was saved
	* with @ref saveProperties
	*/
	void readProperties(KConfig *);
	
	/**
	 * This method replace the editor part with translate part.
	*/
	bool loadPart();
	
	/**
	 * This method replace the translate part with editor part.
	*/
	bool loadEditorPart();
	
private slots:
	void fileNew();
	void fileOpen();
	void optionsShowToolbar();
	void optionsShowStatusbar();
	void optionsConfigureKeys();
	void optionsConfigureToolbars();
	
	void applyNewToolbarConfig();
	void changePart();
    	/*void addRec();
	void editRec(KListViewItem *item);*/

private:
	void setupAccel();
	void setupActions();

private:
	bool partChanged;
    	KParts::ReadWritePart *m_part;
    	KParts::ReadWritePart *m_editorPart;
	KParts::PartManager *m_manager;
	
	KAction *m_changePartAction;
	KToggleAction *m_toolbarAction;
	KToggleAction *m_statusbarAction;
	/*AddWordDialog *m_addWDlg;
	EditWordDialog *m_editWDlg;*/
};

#endif // _KTRANSLATE_H_
