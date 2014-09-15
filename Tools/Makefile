TOOLS= gtest LibUtil mkramdisk mkfatfs mkinitrd

# shitty gmake cannot .WAIT
.NOTPARALLEL: all
# all: LibUtil mkramdisk mkfatfs gtest mkinitrd

# .PHONY: ${TOOLS}
# ${TOOLS}:
# 	cd $@ && ${MAKE}
# 	cd $@/Test && ${MAKE}

# .PHONY: ${TOOLS}-clean
# ${TOOLS}-clean:
# 	cd $@ && ${MAKE} clean
# 	cd $@/Test && ${MAKE} clean

all:
	for d in ${TOOLS} \
	do \
		cd $${d} && ${MAKE} \
	done
