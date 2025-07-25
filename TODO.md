# To do

Post-study of udiskie:

- General functions for utility (mount, unmount, etc)
  Act on interface references, do error handling, logging...
  Put in their own file even?
- Do not do things in the constructors... Just, don't.
- Object classes (BlockDevice, Drive, ...) and (all) their interfaces pointers
- Implement as feature-macro'd functions:
  - Notifications
  - Tray icon

- [x] Automount removable media
- [x] Notifications
- [ ] Loop devices (mounting iso archives)
- [ ] Command line tools for manual un-/mounting
- [ ] Autolaunch or execute action upon disk connect/mounting (using D-Bus
      activation)
- [ ] Unlocking with keyfiles (requires udisks 2.6.4)
- [ ] LUKS encrypted devices
- [ ] Password caching (requires python keyutils 0.3)
- [ ] Tray icon

Most features taken from udiskie.

- [ ] Cleanup headers!!!!!!!!!!!!!!
