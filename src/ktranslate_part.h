
#ifndef _KTRANSLATEPART_H_
#define _KTRANSLATEPART_H_

#include <kparts/part.h>
#include <kparts/factory.h>

class QWidget;
class QPainter;
class QClipboard;
class KURL;
class CentralView;
class KAction;
class KListViewItem;
class KPopupMenu;
class KListView;
class QListViewItem;
class AddWordDialog;
class EditWordDialog;

/**
 * This is a "Part".  It that does all the real work in a KPart
 * application.
 *
 * @short Main Part
 * @author Nikos Zarkadas <nzarko@acn.gr>
 * @version 0.1
 */
class ktranslatePart : public KParts::ReadWritePart
{
    Q_OBJECT
public:
    /**
     * Default constructor
     */
    ktranslatePart(QWidget *parentWidget, const char *widgetName,
                    QObject *parent, const char *name);

    /**
     * Destructor
     */
    virtual ~ktranslatePart();

    /**
     * This is a virtual function inherited from KParts::ReadWritePart.  
     * A shell will use this to inform this Part if it should act
     * read-only
     */
    virtual void setReadWrite(bool rw);

    /**
     * Reimplemented to disable and enable Save action
     */
    virtual void setModified(bool modified);

    signals:
    	/**
	 * This signal is emitted whenever the user try to add a new record.
	 */
    	void addRecRequest();
    	/**
     	 * This signal is emitted whenever the user try to edit the selected record
    	 * in list view.
    	*/
    	void editRecRequest(KListViewItem *selected);
	/**
	 * This signal is emitted whenever the user try to delete the selected record.
	*/
	void deleteRecRequest(KListViewItem *selected);
	/**
	 * This signal is emitted whenever we add , edit , delete a translation.
	 */
	void translationsChanged();
protected:
    /**
     * This must be implemented by each part
     */
    virtual bool openFile();

    /**
     * This must be implemented by each read-write part
     */
    virtual bool saveFile();

protected slots:
    void fileOpen();
    void fileSaveAs();
    void addWord();
    void deleteWord();
    void editWord();
    void stdCopy();
    void translationCount();
    void listContextMenuSlot(KListView *, QListViewItem*, const QPoint&);
    void viewAll();
    void onTranslationChanged();

private:
    CentralView *m_cView;
    AddWordDialog *m_addWDlg;
    EditWordDialog *m_editWDlg;
    KAction *m_addWAction;
    KAction *m_editWAction;
    KAction *m_delWAction;
    KAction *m_editCopyAction;
    KPopupMenu *pMenu;
    KAction *m_viewAllAction;
    KAction *m_configAction;
    
    void setupActions();
    void createLPMenu();
public slots:
    void configSlot();
};

class KInstance;
class KAboutData;

class ktranslatePartFactory : public KParts::Factory
{
    Q_OBJECT
public:
    ktranslatePartFactory();
    virtual ~ktranslatePartFactory();
    virtual KParts::Part* createPartObject( QWidget *parentWidget, const char *widgetName,
                                            QObject *parent, const char *name,
                                            const char *classname, const QStringList &args );
    static KInstance* instance();
 
private:
    static KInstance* s_instance;
    static KAboutData* s_about;
};

#endif // _KTRANSLATEPART_H_
