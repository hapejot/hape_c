#include "platform.h"
#include "cnt_int.h"
#include <stdio.h>

/*---------------------------------------------------------------------------
 * NAME
 *      cnt_get_val - Liest einen Wert aus einem existierenden Container.
 *
 * SYNOPSIS
 */
INT
cnt_get_val
(
    IN      CNT         p_cnt,
    IN      char      * p_field,
    OUT     INT       * p_flg,
    OUT     INT       * p_size,
    OUT     void     ** p_val
)
/*
 * DESCRIPTION
 * 
 * PARAMETERS
 *      p_cnt   Container, aus dem gelesen wird.
 *      p_field Name des Feldes.
 *      p_flg   Rückgabe der Flags zu diesem Eintrag.
 *      p_len   Rückgabe der Länge von p_val. Bei C-Strings wird die Länge
 *              inclusive des NUL Zeichens am ende angegeben.
 *      p_val   Rückgabe eines Zeigers auf den gespeicherten Wert.
 *              Der Wert wird nicht kopiert, sondern der Speicherbereich aus dem
 *              Container wird zurückgegeben.
 * 
 * RETURN VALUES
 *
 * ERRORS
 *      [RC_OK]             Successful completion.
 *      RC_CNT_NOT_FOUND    Element ist nicht vorhanden.
 *
 *---------------------------------------------------------------------------*/
{
    INT status = RC_OK;
    /* end of local var. */

    /* parameter check */
    
    /* environment check */

    /* processing. */
    if ( status == RC_OK )
    {
        char    name[1024];
        char    rest[1024];
        INT     row     = 0;

        assert( strlen(p_field) < sizeof(name) );
        status = cnt_split_name( p_field, name, &row, rest );
        if( name[0] != 0 )
        {
            INT     flg     = 0;
            INT     size    = 0;
            void  * val     = NULL;
            void  * val1    = NULL;

            status = cnt_get_idx_val(p_cnt, row, name, &flg, &size, &val);
            if( RC_OK == status )
            {
                if( rest[0] != 0 )
                {
                    CNT         cnt = (CNT)val;

                    assert( CNT_ISCONT(flg) );
                    status = cnt_get_val(cnt, rest, &flg, &size, &val1);
                    *p_val = val1;
                }
                else
                    *p_val = val;

                if( p_flg )  *p_flg  = flg;
                if( p_size ) *p_size = size;
            }
        }
        else
        {
            CNT     cnt = NULL;

            status = cnt_create( p_cnt->arena, &cnt );
            status = cnt_copy_row( p_cnt, row, 0, cnt );
            if( status == RC_OK )
            {
                *p_val = cnt;
            }
            else
            {
                // cnt_destroy( cnt );
                assert( 0 );
            }
            assert( rest[0] == 0 );
        }
    }

    /* return */
    return status;
}

