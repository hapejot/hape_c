#include <stdio.h>
#include <check.h>
#include <stdbool.h>
#include <hape/cnt.h>
#include <hape/err.h>
#include <bfd.h>
#include <malloc.h>

#include "../cnt/cnt_int.h"

/* cnt.c */
CNT_CELL cell_create(MEM_ARENA, CNT_IDX col, CNT_IDX row);
bool gen_find( CNT_VECTOR_PAGE p, CMP f, void *elem, CNT_IDX * idx ) ;

int col_compare(void *a, void *b);
CNT_COL col_create(MEM_ARENA a, int pos, char* name);
CNT_IDX col_find(CNT_VECTOR_PAGE,char*);

CNT_IDX cnt_columns(CNT cnt);
CNT_IDX cnt_lines(CNT cnt);
CNT_COL_NAME cnt_colname(CNT cnt, CNT_IDX col);
CNT_DATA *data_cpy(MEM_ARENA a, CNT_DATA *data);
CNT_COL cnt_column(CNT cnt, char *name);
void cnt_set_val_b(CNT cnt, CNT_COL_NAME name, CNT_BYTES data);
void cnt_set_idx_b(CNT cnt, CNT_COL_NAME name, CNT_IDX row, CNT_BYTES data);
CNT_IDX cell_pos(CNT cnt, CNT_IDX row, CNT_IDX col);
int pair_compare(CNT_IDX x1, CNT_IDX x2, CNT_IDX y1, CNT_IDX y2);
CNT_IDX cell_find(CNT_VECTOR_PAGE page, CNT_IDX row, CNT_IDX col);
CNT_BYTES cnt_idx_b(CNT cnt, CNT_COL_NAME name, CNT_IDX row);
void cell_page_insert(CNT_VECTOR_PAGE p, CNT_IDX idx, CNT_CELL cell);
void cnt_set_idx_d(CNT cnt, CNT_COL_NAME name, CNT_IDX row, CNT_DATA *data);
void cnt_set_val_d(CNT cnt, CNT_COL_NAME name, CNT_DATA *data);
void cnt_dump(CNT cnt);
void cnt_json(CNT cnt);
CNT_BYTES cnt_val_b(CNT cnt, char *name);
CNT cnt_create(void);
CNT_VECTOR_PAGE page_create(MEM_ARENA arena);
CNT cnt_create_a(MEM_ARENA p_arena);

int lv1_compare(void *a, void *b);
int lv21_compare(void *a, void *b);
int lv2_compare(void *a, void *b);
int lv12_compare(void *a, void *b);

bool gen_assign(CNT_VECTOR_PAGE p, CMP f, void *elem, CNT_IDX *idx);
bool cell_page_split( CNT_VECTOR_PAGE * p_new,
                      MEM_ARENA arena, CNT_VECTOR_PAGE p0 ) ;

CNT_IDX col_assigned(MEM_ARENA, CNT_VECTOR_PAGE p, char* name);


MEM_ARENA a;

START_TEST( _cnt_page_create )
{
    CNT_VECTOR_PAGE p = page_create(a);

    ck_assert_ptr_ne(p, NULL);
}
END_TEST 

START_TEST( _col_assigned_new ){
    CNT_VECTOR_PAGE p = page_create(a);
    p->type = COLLS;
    ck_assert_int_eq(p->used, 0);
    CNT_IDX i1 = col_assigned(a, p, "COL1");
    ck_assert_int_eq(i1, 1);
    ck_assert_int_eq(p->used, 1);
}
END_TEST

START_TEST( _col_assigned_exists ){
    CNT_VECTOR_PAGE p = page_create(a);
    p->type = COLLS;
    ck_assert_int_eq(p->used, 0);
    CNT_IDX i1 = col_assigned(a, p, "COL1");
    ck_assert_int_eq(i1, 1);
    ck_assert_int_eq(p->used, 1);

    CNT_IDX i2 = col_assigned(a, p, "COL0");
    ck_assert_int_eq(i2, 2);
    ck_assert_int_eq(p->used, 2);

    CNT_IDX i3 = col_assigned(a, p, "COL2");
    ck_assert_int_eq(i3, 3);
    ck_assert_int_eq(p->used, 3);

    CNT_IDX i4 = col_assigned(a, p, "COL1");
    ck_assert_int_eq(i4, 1);
    ck_assert_int_eq(p->used, 3);

}
END_TEST

START_TEST( _col_find_exists ){
    CNT_VECTOR_PAGE p = page_create(a);
    p->type = COLLS;
    ck_assert_int_eq(p->used, 0);
    CNT_IDX i1 = col_assigned(a, p, "COL1");
    ck_assert_int_eq(i1, 1);
    ck_assert_int_eq(p->used, 1);

    ck_assert_int_eq(col_find(p, "COL1"), 1);
}
END_TEST

START_TEST( _col_find_not_exists ){
    CNT_VECTOR_PAGE p = page_create(a);
    p->type = COLLS;
    ck_assert_int_eq(p->used, 0);
    CNT_IDX i1 = col_assigned(a, p, "COL1");
    ck_assert_int_eq(i1, 1);
    ck_assert_int_eq(p->used, 1);

    ck_assert_int_eq(col_find(p, "COL0"), 0);
}
END_TEST

START_TEST( _cnt_page_col_fnd )
{
    CNT_VECTOR_PAGE p = page_create(a);
    p->type = COLLS;
    CNT_COL c = col_create(a, 1, "id");
    CNT_COL c2 = col_create(a, 2, "name");
    CNT_COL c3 = col_create(a, 3, "value");
    CNT_IDX idx = -1;
    // assign column 2 first
    bool r = gen_assign(p, col_compare, c2, &idx);
    ck_assert(r);
    ck_assert_int_eq(idx, 0);

    // find column 1 next
    r = gen_find(p, col_compare, c, &idx);
    ck_assert(!r);
    ck_assert_int_eq(idx, 0); 

    // find column 3 next
    r = gen_find(p, col_compare, c3, &idx);
    ck_assert(!r);
    ck_assert_int_eq(idx, 1); 


    r = gen_assign(p, col_compare, c, &idx);
    ck_assert(r);
    ck_assert(idx == 0);
    ck_assert_int_eq(2, p->used);
    ck_assert_ptr_ne(NULL, p->ptr[1]);

    r = gen_find(p, col_compare, c, &idx);
    ck_assert(r);
    ck_assert_int_eq(idx, 0); 
    ck_assert_ptr_ne(NULL, p->ptr[1]);

    r = gen_find(p, col_compare, c2, &idx);
    ck_assert(r);
    ck_assert_int_eq(idx, 1); 
    ck_assert_ptr_ne(NULL, p->ptr[1]);
}
END_TEST

START_TEST( _cnt_page_lv1_fnd )
{
    CNT_VECTOR_PAGE p = page_create(a);
    p->type = CELLS_LEVEL_1;
    CNT_CELL c  = cell_create(a, 0, 0 );
    CNT_CELL c2 = cell_create(a, 1, 0 );
    CNT_CELL c3 = cell_create(a, 2, 0 );
    CNT_CELL c4 = cell_create(a, 1, 1 );
    CNT_CELL c5 = cell_create(a, 2, 1 );


    ck_assert_int_eq(lv1_compare(c, c2), -1); 
    ck_assert_int_eq(lv1_compare(c, c3), -2); 
    ck_assert_int_eq(lv1_compare(c, c4), -1); 
    ck_assert_int_eq(lv1_compare(c, c5), -1); 

    ck_assert_int_eq(lv1_compare(c2, c3), -1); 
    ck_assert_int_eq(lv1_compare(c2, c4), -1); 
    ck_assert_int_eq(lv1_compare(c2, c5), -1); 

    ck_assert_int_eq(lv1_compare(c3, c4), -1); 
    ck_assert_int_eq(lv1_compare(c3, c5), -1); 

    ck_assert_int_eq(lv1_compare(c4, c5), -1); 


    ck_assert_int_eq(lv1_compare(c2, c), 1); 
    ck_assert_int_eq(lv1_compare(c3, c), 2); 
    ck_assert_int_eq(lv1_compare(c4, c), 1); 
    ck_assert_int_eq(lv1_compare(c5, c), 1); 

    ck_assert_int_eq(lv1_compare(c3, c2), 1); 
    ck_assert_int_eq(lv1_compare(c4, c2), 1); 
    ck_assert_int_eq(lv1_compare(c5, c2), 1); 

    ck_assert_int_eq(lv1_compare(c4, c3), 1); 
    ck_assert_int_eq(lv1_compare(c5, c3), 1); 

    ck_assert_int_eq(lv1_compare(c5, c4), 1); 

    CNT_IDX idx = -1;
    bool r = gen_assign(p, lv1_compare, c2, &idx);
    ck_assert(r);
    ck_assert_int_eq(idx, 0); 

    r = gen_assign(p, lv1_compare, c, &idx);
    ck_assert(r);
    ck_assert_int_eq(idx, 0); 

    r = gen_assign(p, lv1_compare, c3, &idx);
    ck_assert(r);
    ck_assert_int_eq(idx, 2); 

    r = gen_find(p, lv1_compare, c2, &idx);
    ck_assert(r);
    ck_assert_int_eq(idx, 1); 
}
END_TEST




START_TEST( _cnt_page_lv1_asn )
{
    CNT_VECTOR_PAGE p = page_create(a);
    p->type = CELLS_LEVEL_1;
    CNT_CELL c = cell_create(a, 0, 0 );
    CNT_CELL c2 = cell_create(a, 0, 1 );;
    CNT_IDX idx = -1;
    bool r = gen_assign(p, lv1_compare, c2, &idx);
    ck_assert(r);
    ck_assert_int_eq(idx, 0); 

    r = gen_assign(p, lv1_compare, c, &idx);
    ck_assert(r);
    ck_assert_int_eq(idx, 0); 

    ck_assert_int_eq(p->used, 2);
}
END_TEST


START_TEST( _cnt_page_column )
{
    char* nm = "col1";
    CNT cnt = cnt_create_a(a);
    ck_assert_ptr_ne(cnt, NULL);
    CNT_COL col = cnt_column(cnt, nm);

    ck_assert_ptr_ne(col, NULL);
    ck_assert_int_eq(col->pos, 0);
    ck_assert_str_eq(nm, col->name);

    ck_assert_int_eq(cnt->cols->used, 1);
}
END_TEST


START_TEST( _cnt_set_value )
{
    CNT cnt = cnt_create_a(a);
    TRY
        cnt_set_col_idx_b(cnt, 1, 1, "Cell-1-1");
        cnt_set_col_idx_b(cnt, 1, 2, "Cell-1-2");
    CATCH(ex)
        fprintf(stderr, "error found %s\n", ex->msg);
    END_TRY
    ck_assert_int_eq(cnt->cells->used, 2);
}
END_TEST

CNT  cnt_set_tst(int max_col, int max_row ){
    CNT cnt = cnt_create_a(a);
    TRY
        for( int i = 1; i <= max_row; i++){
            CNT_DATA * y;
            const CNT_DATA * x;
            for( int j = 1; j <= max_col; j++) {
                y = cnt_create_data(cnt, "cell %d %d", j, i );
                cnt_set_col_idx_d(cnt, j, i, y);
                x = cnt_col_idx_d(cnt, j, i);
                if(x == NULL) break;
            }
            if(x == NULL) break;
        }
    CATCH(ex)
        fprintf(stderr, "error found %s\n", ex->msg);
    END_TRY
    ck_assert_int_eq(cnt->used, max_row * max_col);
    return cnt;
}

CNT  cnt_set_tst_rev(int max_col, int max_row ){
    CNT cnt = cnt_create_a(a);
    TRY
        for( int i = max_row; i > 0; i--){
            CNT_DATA * y;
            const CNT_DATA * x;
            for( int j = max_col; j > 0; j--) {
                y = cnt_create_data(cnt, "cell %d %d", j, i );
                cnt_set_col_idx_d(cnt, i, j, y);
                x = cnt_col_idx_d(cnt, i, j);
                if(x == NULL) break;
            }
            if(x == NULL) break;
        }
    CATCH(ex)
        fprintf(stderr, "error found %s\n", ex->msg);
    END_TRY
    ck_assert_int_eq(cnt->used, max_row * max_col);
    return cnt;
}

START_TEST( _cnt_set_val01 ) { cnt_set_tst( 5, 50); } END_TEST
START_TEST( _cnt_set_val02 ) { cnt_set_tst( 5, 51); } END_TEST

START_TEST( _cnt_set_val03 ) { 
    TRY
        extern CNT_IDX last_index;
        CNT_DATA d;
        d.d = "<new>";
        d.l = strlen(d.d);
        CNT cnt = cnt_set_tst( 8, 32); 
        //ck_assert_int_eq(256, cnt->cells->used); 
        cnt_set_col_idx_d(cnt, 9, 15, &d);
        //ck_assert_int_eq(2, cnt->cells->used); 
        ck_assert_int_eq(257, cnt->used);
        //ck_assert_int_eq(cnt->cells->type, CELLS_LEVEL_2);
        //ck_assert_int_eq(0x100, VECTOR_PAGE_MAX);
        //ck_assert_int_eq(0x100, last_index); 
        ck_assert_ptr_ne(0, cnt_col_idx_d(cnt, 9, 15));
    CATCH(ex)
        ck_abort_msg("exception during proc: %s", ex->msg);
    END_TRY

} END_TEST

START_TEST( _cnt_set_val04 ) { 
    TRY
        extern CNT_IDX last_index;
        CNT_DATA d;
        d.d = "<new>";
        d.l = strlen(d.d);
        CNT cnt = cnt_set_tst( 8, 48); 
        ck_assert_int_eq(8 * 48, cnt->used); 
        cnt_set_col_idx_d(cnt, 9, 15, &d);
        // ck_assert_int_eq(3, cnt->cells->used); 
        // ck_assert_int_eq(cnt->cells->type, CELLS_LEVEL_2);
        //ck_assert_int_eq(0x100, VECTOR_PAGE_MAX);
        // ck_assert_int_eq(0x100, last_index); 
        ck_assert_ptr_ne(0, cnt_col_idx_d(cnt, 9, 15));
    CATCH(ex)
        ck_abort_msg("Exception %s %s", ex->msg, ex->arg[0]);
    END_TRY
} END_TEST

START_TEST( _cnt_set_val05 ) { 
    CNT cnt = cnt_set_tst( 9, 54); 
    ck_assert_int_gt(cnt->cells->used, 2);
    const CNT_DATA * x = cnt_col_idx_d(cnt, 3, 20);
    ck_assert_str_eq(x->d, "cell 3 20");
    x = cnt_col_idx_d(cnt, 9, 54);
    ck_assert_str_eq(x->d, "cell 9 54");
    x = cnt_col_idx_d(cnt, 9, 55);
    ck_assert_ptr_eq(x, 0);
    x = cnt_col_idx_d(cnt, 10, 53);
    ck_assert_ptr_eq(x, 0);
} END_TEST
START_TEST( _cnt_set_val06 ) { cnt_set_tst(10, 55); } END_TEST
START_TEST( _cnt_set_val07 ) { cnt_set_tst(11, 56); } END_TEST
START_TEST( _cnt_set_val08 ) { 
    CNT cnt = cnt_set_tst(12, 57);
    ck_assert_int_eq(cnt->used, 684);
} END_TEST
START_TEST( _cnt_set_val08r ) { 
    //CNT cnt = cnt_set_tst_rev(12, 57);
    // ck_assert_int_eq(cnt->cells->used, 4);
    // cnt_json(cnt);
} END_TEST
START_TEST( _cnt_set_val09 ) { cnt_set_tst(13, 58); } END_TEST
START_TEST( _cnt_set_val10r) { cnt_set_tst_rev(123, 343); } END_TEST
START_TEST( _cnt_set_val10 ) { cnt_set_tst(123, 267); } END_TEST

void print_page(CNT_VECTOR_PAGE p){
    fprintf(stderr, "---\n");
    for(CNT_IDX i = 0; i < p->used; i++){
        CNT_CELL c = p->ptr[i];
        fprintf(stderr, "%d %d %d\n", i, c->row, c->col);
    }
}

CNT_VECTOR_PAGE  test_page_create(){
    CNT_VECTOR_PAGE p = page_create(a);
    p->type = CELLS_LEVEL_1;
    bool r;
    CNT_CELL c;
    CNT_IDX idx = -1;

    for(int i = 1; i <= VECTOR_PAGE_MAX; i++){
        c = cell_create(a, 3, 10 * i );
        r = gen_assign(p, lv1_compare, c, &idx);
        ck_assert(r);
        ck_assert_int_eq(i-1, idx);
        c = p->ptr[idx];
        ck_assert_int_eq(10 * i, c->row);
        c = p->ptr[0];
        ck_assert_int_eq(10, c->row);
    }
    return p;
}


START_TEST( _page_split ) {
    bool rc;
    CNT_IDX idx = -1;
    CNT_VECTOR_PAGE p = test_page_create();
    ck_assert_int_eq( p->used, VECTOR_PAGE_MAX );
    CNT_VECTOR_PAGE p_new;
    rc = cell_page_split( &p_new, a, p );
    ck_assert_ptr_ne(p_new, NULL);
    ck_assert_ptr_ne(p_new->ptr[0], NULL);
    ck_assert_ptr_ne(p->ptr[0], NULL);
    ck_assert_int_eq(p->used, VECTOR_PAGE_MAX / 2);
    ck_assert_int_eq(p_new->used, VECTOR_PAGE_MAX / 2);
    CNT_CELL cx;
    for(int i = 1; i < 128; i++){
        cx = p_new->ptr[i];
        ck_assert_int_eq(3, cx->col);
        ck_assert_int_eq((i+1 + (VECTOR_PAGE_MAX/2))*10, cx->row);
        cx = p->ptr[i];
        ck_assert_int_eq(3, cx->col);
        ck_assert_int_eq((i+1)*10, cx->row);
    }

        
    CNT_VECTOR_PAGE p_top = page_create( a );
    ck_assert_ptr_ne(p_top, 0);
    p_top->type = CELLS_LEVEL_2;
    rc = gen_assign( p_top, lv2_compare, p, &idx );
    ck_assert_ptr_ne(p_top->ptr[0], NULL);
        
    rc = gen_assign( p_top, lv2_compare, p_new, &idx );
    ck_assert_ptr_ne(p_top->ptr[1], NULL);
    ck_assert_ptr_eq(p_top->ptr[2], NULL);
    ck_assert_int_eq(p_top->used, 2);

    CNT_CELL c0 = ((CNT_VECTOR_PAGE)p_top->ptr[0])->ptr[0];
    ck_assert_int_eq(10, c0->row);
        
    // before start of page
    CNT_CELL c = cell_create(a, 1, 11 );
    ck_assert_int_lt(lv21_compare(p_top->ptr[0], c), 0);
    c = cell_create(a, 4, 10 );
    ck_assert_int_lt(lv21_compare(p_top->ptr[0], c), 0);

    // after start of page
    c = cell_create(a, 4, 1 );
    ck_assert_int_gt(lv21_compare(p_top->ptr[0], c), 0);
    c = cell_create(a, 2, 10 );
    ck_assert_int_gt(lv21_compare(p_top->ptr[0], c), 0);
        
    ck_assert_ptr_ne(p_top->ptr[0], NULL);
    ck_assert_ptr_ne(p_top->ptr[1], NULL);

    c = cell_create(a, 1, 1 );  // before first page
    rc = gen_find( p_top, lv12_compare, c, &idx );
    ck_assert(!rc); // cell is not in page
    ck_assert_int_eq(idx, 0);

    c = cell_create(a, 1, 20 ); // in first page
    rc = gen_find( p_top, lv12_compare, c, &idx );
    ck_assert(!rc); // cell is not in page
    ck_assert_int_eq(idx, 1);

    // c = p_new->ptr[0];
    // fprintf(stderr, "*** first in seconds page: %d %d\n", c->col, c->row);

    c = cell_create(a, 3, 10 * (1+ VECTOR_PAGE_MAX/2) ); // first in snd page
    rc = gen_find( p_top, lv12_compare, c, &idx );
    ck_assert(!rc); // cell is in page but still not detectable
    ck_assert_int_eq(idx, 2);


    c = cell_create(a, 1, 40 + (10 * VECTOR_PAGE_MAX / 2) ); // in snd page
    rc = gen_find( p_top, lv12_compare, c, &idx );
    ck_assert(!rc); // cell is not in page
    ck_assert_int_eq(idx, 2);
}
END_TEST


START_TEST( _bfd_open ) {
    bfd_init();
    bfd * abfd = bfd_openr( "cnt/libcnt.so", NULL);
    ck_assert_ptr_ne(abfd, NULL);
    ck_assert( bfd_check_format( abfd, bfd_object) );
    ck_assert_str_eq(bfd_get_target(abfd) , "elf64-x86-64");
    int storage_needed = bfd_get_symtab_upper_bound(abfd);
    ck_assert_int_gt(storage_needed, 0);
    asymbol ** syms = malloc(storage_needed);
    ck_assert_ptr_ne(syms, 0);
    ck_assert(bfd_canonicalize_symtab(abfd, syms));
    asection * sect = bfd_get_section_by_name(abfd, ".text");
    ck_assert_ptr_ne(sect, 0);

    int n = storage_needed / sizeof(*syms);
    ck_assert_int_gt(n, 2);
    ck_assert_ptr_eq(syms[n-1], 0);
    ck_assert_ptr_ne(syms[n-2], 0);

    unsigned lineno;
    const char* filename; 
    const char* functionname;
    long address = 0x01a93;

    ck_assert_int_ge( sect->vma, 9000);
    ck_assert_int_ge( sect->size, 15000);

    ck_assert(bfd_find_nearest_line (   abfd, sect, syms,
                                        address - sect->vma, &filename,
                                        &functionname, &lineno));
    ck_assert_str_eq(filename, "/home/peter/shared-work/hape_c/cnt/cnt.c");
    ck_assert_str_eq(functionname, "check_page");
    ck_assert_int_ge(lineno, 155);
}
END_TEST

START_TEST( _set_record_value){
    CNT cnt = cnt_create();
    cnt_set_val_b(cnt, "name", "Jaeckel");
    ck_assert_ptr_ne(cnt, 0);
    cnt_set_val_b(cnt, "firstname", "Peter");
    ck_assert_ptr_ne(cnt, 0);

    ck_assert_str_eq(cnt_val_b(cnt, "name"), "Jaeckel");
    ck_assert_ptr_ne(cnt, 0);
}
START_TEST(_cnt_columns_all ){
    CNT cnt = cnt_create(); 
    cnt_set_val_b(cnt, "name", "Revonnah");
    cnt_set_val_b(cnt, "firstname", "Peter");
    CNT_COL cols[3];
    cnt_columns_all(cnt, cols, 2);
    ck_assert_str_eq(cols[1]->name, "name");
    ck_assert_str_eq(cols[2]->name, "firstname");
    cnt_json(cnt);
}
END_TEST
START_TEST(_cnt_colname ){
    CNT cnt = cnt_create(); 
    cnt_set_val_b(cnt, "name", "Revonnah");
    cnt_set_val_b(cnt, "firstname", "Peter");
    ck_assert_str_eq(cnt_colname(cnt, 1), "name");
    ck_assert_str_eq(cnt_colname(cnt, 2), "firstname");
}
END_TEST
START_TEST(_cnt_set_val_b ) {
    CNT cnt = cnt_create(); // Negahnegnal
                            // Ledewgrub
                            // Kramedew 
    cnt_set_val_b(cnt, "name", "Revonnah");
    cnt_set_val_b(cnt, "firstname", "Peter");
    ck_assert_str_eq(cnt_val_b(cnt, "name"), "Revonnah");
    ck_assert_str_eq(cnt_val_b(cnt, "firstname"), "Peter");
}
END_TEST
START_TEST(_cnt_set_val_d ) {
}
END_TEST
START_TEST(_cnt_set_col_val_d ) { } END_TEST
START_TEST(_cnt_set_col_val_b ) { } END_TEST

START_TEST(_cnt_set_idx_val_d ){ } END_TEST
START_TEST(_cnt_set_idx_val_b ){
    CNT cnt = cnt_create(); // Negahnegnal
    cnt_set_idx_val_b(cnt, 0, "ARGV0");
    cnt_set_idx_val_b(cnt, 1, "ARGV1");
    cnt_set_idx_val_b(cnt, 3, "ARGV3");
    cnt_set_idx_val_b(cnt, 4, "ARGV4");

    ck_assert_str_eq(cnt_idx_val_b(cnt, 0), "ARGV0");
    ck_assert_str_eq(cnt_idx_val_b(cnt, 1), "ARGV1");
    ck_assert_str_eq(cnt_idx_val_b(cnt, 3), "ARGV3");
    ck_assert_str_eq(cnt_idx_val_b(cnt, 4), "ARGV4");

    cnt_json(cnt);
}
END_TEST
START_TEST(_cnt_set_idx_d ){
}
END_TEST

START_TEST(_cnt_set_idx_b ){
    CNT cnt = cnt_create(); // Negahnegnal
                            // Ledewgrub
                            // Kramedew 
    cnt_set_idx_b(cnt, "name", 1, "Revonnah");
    cnt_set_idx_b(cnt, "firstname", 1, "Peter");
    ck_assert_ptr_ne(cnt, 0);
    ck_assert_str_eq(cnt_idx_b(cnt, "name", 1), "Revonnah");
    ck_assert_str_eq(cnt_idx_b(cnt, "firstname", 1), "Peter");
    cnt_json(cnt);
}
END_TEST


END_TEST
START_TEST(_cnt_col_idx_b ){
}
END_TEST

START_TEST(_cnt_count ){
    CNT cnt = cnt_create(); // Negahnegnal
                            // Ledewgrub
                            // Kramedew 
    cnt_set_idx_b(cnt, "name", 1, "Revonnah");
    cnt_set_idx_b(cnt, "firstname", 1, "Peter");
    ck_assert_ptr_nonnull(cnt);
    ck_assert_int_eq(cnt_columns(cnt), 2);
    ck_assert_int_eq(cnt_lines(cnt), 1);
}
END_TEST

////////////////////////////////////////////////////////////////////////////////
//
// SETUP
//
void setup(  )
{
    a = mem_arena_new(  );
}

//
// TEARDOWN
//
void teardown(  )
{
    mem_arena_free( a );
    mem_arena_dispose( &a );
}

//
//  Suite
//
TCase *test_cnt_page(  )
{
    TCase *tcase = tcase_create( "cnt/page" );

    tcase_add_checked_fixture( tcase, setup, teardown );

    tcase_add_test( tcase, _cnt_set_idx_val_b );
    tcase_add_test( tcase, _cnt_set_idx_val_d );
    tcase_add_test( tcase, _cnt_columns_all  );
    tcase_add_test( tcase, _cnt_page_create  );
    tcase_add_test( tcase, _cnt_page_col_fnd  );
    tcase_add_test( tcase, _cnt_page_column  );
    tcase_add_test( tcase, _cnt_set_value  );
    tcase_add_test( tcase, _cnt_set_val01  );
    tcase_add_test( tcase, _cnt_set_val02  );
    tcase_add_test( tcase, _cnt_set_val03  );
    tcase_add_test( tcase, _cnt_set_val04  );
    tcase_add_test( tcase, _cnt_set_val05  );
    tcase_add_test( tcase, _cnt_set_val06  );
    tcase_add_test( tcase, _cnt_set_val07  );
    tcase_add_test( tcase, _cnt_set_val08  );
    tcase_add_test( tcase, _cnt_set_val08r );
    tcase_add_test( tcase, _cnt_set_val09  );
    tcase_add_test( tcase, _cnt_set_val10  );
    tcase_add_test( tcase, _cnt_set_val10r );
    tcase_add_test( tcase, _cnt_page_lv1_fnd );
    tcase_add_test( tcase, _cnt_page_lv1_asn );
    tcase_add_test( tcase, _page_split );

    tcase_add_test( tcase, _cnt_count );

    (void)_bfd_open;
    // tcase_add_test( tcase, _bfd_open );
    tcase_add_test( tcase, _set_record_value );

    
    tcase_add_test( tcase, _cnt_set_val_b );
    tcase_add_test( tcase, _cnt_set_val_d );
    tcase_add_test( tcase, _cnt_set_col_val_d );
    tcase_add_test( tcase, _cnt_set_col_val_b );
    tcase_add_test( tcase, _cnt_set_idx_d );
    tcase_add_test( tcase, _cnt_set_idx_b );
    tcase_add_test( tcase, _cnt_col_idx_b );
    tcase_add_test( tcase, _col_assigned_new  );
    tcase_add_test( tcase, _col_assigned_exists  );
    tcase_add_test( tcase, _col_find_exists    );
    tcase_add_test( tcase, _col_find_not_exists    );
    tcase_add_test( tcase, _cnt_colname    );
    return tcase;
}


//  Suite
Suite *test_suite( void )
{
    Suite *s;
    s = suite_create( "Values" );
    suite_add_tcase( s, test_cnt_page(  ) );
    return s;
}
