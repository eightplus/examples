#ifndef PACKED_HASH_TABLE_H
#define PACKED_HASH_TABLE_H

#include <glib-object.h>

GHashTable *create_packed_hash_table(const char *name, const char *id, int age, int weight, int height);

GType get_packed_hash_table_type(void);

void release_packed_hash_table(GHashTable *table);

void print_packed_hash_table_value(GHashTable *table);

#endif // PACKED_HASH_TABLE_H
