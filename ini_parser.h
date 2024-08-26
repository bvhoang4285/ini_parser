// INI Data Base Parser by Bui Viet Hoang

#ifndef __INI_PARSER_H_
#define __INI_PARSER_H_

typedef struct __idb_key_
{
	char* name;
	char* val;
} idb_key;

typedef struct __idb_group_
{
	unsigned int n_keys;
	unsigned int n_keys_init;
	char* name;
	idb_key* p_keys;
} idb_group;

typedef struct __idb_data_
{
	unsigned int n_groups;
	unsigned int n_groups_init;
	idb_group* p_groups;
} idb_data;

extern void idb_from_file(idb_data* pdb, char* path);
extern char* idb_get_value(idb_data* pdb, char* group, char* name);
extern void idb_reset(idb_data *pdb);
extern void idb_init(idb_data *pdb, unsigned int n_grp);

#endif // __INI_PARSER_H_
