# udisken

An automounter that stays quite quiet.

## Requires

- [libnotify](https://gitlab.gnome.org/GNOME/libnotify) 0.8.4 or later
- [sdbus-c++](https://github.com/Kistler-Group/sdbus-cpp) 2.1.0 or later
- [UDisks](https://github.com/storaged-project/udisks) 2.10.0 or later

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
aurhelperx -S udisken
```

Dependencies are automatically taken care of. You can still install them with:

```sh
pacman -S --asdeps libnotify sdbus-cpp udisks2
```

## Building

See [BUILDING.md](./docs/BUILDING.md).

## Nitpick

> Why don't you just use `libudisks2` (yes that exists, nowhere mentioned on
> Freedesktop.org's landing page)?

The D-Bus API allows for a more C++-style of doing things in the source,
so we use that instead. Using the library means we have to use GObjects,
which is kind of conflicting with C++'s preexisting class features.

It also eliminates the need for a C++ wrapper library. Just generate bindings
from the interface IDL document (in XML format) and done.

Even still, the `libudisks2` C library itself is mostly generated code: it's the
same thing we're doing here.

It almost certainly incurs a large performance penalty compared to linking
against `libudisks2`, but an automounter doesn't need to be this fast:
udisken was created with a light footprint in mind first and foremost.

...but this is copium, after I spent 2 days on reading about the D-Bus
library we use and almost finding out it wasn't needed
(still it's awesome btw, definitely go to [sdbus-c++])...

[sdbus-c++]: https://github.com/Kistler-Group/sdbus-cpp

## License

Licensed under the [GNU General Public License Version 3](./LICENSE) (GPLv3).
