/** !
 * table library
 *
 * @file table.c
 *
 * @author Jacob Smith
 */

// Headers
#include <table/table.h>

// Structure definitions
struct table_s
{
    size_t   rows,
             columns;
    void   **swap;
    tuple  **pp_tuples;
    mutex   *p_locks;
};

int table_create ( table **const pp_table )
{

    // Argument check
    if ( pp_table == (void *) 0 ) goto no_table;

    // Allocate memory for a table
    table *p_table = TABLE_REALLOC(0, sizeof(table));

    // Error checking
    if ( p_table == (void *) 0 ) goto no_mem;

    // Zero set
    memset(p_table, 0, sizeof(table));

    // Return the allocated memory
    *pp_table = p_table;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_table:
                #ifndef NDEBUG
                    printf("[table] Null pointer provided for parameter \"pp_table\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    printf("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int table_construct ( table **const pp_table, size_t columns, size_t rows )
{

    // Argument check
    if ( pp_table == (void *) 0 ) goto no_table;
    if ( columns  ==          0 ) goto no_columns;
    if ( rows     ==          0 ) goto no_rows;

    // Initialized data
    table *p_table = 0;

    // Allocate a table
    if ( table_create(&p_table) == 0 ) goto failed_to_create_table;

    // Store the quantity of rows 
    p_table->rows = rows;

    // Store the quantity of columns
    p_table->columns = columns;
    
    // Allocate a list of tuples
    p_table->pp_tuples = TABLE_REALLOC(0, sizeof(tuple *) * rows);

    // Error check
    if ( p_table->pp_tuples == 0 ) goto no_mem;

    // Allocate a list of locks
    p_table->p_locks = TABLE_REALLOC(0, sizeof(mutex) * rows);

    // Error check
    if ( p_table->p_locks == 0 ) goto no_mem;

    // Allocate a list of void pointers
    p_table->swap = TABLE_REALLOC(0, sizeof(void *) * rows);

    // Error check
    if ( p_table->swap == 0 ) goto no_mem;

    // Iterate over each row
    for (size_t i = 0; i < rows; i++)
    {

        // Construct a tuple
        if ( tuple_construct(&p_table->pp_tuples[i], columns) == 0 ) goto failed_to_construct_tuple;

        // Construct a mutex
        if ( mutex_create(&p_table->p_locks[i]) == 0 ) goto failed_to_create_mutex;

    }    

    // Return a pointer to the caller
    *pp_table = p_table;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_table:
                #ifndef NDEBUG
                    printf("[table] Null pointer provided for parameter \"pp_table\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_columns:
                #ifndef NDEBUG
                    printf("[table] Zero provided for parameter \"columns\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
            
            no_rows:
                #ifndef NDEBUG
                    printf("[table] Zero provided for parameter \"rows\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

        }

        // Table errors
        {
            failed_to_create_table:
                #ifndef NDEBUG
                    printf("[table] Failed to create table in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_create_mutex:
                #ifndef NDEBUG
                    printf("[table] Failed to create mutex in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Tuple errors
        {

            failed_to_construct_tuple:
                #ifndef NDEBUG
                    printf("[table] Call to \"construct_tuple\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    printf("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int table_set_cell ( table *const p_table, size_t x, size_t y, void *p_element )
{

    // Argument check
    if ( p_table == (void *) 0 ) goto no_table;
    
    // Initialized data
    tuple *p_row = p_table->pp_tuples[y];
    
    // Lock the row
    mutex_lock(p_table->p_locks[y]);

    // Store contents of the tuple on the stack
    // TODO: Error check
    tuple_get(p_row, p_table->swap, 0);

    // Update the element
    p_table->swap[x] = p_element;

    // Destroy the old tuple
    // TODO: Error check
    tuple_destroy(&p_table->pp_tuples[y]);

    // Construct a new tuple from the contents of the old tuple
    // TODO: Error check
    tuple_from_elements(&p_table->pp_tuples[y], p_table->swap, p_table->columns);

    // Unlock the row
    mutex_unlock(p_table->p_locks[y]);

    // Success
    return 1;

    // Error handling
    {

        // Argument check
        {
            no_table:
                #ifndef NDEBUG
                    printf("[table] Null pointer provided for parameter \"p_table\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int table_get_cell ( const table *const p_table, size_t x, size_t y, void **pp_element )
{

    // TODO: Argument check
    //

    // Initialized data
    void *p_element = 0;

    // Lock the row
    mutex_lock(p_table->p_locks[y]);

    // Return a pointer to the caller
    // TODO: Error check
    //if ( 
        tuple_index(p_table->pp_tuples[y], x, &p_element);
    // == 0 ) goto failed_to_index_tuple;

    // Unlock the row
    mutex_unlock(p_table->p_locks[y]);

    // Return the element to the caller
    *pp_element = p_element;

    // Success
    return 1;

    // TODO: Error handling
}

int table_swap_rows ( table *const p_table, size_t row1, size_t row2 )
{

    // Argument check
    if ( p_table == (void *) 0 ) goto no_table;

    // Initialized data
    tuple *p_tmp = 0;

    // Lock the mutexes
    mutex_lock(p_table->p_locks[row1]);
    mutex_lock(p_table->p_locks[row2]);
    
    // Swap the row
    p_tmp                    = p_table->pp_tuples[row1];    
    p_table->pp_tuples[row1] = p_table->pp_tuples[row2];
    p_table->pp_tuples[row2] = p_tmp;

    // Unlock the mutexes
    mutex_unlock(p_table->p_locks[row1]);
    mutex_unlock(p_table->p_locks[row2]);

    // Success
    return 1;

    // Error handling
    {

        // Argument check
        {
            no_table:
                #ifndef NDEBUG
                    printf("[table] Null pointer provided for parameter \"p_table\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}