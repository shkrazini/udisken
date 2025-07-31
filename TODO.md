# To do

## Daemon

- [x] Automount removable media
- [x] Notifications
- [x] Loop devices (mounting iso archives)
      Basically already handled by existing automount logic.
- [ ] Autolaunch or execute action upon disk connect/mounting (using D-Bus
      activation)
- [ ] Unlocking with keyfiles (requires udisks 2.6.4)
- [ ] LUKS encrypted devices
- [ ] Password caching (requires python keyutils 0.3)
- [ ] Tray icon

## Commands

- [ ] Command line tools for manual un-/mounting
- [ ] Shell completion (Zsh, Fish, Bash)
