COMMON_H = \
	$(top_srcdir)/include/sys/fs/zfs.h

KERNEL_H =

USER_H =

#EXTRA_DIST = $(COMMON_H) $(KERNEL_H) $(USER_H)

if CONFIG_USER
#libzfsdir = $(includedir)/libzfs/sys/fs
#libzfs_HEADERS = $(COMMON_H) $(USER_H)
libzfsdir =
libzfs_HEADERS =
endif

if CONFIG_KERNEL
#kerneldir = @prefix@/src/zfs-$(VERSION)/include/sys/fs
#kernel_HEADERS = $(COMMON_H) $(KERNEL_H)
kerneldir =
kernel_HEADERS =
endif
