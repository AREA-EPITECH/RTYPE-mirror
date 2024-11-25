# RTYPE-mirror
RTYPE Epitech project mirror

# CMakeUserPresets

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