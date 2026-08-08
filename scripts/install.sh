#!/bin/bash
set -e

GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
RESET='\033[0m'

INSTALL_DIR="/usr/local/bin"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

echo -e "${BLUE}🍪 Installing kley...${RESET}"

sudo cp "$SCRIPT_DIR/src/kley" "$INSTALL_DIR/kley"
sudo chmod +x "$INSTALL_DIR/kley"

echo -e "${GREEN}✅ Installed to $INSTALL_DIR/kley${RESET}"
echo -e "${BLUE}Test with: ${RESET}kley --version"
