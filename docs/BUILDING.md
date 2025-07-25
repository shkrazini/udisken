# Building

## Prepare

You will need to have the
[Meson](https://mesonbuild.com/SimpleStart.html#installing-meson) build system
installed in order to build UDISKEN.

Additionally, you will need the dependencies listed in
[README.md](../README.md#Requires). Some dependencies are optional, and you can
choose to enable or disable the features that require them in `meson.options`.

Finally, using [mold] as linker is
**recommended**:

1. Install [mold]: depends on your distribution.
2. Set `CC_LD` and `CXX_LD` to use [mold] prior to running
   `meson setup [builddir]`; run

   ```sh
   CC_LD=mold CXX_LD=mold meson setup build
   ```

### UDisks D-Bus API bindings

Laborious, but not much other choice. Ideally UDisks would provide the bindings
by itself.

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

#### Discussion detail about retrieving the interface file

Feel free to ignore.

We need to easily obtain the IDL file describing UDisks's D-Bus API,
but the solutions are only less than ideal:

1. Read the interface file on a common path, and if it fails, bail out
   gracefully.
2. Build UDisks in our project and retrieve the interface file from there.
3. Call quits, and copy-paste the interface file manually occasionally like a
   caveman... Could at least subscribe to news and changes on UDisks's repo.

Even if we find a solution still, how do we keep track of (breaking) changes
between interface file updates? No obvious solution exists, so we should only
manually update the file from time to time.

This is what we'll do: keep track of changes in UDisks's D-Bus API, and
decide to update the interface file (maybe version it?).

## Build

> [!NOTE]
> If you are using [mold] ([recommended](./BUILDING.md#prepare)),
> run `CC_LD=mold CXX_LD=mold meson setup build` instead of `meson setup build`.

```sh
git clone https://codeberg.org/xlacroixx/udisken
cd udisken
meson wrap update-db
meson setup build
meson compile -C build
```

[data/org.freedesktop.UDisks2.xml]: https://github.com/storaged-project/udisks/blob/master/data/org.freedesktop.UDisks2.xml
[mold]: https://github.com/rui314/mold
[UDisks upstream repository]: https://github.com/storaged-project/udisks
