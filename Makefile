# GNU Make wrapper for build system

MACHINE=i686
MAKEOBJDIR=${CURDIR}/obj.${MACHINE}

SYSMAKE=bmake
SYSMAKE_FLAGS=-r
SYSMAKEFILE=KernelMakefile
SYSMAKECMD=env HAS_BUILD_SH=1 MACHINE=i686 MAKEOBJDIR=${MAKEOBJDIR} ${SYSMAKE} ${SYSMAKE_FLAGS} -f ${SYSMAKEFILE}

all: build

${MAKEOBJDIR}:
	mkdir -p ${MAKEOBJDIR}

.PHONY: build
build: ${MAKEOBJDIR}
	@echo "---- Building kernel ----"
	${SYSMAKECMD} depend
	${SYSMAKECMD} all
	@echo "---- Building runtime ----"
	cd Runtime && ./build.sh

.PHONY: iso
iso: build
	@echo "---- Creating ISO ----"
	mkdir -p iso/boot/grub
	i686-elf-objcopy --strip-debug obj.${MACHINE}/kernel.elf iso/boot/kernel.elf
	i686-elf-objcopy --strip-debug Runtime/obj.${MACHINE}/runtime.elf iso/boot/runtime.elf
	cp Loader/menu.lst iso/boot/grub
	cp Loader/stage2_eltorito iso/boot/grub
	mkisofs -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 \
	 -boot-info-table -o esrtk.iso iso
#	rm -rf iso

.PHONY: run
run: iso
	./simulator.sh

.PHONY: debug
debug: build
	${SYSMAKECMD} debug

.PHONY: clean
clean:
	${SYSMAKECMD} clean

.PHONY: autobuild
autobuild:
	${SYSMAKECMD} autobuild

.PHONY: test
test:
	cd Test && make check
