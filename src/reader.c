#include "reader.h"
#include "py/mpthread.h"
#include "py/runtime.h"
#include "py/mperrno.h"
#include "dmcp.h"
#include <stdlib.h>

mp_lexer_t *mp_lexer_new_from_file(const char *filename) {
    mp_reader_t reader;
    // mp_reader_new_file(&reader, filename);

    // use a mem reader for testing
    FIL f;
    MP_THREAD_GIL_EXIT();
    FRESULT fr = f_open(&f, filename, FA_READ);
    MP_THREAD_GIL_ENTER();
    if(fr) {
        mp_raise_OSError(errno);
    } else {
        FSIZE_t fsize = f_size(&f);
        byte *filebuf = malloc(fsize+1);
        if(filebuf==0) {
            mp_raise_OSError(errno);
        } else {
            unsigned int read_bytes;
            f_read(&f, filebuf, fsize, &read_bytes);
            f_close(&f);
            filebuf[fsize]=0;
            mp_reader_new_mem(&reader, filebuf, fsize, fsize+1);
        }
    }

    return mp_lexer_new(qstr_from_str(filename), reader);
}

/*
void mp_reader_new_file(mp_reader_t *reader, const char *filename) {
    lcd_putsRAt(t20, 7, "mp_reader_new_file:");
    lcd_putsRAt(t20, 8, filename);

    MP_THREAD_GIL_EXIT();
    FIL *fp;
    FRESULT fr = f_open(fp, filename, FA_READ);
    MP_THREAD_GIL_ENTER();
    if (fr != FR_OK) {
        mp_raise_OSError(errno);
    }
    //mp_reader_new_file_from_fd(reader, fd, true);
    reader=0;
}

void mp_reader_new_file_from_fd(mp_reader_t *reader, int fd, bool close_fd) {
    mp_reader_posix_t *rp = m_new_obj(mp_reader_posix_t);
    rp->close_fd = close_fd;
    rp->fd = fd;
    MP_THREAD_GIL_EXIT();
    int n = read(rp->fd, rp->buf, sizeof(rp->buf));
    if (n == -1) {
        if (close_fd) {
            close(fd);
        }
        MP_THREAD_GIL_ENTER();
        mp_raise_OSError(errno);
    }
    MP_THREAD_GIL_ENTER();
    rp->len = n;
    rp->pos = 0;
    reader->data = rp;
    reader->readbyte = mp_reader_posix_readbyte;
    reader->close = mp_reader_posix_close;
}
*/
