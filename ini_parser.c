#include "ini_parser.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void idb_init_group(idb_group* pgrp, unsigned int nkeys, char* name, unsigned int namelen)
{
	pgrp->n_keys= 0;
	pgrp->n_keys_init= nkeys;
	if(nkeys > 0)
	{
		pgrp->p_keys= (idb_key*)malloc(sizeof(idb_key)*nkeys);
	}
	pgrp->name= (char*)malloc((namelen+1)*sizeof(char));
	memcpy(pgrp->name, name, namelen);
	pgrp->name[namelen]= 0;
}

void idb_reset_group(idb_group* pgrp)
{
	int i;

	if(pgrp->name)
	{
		free(pgrp->name);
		pgrp->name= NULL;
	}

	for(i= 0; i < pgrp->n_keys; i++)
	{
		free(pgrp->p_keys[i].name);
		free(pgrp->p_keys[i].val);
	}

	if(pgrp->p_keys)
	{
		free(pgrp->p_keys);
		pgrp->p_keys= NULL;
	}
	pgrp->n_keys= 0;
}

void idb_set_key(idb_key* pkey, char* name, char* val)
{
	int len, i, j;
	len= strlen(name);
	if(len > 0)
	{
		for(i= 0; i < len; i++) if(name[i] > ' ') break;
		for(j= len-1; j > 0; j--) if(name[j] > ' ') break;
		pkey->name= (char*) malloc((j-i+2)*sizeof(char));
		memcpy(pkey->name, &name[i], j-i+2);
		pkey->name[j-i+1]= 0;
	}
	len= strlen(val);
	if(len > 0)
	{
		for(i= 0; i < len; i++) if(val[i] > ' ') break;
		for(j= len-1; j > 0; j--) if(val[j] > ' ') break;
		pkey->val= (char*) malloc((j-i+2)*sizeof(char));
		memcpy(pkey->val, &val[i], j-i+2);
		pkey->val[j-i+1]= 0;
	}
}

void idb_add_key(idb_group* pgrp)
{
	idb_key* pnew;
	if(pgrp->n_keys == 0)
	{
		pgrp->p_keys= (idb_key*) malloc(sizeof(idb_key));
	}
	else
	if(pgrp->n_keys >= pgrp->n_keys_init)
	{
		pnew= (idb_key*) realloc(pgrp->p_keys, sizeof(idb_key)*(pgrp->n_keys+1));
		pgrp->p_keys= pnew;
	}

	pgrp->n_keys+= 1;
}

void idb_init(idb_data *pdb, unsigned int n_grp)
{
	pdb->n_groups= 0;
	pdb->n_groups_init= n_grp;
	if(n_grp > 0)
	{
		pdb->p_groups= (idb_group*) malloc(sizeof(idb_group)*n_grp);
	}
}

void idb_reset(idb_data *pdb)
{
	unsigned int i;

	for(i= 0; i < pdb->n_groups; i++)
	{
		idb_reset_group(&pdb->p_groups[i]);
	}

	if(pdb->p_groups)
	{
		free(pdb->p_groups);
	}
	pdb->n_groups= 0;
	pdb->n_groups_init= 0;
	pdb->p_groups= NULL;
}

void idb_add_group(idb_data *pdb)
{
	idb_group* pnew;
	if(pdb->n_groups == 0)
	{
		pdb->p_groups= (idb_group*) malloc(sizeof(idb_group));
	}
	else
	if(pdb->n_groups >= pdb->n_groups_init)
	{
		pnew= (idb_group*) realloc(pdb->p_groups, sizeof(idb_group)*(pdb->n_groups+1));
		pdb->p_groups= pnew;
	}

	pdb->n_groups+= 1;
}

void idb_from_file(idb_data* pdb, char* path)
{
	FILE* pfile;
	char buf[1280];
	char name[128];
	char val[1152];
	char dummy;
	int i, ret, len;
	idb_group* pnew_group;
	idb_key* pnew_key;

	pfile= fopen(path, "rt");
	if(!pfile)
	{
		printf("Error Reading INI file\n");
		return;
	}

	while(1)
	{
		ret= fscanf(pfile, " %[^\r^\n]", buf);
		if(ret == EOF)
		{
			return;
		}
		len= strlen(buf);
		for(i= len; i > 0; i--) if(buf[i] > ' ') break;
		len= i+1;
		buf[len+1]= 0;
		if(buf[0] == '[' && buf[len-1] == ']')
		{
			buf[len-1]= 0;
			idb_add_group(pdb);
			pnew_group= &pdb->p_groups[pdb->n_groups-1];
			idb_init_group(pnew_group, 0, &buf[1], len-2);
		}
		else
		if(buf[0] != ';')
		{
			sscanf(buf, " %[^=] %c %[^\r^\n]", name, &dummy, val);
			idb_add_key(pnew_group);
			pnew_key= &pnew_group->p_keys[pnew_group->n_keys-1];
			idb_set_key(pnew_key, name, val);
		}
	}
}

char* idb_get_value(idb_data* pdb, char* group, char* key)
{
	unsigned int i, j;
	idb_group* pgrp;
	idb_key* pkey;

	for(i= 0; i < pdb->n_groups; i++)
	{
		pgrp= &pdb->p_groups[i];
		if(strstr(group, pgrp->name))
		{
			for(j= 0; j < pgrp->n_keys; j++)
			{
				pkey= &pgrp->p_keys[j];
				if(strstr(key, pkey->name))
				{
					return(pkey->val);
				}
			}
		}
	}
	return(NULL);
}
