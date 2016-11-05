noinst_PROGRAMS = dumbaes-cli

dumbaes_cli_SOURCES = dumbaes-cli.c

dumbaes_cli_CFLAGS = $(WARN_CFLAGS)

dumbaes_cli_CPPFLAGS = \
	-I$(top_srcdir)/api	\
	$(GLIB_CFLAGS)

dumbaes_cli_LDADD = \
	$(top_builddir)/api/libdumbaes.la	\
	$(GLIB_LIBS)

dumbaes_cli_LDFLAGS = $(WARN_LDFLAGS)

-include $(top_srcdir)/git.mk
