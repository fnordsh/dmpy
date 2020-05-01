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

def keytest():
    dmcp.key_pop_all()
    while True:
        while dmcp.key_empty():
            pass
        key = dmcp.key_pop()
        dmpy.lcd_putsAt20(2, "Key: %d"%key)
        dmcp.lcd_refresh()
        if key==33: # EXIT
            break

dmcp.lcd_clear_buf()
keytest()

