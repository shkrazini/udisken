# udisken

An automounter that stays quite quiet.

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
