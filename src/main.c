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

// string buffer for snprintf
char strbuf[128];

// execute python code from string
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

// helper function to display an exception with traceback
void exception_screen(mp_obj_t exc) {
    lcd_clear_buf();
    lcd_writeClr(t20);
    lcd_putsR(t20, "MicroPython Exception");

    char strbuf1[64];
    if (mp_obj_is_exception_instance(exc)) {
        size_t n, *values;
        mp_obj_exception_get_traceback(exc, &n, &values);
        if (n > 0) {
            assert(n % 3 == 0);
            lcd_puts(t20, "Traceback (most recent call last):");
            for (int i = n - 3; i >= 0; i -= 3) {
                #if MICROPY_ENABLE_SOURCE_LINE
                snprintf(strbuf1, sizeof(strbuf1), "  File \"%s\", line %d", qstr_str(values[i]), (int)values[i + 1]);
                #else
                snprintf(strbuf1, sizeof(strbuf1), "  File \"%s\"", qstr_str(values[i]));
                #endif
                
                // the block name can be NULL if it's unknown
                qstr block = values[i + 2];
                if (block == MP_QSTRnull) {
                    snprintf(strbuf, sizeof(strbuf), "%s",strbuf1);
                } else {
                    snprintf(strbuf, sizeof(strbuf), "%s , in %s\n", strbuf1, qstr_str(block));
                }
                lcd_puts(t20, strbuf);
            }
        }
    }

    const mp_obj_type_t *type = mp_obj_get_type(exc);
    mp_obj_t *value = mp_obj_exception_get_value(exc);
    if(value && mp_obj_is_str(value)) {
        snprintf(strbuf, sizeof(strbuf), "%s: %s", qstr_str(type->name), mp_obj_str_get_str(value));
    } else {
        snprintf(strbuf, sizeof(strbuf), "<%s>", qstr_str(type->name));
    }
    lcd_puts(t20, strbuf);

    lcd_refresh();
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

mp_import_stat_t mp_import_stat(const char *path) {
    mp_import_stat_t result = MP_IMPORT_STAT_NO_EXIST;

    FIL f;
    FRESULT fr = f_open(&f, path, FA_READ);
    if(fr==FR_OK) {
        f_close(&f);
        result=MP_IMPORT_STAT_FILE;
    } else {
        snprintf(strbuf, sizeof(strbuf), "%s/", path);
        fr = f_open(&f, strbuf, FA_READ);
        // we expect either FR_NO_PATH if it's not a directory
        // or FR_INVALID_NAME if the directory exists (but "" is not a valid file name)
        if(fr==FR_INVALID_NAME) result=MP_IMPORT_STAT_DIR;
    }

    lcd_putsRAt(t20, 7, "mp_import_stat:");
    lcd_putsRAt(t20, 8, path);
    lcd_putsRAt(t20, 9, result==MP_IMPORT_STAT_FILE?"FILE":result==MP_IMPORT_STAT_DIR?"DIR":"NO_EXIST");
    lcd_refresh();
    wait_for_key_press();

    return result;
}

void program_main() {

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

            mp_obj_t exc = (mp_obj_t) execute_from_str(filebuf);
            if(exc) {
                exception_screen(exc);
            }

            free(filebuf);
        }
    }

    // Deinitialise MicroPython.
    mp_deinit();

    lcd_putsRAt(t20, 10, " Press any key to exit.");
    lcd_refresh();
    wait_for_key_press();

}

