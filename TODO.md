# To do

- [ ] Program structure revamp:

  Instead of having proxies representing each existing interface in UDisks2, we
  will change to a "object-very-oriented" structure, where a struct either
  represents:

  - a block device (`block_devices/`), with one `Block` interface and zero or
    more of its `Filesystem` interfaces
  - a drive device, linked to the block device, and one `Device` interface.

- [ ] Cleanup headers!!!!!!!!!!!!!!

- [ ] Automount removable media
- [ ] D-Bus activation (auto-launch on disk connect)
- [ ] Notifications
- [ ] Tray icon
- [ ] Command line tools for manual un-/mounting
- [ ] LUKS encrypted devices
- [ ] Unlocking with keyfiles (requires udisks 2.6.4)
- [ ] Loop devices (mounting iso archives)
- [ ] Password caching (requires python keyutils 0.3)

Most features taken from udiskie.

- [ ] Autolaunch or execute action upon mounting
