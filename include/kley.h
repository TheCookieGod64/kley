#pragma once
#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <limits.h>
#include <errno.h>
#include <signal.h>
#include <ctype.h>

#define KLEY_VERSION "2.0.0"
#define HOSTS_FILE "/etc/hosts"
#define HOSTS_BACKUP "/etc/hosts.monster"
#define MARKER_START "# === KLEY BLOCKLIST START ==="
#define MARKER_END "# === KLEY BLOCKLIST END ==="
#define DB_URL "https://raw.githubusercontent.com/TheCookieGod64/kley-db/main/blocklist.txt"

// colors
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[0;34m"
#define CYAN "\033[0;36m"
#define MAGENTA "\033[0;35m"
#define BOLD "\033[1m"
#define RESET "\033[0m"

// paths
extern char state_dir[PATH_MAX];
extern char state_file[PATH_MAX];
extern char timeout_file[PATH_MAX];
extern char timeout_pid_file[PATH_MAX];

void init_paths(void);
int has_notify(void);
void notify_send(const char *urgency, const char *icon, const char *title, const char *body, int ms);
void ensure_state_dir(void);
long get_elapsed(void);
void format_duration(long sec, char *out, size_t n);
void cancel_timeout(void);
int file_exists(const char *p);
void get_hostname_custom(char *out, size_t n);

// commands
int cmd_status(void);
int cmd_on(int argc, char **argv);
int cmd_off(int argc, char **argv);
int cmd_toggle(int argc, char **argv);
int cmd_cancel(void);
int cmd_extend(int argc, char **argv);
int cmd_install_db(int argc, char **argv);
int cmd_remove_db(void);
int cmd_db_info(void);
void print_help(void);
