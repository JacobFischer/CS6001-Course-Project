lib_LTLIBRARIES = libdumbaes.la

dumbaes_includedir = $(includedir)/dumbaes
dist_dumbaes_include_HEADERS = dumbaes.h

libdumbaes_la_SOURCES = dumbaes.cc

libdumbaes_la_CXXFLAGS = \
	$(WARN_CXXFLAGS)	\
	-I$(top_srcdir)

libdumbaes_la_LIBADD = $(top_builddir)/src/libdumbaesinternal.la

libdumbaes_la_LDFLAGS = $(WARN_LDFLAGS) -avoid-version -no-undefined

-include $(top_srcdir)/git.mk
