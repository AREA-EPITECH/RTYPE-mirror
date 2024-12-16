# RTYPE-mirror
___
RTYPE Epitech project mirror

# Table of Contents
___
1. [Server](./server/Server.md)
2. [Client](./client/GraphicalLibraryChoice.md)
3. [ECS](./shared/Ecs.md)
4. [Networking](./shared/Network_library.md)

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