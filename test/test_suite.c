#include <stdio.h>
#include <check.h>
#include <stdbool.h>
#include <hape/cnt.h>
#include <hape/err.h>

#include "../cnt/cnt_int.h"

/* cnt.c */
CNT_CELL cell_create(MEM_ARENA, CNT_IDX col, CNT_IDX row);
bool gen_find(CNT_VECTOR_PAGE p, CMP f, void *elem, CNT_IDX *idx);

int col_compare(void *a, void *b);
CNT_COL col_create(MEM_ARENA a, int pos, char* name);

CNT_IDX cnt_columns(CNT cnt);
CNT_IDX cnt_lines(CNT cnt);
CNT_COL_NAME cnt_colname(CNT cnt, CNT_IDX col);
CNT_DATA *data_cpy(MEM_ARENA a, CNT_DATA *data);
CNT_COL cnt_column(CNT cnt, char *name);
void cnt_set_val_b(CNT cnt, CNT_COL_NAME name, void *data);
void cnt_set_idx_b(CNT cnt, CNT_COL_NAME name, CNT_IDX row, void *data);
CNT_IDX cell_pos(CNT cnt, CNT_IDX row, CNT_IDX col);
int pair_compare(CNT_IDX x1, CNT_IDX x2, CNT_IDX y1, CNT_IDX y2);
CNT_IDX cell_find(CNT_VECTOR_PAGE page, CNT_IDX row, CNT_IDX col);
void *cnt_idx_b(CNT cnt, CNT_COL_NAME name, CNT_IDX row);
CNT_VECTOR_PAGE cell_page_split(MEM_ARENA arena, CNT_VECTOR_PAGE p0);
void cell_page_insert(CNT_VECTOR_PAGE p, CNT_IDX idx, CNT_CELL cell);
void cnt_set_idx_d(CNT cnt, CNT_COL_NAME name, CNT_IDX row, CNT_DATA *data);
void cnt_set_val_d(CNT cnt, CNT_COL_NAME name, CNT_DATA *data);
void cnt_dump(CNT cnt);
void cnt_json(CNT cnt);
void *cnt_val_b(CNT cnt, char *name);
CNT cnt_create(void);
CNT_VECTOR_PAGE page_create(MEM_ARENA arena);
CNT cnt_create_a(MEM_ARENA p_arena);
int lv1_compare(void *a, void *b);
bool gen_assign(CNT_VECTOR_PAGE p, CMP f, void *elem, CNT_IDX *idx);
MEM_ARENA a;

START_TEST( _cnt_page_create )
{
    CNT_VECTOR_PAGE p = page_create(a);

    ck_assert_ptr_ne(p, NULL);
}
END_TEST 


START_TEST( _cnt_page_col_fnd )
{
    CNT_VECTOR_PAGE p = page_create(a);
    CNT_COL c = col_create(a, 1, "id");
    CNT_COL c2 = col_create(a, 2, "name");
    CNT_IDX idx = -1;
    bool r = gen_assign(p, lv1_compare, c2, &idx);
    ck_assert(r);
    ck_assert(idx == 0);
    r = gen_assign(p, lv1_compare, c, &idx);
    ck_assert(r);
    ck_assert(idx == 0);
    r = gen_find(p, col_compare, c, &idx);
    ck_assert(r);
    ck_assert_int_eq(idx, 0); 
    r = gen_find(p, col_compare, c2, &idx);
    ck_assert(r);
    ck_assert_int_eq(idx, 1); 
}
END_TEST

START_TEST( _cnt_page_lv1_fnd )
{
    CNT_VECTOR_PAGE p = page_create(a);
    CNT_CELL c = cell_create(a, 0, 0 );
    CNT_CELL c2 = cell_create(a, 1, 0 );
    CNT_CELL c3 = cell_create(a, 4, 0 );
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
    CATCH("cnt", "01")
        fprintf(stderr, "error found\n");
    END_TRY
    ck_assert_int_eq(cnt->cells->used, 2);
}
END_TEST

void cnt_set_tst(int max_col, int max_row ){
    CNT_DATA d;
    d.d = "<dummy>";
    d.l = strlen(d.d);
    CNT cnt = cnt_create_a(a);
    TRY
        for( int i = 1; i < max_row; i++){
            CNT_DATA * x;
            for( int j = 1; j < max_col; j++) {
                cnt_set_col_idx_d(cnt, j, i, &d);
                x = cnt_col_idx_d(cnt, j, i);
                if(x == NULL) break;
            }
            if(x == NULL) break;
        }
    CATCH("cnt", "01")
        fprintf(stderr, "error found\n");
    END_TRY
    ck_assert_int_eq(cnt->cells->used, (max_row-1) * (max_col-1));
}

START_TEST( _cnt_set_val01 ) { cnt_set_tst( 5, 50); } END_TEST
START_TEST( _cnt_set_val02 ) { cnt_set_tst( 6, 51); } END_TEST
START_TEST( _cnt_set_val03 ) { cnt_set_tst( 7, 52); } END_TEST
START_TEST( _cnt_set_val04 ) { cnt_set_tst( 8, 53); } END_TEST
START_TEST( _cnt_set_val05 ) { cnt_set_tst( 9, 54); } END_TEST
START_TEST( _cnt_set_val06 ) { cnt_set_tst(10, 55); } END_TEST
START_TEST( _cnt_set_val07 ) { cnt_set_tst(11, 56); } END_TEST
START_TEST( _cnt_set_val08 ) { cnt_set_tst(12, 57); } END_TEST
START_TEST( _cnt_set_val09 ) { cnt_set_tst(13, 58); } END_TEST

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
    tcase_add_test( tcase, _cnt_set_val09  );
    tcase_add_test( tcase, _cnt_page_lv1_fnd );
    tcase_add_test( tcase, _cnt_page_lv1_asn );
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
