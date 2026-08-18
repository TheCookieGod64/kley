# Changelog

All notable changes to **kley** are documented here.

Format: [Keep a Changelog](https://keepachangelog.com/en/1.1.0/)  
Versioning: [Semantic Versioning](https://semver.org/spec/v2.0.0.html)

## [2.0.2] - 2026-08-18

### Changed
- Makefile: added `TARGET`, parallel build `MAKEFLAGS += -j$(nproc)`, `debug` target with `-g -O0`
- Improved super modular build (13 files) — no functional change

## [2.0.1] - 2026-08-18

### Changed
- Refactor: split monolithic `kley.c` into super modular structure (`include/kley.h` + 13 src files)
- No functional change, pure maintainability

## [2.0.0] - 2026-08-18

### Changed — MAJOR REWRITE
- **BREAKING:** Ported from Bash to **C** (compiled ELF `7F 45 4C 46`)
- 10x faster startup, no bash dependency, perfect for AUR `kley-bin`
- Same CLI kept 100% compatible: `on/off/toggle/status/cancel/extend/install-db/remove-db/db-info`
- Auto-timeout now via `fork()` + `kill()` instead of background `sleep &`
- Build system: `Makefile` with `make` / `sudo make install` to `/usr/local/bin/kley`
- License changed to **CKL-2.0** (TheCookieGod64 Public License - Work Hard, Work Dumb Edition)

### Added
- `.gitignore` for C (ignores `kley` binary, `*.o`, `build/`)
- `LICENSE` CKL-2.0
- `README.md` updated for C build instructions

## [1.2.0] - 2026-08-18

### Added
- Desktop notifications via `notify-send`
- Auto-timeout: `kley on <minutes>` schedules automatic restore
- `kley cancel` to abort scheduled auto-off
- `kley extend <minutes>` to reschedule the timer
- Elapsed time shown on `kley off`
- Max 120 min timeout cap for safety

### Changed
- Improved `kley status` with remaining timer info
- Better help text alignment
- Additional alias variants (`nocancel`, `reset-timer`, `reschedule`)

### Fixed
- Cosmetic misalignment in help output

## [1.1.0]

### Added
- Database management: `install-db`, `update-db`, `remove-db`, `db-info`
- Smart merge with deduplication
- Markers for clean removal
- Backup before database operations
- Local file install support (`file://`)

## [1.0.0]

### Added
- Initial release
- Toggle commands: `on`, `off`, `toggle`, `status`
- `/etc/hosts` backup/restore
- Session tracking with elapsed time
- Multi-language aliases

[2.0.2]: https://github.com/TheCookieGod64/kley/compare/v2.0.1...v2.0.2
[2.0.1]: https://github.com/TheCookieGod64/kley/compare/v2.0.0...v2.0.1
[2.0.0]: https://github.com/TheCookieGod64/kley/compare/v1.2.0...v2.0.0
[1.2.0]: https://github.com/TheCookieGod64/kley/compare/v1.1.0...v1.2.0
[1.1.0]: https://github.com/TheCookieGod64/kley/compare/v1.0.0...v1.1.0
[1.0.0]: https://github.com/TheCookieGod64/kley/releases/tag/v1.0.0
