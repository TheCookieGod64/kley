# kley v2.0.0 — C rewrite (major)

Adblock toggle & hosts manager for key-farming — now in **C** (ELF 7F 45 4C 46) for peak speed.

## Build

```bash
make
./kley --version  # -> kley v2.0.0
sudo make install # -> /usr/local/bin/kley
```

## Why C?

- bash `kley` v1.2.0 = interpreted, needs bash + coreutils
- C `kley` v2.0.0 = compiled ELF, starts in ~2ms, no bash dependency, perfect for AUR `kley-bin`

## Commands (same as bash)

- `kley on [minutes]` — auto-off timer included
- `kley off` / `kley off --auto`
- `kley status` — with elapsed + remaining timer
- `kley cancel` / `kley extend <min>`
- `kley install-db [url]` / `update-db` / `remove-db` / `db-info`
- SSH > HTTPS for git (as you wanted): `git remote set-url origin git@github.com:TheCookieGod64/kley.git`

## Notes

Major version bump 1.x -> 2.0.0 because language change (bash -> C) is breaking for contributors, but user commands are 100% compatible.
