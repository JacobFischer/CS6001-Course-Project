AM_CFLAGS = $(WARN_CFLAGS)

AM_LDFLAGS = $(WARN_LDFLAGS)

noinst_PROGRAMS = dumbaes-cli dumbaes-gui

dumbaes_cli_SOURCES = dumbaes-cli.c

dumbaes_cli_CPPFLAGS = \
	-I$(top_srcdir)/api	\
	$(GLIB_CFLAGS)		\
	$(GIO_CFLAGS)

dumbaes_cli_LDADD = \
	$(top_builddir)/api/libdumbaes.la	\
	$(GLIB_LIBS)				\
	$(GIO_LIBS)

dumbaes_gui_SOURCES = dumbaes-gui.c

dumbaes_gui_CPPFLAGS = \
	-I$(top_srcdir)/api	\
	$(GLIB_CFLAGS)		\
	$(GIO_CFLAGS)		\
	$(GTK_CFLAGS)

dumbaes_gui_LDADD = \
	$(top_builddir)/api/libdumbaes.la	\
	$(GLIB_LIBS)				\
	$(GIO_LIBS)				\
	$(GTK_LIBS)

-include $(top_srcdir)/git.mk
