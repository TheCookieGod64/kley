# 🔑 kley

> **Adblock toggle & smart hosts database manager for Linux.**  
> Temporarily disable `/etc/hosts` adblock to bypass key-farming checkpoints (Luarmor, Lootlabs) without annoying pop-up nightmares, or install TheCookieGod64's curated blocklist.

---

## 🚀 Features

- 🎨 **Colored CLI Output** with status & duration indicators
- ⏱ **Session Timer** — warns you if adblock has been disabled for more than 10 minutes
- 🔄 **One-Command Toggle** — switch between protected and unprotected instantly
- 📦 **Smart Database Merge** — install community blocklists with zero duplicates (powered by `awk`)
- 🛡️ **Safe & Reversible** — automatic backups, transparent markers (`# === KLEY BLOCKLIST START ===`)
- 📦 **XDG Compliant** — saves runtime state in `~/.local/state/kley`
- 🎯 **Typo-Tolerant** — understands `toggly`, `stauts`, `aan`, `uit`, etc.

---

## 📦 Installation

```bash
git clone https://github.com/TheCookieGod64/kley.git
cd kley
./scripts/install.sh
```

---

## 🎮 Usage

### 1. Adblock Toggle
```bash
kley on          # Disable adblock (go get your key)
kley status      # Check current mode and timer
kley off         # Re-enable adblock (full protection restored)
kley toggle      # Switch state with one command
```

### 2. Database Management
```bash
kley install-db  # Download and smart-merge TheCookieGod64's blocklist
kley update-db   # Update database to the latest version
kley db-info     # View stats (installed date, active rules)
kley remove-db   # Safely remove all kley-injected entries
```

---

## 🛡️ Safety & Architecture

- When `kley on` runs, `/etc/hosts` is moved to `/etc/hosts.monster` and replaced with a minimal hosts configuration.
- When `kley off` runs, `/etc/hosts.monster` is moved back to `/etc/hosts`.
- When `kley install-db` runs, duplicate domains are skipped and new entries are tagged with start/end markers so they can be uninstalled anytime.

---

## 🗑️ Uninstallation

```bash
./scripts/uninstall.sh
```

---

## 📝 License

[MIT](LICENSE) © 2026 TheCookieGod64

---

## 🤔 Origin Story

Started as a typo of "key" during late-night coding. Turned out to be a much better name — unique, memorable, and impossible to confuse with generic key utilities. 🕶️

## 📝 Note

Not affiliated with KLEY student residences. This is a CLI adblock tool, they rent apartments. Different worlds. 🏠 vs 💻
