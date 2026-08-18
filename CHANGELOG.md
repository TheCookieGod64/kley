# Changelog

All notable changes to **kley** are documented here.

Format: [Keep a Changelog](https://keepachangelog.com/en/1.1.0/)  
Versioning: [Semantic Versioning](https://semver.org/spec/v2.0.0.html)

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

[1.2.0]: https://github.com/TheCookieGod64/kley/compare/v1.1.0...v1.2.0
[1.1.0]: https://github.com/TheCookieGod64/kley/compare/v1.0.0...v1.1.0
[1.0.0]: https://github.com/TheCookieGod64/kley/releases/tag/v1.0.0
