check_PROGRAMS = \
	test-encryption		\
	test-key-schedule	\
	test-polynomials

TESTS = $(check_PROGRAMS)

LDADD = \
	$(top_builddir)/api/libdumbaes.la		\
	$(top_builddir)/src/libdumbaesinternal.la	\
	$(GLIB_LIBS)	\
	$(NETTLE_LIBS)

AM_CPPFLAGS = \
	-I$(top_srcdir)	\
	$(GLIB_CFLAGS)	\
	$(NETTLE_CFLAGS)

AM_CXXFLAGS = $(WARN_CXXFLAGS)

AM_LDFLAGS = $(WARN_LDFLAGS)

test_encryption_SOURCES = test-encryption.cc

test_key_schedule_SOURCES = test-key-schedule.cc

test_polynomials_SOURCES = test-polynomials.cc

-include $(top_srcdir)/git.mk
