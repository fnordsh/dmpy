import dmcp
import dmpy

dmcp.lcd_clear_buf()

#dmpy.lcd_putsAt20(1, "Heap free: %d, alloc: %d  "%(gc.mem_free(), gc.mem_alloc()))
dmcp.lcd_refresh()

for i in range(100):
    dmpy.lcd_putsAt20(2, "Count: %d"%i)
    dmcp.lcd_refresh()


#dmpy.lcd_putsAt20(3, "Heap free: %d, alloc: %d  "%(gc.mem_free(), gc.mem_alloc()))
dmcp.lcd_refresh()
