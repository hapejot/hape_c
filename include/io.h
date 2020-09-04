/*
 * $id$
 */
#ifndef _IO_H
#define _IO_H
#include "platform.h"

#define RC_IO_MIN           0x400
#define RC_IO_NOSUCHFILE    RC_IO_MIN + 1
#define RC_IO_NOPERM        RC_IO_MIN + 2
#define RC_IO_NOBUFSPACE    RC_IO_MIN + 3       /* Nicht genug Platz im Puffer für die Daten.  */
#define RC_IO_EOF           RC_IO_MIN + 4

typedef struct IO_FILE *IO_FILE;

/*
 * BUFFER - gibt einen bereitgestellten Speicherbereich an, der
 * gefüllt werden kann. Die aktuelle Befüllung wird durch das
 * Attribut 'len' gekennzeichnet.
 *
 * es gilt immer 'len' <= 'size'.
 */

typedef struct IO_BUFFER *IO_BUFFER;

struct IO_BUFFER {
    UINT len;
    UINT8 *pch;
    UINT size;
    IO_BUFFER next;
    IO_BUFFER prev;
};

extern INT io_connect_file( INOUT IO_FILE *, IN char *, IN char * );
extern INT io_connect_buffered( INOUT IO_FILE *, IN IO_FILE );
extern INT io_connect_fd( INOUT IO_FILE *, INT );
extern INT io_connect_buf( INOUT IO_FILE *, IN IO_BUFFER );

extern INT io_close( IO_FILE * );
extern INT io_seek( IO_FILE, UINT64 );
extern INT io_read( IO_FILE, void *, INT, INT * );
extern INT io_write( IO_FILE, void *, INT, INT * );
extern INT io_flush( IO_FILE );
extern INT io_getc( IO_FILE );
extern INT io_ungetc( IO_FILE, INT );
extern INT io_putc( IO_FILE, INT );
extern INT io_puts( IO_FILE out, char* line);
extern INT io_attr_get( IO_FILE, char *, char *, INT );
extern INT io_gets( IO_FILE, INT8 *, INT, INT * );

extern INT io_buf_new( IO_BUFFER * );
extern INT io_buf_expand( IO_BUFFER, INT );
extern INT io_buf_add( IO_BUFFER, char *, INT );
extern INT io_buf_free( IO_BUFFER * );
extern INT io_buf_free_all(  );

#endif
