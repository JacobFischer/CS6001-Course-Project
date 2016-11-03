bin_PROGRAMS = dumbaes

noinst_LTLIBRARIES = libdumbaes.la

dumbaes_SOURCES = \
	main.cc

dumbaes_CXXFLAGS = $(WARN_CXXFLAGS)

dumbaes_LDADD = libdumbaes.la

dumbaes_LDFLAGS = $(WARN_LDFLAGS)

libdumbaes_la_SOURCES = \
	dumbaes.cc	\
	dumbaes.h	\
	polynomial.cc	\
	polynomial.h

libdumbaes_la_CXXFLAGS = $(WARN_CXXFLAGS)

libdumbaes_la_LDFLAGS = $(WARN_LDFLAGS) -avoid-version -no-undefined

-include $(top_srcdir)/git.mk
