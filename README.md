# udisken

A Linux automounter that stays quite quiet.

## Requires

- [UDisks](https://github.com/storaged-project/udisks) 2.10.0 or later
- [sdbus-c++](https://github.com/Kistler-Group/sdbus-cpp) 2.1.0 or later
- [libnotify](https://gitlab.gnome.org/GNOME/libnotify) 0.8.4 or later

## Getting started

The project consists of two main parts:

- the daemon: `udiskend`
- the CLI interface/utilities: `udisken COMMAND`, transparent with udiskie:
  - `udisken eject`
  - `udiskie-mount` -> `udisken mount`
  - `udiskie-unmount` -> `udisken unmount` (alias: `udisken umount`)
  - `udiskie-info` -> `udisken show` (alias: `udisken info`)
- (soon) the (?) toolkit interface: `udiskengtk`

The CLI interface uses udisken's D-Bus front end to communicate with it (?).

## Install

### Arch Linux User Repository (AUR)

<!-- NOTE: not published yet so these links are fake. -->

Available as [udisken](https://aur.archlinux.org/packages/udisken),
[udisken-bin](https://aur.archlinux.org/packages/udisken-bin) (binary releases)
or [udisken-git](https://aur.archlinux.org/packages/udisken-git)
(development branch).

```sh
paru -S udisken
# ...or...
yay -S udisken
# ...or...
aurwrapperx -S udisken
```

Dependencies are automatically taken care of. You can still install them with:

```sh
pacman -S --asdeps libnotify sdbus-cpp udisks2
```

## Building

See [BUILDING.md](./docs/BUILDING.md).

## License

Licensed under the [GNU General Public License Version 3](./LICENSE) (GPLv3).
