import dmcp
import dmpy

def filetest():
    f = open("test.dat", "w")
    f.write("foo\n")
    f.close()

def count(n=100):
    for i in range(n):
        dmpy.lcd_putsAt20(2, "Count: %d"%i)
        dmcp.lcd_refresh()
    dmpy.lcd_putsAt20(3, "Done.")
    dmcp.lcd_refresh()


dmcp.lcd_clear_buf()

try:
    import test
except:
    pass

try:
    import OFFIMG
except:
    pass

dmcp.lcd_refresh()

