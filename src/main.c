#include "kley.h"

int main(int argc, char *argv[]){
    init_paths();
    const char *cmd = argc>1 ? argv[1] : "status";
    if(strcmp(cmd,"on")==0||strcmp(cmd,"start")==0||strcmp(cmd,"aan")==0||strcmp(cmd,"onn")==0) return cmd_on(argc-2, argv+2);
    if(strcmp(cmd,"off")==0||strcmp(cmd,"stop")==0||strcmp(cmd,"uit")==0||strcmp(cmd,"oof")==0) return cmd_off(argc-2, argv+2);
    if(strcmp(cmd,"status")==0||strcmp(cmd,"check")==0||strcmp(cmd,"stauts")==0||strcmp(cmd,"")==0) return cmd_status();
    if(strcmp(cmd,"toggle")==0||strcmp(cmd,"t")==0||strcmp(cmd,"toggly")==0||strcmp(cmd,"togly")==0||strcmp(cmd,"toggel")==0){ return cmd_toggle(argc-2, argv+2); }
    if(strcmp(cmd,"cancel")==0||strcmp(cmd,"abort")==0||strcmp(cmd,"nocancel")==0) return cmd_cancel();
    if(strcmp(cmd,"extend")==0||strcmp(cmd,"reset-timer")==0||strcmp(cmd,"reschedule")==0) return cmd_extend(argc-2, argv+2);
    if(strcmp(cmd,"install-db")==0||strcmp(cmd,"installdb")==0) return cmd_install_db(argc-2, argv+2);
    if(strcmp(cmd,"update-db")==0||strcmp(cmd,"updatedb")==0) return cmd_install_db(argc-2, argv+2);
    if(strcmp(cmd,"remove-db")==0||strcmp(cmd,"removedb")==0) return cmd_remove_db();
    if(strcmp(cmd,"db-info")==0||strcmp(cmd,"dbinfo")==0||strcmp(cmd,"info")==0) return cmd_db_info();
    if(strcmp(cmd,"version")==0||strcmp(cmd,"-v")==0||strcmp(cmd,"--version")==0){ printf("kley v%s\n", KLEY_VERSION); return 0; }
    if(strcmp(cmd,"help")==0||strcmp(cmd,"-h")==0||strcmp(cmd,"--help")==0||strcmp(cmd,"halp")==0){ print_help(); return 0; }
    fprintf(stderr, RED "❌ Unknown command: %s" RESET "\n" "Type " BLUE "kley help" RESET " for usage.\n", cmd);
    return 1;
}
