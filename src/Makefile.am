
AUTOMAKE_OPTIONS = subdir-objects 

ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

bin_PROGRAMS = compilador

compilador_SOURCES = main.c \
										 keywords.c \
										 parser.c \
										 lexer.c \
										 debug.c \
										 builtin_functions.c \
										 symtab.c

compilador_LDADD = ${COMPILADOR_LIBS} -lpthread

