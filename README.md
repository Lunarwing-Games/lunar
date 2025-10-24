# Lunar 💫

<!-- Language badges are pretty cool, license and docs too -->
![Built with C++](https://img.shields.io/badge/Built_with-C%2B%2B-pink?logo=cplusplus&logoColor=%23FFFFFF)
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
Lunar is written in modern C++. 

While optimized for **2D development** by default, Lunar is capable of **3D rendering** for games development.

## Features

- Modular and lightweight C++ framework
- Cross-platform support (Windows, Linux, macOS)
- 2D rendering API built on top of modern graphics backends
- Input, audio, timing, and filesystem utilities
- Extendable component system
- Minimal external dependencies
- Doxygen-powered documentation system

## How to Build (Source + Docs)
<!-- Docs use Doxygen, since that's fast and really good for organizing -->

> [!NOTE]
> You may need to change some of the directories coded into the `CMakeLists.txt` file! It expects the Steamworks SDK (not available through Lunar) to be at the users HOME directory. You may need to change that to fit your environment!

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

We take security seriously here. If you come across a potential security issue, **do not open a public issue.** Please email us at `lunarwinggames@outlook.com` to disclose security issues, be as detailed as possible when disclosing issues of this nature.

Email responses can take up to one week for a response.

## Licensing

Lunar is licensed under the [GNU Lesser General Public License v2.1 (LGPL-2.1)](./LICENSE). \
This means you can use and link Lunar in both **open and closed-source projects**, provided that any modifications to the Lunar framework itself are shared under the same license.

<sub>© 2025 Lunarwing Games. All rights reserved.</sub>

## Third-Parties

Lunar uses the following open-source software:
 * SDL3 $\rightarrow$ zlib license $\rightarrow$ © 1997–2025 Sam Lantinga
 * ImGui $\rightarrow$ MIT license $\rightarrow$ © 2014–2025 Ocornut

Licenses for the aforementioned softwares are available in the `/licenses/` directory.

