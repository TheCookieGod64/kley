#include "kley.h"

int has_notify(void){ return system("command -v notify-send >/dev/null 2>&1")==0; }

void notify_send(const char *urgency, const char *icon, const char *title, const char *body, int ms){
    if(!has_notify()) return;
    char cmd[1024];
    snprintf(cmd,sizeof(cmd),"notify-send -u '%s' -i '%s' -t %d -a kley '%s' '%s' 2>/dev/null || true", urgency, icon, ms, title, body);
    system(cmd);
}
