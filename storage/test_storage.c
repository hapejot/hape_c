#include "storage_int.h"

int callback( void *, int, char **, char ** );

int test_transaction( void ) {

    sqlite3 *db;
    char *err_msg = 0;
    char *sql = "DROP TABLE IF EXISTS Friends;"
            "BEGIN TRANSACTION;"
            "CREATE TABLE Friends(Id INTEGER PRIMARY KEY, Name TEXT);"
            "INSERT INTO Friends(Name) VALUES ('Tom');"
            "INSERT INTO Friends(Name) VALUES ('Rebecca');"
            "INSERT INTO Friends(Name) VALUES ('Jim');"
            "INSERT INTO Friends(Name) VALUES ('Robert');" "COMMIT;";

    if( SQLITE_OK == sqlite3_open( "test.db", &db )
        && SQLITE_OK == sqlite3_exec( db, sql, 0, 0, &err_msg ) )
        sqlite3_free( err_msg );

    sqlite3_close( db );

    return 0;
}

int test_autocommit(  ) {
    sqlite3 *db;
    if( SQLITE_OK == sqlite3_open( "test.db", &db ) )
        printf( "Autocommit: %d\n", sqlite3_get_autocommit( db ) );
    sqlite3_close( db );
    return 0;
}

int test_list_tables( void ) {

    sqlite3 *db;
    char *err_msg = 0;
    char *sql = "SELECT name FROM sqlite_master WHERE type='table'";
    MEM_ARENA arena = mem_arena_new(  );
    CNT cnt;
    cnt_create( arena, &cnt );

    if( SQLITE_OK == sqlite3_open( "test.db", &db )
        && SQLITE_OK == sqlite3_exec( db, sql, callback, cnt, &err_msg ) ) {
        cnt_dump( cnt );
    }
    sqlite3_free( err_msg );
    sqlite3_close( db );
    return 0;
}

int test_metadata( void ) {
    sqlite3 *db;
    char *err_msg = 0;
    char *sql = "PRAGMA table_info(Cars)";
    MEM_ARENA arena = mem_arena_new(  );
    CNT cnt;
    cnt_create( arena, &cnt );

    if( SQLITE_OK == sqlite3_open( "test.db", &db )
        && SQLITE_OK == sqlite3_exec( db, sql, callback, cnt, &err_msg ) ) {
        cnt_dump( cnt );
    }
    sqlite3_close( db );
    return 0;
}


int test_named_params(  ) {
    sqlite3 *db;
    sqlite3_stmt *res;
    char *sql = "SELECT Id, Name FROM Cars WHERE Id = @id";

    MEM_ARENA arena = mem_arena_new(  );
    CNT cnt;
    cnt_create( arena, &cnt );

    if( SQLITE_OK == sqlite3_open( "test.db", &db )
        && SQLITE_OK == sqlite3_prepare_v2( db, sql, -1, &res, 0 ) ) {
        int idx = sqlite3_bind_parameter_index( res, "@id" );
//int value = 4;
//sqlite3_bind_int( res, idx, value );
        char value[] = "4";
        sqlite3_bind_text( res, idx, value, strlen( value ), NULL );



        if( sqlite3_step( res ) == SQLITE_ROW ) {
            MEM_ARENA arena = mem_arena_new(  );
            CNT cnt;
            cnt_create( arena, &cnt );
            INT rows;
            INT cols;
            INT cells;
            cnt_get_dim( cnt, &rows, &cols, &cells );
            int argc = sqlite3_column_count( res );
            char buffer[20];

            for( int i = 0; i < argc; i++ ) {
                const char *col_name = sqlite3_column_name( res, i );
                const char *arg = ( char * )sqlite3_column_text( res, i );
                if( arg == NULL ) {
                    int v = sqlite3_column_int( res, i );
                    sprintf( buffer, "%06d", v );
                    arg = buffer;
                }
                cnt_set_idx_val( cnt, rows, col_name, 0, strlen( arg ), arg );
            }
            sqlite3_finalize( res );
            printf( "DUMP\n----------\n" );
            cnt_dump( cnt );
            printf( "----------\n" );
        }
        else
            printf( "no data for id: 4\n" );
    }

    sqlite3_close( db );
    return 0;
}

int test_params(  ) {
    sqlite3 *db;
    sqlite3_stmt *res;
    int rc = sqlite3_open( "test.db", &db );
    if( rc != SQLITE_OK ) {
        fprintf( stderr, "Cannot open database: %s\n", sqlite3_errmsg( db ) );
        sqlite3_close( db );
        return 1;
    }
    char *sql = "SELECT Id, Name FROM Cars WHERE Id = ?";
    rc = sqlite3_prepare_v2( db, sql, -1, &res, 0 );
    if( rc == SQLITE_OK ) {
        sqlite3_bind_int( res, 1, 3 );
    }
    else {
        fprintf( stderr, "Failed to execute statement: %s\n",
                 sqlite3_errmsg( db ) );
    }
    if( SQLITE_ROW == sqlite3_step( res ) ) {
        printf( "%s: ", sqlite3_column_text( res, 0 ) );
        printf( "%s\n", sqlite3_column_text( res, 1 ) );
    }
    sqlite3_finalize( res );
    sqlite3_close( db );
    return 0;
}


int test_select(  ) {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open( "test.db", &db );
    if( rc != SQLITE_OK ) {
        fprintf( stderr, "Cannot open database: %s\n", sqlite3_errmsg( db ) );
        sqlite3_close( db );
        return 1;
    }
    char *sql = "SELECT * FROM Cars";
    MEM_ARENA arena = mem_arena_new(  );
    CNT cnt;
    cnt_create( arena, &cnt );

    rc = sqlite3_exec( db, sql, callback, cnt, &err_msg );
    if( rc != SQLITE_OK ) {
        fprintf( stderr, "Failed to select data\n" );
        fprintf( stderr, "SQL error: %s\n", err_msg );
        sqlite3_free( err_msg );
        sqlite3_close( db );
        return 1;
    }
    sqlite3_close( db );
    printf( "DUMP\n----------\n" );
    cnt_dump( cnt );
    printf( "----------\n" );
    return 0;
}

int callback( void *cnt, int argc, char **argv, char **azColName ) {

    if( cnt ) {
        INT rows;
        INT cols;
        INT cells;
        cnt_get_dim( cnt, &rows, &cols, &cells );
        for( int i = 0; i < argc; i++ ) {
            if( argv[i] ) {
                cnt_set_idx_val( cnt,
                                 rows,
                                 azColName[i],
                                 0, strlen( argv[i] ), argv[i] );
            }
        }
    }
    else {
        for( int i = 0; i < argc; i++ ) {
            printf( "%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL" );
        }
        printf( "\n" );
    }
    return 0;
}

int test_dbversion(  ) {
    sqlite3 *db;
    sqlite3_stmt *res;

    int rc = sqlite3_open( ":memory:", &db );

    if( rc != SQLITE_OK ) {

        fprintf( stderr, "Cannot open database: %s\n", sqlite3_errmsg( db ) );
        sqlite3_close( db );

        return 1;
    }

    rc = sqlite3_prepare_v2( db, "SELECT SQLITE_VERSION()", -1, &res, 0 );

    if( rc != SQLITE_OK ) {

        fprintf( stderr, "Failed to fetch data: %s\n", sqlite3_errmsg( db ) );
        sqlite3_close( db );

        return 1;
    }

    rc = sqlite3_step( res );

    if( rc == SQLITE_ROW ) {
        printf( "%s\n", sqlite3_column_text( res, 0 ) );
    }

    sqlite3_finalize( res );
    sqlite3_close( db );
    return 0;
}

int test_insert(  ) {

    sqlite3 *db;
    char *err_msg = 0;

    int rc = sqlite3_open( "test.db", &db );

    if( rc != SQLITE_OK ) {

        fprintf( stderr, "Cannot open database: %s\n", sqlite3_errmsg( db ) );
        sqlite3_close( db );

        return 1;
    }

    char *sql = "DROP TABLE IF EXISTS Cars;"
            "CREATE TABLE Cars(Id INT, Name TEXT, Price INT);"
            "INSERT INTO Cars VALUES(1, 'Audi', 52642);"
            "INSERT INTO Cars VALUES(2, 'Mercedes', 57127);"
            "INSERT INTO Cars VALUES(3, 'Skoda', 9000);"
            "INSERT INTO Cars VALUES(4, 'Volvo', 29000);"
            "INSERT INTO Cars VALUES(5, 'Bentley', 350000);"
            "INSERT INTO Cars VALUES(6, 'Citroen', 21000);"
            "INSERT INTO Cars VALUES(7, 'Hummer', 41400);"
            "INSERT INTO Cars VALUES(8, 'Volkswagen', 21600);";

    rc = sqlite3_exec( db, sql, 0, 0, &err_msg );

    if( rc != SQLITE_OK ) {

        fprintf( stderr, "SQL error: %s\n", err_msg );

        sqlite3_free( err_msg );
        sqlite3_close( db );

        return 1;
    }


    int last_id = sqlite3_last_insert_rowid( db );
    printf( "The last Id of the inserted row is %d\n", last_id );

    sqlite3_close( db );

    return 0;
}

int main(  ) {
    storage_init(  );
    printf( "%s\n", sqlite3_libversion(  ) );
    test_dbversion(  );
    test_insert(  );
    test_select(  );
    test_params(  );
    test_named_params(  );
    test_metadata(  );
    test_list_tables(  );
    test_autocommit(  );
    test_transaction(  );


    printf( "READY.\n" );
    return 0;
}
