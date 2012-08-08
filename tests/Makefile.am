
AUTOMAKE_OPTIONS = subdir-objects 

ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

bin_PROGRAMS = lexer_test 

lexer_test_SOURCES = lexer_test.c \
										 tap.c

TESTS=lexer_test

lexer_test_LDADD = ${LEXER_LIBS} \
                   $(top_srcdir)/src/lexer.o

