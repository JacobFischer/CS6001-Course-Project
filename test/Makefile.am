check_PROGRAMS = test-dumbaes

TESTS = test-dumbaes

test_dumbaes_SOURCES = test.cc

test_dumbaes_CPPFLAGS = \
	-I$(top_srcdir)/src	\
	$(GLIB_CFLAGS)

test_dumbaes_CXXFLAGS = $(WARN_CXXFLAGS)

test_dumbaes_LDADD = \
	$(top_builddir)/src/libdumbaes.la	\
	$(GLIB_LIBS)

test_dumbaes_LDFLAGS = $(WARN_LDFLAGS)

-include $(top_srcdir)/git.mk
