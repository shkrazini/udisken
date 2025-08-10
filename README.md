# UDISKEN

LINUX DRIVE AUTOMOUNTER FOR THE IMPATIENT.

---

A small GNU/Linux desktop removable media mounting daemon.

Mount automatically your...:

- USB drive
- CD or DVD disk
- encrypted drive
- loop devices

Low on memory usage, lightweight footprint, fast.

## Requires

- [sdbus-c++] 2.1.0 or later
- [spdlog] 1.15.0 or later
- [UDisks] 2.10.0 or later
- [libnotify] 0.8.0 or later (optional)
- [argparse] 3.1 or later (building only)

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

## Getting started

### Running

<!-- TODO(blackma9ick): add systemd service instructions. -->

Simply run the daemon like this:

```sh
udisken
```

### Configuring

UDISKEN takes a few command arguments.
These are higher priority than environment variables (see below).

Disabling notifications:

```sh
udisken --no-notify
```

UDISKEN also reads from some environment variables.
These are lower priority than command flags.

Disabling notifications:

```sh
UDISKEN_NO_NOTIFY=1 udisken
```

## Building

### Prepare

You will need to have the [Meson] build system installed in order to build
UDISKEN.

Additionally, you will need the dependencies [listed above](#requires). Some
dependencies are optional, and you can choose to enable or disable the features
that require them in `meson.options`.

Finally, using [mold] as the linker is
**recommended**:

1. Install [mold]: depends on your distribution.
2. Set `CC_LD` and `CXX_LD` to use [mold] prior to running
   `meson setup [builddir]`; run

   ```sh
   CC_LD=mold CXX_LD=mold meson setup build
   ```

> [!IMPORTANT]
> Make sure that the D-Bus API interface file option is correct before running
> this command: see [UDisks D-Bus API bindings](#udisks-d-bus-api-bindings)

#### UDisks D-Bus API bindings

You absolutely need to have [sdbus-c++] installed, which provides the
`sdbus-c++-xml2cpp` program required for this step.

Simply make sure that the option `udisks_dbus_interface` in the project root
`meson.build`, points to the correct D-Bus interface XML file: this
corresponds to the Meson option being yielded to, as mentioned in
udisks-sdbus-c++'s README file.
Alternatively, you can provide the `udisks_dbus_interface` option when running
`meson setup`:

```sh
meson setup build -Dudisks_dbus_interface=/path/to/org.freedesktop.UDisks2.xml
```

The file's location may vary depending on your distribution.

- Usually, it is located in the `/usr/share/dbus-1/interfaces/`
  system directory.
- In the [UDisks] upstream repository, it is located at
  [data/org.freedesktop.UDisks2.xml]
- Alternatively, you may change the file location in the root project
  `meson.build`

For more information, see [udisks-sdbus-c++].

### Build

> [!TIP]
> If you want to use [mold] ([recommended](#prepare)),
> run `CC_LD=mold CXX_LD=mold meson setup build` instead of `meson setup build`.

```sh
git clone https://github.com/blackma9ick/udisken
cd udisken
meson wrap update-db
meson setup build
meson compile -C build
```

## Copyright

Licensed under the [GNU General Public License Version 3](./LICENSE) (GPLv3).

Copyright © 2025 BlackMa9ick.

---

_~BlackMa9ick (black magic)_

[argparse]: https://github.com/p-ranav/argparse
[data/org.freedesktop.UDisks2.xml]: https://github.com/storaged-project/udisks/blob/master/data/org.freedesktop.UDisks2.xml
[libnotify]: https://gitlab.gnome.org/GNOME/libnotify
[Meson]: https://mesonbuild.com/SimpleStart.html#installing-meson
[mold]: https://github.com/rui314/mold
[sdbus-c++]: https://github.com/Kistler-Group/sdbus-cpp
[spdlog]: https://github.com/gabime/spdlog
[udisks-sdbus-c++]: https://github.com/blackma9ick/udisks-sdbus-cpp
[UDisks]: https://github.com/storaged-project/udisks
