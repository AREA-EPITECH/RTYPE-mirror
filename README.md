# RTYPE-mirror
<!-- ALL-CONTRIBUTORS-BADGE:START - Do not remove or modify this section -->
[![All Contributors](https://img.shields.io/badge/all_contributors-1-orange.svg?style=flat-square)](#contributors-)
<!-- ALL-CONTRIBUTORS-BADGE:END -->
___
RTYPE Epitech project mirror

# Table of Contents
___
1. [Server](./server/Server.md)
2. [Client](./client/GraphicalLibraryChoice.md)
3. [ECS](./shared/Ecs.md)
4. [Networking](./shared/Network_library.md)
5. [Doxygen](./html/index.html)
# Usage
___
### Install VCPKG
___

Follow this tutorial: [https://learn.microsoft.com/en-us/vcpkg/get_started/get-started?pivots=shell-bash](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started?pivots=shell-bash)

### CMakeUserPresets
___
You need to setup a CMake configuration file where you set up your VCPKG installation root like this:

The file is named `CMakeUserPresets.json`
```json
{
  "version": 2,
  "cmakeMinimumRequired": {
    "major": 3,
    "minor": 14,
    "patch": 0
  },
  "configurePresets": [
    {
      "name": "default",
      "inherits": "vcpkg",
      "environment": {
        "VCPKG_ROOT": "VCPKG_ROOT"
      }
    }
  ]
}
```

# Contributing
___
We welcome contributions! Please follow these steps:
1. Fork the repository
2. Create a feature branch feat-<feature-name>
3. Submit a pull request
## Contributors ✨

Thanks goes to these wonderful people ([emoji key](https://allcontributors.org/docs/en/emoji-key)):

<!-- ALL-CONTRIBUTORS-LIST:START - Do not remove or modify this section -->
<!-- prettier-ignore-start -->
<!-- markdownlint-disable -->
<table>
  <tbody>
    <tr>
      <td align="center" valign="top" width="14.28%"><a href="https://github.com/maxencelupion"><img src="https://avatars.githubusercontent.com/u/114016583?v=4?s=100" width="100px;" alt="Maxence Lupion"/><br /><sub><b>Maxence Lupion</b></sub></a><br /><a href="https://github.com/AREA-EPITECH/RTYPE-mirror/commits?author=maxencelupion" title="Code">💻</a> <a href="#projectManagement-maxencelupion" title="Project Management">📆</a></td>
    </tr>
  </tbody>
</table>

<!-- markdownlint-restore -->
<!-- prettier-ignore-end -->

<!-- ALL-CONTRIBUTORS-LIST:END -->

This project follows the [all-contributors](https://github.com/all-contributors/all-contributors) specification. Contributions of any kind welcome!