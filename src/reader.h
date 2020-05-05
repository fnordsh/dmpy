#include "py/lexer.h"

mp_lexer_t *mp_lexer_new_from_file(const char *filename);

void mp_reader_new_file(mp_reader_t *reader, const char *filename);
STATIC mp_uint_t mp_reader_dmcp_readbyte(void *data);
STATIC void mp_reader_dmcp_close(void *data);
