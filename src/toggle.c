#include "kley.h"
int cmd_toggle(int argc, char **argv){ if(file_exists(HOSTS_BACKUP)) return cmd_off(argc,argv); else return cmd_on(argc,argv); }
