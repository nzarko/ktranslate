INCLUDES = -I$(top_srcdir)/lib $(all_includes)
METASOURCES = AUTO
lib_LTLIBRARIES = libktranslateio.la
libktranslateio_la_LDFLAGS = $(all_libraries)
inst_HEADERS = ktranslatedata.h ktranenelement.h separators.h ktranslateio.h
instdir = $(prefix)/include/ktranslate

libktranslateio_la_SOURCES = ktranenelement.cpp ktranslatedata.cpp \
	ktranslateio.cpp
libktranslateio_la_LIBADD = $(LIB_KFILE) $(LIB_QT)

