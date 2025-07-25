# Building

## Prepare

You will need to have the
[Meson](https://mesonbuild.com/SimpleStart.html#installing-meson) build system
installed in order to build udisken.

Additionally, you will need the dependencies listed in
[README.md](../README.md#Requires).

### UDisks D-Bus API bindings

Laborious, but not much other choice. Ideally UDisks would provide the bindings
by itself.

You absolutely need to have
[sdbus-cpp](https://github.com/Kistler-Group/sdbus-cpp) installed, which
provides the `sdbus-c++-xml2cpp` program required for this step.

1. Make sure the version of the UDisks dependency matches your system-installed
   UDisks.
   - Otherwise you can go to the
     [UDisks upstream repository]
2. Copy the interface description file to `dbus/`. For example:

   ```sh
    cp /usr/share/dbus-1/interfaces/org.freedesktop.UDisks2.xml dbus/
   ```

   The file's location may vary depending on your distribution.

   - In the [UDisks upstream repository], it is located at
     [data/org.freedesktop.UDisks2.xml]

3. Update the version in `dbus/version.txt` accordingly

   <!-- TODO(xlacroixx): confirm this is required -->

4. Run `meson setup --reconfigure`, and `meson compile -C build` (assuming your
   current directory is at the project root) at least once, in order to generate
   the D-Bus API bindings.

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

```sh
git clone https://codeberg.org/xlacroixx/udisken
cd udisken
meson setup build
meson compile -C build
```

[data/org.freedesktop.UDisks2.xml]: https://github.com/storaged-project/udisks/blob/master/data/org.freedesktop.UDisks2.xml
[UDisks upstream repository]: https://github.com/storaged-project/udisks
