// MicroPython module for my extensions to the DMCP API

#include "module_dmpy.h"

#include <dmcp.h>

#include "py/builtin.h"
#include "py/obj.h"
#include "py/objmodule.h"


///// Text helpers /////

// int lcd_for_calc(int what)
STATIC mp_obj_t dmpy_lcd_putsAt20(mp_obj_t line_obj, mp_obj_t str_obj) {
    int line = mp_obj_get_int(line_obj);
    const char* str = mp_obj_str_get_str(str_obj);
    lcd_putsAt(t20, line, str);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(dmpy_lcd_putsAt20_obj, dmpy_lcd_putsAt20);



//////////////////////////////////////////////////////////////////////////////

// Define all properties of the module.
// Table entries are key/value pairs of the attribute name (a string)
// and the MicroPython object reference.
// All identifiers and strings are written as MP_QSTR_xxx and will be
// optimized to word-sized integers by the build system (interned strings).
STATIC const mp_rom_map_elem_t dmpy_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_dmpy) },
    { MP_ROM_QSTR(MP_QSTR_lcd_putsAt20), MP_ROM_PTR(&dmpy_lcd_putsAt20_obj) },
};
STATIC MP_DEFINE_CONST_DICT(dmpy_module_globals, dmpy_module_globals_table);

// Define module object.
const mp_obj_module_t module_dmpy = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&dmpy_module_globals,
};

