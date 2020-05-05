#include "reader.h"
#include "py/mpthread.h"
#include "py/runtime.h"
#include "py/mperrno.h"
#include "dmcp.h"
#include <stdlib.h>

mp_lexer_t *mp_lexer_new_from_file(const char *filename) {
    mp_reader_t reader;
    mp_reader_new_file(&reader, filename);
    return mp_lexer_new(qstr_from_str(filename), reader);
}

typedef struct _mp_reader_dmcp_t {
    FIL f;
    size_t len;
    size_t pos;
    byte buf[20];
} mp_reader_dmcp_t;

void mp_reader_new_file(mp_reader_t *reader, const char *filename) {
    mp_reader_dmcp_t *rd = m_new_obj(mp_reader_dmcp_t);
    FRESULT fr;

    MP_THREAD_GIL_EXIT();
    fr = f_open(&(rd->f), filename, FA_READ);
    MP_THREAD_GIL_ENTER();
    if(fr) {
        mp_raise_OSError(errno); // TODO: Find out why this even compiles and if we can raise a more meaningful exception
        // does not return
    }

    fr = f_read(&(rd->f), &(rd->buf), sizeof(rd->buf), &(rd->len));
    if(fr) {
        mp_raise_OSError(errno); // TODO: Find out why this even compiles and if we can raise a more meaningful exception
        // does not return
    }

    reader->data = rd;
    reader->readbyte = mp_reader_dmcp_readbyte;
    reader->close = mp_reader_dmcp_close;
}

STATIC mp_uint_t mp_reader_dmcp_readbyte(void *data) {
    mp_reader_dmcp_t *rd = (mp_reader_dmcp_t *)data;
    FRESULT fr;

    if (rd->pos >= rd->len) {
        if (rd->len == 0) {
            return MP_READER_EOF;
        } else {
            MP_THREAD_GIL_EXIT();
            fr = f_read(&(rd->f), &(rd->buf), sizeof(rd->buf), &(rd->len));
            MP_THREAD_GIL_ENTER();
            if(fr) {
                mp_raise_OSError(errno); // TODO: Find out why this even compiles and if we can raise a more meaningful exception
                // does not return
            }
            if (rd->len <= 0) {
                rd->len = 0;
                return MP_READER_EOF;
            }
            rd->pos = 0;
        }
    }
    return rd->buf[rd->pos++];
}

STATIC void mp_reader_dmcp_close(void *data) {
    mp_reader_dmcp_t *rd = (mp_reader_dmcp_t *)data;
    MP_THREAD_GIL_EXIT();
    f_close(&(rd->f));
    MP_THREAD_GIL_ENTER();
    m_del_obj(mp_reader_dmcp_t, rd);
}
