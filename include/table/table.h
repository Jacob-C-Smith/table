﻿/** !
 * Include header for table library
 *
 * @file table/table.h 
 * 
 * @author Jacob Smith
 */

// Include guard
#pragma once

#undef NDEBUG

// Standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// sync submodule
#include <sync/sync.h>

// tuple submodule
#include <tuple/tuple.h>

// Platform dependent macros
#ifdef _WIN64
#define DLLEXPORT extern __declspec(dllexport)
#else
#define DLLEXPORT
#endif

// Memory management macro
#ifndef TABLE_REALLOC
#define TABLE_REALLOC(p, sz) realloc(p,sz)
#endif

// Forward declarations
struct table_s;

// Type definitions
/** !
 *  @brief The type definition of a table struct
 */
typedef struct table_s table;

// Allocaters
/** !
 *  Allocate memory for a table
 *
 * @param pp_table return
 *
 * @sa table_destroy
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int table_create ( table **const pp_table );

// Constructors
/** !
 *  Construct a table with a specific quantity of rows and columns
 *
 * @param pp_table return
 * @param columns  quantity of columns
 * @param rows     quantity of rows
 *
 * @sa table_create
 * @sa table_destroy
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int table_construct ( table **const pp_table, size_t columns, size_t rows );

// Accessors
/** !
 *  Get the contents of a table cell
 *
 * @param p_table   the table
 * @param x         the column cell
 * @param y         the row of the cell
 * @param p_element return
 *
 * @sa table_set_cell
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int table_get_cell ( const table *const p_table, size_t x, size_t y, void **pp_element );

// Mutators
/** !
 *  Set the contents of a table cell
 *
 * @param p_table   the table
 * @param x         the column cell
 * @param y         the row of the cell
 * @param p_element the content of the new cell
 *
 * @sa table_get_cell
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int table_set_cell ( table *const p_table, size_t x, size_t y, void *p_element );

// Destructors
/** !
 *  Destroy and deallocate a table
 *
 * @param pp_table the table
 *
 * @sa table_create
 *
 * @return 1 on success, 0 on error
 */
// TODO: Implement
//DLLEXPORT int table_destroy ( table **const pp_table );