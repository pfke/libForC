#include "lfcCore.h"

#include <errno.h>
#include <execinfo.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ucontext.h>
#include <unistd.h>

/* This structure mirrors the one found in /usr/include/asm/ucontext.h */
typedef struct _sig_ucontext {
 unsigned long     uc_flags;
 struct ucontext   *uc_link;
 stack_t           uc_stack;
 struct sigcontext uc_mcontext;
 sigset_t          uc_sigmask;
} sig_ucontext_t;

void crit_err_hdlr(int sig_num, siginfo_t * info, void * ucontext)
{
    void *             array[50];
    void *             caller_address;
    char **            messages;
    int                size, i;
    sig_ucontext_t *   uc;

    uc = (sig_ucontext_t *)ucontext;

     /* Get the address at the time the signal was raised */
#if defined(__i386__) // gcc specific
    caller_address = (void *) uc->uc_mcontext.eip; // EIP: x86 specific
#elif defined(__x86_64__) // gcc specific
    caller_address = (void *) uc->uc_mcontext.rip; // RIP: x86_64 specific
#elif defined(__mips__)
    caller_address = (void *) uc->uc_mcontext.sc_pc;
#elif defined(__arm__)
    caller_address = (void *) uc->uc_mcontext.arm_pc;
#else
    #error Unsupported architecture. // TODO: Add support for other arch.
#endif

    fprintf(
        stderr,
        "signal %d (%s), address is %p from %p\n",
        sig_num, strsignal(sig_num), info->si_addr,
        (void *)caller_address
    );

    size = backtrace(array, 50);

    /* overwrite sigaction with caller's address */
    array[1] = caller_address;

    messages = backtrace_symbols(array, size);

    /* skip first stack frame (points here) */
    for (i = 1; i < size && messages != NULL; ++i) {
        fprintf(stderr, "[bt]: (%d) %s\n", i, messages[i]);
    }

    free(messages);

    exit(EXIT_FAILURE);
}

int lfcCore_install_sigsegv_handler() {
    struct sigaction sigact;

    sigact.sa_sigaction = crit_err_hdlr;
    sigact.sa_flags = SA_RESTART | SA_SIGINFO;

    if (sigaction(SIGSEGV, &sigact, (struct sigaction *)NULL) != 0) {
        fprintf(
            stderr,
            "error setting signal handler for %d (%s)\n",
            SIGSEGV, strsignal(SIGSEGV)
        );

        return -1;
    }

    return 0;
}

const char *lfcCore_strerrno(
    int in
) {
    switch (in) {
        case EPERM:             return "EPERM";
        case ENOENT:            return "ENOENT";
        case ESRCH:             return "ESRCH";
        case EINTR:             return "EINTR";
        case EIO:               return "EIO";
        case ENXIO:             return "ENXIO";
        case E2BIG:             return "E2BIG";
        case ENOEXEC:           return "ENOEXEC";
        case EBADF:             return "EBADF";
        case ECHILD:            return "ECHILD";
        case EAGAIN:            return "EAGAIN";
        case ENOMEM:            return "ENOMEM";
        case EACCES:            return "EACCES";
        case EFAULT:            return "EFAULT";
        case ENOTBLK:           return "ENOTBLK";
        case EBUSY:             return "EBUSY";
        case EEXIST:            return "EEXIST";
        case EXDEV:             return "EXDEV";
        case ENODEV:            return "ENODEV";
        case ENOTDIR:           return "ENOTDIR";
        case EISDIR:            return "EISDIR";
        case EINVAL:            return "EINVAL";
        case ENFILE:            return "ENFILE";
        case EMFILE:            return "EMFILE";
        case ENOTTY:            return "ENOTTY";
        case ETXTBSY:           return "ETXTBSY";
        case EFBIG:             return "EFBIG";
        case ENOSPC:            return "ENOSPC";
        case ESPIPE:            return "ESPIPE";
        case EROFS:             return "EROFS";
        case EMLINK:            return "EMLINK";
        case EPIPE:             return "EPIPE";
        case EDOM:              return "EDOM";
        case ERANGE:            return "ERANGE";
        case EDEADLK:           return "EDEADLK";
        case ENAMETOOLONG:      return "ENAMETOOLONG";
        case ENOLCK:            return "ENOLCK";
        case ENOSYS:            return "ENOSYS";
        case ENOTEMPTY:         return "ENOTEMPTY";
        case ELOOP:             return "ELOOP";
        case ENOMSG:            return "ENOMSG";
        case EIDRM:             return "EIDRM";
        case ECHRNG:            return "ECHRNG";
        case EL2NSYNC:          return "EL2NSYNC";
        case EL3HLT:            return "EL3HLT";
        case EL3RST:            return "EL3RST";
        case ELNRNG:            return "ELNRNG";
        case EUNATCH:           return "EUNATCH";
        case ENOCSI:            return "ENOCSI";
        case EL2HLT:            return "EL2HLT";
        case EBADE:             return "EBADE";
        case EBADR:             return "EBADR";
        case EXFULL:            return "EXFULL";
        case ENOANO:            return "ENOANO";
        case EBADRQC:           return "EBADRQC";
        case EBADSLT:           return "EBADSLT";
        case EBFONT:            return "EBFONT";
        case ENOSTR:            return "ENOSTR";
        case ENODATA:           return "ENODATA";
        case ETIME:             return "ETIME";
        case ENOSR:             return "ENOSR";
        case ENONET:            return "ENONET";
        case ENOPKG:            return "ENOPKG";
        case EREMOTE:           return "EREMOTE";
        case ENOLINK:           return "ENOLINK";
        case EADV:              return "EADV";
        case ESRMNT:            return "ESRMNT";
        case ECOMM:             return "ECOMM";
        case EPROTO:            return "EPROTO";
        case EMULTIHOP:         return "EMULTIHOP";
        case EDOTDOT:           return "EDOTDOT";
        case EBADMSG:           return "EBADMSG";
        case EOVERFLOW:         return "EOVERFLOW";
        case ENOTUNIQ:          return "ENOTUNIQ";
        case EBADFD:            return "EBADFD";
        case EREMCHG:           return "EREMCHG";
        case ELIBACC:           return "ELIBACC";
        case ELIBBAD:           return "ELIBBAD";
        case ELIBSCN:           return "ELIBSCN";
        case ELIBMAX:           return "ELIBMAX";
        case ELIBEXEC:          return "ELIBEXEC";
        case EILSEQ:            return "EILSEQ";
        case ERESTART:          return "ERESTART";
        case ESTRPIPE:          return "ESTRPIPE";
        case EUSERS:            return "EUSERS";
        case ENOTSOCK:          return "ENOTSOCK";
        case EDESTADDRREQ:      return "EDESTADDRREQ";
        case EMSGSIZE:          return "EMSGSIZE";
        case EPROTOTYPE:        return "EPROTOTYPE";
        case ENOPROTOOPT:       return "ENOPROTOOPT";
        case EPROTONOSUPPORT:   return "EPROTONOSUPPORT";
        case ESOCKTNOSUPPORT:   return "ESOCKTNOSUPPORT";
        case EOPNOTSUPP:        return "EOPNOTSUPP";
        case EPFNOSUPPORT:      return "EPFNOSUPPORT";
        case EAFNOSUPPORT:      return "EAFNOSUPPORT";
        case EADDRINUSE:        return "EADDRINUSE";
        case EADDRNOTAVAIL:     return "EADDRNOTAVAIL";
        case ENETDOWN:          return "ENETDOWN";
        case ENETUNREACH:       return "ENETUNREACH";
        case ENETRESET:         return "ENETRESET";
        case ECONNABORTED:      return "ECONNABORTED";
        case ECONNRESET:        return "ECONNRESET";
        case ENOBUFS:           return "ENOBUFS";
        case EISCONN:           return "EISCONN";
        case ENOTCONN:          return "ENOTCONN";
        case ESHUTDOWN:         return "ESHUTDOWN";
        case ETOOMANYREFS:      return "ETOOMANYREFS";
        case ETIMEDOUT:         return "ETIMEDOUT";
        case ECONNREFUSED:      return "ECONNREFUSED";
        case EHOSTDOWN:         return "EHOSTDOWN";
        case EHOSTUNREACH:      return "EHOSTUNREACH";
        case EALREADY:          return "EALREADY";
        case EINPROGRESS:       return "EINPROGRESS";
        case ESTALE:            return "ESTALE";
        case EUCLEAN:           return "EUCLEAN";
        case ENOTNAM:           return "ENOTNAM";
        case ENAVAIL:           return "ENAVAIL";
        case EISNAM:            return "EISNAM";
        case EREMOTEIO:         return "EREMOTEIO";
        case EDQUOT:            return "EDQUOT";
        case ENOMEDIUM:         return "ENOMEDIUM";
        case EMEDIUMTYPE:       return "EMEDIUMTYPE";
        case ECANCELED:         return "ECANCELED";
        case ENOKEY:            return "ENOKEY";
        case EKEYEXPIRED:       return "EKEYEXPIRED";
        case EKEYREVOKED:       return "EKEYREVOKED";
        case EKEYREJECTED:      return "EKEYREJECTED";
        case EOWNERDEAD:        return "EOWNERDEAD";
        case ENOTRECOVERABLE:   return "ENOTRECOVERABLE";
        case ERFKILL:           return "ERFKILL";
        case EHWPOISON:         return "EHWPOISON";
    }

    return "UNDEFINED";
}
