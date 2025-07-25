# Building

## Prepare

You will need to have the
[Meson](https://mesonbuild.com/SimpleStart.html#installing-meson) build system
installed in order to build UDISKEN.

Additionally, you will need the dependencies listed in
[README.md](../README.md#Requires). Some dependencies are optional, and you can
choose to enable or disable the features that require them in `meson.options`.

Finally, using [mold] as the linker is
**recommended**:

1. Install [mold]: depends on your distribution.
2. Set `CC_LD` and `CXX_LD` to use [mold] prior to running
   `meson setup [builddir]`; run

   ```sh
   CC_LD=mold CXX_LD=mold meson setup build
   ```

### UDisks D-Bus API bindings

You absolutely need to have
[sdbus-cpp](https://github.com/Kistler-Group/sdbus-cpp) installed, which
provides the `sdbus-c++-xml2cpp` program required for this step.

1. Make sure the MAJOR version of the UDisks dependency matches your
   system-installed UDisks.
	- Otherwise you can go to the
	  [UDisks upstream repository]
2. Copy the interface description file to `dbus/`. For example:

   ```sh
    cp /usr/share/dbus-1/interfaces/org.freedesktop.UDisks2.xml dbus/
   ```

   The file's location may vary depending on your distribution.
	- In the [UDisks upstream repository], it is located at
	  [data/org.freedesktop.UDisks2.xml]
	- Alternatively, you may change the file location in the root project
	  `meson.build`

## Build

> [!NOTE]
> If you are using [mold] ([recommended](./BUILDING.md#prepare)),
> run `CC_LD=mold CXX_LD=mold meson setup build` instead of `meson setup build`.

```sh
git clone https://github.com/blackma9ick/udisken
cd udisken
meson wrap update-db
meson setup build
meson compile -C build
```

[data/org.freedesktop.UDisks2.xml]: https://github.com/storaged-project/udisks/blob/master/data/org.freedesktop.UDisks2.xml

[mold]: https://github.com/rui314/mold

[UDisks upstream repository]: https://github.com/storaged-project/udisks
