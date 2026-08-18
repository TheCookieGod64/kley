#include "kley.h"
void print_help(void){
    printf(BOLD BLUE "🔑 KLEY" RESET " - Adblock toggle & hosts manager for key-farming\n" CYAN "Version %s by TheCookieGod64 (C modular v2.0.0)" RESET "\n\n", KLEY_VERSION);
    printf(BOLD YELLOW "USAGE:" RESET "\n    kley <command> [args]\n\n");
    printf(BOLD YELLOW "ADBLOCK TOGGLE:" RESET "\n");
    printf("    " GREEN "on" RESET " [minutes]      Disable adblock (with optional auto-off timer)\n");
    printf("    " GREEN "off" RESET "               Restore adblock\n");
    printf("    " GREEN "toggle" RESET ", t         Toggle between on/off\n");
    printf("    " GREEN "status" RESET ", check     Show current adblock status\n");
    printf("    " GREEN "cancel" RESET "            Cancel scheduled auto-off timer\n");
    printf("    " GREEN "extend" RESET " <min>      Reschedule auto-off to X minutes from now\n\n");
    printf(BOLD YELLOW "DATABASE COMMANDS:" RESET "\n");
    printf("    " GREEN "install-db" RESET " [url]  Download and smart-merge community blocklist\n");
    printf("    " GREEN "update-db" RESET "         Update the blocklist to latest version\n");
    printf("    " GREEN "remove-db" RESET "         Remove only kley's entries from /etc/hosts\n");
    printf("    " GREEN "db-info" RESET "           Show stats on installed blocklist entries\n\n");
    printf(BOLD YELLOW "GENERAL:" RESET "\n");
    printf("    " GREEN "version" RESET ", -v       Show version\n");
    printf("    " GREEN "help" RESET ", -h          Show this help\n\n");
}
