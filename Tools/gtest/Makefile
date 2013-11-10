VER=1.7.0

all: libgtest.a include

gtest-$(VER).zip:
	wget http://googletest.googlecode.com/files/gtest-1.7.0.zip

gtest-$(VER): gtest-$(VER).zip
	unzip gtest-$(VER).zip

gtest-$(VER)-build:
	mkdir gtest-$(VER)-build

gtest-$(VER)-build/libgtest.a: gtest-$(VER)-build gtest-$(VER)
	cd gtest-$(VER)-build && cmake ../gtest-$(VER) && make

libgtest.a: gtest-$(VER)-build/libgtest.a
	cp -v gtest-$(VER)-build/libgtest.a .

include: gtest-$(VER)
	cp -vr gtest-$(VER)/include .


clean:
	-rm -rf gtest-$(VER).zip gtest-$(VER) gtest-$(VER)-build libgtest.a include
