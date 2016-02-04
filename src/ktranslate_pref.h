#ifndef KTRANSLATE_PREF_H
#define  KTRANSLATE_PREF_H

#include <kdialogbase.h>
#include <qframe.h>
#include <qimage.h>

class QGroupBox;
class QCheckBox;
class QVBoxLayout;
class QHBoxLayout;
class KPushButton;
class QSpacerItem;
class KTranslatePrefGeneral;
class KTranslatePrefStyle;

class KTranslatePreferences : public KDialogBase
{
    Q_OBJECT
public:
    KTranslatePreferences();

private:
    KTranslatePrefGeneral *m_pageGeneral;
    KTranslatePrefStyle *m_pageStyle;
};

class KTranslatePrefGeneral : public QFrame
{
    Q_OBJECT
public:
	QCheckBox*  m_stayInTray;
	QCheckBox *m_alwaysVisible;
	KTranslatePrefGeneral(QWidget *parent = 0);
private:
	KPushButton *m_delCacheFileBtn;
	QImage image_delPrefGen;
	
	QGroupBox* groupBox1;
	QVBoxLayout* groupBox1Layout;
	QSpacerItem* vspacer;
	QSpacerItem *hspacer;
public slots:
	void delLocalSlot();
};

class KTranslatePrefStyle : public QFrame
{
    Q_OBJECT
public:
    KTranslatePrefStyle(QWidget *parent = 0);
};

#endif // KTRANSLATE_PREF_H
