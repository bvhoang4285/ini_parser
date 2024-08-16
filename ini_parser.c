#include "ini_parser.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void idb_init_group(idb_group* pgrp, unsigned int nkeys, char* name, unsigned int keylen)
{
	pgrp->n_keys= 0;
	pgrp->n_keys_init= nkeys;
	pgrp->p_keys= (idb_key*) malloc(sizeof(idb_key)*nkeys);
	pgrp->name= (char*)malloc((keylen+1)*sizeof(char));
	strcpy(pgrp->name, name);
	pgrp->name[keylen+1]= 0;
}

void idb_reset_group(idb_group* pgrp)
{
	pgrp->n_keys= 0;
	if(pgrp->p_keys)
	{
		free(pgrp->p_keys);
	}
	pgrp->p_keys= NULL;
}

void idb_set_key(idb_key* pkey, char* name, char* val)
{
	unsigned int len, i, j;
	len= strlen(name);
	if(len > 0)
	{
		for(i= 0; i < len; i++) if(name[i] > ' ') break;
		for(j= len-1; j > 0; j--) if(name[j] > ' ') break;
		pkey->name= (char*) malloc((j+1)*sizeof(char));
		strncpy(pkey->name, &name[i], j+1);
		pkey->name[j+1]= 0;
	}
	len= strlen(val);
	if(len > 0)
	{
		for(i= 0; i < len; i++) if(val[i] > ' ') break;
		for(j= len-1; j > 0; j--) if(val[j] > ' ') break;
		pkey->val= (char*) malloc((j+1)*sizeof(char));
		strncpy(pkey->val, &val[i], j+1);
		pkey->val[j+1]= 0;
	}
}

void idb_add_key(idb_group* pgrp, idb_key* pkey)
{
	idb_key* pnew;
	if(pgrp->n_keys >= pgrp->n_keys_init)
	{
		pnew= (idb_key*) realloc(pgrp->p_keys, sizeof(idb_key)*(pgrp->n_keys+1));
		pgrp->p_keys= pnew;
	}

	memcpy((void*)&pgrp->p_keys[pgrp->n_keys], (void*)pkey, sizeof(idb_key));
	pgrp->n_keys+= 1;
}

void idb_init(idb_data *pdb, unsigned int n_grp)
{
	pdb->n_groups= 0;
	pdb->n_groups_init= n_grp;
	pdb->p_groups= (idb_group*) malloc(sizeof(idb_group)*n_grp);
}

void idb_reset(idb_data *pdb)
{
	unsigned int i;

	for(i= 0; i < pdb->n_groups; i++)
	{
		idb_reset_group(&pdb->p_groups[i]);
	}
	pdb->n_groups= 0;
	pdb->p_groups= NULL;
	free(pdb->p_groups);
}

void idb_add_group(idb_data *pdb, idb_group* pgr)
{
	idb_group* pnew;
	if(pdb->n_groups >= pdb->n_groups_init)
	{
		pnew= (idb_group*) realloc(pdb->p_groups, sizeof(idb_group)*(pdb->n_groups+1));
		pdb->p_groups= pnew;
	}

	memcpy((void*)&pdb->p_groups[pdb->n_groups], (void*)pgr, sizeof(idb_group));
	pdb->n_groups+= 1;
}



void idb_from_file(idb_data* pdb, char* path)
{
	FILE* pfile;
	char buf[1024];
	char name[512];
	char val[512];
	char dummy;
	int i, ret, len;
	idb_group* pnew_group;
	idb_key* pnew_key;

	idb_init(pdb, 1);

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
			strcpy(name, &buf[1]);
			pnew_group= (idb_group*) malloc(sizeof(idb_group));
			idb_init_group(pnew_group, 1, name, len);
			idb_add_group(pdb, pnew_group);
		}
		else
		if(buf[0] != ';')
		{
			sscanf(buf, " %[^=] %c %[^\r^\n]", name, &dummy, val);
			pnew_key= (idb_key*) malloc(sizeof(idb_key));
			idb_set_key(pnew_key, name, val);
			idb_add_key(&pdb->p_groups[pdb->n_groups-1], pnew_key);
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
