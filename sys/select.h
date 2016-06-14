/* `fd_set' type and related macros, and `select'/`pselect' declarations.
   Copyright (C) 1996,97,98,99,2000,01,02,2003 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

/*	POSIX 1003.1g: 6.2 Select from File Descriptor Sets <sys/select.h>  */

#ifndef _SYS_SELECT_H
#define _SYS_SELECT_H	1

//#include <features.h>

/* Get definition of needed basic types.  */
#include "../bits/types.h"

/* Get __FD_* definitions.  */
#include "../bits/select.h"

///* Get __sigset_t.  */
//#include <bits/sigset.h>
//#ifndef __sigset_t_defined
//# define __sigset_t_defined
//typedef __sigset_t sigset_t;
//#endif

///* Get definition of timer specification structures.  */
//#define __need_time_t
//#define __need_timespec
//#define __need_timeval
//#include <time.h>
//#include <bits/time.h>

//#ifndef __suseconds_t_defined
//typedef __suseconds_t suseconds_t;
//# define __suseconds_t_defined
//#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Some versions of <linux/posix_types.h> define these macros.  */
#undef	__NFDBITS
#undef	__FDELT
#undef	__FDMASK
#undef __FD_SETSIZE

/* It's easier to assume 8-bit bytes than to get CHAR_BIT.  */
#define __NFDBITS	(8 * sizeof (__fd_mask))                   // (8 * sizeof (long int))
#define	__FDELT(d)	((d) / __NFDBITS)
#define	__FDMASK(d)	((__fd_mask) 1 << ((d) % __NFDBITS))
#define __FD_SETSIZE    1024

/* The fd_set member is required to be an array of longs.  */
typedef long int __fd_mask;

/* fd_set for select and pselect.  */
typedef struct
  {
    /* XPG4.2 requires this member name.  Otherwise avoid the name
       from the global namespace.  */
#ifdef __USE_XOPEN
    __fd_mask fds_bits[__FD_SETSIZE / __NFDBITS];              // long int[]
# define __FDS_BITS(set) ((set)->fds_bits)
#else
    __fd_mask __fds_bits[__FD_SETSIZE / __NFDBITS];
# define __FDS_BITS(set) ((set)->__fds_bits)
#endif
  } fd_set;

/* Maximum number of file descriptors in `fd_set'.  */
#define	FD_SETSIZE		__FD_SETSIZE

#ifdef __USE_MISC
/* Sometimes the fd_set member is assumed to have this type.  */
typedef __fd_mask fd_mask;

/* Number of bits per word of `fd_set' (some code assumes this is 32).  */
# define NFDBITS		__NFDBITS
#endif


/* Access macros for `fd_set'.  */
#define	FD_SET(fd, fdsetp)	__FD_SET (fd, fdsetp)
#define	FD_CLR(fd, fdsetp)	__FD_CLR (fd, fdsetp)
#define	FD_ISSET(fd, fdsetp)	__FD_ISSET (fd, fdsetp)
#define	FD_ZERO(fdsetp)		__FD_ZERO (fdsetp)


//__BEGIN_DECLS

/* Check the first NFDS descriptors each in READFDS (if not NULL) for read
   readiness, in WRITEFDS (if not NULL) for write readiness, and in EXCEPTFDS
   (if not NULL) for exceptional conditions.  If TIMEOUT is not NULL, time out
   after waiting the interval specified therein.  Returns the number of ready
   descriptors, or -1 for errors.

   This function is a cancellation point and therefore not marked with
   __THROW.  */

//typedef struct timeval tval;

int myselect (int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, void *useless);
#ifdef __cplusplus
}
#endif

#endif /* sys/select.h */
