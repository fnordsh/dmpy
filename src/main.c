#include <main.h>
#include <dmcp.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "py/compile.h"
#include "py/runtime.h"
#include "py/gc.h"
#include "py/stackctrl.h"
#include "py/qstr.h"

#include "py/obj.h"
#include "py/builtin.h"
#include "py/objmodule.h"

#include "module_dmcp.h"
#include "module_dmpy.h"

// Use a fixed static buffer for the heap.
//static char heap[16384];
static char *heap;

mp_obj_t execute_from_str(const char *str) {
    nlr_buf_t nlr;
    if (nlr_push(&nlr) == 0) {
        // Use the empty string as the "filename".
        qstr src_name = MP_QSTR_;
        mp_lexer_t *lex = mp_lexer_new_from_str_len(src_name, str, strlen(str), false);
        mp_parse_tree_t pt = mp_parse(lex, MP_PARSE_FILE_INPUT);
        mp_obj_t module_fun = mp_compile(&pt, src_name, false);
        mp_call_function_0(module_fun);
        nlr_pop();
        return 0;
    } else {
        // uncaught exception
        return (mp_obj_t)nlr.ret_val;
    }
}

void shutdown_hook(void) {
    printf("Shutdown hook called.\n");
}

void nlr_jump_fail(void *val) {
    printf("FATAL: uncaught NLR %p\n", val);
    exit(1);
}

void _exit(int v) {
    while(1);
}

int _open() {
    return 0;
}


void program_main() {
    char strbuf[128];

    lcd_clear_buf();
    lcd_writeClr(t24);
    lcd_putsR(t24, "MicroPython Test");
    lcd_refresh();

    // Configure stack limit and heap (with our static buffer).
    mp_stack_set_limit(40000 * (BYTES_PER_WORD / 4));
    int heapsize = 16384;
    while ((heap=malloc(heapsize)) == 0) {
        heapsize /= 2;
    }
    snprintf(strbuf, 128, "heap: %d bytes", heapsize);
    lcd_putsAt(t24, 1, strbuf);
    lcd_refresh();
    gc_init(heap, heap + heapsize);

    // Initialise MicroPython.
    mp_init();

    nlr_buf_t nlr;

    // register modules
    mp_module_register(MP_QSTR_dmcp, MP_OBJ_FROM_PTR(&module_dmcp));
    mp_module_register(MP_QSTR_dmpy, MP_OBJ_FROM_PTR(&module_dmpy));

    FIL f;
    FRESULT fr = f_open(&f, "test.py", FA_READ);
    if(fr) {
        snprintf(strbuf, 128, "Could not open test.py (%d)", fr);
        lcd_putsAt(t24, 2, strbuf);
        lcd_refresh();
    } else {
        FSIZE_t fsize = f_size(&f);
        snprintf(strbuf, 128, "loading test.py (%lu bytes)", fsize);
        lcd_putsAt(t24, 2, strbuf);
        lcd_refresh();

        char *filebuf = malloc(fsize+1);
        if(filebuf==0) {
            snprintf(strbuf, 128, "could not allocate memory");
            lcd_putsAt(t24, 3, strbuf);
            lcd_refresh();
        } else {
            unsigned int read_bytes;
            f_read(&f, filebuf, fsize, &read_bytes);
            f_close(&f);
            filebuf[fsize]=0;
            snprintf(strbuf, 128, "read %u bytes", read_bytes);
            lcd_putsAt(t24, 3, strbuf);
            lcd_putsAt(t24, 4, "press any key to start");
            lcd_refresh();
            wait_for_key_press();

            if(execute_from_str(filebuf)) {
                snprintf(strbuf, 128, "python error");
                lcd_putsAt(t24, 5, strbuf);
                lcd_refresh();
            }
            free(filebuf);
        }
    }

    /*
    // Simple exception handling.
    if (nlr_push(&nlr) == 0) {
        lcd_putsAt(t24, 6, "Raising a test exception...");
        mp_raise_ValueError(MP_ERROR_TEXT("embed exception"));
        nlr_pop();
    } else {
        lcd_putsAt(t24, 7, "  caught");
        printf(" caught: ");
        mp_obj_t exc = (mp_obj_t)nlr.ret_val;
        mp_obj_print_exception(&mp_plat_print, exc);
    }
    */

    // Deinitialise MicroPython.
    mp_deinit();

    wait_for_key_press();
}

