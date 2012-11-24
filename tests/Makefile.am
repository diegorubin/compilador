
AUTOMAKE_OPTIONS = subdir-objects 

ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

DEFAULT_INCLUDES = -I ../includes

bin_PROGRAMS = lexer_test parser_test symtab_test builtin_functions_test

lexer_test_SOURCES = lexer_test.c \
										 tap.c

parser_test_SOURCES = parser_test.c \
										  tap.c

symtab_test_SOURCES = symtab_test.c \
										  tap.c

builtin_functions_test_SOURCES = builtin_functions_test.c \
										  tap.c

TESTS=lexer_test parser_test symtab_test builtin_functions_test

lexer_test_LDADD = ${LEXER_LIBS} \
                   $(top_srcdir)/src/lexer.o \
                   $(top_srcdir)/src/keywords.o \
                   -lpthread

parser_test_LDADD = ${PARSER_LIBS} \
                   $(top_srcdir)/src/lexer.o \
                   $(top_srcdir)/src/parser.o \
                   $(top_srcdir)/src/symtab.o \
                   $(top_srcdir)/src/debug.o \
                   $(top_srcdir)/src/gencode.o \
                   $(top_srcdir)/src/builtin_functions.o \
                   $(top_srcdir)/src/keywords.o \
                   -lpthread

symtab_test_LDADD = ${SYMTAB_LIBS} \
                   $(top_srcdir)/src/debug.o \
                   $(top_srcdir)/src/symtab.o \
                   -lpthread

builtin_functions_test_LDADD = ${BUILTIN_FUNCTIONS_LIBS} \
                   $(top_srcdir)/src/debug.o \
                   $(top_srcdir)/src/symtab.o \
                   $(top_srcdir)/src/builtin_functions.o \
                   -lpthread

