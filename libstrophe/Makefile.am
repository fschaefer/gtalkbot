AUTOMAKE_OPTIONS = subdir-objects

CFLAGS = -g -Wall

builtin_CFLAGS = -I$(top_srcdir)/expat/lib
builtin_LIBS = libexpat.a

PARSER_CFLAGS=@PARSER_CFLAGS@
PARSER_LIBS=@PARSER_LIBS@

STROPHE_FLAGS = -I$(top_srcdir)
STROPHE_LIBS = libstrophe.a $(PARSER_LIBS) -lssl -lresolv

## Main build targets
if BUILD_EXPAT
lib_LIBRARIES = libstrophe.a libexpat.a
else
lib_LIBRARIES = libstrophe.a
endif

libstrophe_a_CFLAGS=$(STROPHE_FLAGS) $(PARSER_CFLAGS)
libstrophe_a_SOURCES = src/auth.c src/conn.c src/ctx.c \
	src/event.c src/handler.c src/hash.c \
	src/jid.c src/md5.c src/sasl.c src/sha1.c \
	src/snprintf.c src/sock.c src/stanza.c src/thread.c \
	src/tls_openssl.c src/util.c
if PARSER_EXPAT
libstrophe_a_SOURCES += src/parser_expat.c
else
libstrophe_a_SOURCES += src/parser_libxml2.c
endif

libexpat_a_CFLAGS=-DXML_DTD -DXML_NS -DXML_CONTEXT_BYTES=1024 -DXML_STATIC \
	-I$(top_srcdir)/expat/lib
libexpat_a_SOURCES=expat/lib/xmlparse.c expat/lib/xmltok.c expat/lib/xmlrole.c

