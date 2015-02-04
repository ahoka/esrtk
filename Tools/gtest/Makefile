VER=1.7.0

all: libgtest.a libgtest_main.a include

gtest-$(VER).zip:
	wget http://googletest.googlecode.com/files/gtest-1.7.0.zip
	touch gtest-$(VER).zip

gtest-$(VER)/CMakeLists.txt: gtest-$(VER).zip
	unzip gtest-$(VER).zip
	touch gtest-$(VER)/CMakeLists.txt

gtest-$(VER)-build:
	mkdir gtest-$(VER)-build

gtest-$(VER)-build/libgtest.a: gtest-$(VER)-build gtest-$(VER)/CMakeLists.txt
	cd gtest-$(VER)-build && cmake ../gtest-$(VER) && make

libgtest.a: gtest-$(VER)-build/libgtest.a
	cp -v gtest-$(VER)-build/libgtest.a .

libgtest_main.a: gtest-$(VER)-build/libgtest_main.a
	cp -v gtest-$(VER)-build/libgtest_main.a .

include: gtest-$(VER)
	cp -vr gtest-$(VER)/include .

clean:
	-rm -rf gtest-$(VER).zip gtest-$(VER) gtest-$(VER)-build libgtest.a include
