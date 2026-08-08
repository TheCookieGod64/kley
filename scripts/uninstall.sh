#!/bin/bash
YELLOW='\033[1;33m'
GREEN='\033[0;32m'
RESET='\033[0m'

echo -e "${YELLOW}🗑  Uninstalling kley...${RESET}"

if [ -f "/etc/hosts.monster" ]; then
    echo -e "${YELLOW}⚠  Kley mode is on, disabling first...${RESET}"
    sudo mv /etc/hosts.monster /etc/hosts
fi

sudo rm -f /usr/local/bin/kley
rm -rf "${XDG_STATE_HOME:-$HOME/.local/state}/kley"

echo -e "${GREEN}✅ Uninstalled${RESET}"
