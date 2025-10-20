# Lunar 💫

<!-- Language badges are pretty cool, license and docs too -->
![Built with C++](https://img.shields.io/badge/Built_with-C%2B%2B-pink?logo=cplusplus&logoColor=%23FFFFFF)
![Scripting with Lua](https://img.shields.io/badge/Scripting_with-Lua-blue?logo=lua&logoColor=%23FFFFFF)
![License: LGPLv3](https://img.shields.io/badge/License-LGPLv3-green?logo=gnu&logoColor=%23FFFFFF)
![Docs: Doxygen](https://img.shields.io/badge/Docs-Doxygen-blueviolet?logo=readthedocs&logoColor=%23FFFFFF)

<!-- Build targets, just Windows x86_64, Linux x86_64, and MacOS-latest (arm64) -->
<!-- COMING SOON (ish) -->

<!-- Misc badges such as 'commits since release X' or 'total issues', etc.. -->
![Commits since latest release](https://img.shields.io/github/commits-since/Lunarwing-Games/lunar/latest?color=violet)
![Open issues](https://img.shields.io/github/issues/Lunarwing-Games/lunar?color=orange)
![Last updated](https://img.shields.io/github/last-commit/Lunarwing-Games/lunar?color=brightgreen)

---

Welcome to the *canonical* repository for **Lunar**, the in-house game development framework used by [Lunarwing Games](https://github.com/Lunarwing-Games).  
Lunar is written in modern C++ and supports optional **Lua scripting** for game logic.  

While optimized for **2D development** by default, Lunar is capable of **3D rendering** and general-purpose multimedia applications.

## Features

- Modular and lightweight C++ framework
- Lua integration for flexible runtime scripting
- Cross-platform support (Windows, Linux, macOS)
- 2D rendering API built on top of modern graphics backends
- Input, audio, timing, and filesystem utilities
- Extendable component system
- Minimal external dependencies
- Doxygen-powered documentation system

## How to Build (Source + Docs)
<!-- Docs use Doxygen, since that's fast and really good for organizing -->

**Source Code:**
```bash
git clone https://github.com/Lunarwing-Games/lunar.git
cd lunar
cmake -S . -B build -G (generator) (misc flags)
```

**Documentation:**
```bash
git clone https://github.com/Lunarwing-Games/lunar.git
cd lunar
doxygen Doxyfile
```

## How to Contribute

We welcome any and all community contributions! \
If you find a bug, please open up an [Issue](https://github.com/Lunarwing-Games/lunar/issues) to discuss fixes or improvements. \
Check out our **[CONTRIBUTING](./CONTRIBUTING.md)** file for full guidelines.

## Security

We take security seriously here, so if you come across one, **do not open a public issue.** \
Please email us at `lunarwinggames@outlook.com` to disclose security issues, be as detailed as possible when disclosing issues of this nature.

Email responses can take up to one week for a response.

## Licensing

Lunar is licensed under the [GNU Lesser General Public License v3.0 (LGPL-3.0)](./LICENSE). \
This means you can use and link Lunar in both **open and closed-source projects**, provided that any modifications to the Lunar framework itself are shared under the same license.

<sub>© 2025 Lunarwing Games. All rights reserved.</sub>
