/*
 * Nintendo Switch (libnx) build configuration for xemu
 *
 * This file provides hardcoded configuration values for building
 * xemu on Nintendo Switch using devkitpro/libnx.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef CONFIG_SWITCH_H
#define CONFIG_SWITCH_H

/*
 * Platform identification
 */
#define CONFIG_SWITCH 1
#define CONFIG_POSIX 1

/* Disable Linux-specific features */
#undef CONFIG_LINUX
#undef CONFIG_DARWIN
#undef CONFIG_BSD
#undef CONFIG_WIN32
#undef CONFIG_SOLARIS
#undef CONFIG_FREEBSD

/*
 * Host architecture - Nintendo Switch uses ARM64
 */
#define HOST_AARCH64 1
#undef HOST_X86_64
#undef HOST_I386

/*
 * Version information
 */
#define QEMU_VERSION "9.2.1"
#define QEMU_VERSION_MAJOR 9
#define QEMU_VERSION_MINOR 2
#define QEMU_VERSION_MICRO 1

/*
 * Memory and threading features
 */
#define CONFIG_ATOMIC64 1
#define CONFIG_INT128 1
#define CONFIG_INT128_TYPE 1
#define CONFIG_IOVEC 1
#define CONFIG_MEMALIGN 1
#define CONFIG_POSIX_MEMALIGN 1
#define CONFIG_VALLOC 1
#undef CONFIG_ALIGNED_MALLOC

/*
 * Coroutine backend - use ucontext (libnx provides this)
 */
#define CONFIG_COROUTINE_POOL 1
/* libnx doesn't have full ucontext - use setjmp/longjmp based approach */
#undef CONFIG_UCONTEXT
#define CONFIG_COROUTINE_SETJMP 1

/*
 * Event/IO subsystem
 * libnx provides basic poll() - no epoll/kqueue
 */
#undef CONFIG_EPOLL
#undef CONFIG_EPOLL_CREATE1
#undef CONFIG_EVENTFD
#undef CONFIG_SIGNALFD
#undef CONFIG_TIMERFD
#undef CONFIG_INOTIFY
#undef CONFIG_INOTIFY1
#undef CONFIG_LINUX_AIO
#undef CONFIG_LINUX_IO_URING

/*
 * Graphics - OpenGL 4.3 available via libnx mesa
 */
#define CONFIG_OPENGL 1
#define CONFIG_SDL 1
#undef CONFIG_VULKAN
#undef CONFIG_GBM
#undef CONFIG_GTK
#undef CONFIG_VTE
#undef CONFIG_CURSES
#undef CONFIG_COCOA
#undef CONFIG_VNC
#undef CONFIG_SPICE
#undef CONFIG_DBUS_DISPLAY

/*
 * Audio - SDL audio backend only
 */
#define CONFIG_AUDIO_SDL 1
#define CONFIG_AUDIO_DRIVERS "sdl",
#undef CONFIG_AUDIO_ALSA
#undef CONFIG_AUDIO_COREAUDIO
#undef CONFIG_AUDIO_DSOUND
#undef CONFIG_AUDIO_JACK
#undef CONFIG_AUDIO_OSS
#undef CONFIG_AUDIO_PA
#undef CONFIG_AUDIO_PIPEWIRE
#undef CONFIG_AUDIO_SNDIO

/*
 * Networking - disabled for initial port
 */
#undef CONFIG_SLIRP
#undef CONFIG_VDE
#undef CONFIG_NETMAP
#undef CONFIG_L2TPV3
#undef CONFIG_AF_VSOCK
#undef CONFIG_AF_XDP
#undef CONFIG_RTNETLINK

/*
 * Virtualization - TCG only (no hardware acceleration)
 */
#define CONFIG_TCG 1
#undef CONFIG_KVM
#undef CONFIG_XEN_BACKEND
#undef CONFIG_VHOST
#undef CONFIG_VHOST_USER
#undef CONFIG_VHOST_KERNEL
#undef CONFIG_VHOST_NET
#undef CONFIG_VHOST_CRYPTO
#undef CONFIG_VHOST_VDPA
#define CONFIG_KVM_TARGETS ""

/*
 * Block backends - minimal for game disc images
 */
#undef CONFIG_CURL
#undef CONFIG_GLUSTERFS
#undef CONFIG_LIBISCSI
#undef CONFIG_LIBNFS
#undef CONFIG_LIBSSH
#undef CONFIG_RBD
#undef CONFIG_BRLAPI
#undef CONFIG_BLKIO
#undef CONFIG_REPLICATION

/*
 * Crypto - use internal implementations
 */
#undef CONFIG_GNUTLS
#undef CONFIG_GNUTLS_CRYPTO
#undef CONFIG_NETTLE
#undef CONFIG_GCRYPT
#undef CONFIG_TASN1
#undef CONFIG_HOGWEED
#define CONFIG_QEMU_PRIVATE_XTS 1

/*
 * Filesystem features
 */
#undef CONFIG_VIRTFS
#undef CONFIG_FUSE
#undef CONFIG_FUSE_LSEEK
#undef CONFIG_MPATH
#undef CONFIG_BLKZONED
#undef CONFIG_FALLOCATE
#undef CONFIG_FALLOCATE_PUNCH_HOLE
#undef CONFIG_FALLOCATE_ZERO_RANGE
#undef CONFIG_POSIX_FALLOCATE
#undef CONFIG_FDATASYNC
#undef CONFIG_SYNCFS
#undef CONFIG_SYNC_FILE_RANGE
#undef CONFIG_FIEMAP
#undef CONFIG_STATX
#undef CONFIG_STATX_MNT_ID
#undef CONFIG_GETRANDOM

/*
 * Misc disabled features
 */
#undef CONFIG_TPM
#undef CONFIG_SECCOMP
#undef CONFIG_NUMA
#undef CONFIG_LIBCAP_NG
#undef CONFIG_ATTR
#undef CONFIG_LIBATTR
#undef CONFIG_USB_LIBUSB
#undef CONFIG_LIBUDEV
#undef CONFIG_LIBDW
#undef CONFIG_LIBPMEM
#undef CONFIG_LIBDAXCTL
#undef CONFIG_RENDERDOC
#undef CONFIG_PLUGIN
#undef CONFIG_MODULES
#undef CONFIG_SELINUX
#undef CONFIG_CAPSTONE
#undef CONFIG_KEYUTILS
#undef CONFIG_SECRET_KEYRING

/*
 * Tracing - log backend only
 */
#define CONFIG_TRACE_LOG 1
#define CONFIG_TRACE_FILE "trace"
#undef CONFIG_TCG_INTERPRETER

/*
 * Debugging features (can enable for debug builds)
 */
#undef CONFIG_DEBUG_MUTEX
#undef CONFIG_DEBUG_STACK_USAGE
#undef CONFIG_DEBUG_TCG
#undef CONFIG_VALGRIND_H
#undef CONFIG_TSAN
#undef CONFIG_GCOV
#undef CONFIG_CFI
#undef CONFIG_SAFESTACK

/*
 * Thread features
 * libnx provides pthreads via newlib
 */
#undef CONFIG_PTHREAD_SETNAME_NP_W_TID
#undef CONFIG_PTHREAD_SETNAME_NP_WO_TID
#undef CONFIG_PTHREAD_SET_NAME_NP
#undef CONFIG_PTHREAD_AFFINITY_NP
#undef CONFIG_PTHREAD_CONDATTR_SETCLOCK
#undef CONFIG_PTHREAD_FCHDIR_NP
#undef CONFIG_MEMBARRIER
#undef CONFIG_SCHED_GETCPU

/*
 * Process features not available on Switch
 */
#undef CONFIG_PRCTL_PR_SET_TIMERSLACK
#undef CONFIG_DUP3
#undef CONFIG_ACCEPT4
#undef CONFIG_CLOSE_RANGE
#undef CONFIG_SETNS
#undef CONFIG_PREADV
#undef CONFIG_SENDFILE
#undef CONFIG_SPLICE

/*
 * Disabled system features
 */
#undef CONFIG_MADVISE
#undef CONFIG_POSIX_MADVISE
#undef CONFIG_MALLOC_TRIM
#undef CONFIG_MEMFD
#undef CONFIG_FSFREEZE
#undef CONFIG_FSTRIM
#undef CONFIG_OPEN_BY_HANDLE

/*
 * Path configuration for Switch
 */
#define CONFIG_PREFIX "sdmc:/switch/xemu"
#define CONFIG_BINDIR "sdmc:/switch/xemu"
#define CONFIG_SYSCONFDIR "sdmc:/switch/xemu"
#define CONFIG_QEMU_CONFDIR "sdmc:/switch/xemu"
#define CONFIG_QEMU_DATADIR "sdmc:/switch/xemu/data"
#define CONFIG_QEMU_MODDIR "sdmc:/switch/xemu"
#define CONFIG_QEMU_LOCALEDIR "sdmc:/switch/xemu/locale"
#define CONFIG_QEMU_HELPERDIR "sdmc:/switch/xemu"
#define CONFIG_QEMU_LOCALSTATEDIR "sdmc:/switch/xemu"
#define CONFIG_QEMU_DESKTOPDIR "sdmc:/switch/xemu"
#define CONFIG_QEMU_ICONDIR "sdmc:/switch/xemu/icons"
#define CONFIG_QEMU_FIRMWAREPATH "sdmc:/switch/xemu/firmware",

/*
 * Block device whitelist (empty = allow all)
 */
#define CONFIG_BDRV_RO_WHITELIST
#define CONFIG_BDRV_RW_WHITELIST
#undef CONFIG_BDRV_WHITELIST_TOOLS

/*
 * TLS priority string
 */
#define CONFIG_TLS_PRIORITY "NORMAL"

/*
 * DSO suffix for this platform
 */
#define CONFIG_HOST_DSOSUF ".so"

/*
 * Relocatable installation
 */
#define CONFIG_RELOCATABLE 1

/*
 * Misc glib-related defines
 */
#define HAVE_GLIB_WITH_SLICE_ALLOCATOR 1
#undef HAVE_GLIB_WITH_ALIGNED_ALLOC

/*
 * Other missing features
 */
#undef HAVE_GETIFADDRS
#undef HAVE_MLOCKALL
#undef HAVE_OPENPTY
#undef HAVE_PTY_H
#undef HAVE_UTMPX
#undef HAVE_HOST_BLOCK_DEVICE
#undef HAVE_IPPROTO_MPTCP
#undef HAVE_STRCHRNUL
#undef HAVE_STRUCT_STAT_ST_ATIM
#undef HAVE_SYSTEM_FUNCTION
#undef HAVE_COPY_FILE_RANGE
#undef HAVE_DRM_H
#undef HAVE_BTRFS_H
#undef HAVE_OPENAT2_H
#undef HAVE_FSXATTR
#undef HAVE_BLK_ZONE_REP_CAPACITY
#undef HAVE_OPTRESET
#undef HAVE_BROKEN_SIZE_MAX
#undef HAVE_SIGEV_NOTIFY_THREAD_ID

/*
 * Feature flags for conditional code
 */
#undef CONFIG_CPUID_H
#undef CONFIG_GETAUXVAL
#undef CONFIG_GETCPU
#undef CONFIG_GETTID
#undef CONFIG_CLOCK_ADJTIME
#undef CONFIG_PPOLL
#undef CONFIG_ASAN_IFACE_FIBER
#undef CONFIG_ARM_AES_BUILTIN
#undef CONFIG_AVX2_OPT
#undef CONFIG_AVX512BW_OPT
#undef CONFIG_CMPXCHG128
#undef CONFIG_ATOMIC128
#undef CONFIG_ATOMIC128_OPT
#undef CONFIG_SYSMACROS
#undef CONFIG_LINUX_MAGIC_H
#undef CONFIG_GIO
#undef CONFIG_HEXAGON_IDEF_PARSER
#undef CONFIG_CRYPTO_SM3
#undef CONFIG_CRYPTO_SM4
#undef CONFIG_HAVE_RUST
#undef CONFIG_QOM_CAST_DEBUG
#undef CONFIG_X11
#undef CONFIG_XKBCOMMON
#undef CONFIG_PNG
#undef CONFIG_SDL_IMAGE
#undef CONFIG_GTK_CLIPBOARD
#undef CONFIG_ZSTD
#undef CONFIG_SNAPPY
#undef CONFIG_LZO
#undef CONFIG_QATZIP
#undef CONFIG_QPL
#undef CONFIG_UADK
#undef CONFIG_USBFS
#undef CONFIG_EBPF
#undef CONFIG_RDMA

/*
 * Xbox-specific flag (required for xemu)
 */
#define XBOX 1

/*
 * USB subsystem - Enable OHCI for Xbox controllers
 */
#define CONFIG_USB 1
#define CONFIG_USB_OHCI 1
#define CONFIG_USB_OHCI_PCI 1

#endif /* CONFIG_SWITCH_H */
