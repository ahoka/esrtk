# Makefile to be included for test cases using gtest
#

LIBUTILPATH:=	$(abspath $(dir $(lastword $(MAKEFILE_LIST))))

CXXFLAGS+=	-I$(LIBUTILPATH)
LDFLAGS+=	-L$(LIBUTILPATH)
STATICLIBS+=	-lutility_s
