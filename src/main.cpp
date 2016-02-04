#include "ktranslate.h"

#include <iostream>
#include <qcstring.h>
#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>

using std::cout;

static const char description[] =
    I18N_NOOP("A Greek - English Dictionary for KDE");

static const char version[] = "1.0";

static KCmdLineOptions options[] =
{
	{"ng <word>", I18N_NOOP("Translate word without application ui."), 0 },
    { "+[URL]", I18N_NOOP( "Document to open." ), 0 },
    KCmdLineLastOption
};

int main(int argc, char **argv)
{
	bool wantGUI = true; 
	
    KAboutData about("ktranslate", I18N_NOOP("ktranslate"), version, description,
                     KAboutData::License_LGPL, "(C) 2004 Nikos Zarkadas", 0, 0, "nzarko@trk.forthnet.gr");
    about.addAuthor( "Nikos Zarkadas", 0, "nzarko@trk.forthnet.gr" );
    KCmdLineArgs::init(argc, argv, &about);
    KCmdLineArgs::addCmdLineOptions( options );
    KApplication app;

    // see if we are starting with session management
    if (app.isRestored())
    {
        RESTORE(ktranslate);
    }
    else
    {
        // no session.. just start up normally
        KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

		QCString toTranslateArg = args->getOption("ng");
		if ( !toTranslateArg.isNull() )
		{
			cout << "String to translate : " << toTranslateArg << '\n';
			wantGUI = false;
		}
		
        if ( args->count() == 0 )
        {
        ktranslate *widget = new ktranslate;
        widget->show();
        }
        else
        {
            int i = 0;
            for (; i < args->count(); i++ )
            {
                ktranslate *widget = new ktranslate;
                widget->show();
                widget->load( args->url( i ) );
            }
        }
        args->clear();
		
    }

    return app.exec();
}
