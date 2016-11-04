check_PROGRAMS = \
	test-key-schedule
	test-polynomials

TESTS = $(check_PROGRAMS)

LDADD = \
	$(top_builddir)/src/libdumbaes.la	\
	$(GLIB_LIBS)

AM_CPPFLAGS = \
	-I$(top_srcdir)/src	\
	$(GLIB_CFLAGS)

AM_CXXFLAGS = $(WARN_CXXFLAGS)

AM_LDFLAGS = $(WARN_LDFLAGS)

test_polynomials_SOURCES = test-polynomials.cc

test_key_schedule_SOURCES = test-key-schedule.cc

-include $(top_srcdir)/git.mk
