
AUTOMAKE_OPTIONS = subdir-objects 

ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

DEFAULT_INCLUDES = -I ../includes

bin_PROGRAMS = mypas

mypas_SOURCES = main.c \
                keywords.c \
                parser.c \
                lexer.c \
                debug.c \
                builtin_functions.c \
                gencode.c \
                symtab.c

mypas_LDADD = ${MYPAS_LIBS} -lpthread

