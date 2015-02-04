# Makefile to be included for test cases using gtest
#

GTESTPATH:=	$(abspath $(dir $(lastword $(MAKEFILE_LIST))))

CXXFLAGS+=	-I$(GTESTPATH)/include
STATICLIBS+=	-lgtest -lgtest_main
LDFLAGS+=	-L$(GTESTPATH) -lpthread

TESTS_BIN=	${TESTS:.cc=.gtest}

all: ${TESTS_BIN}
	@echo TESTING
	@echo ${TESTS_BIN}

%.gtest: %.cc
	${CXX} -Wall ${CXXFLAGS}  ${LDFLAGS} ${STATICLIBS} $^ -o $@
