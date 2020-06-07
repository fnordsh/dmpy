import dmcp

def filetest():
    f = open("test.dat", "w")
    f.write("foo\n")
    f.close()

def count(n=100):
    for i in range(n):
        dmcp.lcd_putsAt20(2, "Count: %d"%i)
        dmcp.lcd_refresh()
    dmcp.lcd_putsAt20(3, "Done.")
    dmcp.lcd_refresh()

def keytest():
    dmcp.key_pop_all()
    while True:
        while dmcp.key_empty():
            pass
        key = dmcp.key_pop()
        dmcp.lcd_putsAt20(2, "Key: %d"%key)
        dmcp.lcd_refresh()
        if key==33: # EXIT
            break

dmcp.lcd_clear_buf()

try:
    dmcp.lcd_putsAt20(2, "import foo:")
    import foo
    dmcp.lcd_putsAt20(3, "ok")
    dmcp.lcd_putsAt20(4, "%g"%(foo.bar(23)))

except:
    dmcp.lcd_putsAt20(3, "fail")
    pass

dmcp.lcd_refresh()
dmcp.wait_for_key_press()
dmcp.lcd_putsAt20(5, "%g"%(foo.bar(0)))
dmcp.lcd_refresh()

