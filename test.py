import dmcp
import dmpy

dmcp.lcd_clear_buf()
dmcp.lcd_fill_rect(100,100,100,100,1)
dmcp.lcd_refresh()

dmpy.lcd_putsAt20(2, "Hallo")
dmcp.lcd_refresh()

