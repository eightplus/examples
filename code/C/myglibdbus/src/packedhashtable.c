#include "packedhashtable.h"

static void release_value(gpointer data)
{
	GValue *val = (GValue *)data;
	g_value_unset(val);
	g_free(val);
}

GHashTable *create_packed_hash_table(const char *name, const char *id, int age, int weight, int height)
{
	GHashTable *hash;
	GValue *val;

	hash = g_hash_table_new_full(g_str_hash, NULL, NULL, release_value);

	val = g_new0(GValue, 1);
	g_value_init(val, G_TYPE_STRING);
	g_value_set_string(val, name);
	g_hash_table_insert(hash, "name", val);

	val = g_new0(GValue, 1);
	g_value_init(val, G_TYPE_STRING);
	g_value_set_string(val, id);
	g_hash_table_insert(hash, "id", val);

	val = g_new0(GValue, 1);
	g_value_init(val, G_TYPE_INT);
	g_value_set_int(val, age);
	g_hash_table_insert(hash, "age", val);

	val = g_new0(GValue, 1);
	g_value_init(val, G_TYPE_INT);
	g_value_set_int(val, weight);
	g_hash_table_insert(hash, "weight", val);

	val = g_new0(GValue, 1);
	g_value_init(val, G_TYPE_INT);
	g_value_set_int(val, height);
	g_hash_table_insert(hash, "height", val);

	return hash;
}

GType get_packed_hash_table_type(void)
{
	return dbus_g_type_get_map("GHashTable", G_TYPE_STRING, G_TYPE_VALUE);//键-值对的键类型是字符串，值类型是VARIANT
}

static void print_key_value(gpointer key, gpointer value, gpointer user_data)
{
    printf("%s ---> %s/n", key, value);
}

void print_packed_hash_table_value(GHashTable *table)
{
	//g_hash_table_foreach(table, print_key_value, NULL);

	GList *keys = g_hash_table_get_keys(table);
	gint len = g_list_length(keys);
	gint i;
 
	for (i = 0; i < len; i++) {
		gchar  *key = g_list_nth_data(keys, i);
		GValue *val = g_hash_table_lookup(table, key);
 
		g_print("%s=", key);
		switch (G_VALUE_TYPE(val)) {
		case G_TYPE_STRING:
			g_print("%s\n", g_value_get_string(val));
			break;
		case G_TYPE_INT:
			g_print("%d\n", g_value_get_int(val));
			break;
		default:
			g_print("Value is of unmanaged type!\n");
		}
	}

	g_list_free(keys);
}

void release_packed_hash_table(GHashTable *table)
{
	g_hash_table_destroy(table);
}

