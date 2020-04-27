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

// Use a fixed static buffer for the heap.
//static char heap[16384];
static char heap[6384];

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
    lcd_clear_buf();
    lcd_writeClr(t24);
    lcd_putsR(t24, "MicroPython Test");
    lcd_refresh();

    // Configure stack limit and heap (with our static buffer).
    mp_stack_set_limit(40000 * (BYTES_PER_WORD / 4));
    gc_init(heap, heap + sizeof(heap));

    // Initialise MicroPython.
    mp_init();

    nlr_buf_t nlr;

    // register dmcp module
    mp_module_register(MP_QSTR_dmcp, MP_OBJ_FROM_PTR(&module_dmcp));

    // import dmcp module
    if (execute_from_str("import dmcp")) { 
        lcd_putsAt(t24, 1, "import dmcp error");
    } else {
        lcd_putsAt(t24, 1, "import dmcp ok");
    }
    lcd_refresh();

    // Set some global variables.
    //const char str_set_global[] = "foo = dmcp.add_ints(12,11)\nbar = 50-8\nhellotext = ' '.join(['Hello', 'Micro', 'Python']).replace('o P', 'oP')";
    const char str_set_global[] =
        "foo = dmcp.add_ints(12,11)\n"
        "bar = 50-8\n"
        "hellotext = ' '.join(['Hello', 'Micro', 'Python']).replace('o P', 'oP')";

    if (execute_from_str(str_set_global)) {
        lcd_putsAt(t24, 2, "set global error");
    }
    lcd_refresh();

    char charbuf[256];
    // Read back the global variables (see qstrdefsembed.h for where the QSTR is defined).
    if (nlr_push(&nlr) == 0) {
        mp_obj_dict_t *globals = mp_globals_get();
        mp_obj_t v;

        v = mp_obj_dict_get(MP_OBJ_FROM_PTR(globals), MP_ROM_QSTR(MP_QSTR_hellotext));
        lcd_putsAt(t24, 2, mp_obj_str_get_str(v));

        v = mp_obj_dict_get(MP_OBJ_FROM_PTR(globals), MP_ROM_QSTR(MP_QSTR_foo));
        snprintf(charbuf, 256, "foo = %d\n", mp_obj_get_int(v));
        lcd_putsAt(t24, 4, charbuf);

        v = mp_obj_dict_get(MP_OBJ_FROM_PTR(globals), MP_ROM_QSTR(MP_QSTR_bar));
        snprintf(charbuf, 256, "bar = %d\n", mp_obj_get_int(v));
        lcd_putsAt(t24, 5, charbuf);

        nlr_pop();
    } else {
        // uncaught exception
        lcd_putsAt(t24, 4, "read global error");
    }
    lcd_refresh();

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

    // Deinitialise MicroPython.
    mp_deinit();

    wait_for_key_press();
}

