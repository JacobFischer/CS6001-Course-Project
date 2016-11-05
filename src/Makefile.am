noinst_LTLIBRARIES = libdumbaes.la

libdumbaes_la_SOURCES = \
	dumbaes.cc			\
	dumbaes.h			\
	polynomial.cc			\
	polynomial.h			\
	polynomial-polynomial.cc	\
	polynomial-polynomial.h		\
	word.cc				\
	word.h

libdumbaes_la_CXXFLAGS = $(WARN_CXXFLAGS)

libdumbaes_la_LDFLAGS = $(WARN_LDFLAGS) -avoid-version -no-undefined

-include $(top_srcdir)/git.mk
