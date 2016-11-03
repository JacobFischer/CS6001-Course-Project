bin_PROGRAMS = multinv

noinst_LTLIBRARIES = libmultinv.la

multinv_SOURCES = \
	main.cc

multinv_CXXFLAGS = $(WARN_CXXFLAGS)

multinv_LDADD = libmultinv.la

multinv_LDFLAGS = $(WARN_LDFLAGS)

libmultinv_la_SOURCES = \
	polynomial.cc	\
	polynomial.h

libmultinv_la_CXXFLAGS = $(WARN_CXXFLAGS)

libmultinv_la_LDFLAGS = $(WARN_LDFLAGS) -avoid-version -no-undefined

-include $(top_srcdir)/git.mk
