# Makefile to be included for test cases using gtest
#

GTESTPATH:=	$(abspath $(dir $(lastword $(MAKEFILE_LIST))))

CXXFLAGS+=	-I$(GTESTPATH)/include
STATICLIBS+=	-lgtest
LDFLAGS+=	-L$(GTESTPATH) -lpthread
