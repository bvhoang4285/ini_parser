##Fast and simple INI database file parser C library

Usage:
1. Use idb_from_file() to parse ini file and store in idb_data structure
2. Use idb_get_value() to read the value of a key in a group

Note:
The return value of idb_get_value() is a pointer to a stack memory region. Remember to clean it after use.

Example:
- File config.ini:
```ini
[SETTINGS]
option 1 = yes
option 2 = 100
```

- File main.c:
```ruby
#include <stdio.h>
#include "ini_parser.h"

void main()
{
  char* pval;
  idb_data inidb;

  idb_from_file(&inidb, "./config.ini");

  pval= idb_get_value(&inidb, "SETTINGS", "option 1");
  if(pval)
  {
    printf("option 1 value is: %s\n", pval);
    free(pval);
  }
}
```
