// alloc_trace.c
#define _GNU_SOURCE
#include <dlfcn.h>
#include <execinfo.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef void *(*malloc_t)(size_t);
typedef void (*free_t)(void *);
typedef void *(*calloc_t)(size_t, size_t);
typedef void *(*realloc_t)(void *, size_t);

static malloc_t real_malloc = NULL;
static free_t real_free = NULL;
static calloc_t real_calloc = NULL;
static realloc_t real_realloc = NULL;

static pthread_mutex_t log_lock = PTHREAD_MUTEX_INITIALIZER;
static FILE *logf = NULL;

static void init_real_funcs(void) {
    if (real_malloc) return;
    real_malloc = (malloc_t)dlsym(RTLD_NEXT, "malloc");
    real_free   = (free_t)dlsym(RTLD_NEXT, "free");
    real_calloc = (calloc_t)dlsym(RTLD_NEXT, "calloc");
    real_realloc= (realloc_t)dlsym(RTLD_NEXT, "realloc");
    if (!real_malloc || !real_free) {
        // Something is very wrong
        _exit(1);
    }
    const char *env = getenv("ALLOC_TRACE_LOG");
    if (!env) env = "/tmp/alloc_trace.log";
    logf = fopen(env, "a");
    if (!logf) logf = stderr;
}

static void log_stack_and_size(size_t size, const char *op, void *ptr) {
    void *buf[64];
    int n = backtrace(buf, 64);
    // Use backtrace_symbols_fd or backtrace_symbols for quick output
    char **sym = backtrace_symbols(buf, n); // allocates; avoid in hot path if needed
    pthread_mutex_lock(&log_lock);
    if (logf) {
        fprintf(logf, "%s pid=%d ptr=%p size=%zu frames=%d\n", op, getpid(), ptr, size, n);
        for (int i = 0; i < n; ++i) {
            fprintf(logf, "  %s\n", sym ? sym[i] : "??");
        }
        fflush(logf);
    }
    pthread_mutex_unlock(&log_lock);
    if (sym) free(sym);
}

void *malloc(size_t size) {
    init_real_funcs();
    void *p = real_malloc(size);
    // Avoid recursion: do minimal work
    log_stack_and_size(size, "MALLOC", p);
    return p;
}

void free(void *ptr) {
    init_real_funcs();
    log_stack_and_size(0, "FREE", ptr);
    real_free(ptr);
}

void *calloc(size_t nmemb, size_t size) {
    init_real_funcs();
    void *p = real_calloc(nmemb, size);
    log_stack_and_size(nmemb * size, "CALLOC", p);
    return p;
}

void *realloc(void *ptr, size_t size) {
    init_real_funcs();
    void *p = real_realloc(ptr, size);
    log_stack_and_size(size, "REALLOC", p);
    return p;
}
