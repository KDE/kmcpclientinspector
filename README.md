# KMcpClientInspector

Application to inspect and debug servers implementing
the Model Context Protocol (MCP).

## Requirements

`cmake` >= 3.29, `extra-cmake-modules`, Qt >= 6.10,
`kcoreaddons`, `ki18n`, `kxmlgui`, `kcrash`, `knotifications`,
`knotifyconfig`, `kiconthemes`, `syntax-highlighting`, `kitemviews`,
`ktextaddons` >= 2.2.82 (with MCP protocol support)

Optional: `kdoctools` (documentation)

## Building

````bash
mkdir build
cd build
cmake ..
make
make install
````

or using CMakePresets support

````bash
cmake --preset dev && cmake --build --preset dev
````

or (using mold linker)

````bash
cmake --preset dev-mold && cmake --build --preset dev-mold
````

## ASAN

Use the `sanitizers.supp` file to suppress known leaks, for example:

````bash
LSAN_OPTIONS=suppressions=../sanitizers.supp ./bin/kmcpclientinspector
````

## UNITY

KMcpClientInspector can be built with CMake unity build support:

````bash
cmake --preset unity && cmake --build --preset unity
````

## Contributing

Contributions are welcome. We use [invent.kde.org](https://invent.kde.org/).

The [KDE Community Code of Conduct](https://kde.org/code-of-conduct) is applied.

## License

This project is licensed under the GPL-2.0-or-later.
More information can be found in the `LICENSES` folder.

## Maintainer

- Laurent Montel <montel@kde.org>
