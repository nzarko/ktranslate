# set the include path for X, qt and KDE
INCLUDES = -I$(top_srcdir)/lib -I$(top_srcdir)/src $(all_includes)

# these are the headers for your project
inst_HEADERS = ktranslate.h ktranslate_part.h centralview.h addworddialog.h \
	editworddialog.h exportdialog.h
instdir = $(prefix)/include/ktranslate

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/ktranslate.pot

KDE_ICON = ktranslate

# this Makefile creates both a KPart application and a KPart
#########################################################################
# APPLICATION SECTION
#########################################################################
# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = ktranslate

# the application source, library search path, and link libraries
ktranslate_SOURCES = main.cpp ktranslate.cpp
ktranslate_LDFLAGS = $(KDE_RPATH) $(all_libraries)
ktranslate_LDADD = $(top_builddir)/lib/libktranslateio.la $(LIB_KPARTS)

# this is where the desktop file will go 
shelldesktopdir = $(kde_appsdir)/Utilities
shelldesktop_DATA = ktranslate.desktop

# this is where the shell's XML-GUI resource file goes
shellrcdir = $(kde_datadir)/ktranslate
shellrc_DATA = ktranslate_shell.rc

#########################################################################
# KPART SECTION
#########################################################################
kde_module_LTLIBRARIES = libktranslatepart.la

# the Part's source, library search path, and link libraries
libktranslatepart_la_SOURCES = addworddialogbase.ui editworddialogbase.ui \
	centralviewbase.ui 		ExportDialogBase.ui ktranslate_part.cpp centralview.cpp \
	addworddialog.cpp 		editworddialog.cpp exportdialog.cpp ktranslate_pref.cpp \
	ktranslate_pref.h
libktranslatepart_la_LDFLAGS = -module $(KDE_PLUGIN) $(all_libraries)
libktranslatepart_la_LIBADD = $(top_builddir)/lib/libktranslateio.la \
	$(LIB_KPARTS) $(LIB_KFILE)

# this is where the desktop file will go 
partdesktopdir = $(kde_servicesdir)
partdesktop_DATA = ktranslate_part.desktop

# this is where the part's XML-GUI resource file goes
partrcdir = $(kde_datadir)/ktranslatepart
partrc_DATA = ktranslate_part.rc

#This is where the application related files will go
partdir = $(kde_datadir)/ktranslatepart
part_DATA = dictionary_elGR.zdf dictionary_utf8.zdf
noinst_HEADERS = images.h
