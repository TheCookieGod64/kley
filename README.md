# kley v2.0.3 — C rewrite + patches

Adblock toggle & `/etc/hosts` manager for key-farming — rewritten in C for peak speed.  
Compiled ELF (`7F 45 4C 46`) with instant startup, zero bash dependency, and perfect AUR compatibility.

## Build

```bash
make
./kley --version  # -> kley v2.0.3
sudo make install # -> /usr/local/bin/kley
```

## Why C?

- bash `kley` v1.2.0 → interpreted, requires bash + coreutils  
- C `kley` v2.0.x → compiled ELF, starts in ~2ms, no bash dependency  
- Perfect for AUR `kley-bin`  
- Cleaner architecture, safer timers, easier modularity

## Commands

- [kley on](ca://s?q=Vertel_me_meer_over_kley_on) `[minutes]` — auto-off timer included  
- [kley off](ca://s?q=Vertel_me_meer_over_kley_off) / `kley off --auto`  
- [kley status](ca://s?q=Vertel_me_meer_over_kley_status) — shows elapsed + remaining timer  
- [kley cancel](ca://s?q=Vertel_me_meer_over_kley_cancel)  
- [kley extend](ca://s?q=Vertel_me_meer_over_kley_extend) `<min>`  
- [kley install-db](ca://s?q=Vertel_me_meer_over_kley_install_db) `[url]`  
- `update-db` / `remove-db` / `db-info`

Git remote (SSH > HTTPS):

```bash
git remote set-url origin git@github.com:TheCookieGod64/kley.git
```

## v2.0.3 Patches

- Internal cleanups  
- Micro-optimizations in module layout  
- No breaking changes — commands remain fully compatible

## Notes

Major version bump 1.x → 2.0.x because the language change (bash → C) is breaking for contributors.  
User commands remain fully compatible across all 2.0.x versions.
