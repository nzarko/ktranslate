#include "ktranslate_pref.h"
#include "images.h"

#include <qlayout.h>
#include <qlabel.h>
#include <qgroupbox.h>
#include <qcheckbox.h>

#include <kglobal.h>
#include <kdebug.h>
#include <kiconloader.h>    // for KIconLoader
#include <kpushbutton.h>
#include <klocale.h>
#include <kmessagebox.h>

KTranslatePreferences::KTranslatePreferences()
    : KDialogBase(IconList, "KTranslate Preferences",
                  Help|Default|Ok|Apply|Cancel, Ok)
{
    // this is the base class for your preferences dialog.  it is now
    // a Treelist dialog.. but there are a number of other
    // possibilities (including Tab, Swallow, and just Plain)
     	QFrame *frame;
	frame = addPage(i18n("General"), i18n("General options"),
				KGlobal::iconLoader()->loadIcon("kfm",KIcon::Panel,0,false) );
	QVBoxLayout *frameLayout = new QVBoxLayout( frame, 0, 0 );
	m_pageGeneral = new KTranslatePrefGeneral(frame);
	frameLayout->addWidget(m_pageGeneral);

    // adding page "Style settings"
	frame = addPage(i18n("Style"), i18n("Style settings"),
			KGlobal::iconLoader()->loadIcon("style",KIcon::Panel,0,false) );
	frameLayout = new QVBoxLayout( frame, 0, 0 );
	m_pageStyle = new KTranslatePrefStyle(frame);
	frameLayout->addWidget(m_pageStyle);
}

KTranslatePrefGeneral::KTranslatePrefGeneral(QWidget *parent)
    : QFrame(parent)
{
	QImage img;
	img.loadFromData( deletecacheprefgeneral_img, sizeof( deletecacheprefgeneral_img ), "PNG" );
	image_delPrefGen = img;

	QVBoxLayout *pageLayout = new QVBoxLayout(this);
	QVBoxLayout *vlayout = new QVBoxLayout(0,0,6,"vlayout");
	QHBoxLayout *hlayout = new QHBoxLayout(0,0,6, "hlayout");
// 	layout->setAutoAdd(true);

	groupBox1 = new QGroupBox( this, "groupBox1" );
	groupBox1->setColumnLayout(0, Qt::Vertical );
	groupBox1->setInsideMargin(5);
	groupBox1->layout()->setSpacing( 6 );
	groupBox1->layout()->setMargin( 11 );
	groupBox1Layout = new QVBoxLayout( groupBox1->layout() );
	groupBox1Layout->setAlignment( Qt::AlignTop );
	vlayout->addWidget(groupBox1);

	m_stayInTray = new QCheckBox(i18n("Stay in Tray"), groupBox1, "check_box");
	groupBox1Layout->addWidget(m_stayInTray);

	m_alwaysVisible = new QCheckBox(i18n("Always Visible"), groupBox1, "always_visible");
	groupBox1Layout->addWidget(m_alwaysVisible);
	
	QLabel *label1 = new QLabel("Delete local dictionary file ",this);
	hlayout->addWidget(label1);
	
	hspacer = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	hlayout->addItem(hspacer);
	
	m_delCacheFileBtn = new KPushButton(this, "m_delCacheFileBtn");
	m_delCacheFileBtn->setText(i18n("Delete"));
	m_delCacheFileBtn->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, m_delCacheFileBtn->sizePolicy().hasHeightForWidth() ) );
	m_delCacheFileBtn->setIconSet(QIconSet(image_delPrefGen));
	hlayout ->addWidget(m_delCacheFileBtn);

	pageLayout->addLayout(vlayout);
	vspacer = new QSpacerItem(40,20, QSizePolicy::Minimum, QSizePolicy::Expanding );
	pageLayout->addItem(vspacer);
	pageLayout->addLayout(hlayout);
	
	connect(m_delCacheFileBtn, SIGNAL(clicked() ), this, SLOT(delLocalSlot()));
}

void KTranslatePrefGeneral::delLocalSlot()
{
	QString message = "<b>WARNING!!</b> This will delete the local dictionary file.";
	message += "\nThis means that you 'll not be able to translate another word in current session";
	message +="\nYou should do this action only if you have change the locale enconding";
	message += "\nTo create a new local file just restart the application.";
	if ( KMessageBox::warningContinueCancel(this, message, i18n("Question!")) == KMessageBox::Continue)
	{
		kdDebug() << "Delete button pressed" << endl;
	}
}

KTranslatePrefStyle::KTranslatePrefStyle(QWidget *parent)
    : QFrame(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setAutoAdd(true);

    new QLabel("Add something here", this);
}
#include "ktranslate_pref.moc"
