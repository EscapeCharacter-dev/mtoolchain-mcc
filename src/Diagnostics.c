#include "Mcc.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

// if any diagnostics were present
static int criticalDiagnostics = 0;

// overrides for gravities (-Werror on GCC, for example)
static int criticalNote = 0;
static int criticalWarn = 0;

// allows other parts of the compiler to override note and warnings.
void DiagnosticsOverride(int onNoteStop, int onWarnStop) {
    criticalNote = onNoteStop;
    criticalWarn = onWarnStop;
}

// is a valid gravity?
static int isValidGravity(I gravity) {
    return gravity == DIAG_GRAVITY_NOTE ||
        gravity == DIAG_GRAVITY_WARN ||
        gravity == DIAG_GRAVITY_ERR ||
        gravity == DIAG_GRAVITY_IERR
        ? 1 : 0;
}

// adds a diagnostic internally.
static void addDiagInternal(I gravity, const char *format, va_list list,
        I fpos, const char *filename) {
    char *buf = calloc(128, 1);
    const I length = vsprintf(buf, format, list);
    if (length < 0) {
        fprintf(stderr, "diagnostic handler problem\n");
        exit(EXIT_FAILURE);
    }
    switch (gravity) {
    case DIAG_GRAVITY_NOTE:
        printf("[note] at char %d in file %s: %s\n", fpos, filename, buf);
        break;
    case DIAG_GRAVITY_WARN:
        printf("[warn] at char %d in file %s: %s\n", fpos, filename, buf);
        break;
    case DIAG_GRAVITY_ERR:
        printf("[err] at char %d in file %s: %s\n", fpos, filename, buf);
        criticalDiagnostics = 1;
        break;
    case DIAG_GRAVITY_IERR:
        printf("[internal error] caused by file %s: %s\n", filename, buf);
        criticalDiagnostics = 1;
        break;
    default:
        fprintf(stderr, "addDiagInternal() bad usage\n");
        exit(EXIT_FAILURE);
    }
    free(buf);
}

// checks if any critical diagnostics were caused
int DiagnosticsAny() {
    return criticalDiagnostics;
}

// add a new diagnostic
void DiagnosticsAdd(I gravity, const char *format, const char *filename,
        I fpos, ...) {
    va_list list;
    va_start(list, fpos);
    if (isValidGravity(gravity)) {
        addDiagInternal(gravity, format, list, fpos, filename);
    } else {
        addDiagInternal(gravity, format, list, 0, "(MCC sources)");
    }
    va_end(list);
}