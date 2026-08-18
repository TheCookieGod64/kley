#include "kley.h"

int cmd_cancel(void){
    if(!file_exists(timeout_file)){ printf(YELLOW "⚠  No auto-off timer is scheduled." RESET "\n"); return 0; }
    cancel_timeout();
    printf(GREEN "✅ Auto-off timer cancelled." RESET "\n");
    printf(YELLOW "   Kley mode is still ON — turn off manually with: " BOLD "kley off" RESET "\n");
    notify_send("normal","dialog-information","⏰ Timer Cancelled","Auto-off cancelled — kley mode still ON",3000);
    return 0;
}
