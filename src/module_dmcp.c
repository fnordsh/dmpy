// MicroPython module for DMCP API

#include "module_dmcp.h"

#include <dmcp.h>

#include "py/builtin.h"
#include "py/obj.h"
#include "py/objmodule.h"


///// LCD and draw functions / Display screens /////

// int lcd_for_calc(int what)
STATIC mp_obj_t dmcp_lcd_for_calc(mp_obj_t what_obj) {
    int what = mp_obj_get_int(what_obj);
    int result = lcd_for_calc(what);
    return mp_obj_new_int(result);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(dmcp_lcd_for_calc_obj, dmcp_lcd_for_calc);



///// LCD and draw functions / Graphics functions /////

// void lcd_clear_buf()
STATIC mp_obj_t dmcp_lcd_clear_buf() {
    lcd_clear_buf();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(dmcp_lcd_clear_buf_obj, dmcp_lcd_clear_buf);


// void lcd_refresh()
STATIC mp_obj_t dmcp_lcd_refresh() {
    lcd_refresh();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(dmcp_lcd_refresh_obj, dmcp_lcd_refresh);

// void lcd_refresh_dma()
STATIC mp_obj_t dmcp_lcd_refresh_dma() {
    lcd_refresh_dma();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(dmcp_lcd_refresh_dma_obj, dmcp_lcd_refresh_dma);

// void lcd_refresh_wait()
STATIC mp_obj_t dmcp_lcd_refresh_wait() {
    lcd_refresh_wait();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(dmcp_lcd_refresh_wait_obj, dmcp_lcd_refresh_wait);

// void lcd_forced_refresh()
STATIC mp_obj_t dmcp_lcd_forced_refresh() {
    lcd_forced_refresh();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(dmcp_lcd_forced_refresh_obj, dmcp_lcd_forced_refresh);

// void lcd_refresh_lines(int ln, int cnt)
STATIC mp_obj_t dmcp_lcd_refresh_lines(mp_obj_t ln_obj, mp_obj_t cnt_obj) {
    int ln = mp_obj_get_int(ln_obj);
    int cnt = mp_obj_get_int(cnt_obj);
    lcd_refresh_lines(ln, cnt);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(dmcp_lcd_refresh_lines_obj, dmcp_lcd_refresh_lines);


// This is the function which will be called from Python as example.add_ints(a, b).
STATIC mp_obj_t dmcp_add_ints(mp_obj_t a_obj, mp_obj_t b_obj) {
    // Extract the ints from the micropython input objects
    int a = mp_obj_get_int(a_obj);
    int b = mp_obj_get_int(b_obj);

    // Calculate the addition and convert to MicroPython object.
    return mp_obj_new_int(a + b);
}
// Define a Python reference to the function above
STATIC MP_DEFINE_CONST_FUN_OBJ_2(dmcp_add_ints_obj, dmcp_add_ints);


// Define all properties of the example module.
// Table entries are key/value pairs of the attribute name (a string)
// and the MicroPython object reference.
// All identifiers and strings are written as MP_QSTR_xxx and will be
// optimized to word-sized integers by the build system (interned strings).
STATIC const mp_rom_map_elem_t dmcp_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_dmcp) },
    { MP_ROM_QSTR(MP_QSTR_add_ints), MP_ROM_PTR(&dmcp_add_ints_obj) },
    { MP_ROM_QSTR(MP_QSTR_lcd_for_calc), MP_ROM_PTR(&dmcp_lcd_for_calc_obj) },
    { MP_ROM_QSTR(MP_QSTR_lcd_clear_buf), MP_ROM_PTR(&dmcp_lcd_clear_buf_obj) },
    { MP_ROM_QSTR(MP_QSTR_lcd_refresh), MP_ROM_PTR(&dmcp_lcd_refresh_obj) },
    { MP_ROM_QSTR(MP_QSTR_lcd_refresh_dma), MP_ROM_PTR(&dmcp_lcd_refresh_dma_obj) },
    { MP_ROM_QSTR(MP_QSTR_lcd_refresh_wait), MP_ROM_PTR(&dmcp_lcd_refresh_wait_obj) },
    { MP_ROM_QSTR(MP_QSTR_lcd_forced_refresh), MP_ROM_PTR(&dmcp_lcd_forced_refresh_obj) },
    { MP_ROM_QSTR(MP_QSTR_lcd_refresh_lines), MP_ROM_PTR(&dmcp_lcd_refresh_lines_obj) },
};
STATIC MP_DEFINE_CONST_DICT(dmcp_module_globals, dmcp_module_globals_table);

// Define module object.
const mp_obj_module_t module_dmcp = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&dmcp_module_globals,
};

// Register the module to make it available in Python
//MP_REGISTER_MODULE(MP_QSTR_example, example_user_cmodule, MODULE_EXAMPLE_ENABLED);

