## Fast and simple INI file parser library in C language

### Usage:
1. Use idb_init() to initialize the database structure
2. Use idb_from_file() to parse ini file and store in idb_data structure
3. Use idb_get_value() to read the value of a key in a group

### Example:
- File config.ini:
```ini
[SETTINGS]
option 1 = yes
option 2 = 100
```

- File main.c:
```c
#include <stdio.h>
#include <stdlib.h>
#include "ini_parser.h"

void main()
{
  char* pval;
  idb_data inidb;

  idb_init(&inidb, 0);
  idb_from_file(&inidb, "./config.ini");

  pval= idb_get_value(&inidb, "SETTINGS", "option 1");
  if(pval)
  {
    printf("option 1 value is: %s\n", pval);
  }

  pval= idb_get_value(&inidb, "SETTINGS", "option 2");
  if(pval)
  {
    printf("option 2 value is: %s\n", atoi(pval));
  }
}
```
