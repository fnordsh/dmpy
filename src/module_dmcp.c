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

// void bitblt24(uint32_t x, uint32_t dx, uint32_t y, uint32_t val, int blt_op, int fill)
STATIC mp_obj_t dmcp_bitblt24(size_t n_args, const mp_obj_t *args) {
    (void)n_args; // not used; always 6
    int x      = mp_obj_get_int(args[0]);
    int dx     = mp_obj_get_int(args[1]);
    int y      = mp_obj_get_int(args[2]);
    int val    = mp_obj_get_int(args[3]);
    int blt_op = mp_obj_get_int(args[4]);
    int fill   = mp_obj_get_int(args[5]);
    bitblt24(x, dx, y, val, blt_op, fill);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(dmcp_bitblt24_obj, 6, 6, dmcp_bitblt24);

// TODO: lcd_line_addr(int y)

// void lcd_fill_rect (uint32_t x, uint32_t y, uint32_t dx, uint32_t dy, int val)
STATIC mp_obj_t dmcp_lcd_fill_rect(size_t n_args, const mp_obj_t *args) {
    (void)n_args; // not used; always 5
    int x   = mp_obj_get_int(args[0]);
    int y   = mp_obj_get_int(args[1]);
    int dx  = mp_obj_get_int(args[2]);
    int dy  = mp_obj_get_int(args[3]);
    int val = mp_obj_get_int(args[4]);
    lcd_fill_rect(x, y, dx, dy, val);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(dmcp_lcd_fill_rect_obj, 5, 5, dmcp_lcd_fill_rect);

// void lcd_fillLine (int ln, uint8_t val)
STATIC mp_obj_t dmcp_lcd_fillLine(mp_obj_t ln_obj, mp_obj_t val_obj) {
    int ln = mp_obj_get_int(ln_obj);
    int val = mp_obj_get_int(val_obj);
    lcd_fillLine(ln, val);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(dmcp_lcd_fillLine_obj, dmcp_lcd_fillLine);

// void lcd_fillLines (int ln, uint8_t val, int cnt)
STATIC mp_obj_t dmcp_lcd_fillLines(mp_obj_t ln_obj, mp_obj_t val_obj, mp_obj_t cnt_obj) {
    int ln = mp_obj_get_int(ln_obj);
    int val = mp_obj_get_int(val_obj);
    int cnt = mp_obj_get_int(cnt_obj);
    lcd_fillLines(ln, val, cnt);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_3(dmcp_lcd_fillLines_obj, dmcp_lcd_fillLines);

// void lcd_set_buf_cleared (int val)
STATIC mp_obj_t dmcp_lcd_set_buf_cleared(mp_obj_t val_obj) {
    int val = mp_obj_get_int(val_obj);
    lcd_set_buf_cleared(val);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(dmcp_lcd_set_buf_cleared_obj, dmcp_lcd_set_buf_cleared);

// int lcd_get_buf_cleared ()
STATIC mp_obj_t dmcp_lcd_get_buf_cleared() {
    return mp_obj_new_int(lcd_get_buf_cleared());
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(dmcp_lcd_get_buf_cleared_obj, dmcp_lcd_get_buf_cleared);

// uint8_t reverse_byte(uint8_t x)
STATIC mp_obj_t dmcp_reverse_byte(mp_obj_t x_obj) {
    int x = mp_obj_get_int(x_obj);
    return mp_obj_new_int(reverse_byte(x));
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(dmcp_reverse_byte_obj, dmcp_reverse_byte);


///// LCD and draw functions / Text functions /////

STATIC mp_obj_t dmcp_lcd_putsAt20(mp_obj_t line_obj, mp_obj_t str_obj) {
    int line = mp_obj_get_int(line_obj);
    const char* str = mp_obj_str_get_str(str_obj);
    lcd_putsAt(t20, line, str);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(dmcp_lcd_putsAt20_obj, dmcp_lcd_putsAt20);


///// Keyboard / Basic functions /////

// int key_empty()
STATIC mp_obj_t dmcp_key_empty() {
    return key_empty() ? mp_const_true : mp_const_false;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(dmcp_key_empty_obj, dmcp_key_empty);

// int key_pop()
STATIC mp_obj_t dmcp_key_pop() {
    return mp_obj_new_int(key_pop());
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(dmcp_key_pop_obj, dmcp_key_pop);

// int key_pop_all()
STATIC mp_obj_t dmcp_key_pop_all() {
    key_pop_all();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(dmcp_key_pop_all_obj, dmcp_key_pop_all);

// int key_push(int k1)
STATIC mp_obj_t dmcp_key_push(mp_obj_t k1_obj) {
    int k1 = mp_obj_get_int(k1_obj);
    return key_push(k1) ? mp_const_true : mp_const_false;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(dmcp_key_push_obj, dmcp_key_push);

// int key_tail()
STATIC mp_obj_t dmcp_key_tail() {
    return mp_obj_new_int(key_tail());
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(dmcp_key_tail_obj, dmcp_key_tail);

// int key_to_nr(int key)
STATIC mp_obj_t dmcp_key_to_nr(mp_obj_t key_obj) {
    int key = mp_obj_get_int(key_obj);
    return mp_obj_new_int(key_to_nr(key));
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(dmcp_key_to_nr_obj, dmcp_key_to_nr);

// int sys_last_key()
STATIC mp_obj_t dmcp_sys_last_key() {
    return mp_obj_new_int(sys_last_key());
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(dmcp_sys_last_key_obj, dmcp_sys_last_key);

// void wait_for_key_press()
STATIC mp_obj_t dmcp_wait_for_key_press() {
    wait_for_key_press();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(dmcp_wait_for_key_press_obj, dmcp_wait_for_key_press);

// void wait_for_key_release(int tout)
STATIC mp_obj_t dmcp_wait_for_key_release(mp_obj_t tout_obj) {
    int tout = mp_obj_get_int(tout_obj);
    wait_for_key_release(tout);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(dmcp_wait_for_key_release_obj, dmcp_wait_for_key_release);


//////////////////////////////////////////////////////////////////////////////

// Define all properties of the example module.
// Table entries are key/value pairs of the attribute name (a string)
// and the MicroPython object reference.
// All identifiers and strings are written as MP_QSTR_xxx and will be
// optimized to word-sized integers by the build system (interned strings).
STATIC const mp_rom_map_elem_t dmcp_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_dmcp) },
    { MP_ROM_QSTR(MP_QSTR_lcd_for_calc), MP_ROM_PTR(&dmcp_lcd_for_calc_obj) },
    { MP_ROM_QSTR(MP_QSTR_lcd_clear_buf), MP_ROM_PTR(&dmcp_lcd_clear_buf_obj) },
    { MP_ROM_QSTR(MP_QSTR_lcd_refresh), MP_ROM_PTR(&dmcp_lcd_refresh_obj) },
    { MP_ROM_QSTR(MP_QSTR_lcd_refresh_dma), MP_ROM_PTR(&dmcp_lcd_refresh_dma_obj) },
    { MP_ROM_QSTR(MP_QSTR_lcd_refresh_wait), MP_ROM_PTR(&dmcp_lcd_refresh_wait_obj) },
    { MP_ROM_QSTR(MP_QSTR_lcd_forced_refresh), MP_ROM_PTR(&dmcp_lcd_forced_refresh_obj) },
    { MP_ROM_QSTR(MP_QSTR_lcd_refresh_lines), MP_ROM_PTR(&dmcp_lcd_refresh_lines_obj) },
    { MP_ROM_QSTR(MP_QSTR_bitblt24), MP_ROM_PTR(&dmcp_bitblt24_obj) },
    { MP_ROM_QSTR(MP_QSTR_lcd_fill_rect), MP_ROM_PTR(&dmcp_lcd_fill_rect_obj) },
    { MP_ROM_QSTR(MP_QSTR_lcd_fillLine), MP_ROM_PTR(&dmcp_lcd_fillLine_obj) },
    { MP_ROM_QSTR(MP_QSTR_lcd_fillLines), MP_ROM_PTR(&dmcp_lcd_fillLines_obj) },
    { MP_ROM_QSTR(MP_QSTR_lcd_set_buf_cleared), MP_ROM_PTR(&dmcp_lcd_set_buf_cleared_obj) },
    { MP_ROM_QSTR(MP_QSTR_lcd_get_buf_cleared), MP_ROM_PTR(&dmcp_lcd_get_buf_cleared_obj) },
    { MP_ROM_QSTR(MP_QSTR_reverse_byte), MP_ROM_PTR(&dmcp_reverse_byte_obj) },

    { MP_ROM_QSTR(MP_QSTR_lcd_putsAt20), MP_ROM_PTR(&dmcp_lcd_putsAt20_obj) },

    { MP_ROM_QSTR(MP_QSTR_key_empty), MP_ROM_PTR(&dmcp_key_empty_obj) },
    { MP_ROM_QSTR(MP_QSTR_key_pop), MP_ROM_PTR(&dmcp_key_pop_obj) },
    { MP_ROM_QSTR(MP_QSTR_key_pop_all), MP_ROM_PTR(&dmcp_key_pop_all_obj) },
    { MP_ROM_QSTR(MP_QSTR_key_push), MP_ROM_PTR(&dmcp_key_push_obj) },
    { MP_ROM_QSTR(MP_QSTR_key_tail), MP_ROM_PTR(&dmcp_key_tail_obj) },
    { MP_ROM_QSTR(MP_QSTR_key_to_nr), MP_ROM_PTR(&dmcp_key_to_nr_obj) },
    { MP_ROM_QSTR(MP_QSTR_sys_last_key), MP_ROM_PTR(&dmcp_sys_last_key_obj) },
    { MP_ROM_QSTR(MP_QSTR_wait_for_key_press), MP_ROM_PTR(&dmcp_wait_for_key_press_obj) },
    { MP_ROM_QSTR(MP_QSTR_wait_for_key_release), MP_ROM_PTR(&dmcp_wait_for_key_release_obj) },

};
STATIC MP_DEFINE_CONST_DICT(dmcp_module_globals, dmcp_module_globals_table);

// Define module object.
const mp_obj_module_t module_dmcp = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&dmcp_module_globals,
};
