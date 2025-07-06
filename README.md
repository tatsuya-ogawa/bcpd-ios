# BCPD iOS App

A Bayesian Coherent Point Drift (BCPD) implementation for iOS devices, providing advanced point cloud registration capabilities.

## Overview

This repository contains an iOS application that implements the Bayesian Coherent Point Drift algorithm for point cloud registration. The original BCPD library was designed for macOS with dynamic library (.dylib) support. This project adapts it for iOS by building static libraries with all necessary dependencies.

## Features

- **Point Cloud Registration**: Advanced Bayesian approach to point cloud alignment
- **iOS Native Integration**: Objective-C wrapper for seamless iOS integration
- **Static Library Build**: Self-contained static libraries for iOS deployment
- **Cross-platform Dependencies**: Includes OpenMP and OpenBLAS for iOS

## Project Structure

```
bcpd-ios/
├── bcpd-ios-app/              # iOS application project
│   ├── bcpd-ios-app/          # Main app source code
│   ├── bcpd-ios-app.xcodeproj/ # Xcode project files
│   └── bcpd-lib/              # BCPD Objective-C wrapper
├── build_staticlib/           # Static library build system
│   ├── bcpd/                  # Core BCPD algorithm
│   ├── ios_dependencies/      # iOS-specific dependencies
│   └── src/                   # Library source files
└── README.md
```

## Building

### Prerequisites

- Xcode 12.0 or later
- iOS 12.0 or later
- CMake (for building dependencies)

### Build Instructions

1. **Build Dependencies**:
   ```bash
   cd build_staticlib
   ./build_dependency.sh
   ```

2. **Build BCPD Static Library**:
   ```bash
   make -f Makefile
   ```

3. **Open and Build iOS App**:
   - Open `bcpd-ios-app/bcpd-ios-app.xcodeproj` in Xcode
   - Select your target device or simulator
   - Build and run the project

## Usage

The iOS app provides an Objective-C interface to the BCPD algorithm:

```objc
BCPD *bcpd = [[BCPD alloc] init];
NSDictionary *results;

int status = [bcpd execute_bcpWithTargetPoints:targetPoints 
                                   targetCount:targetCount
                                  sourcePoints:sourcePoints 
                                   sourceCount:sourceCount
                                     dimension:dimension
                               targetFeatures:targetFeatures 
                            featureDimension:featureDimension
                               sourceFeatures:sourceFeatures
                                    parameters:&parameters
                                     sizeInfo:&sizeInfo
                                      results:&results];
```

## Dependencies

- **OpenBLAS**: Linear algebra operations
- **OpenMP**: Parallel processing support
- **BCPD Core**: Original BCPD algorithm implementation

## Citation

This project is based on the original BCPD implementation:

```
@software{bcpd_original,
  title = {Bayesian Coherent Point Drift},
  author = {Hirose, Osamu},
  url = {https://github.com/ohirose/bcpd},
  year = {2021}
}
```

## License

Please refer to the original BCPD license terms in the `build_staticlib/bcpd/LICENSE.md` file.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## Support

For issues related to the iOS implementation, please open an issue in this repository.
For questions about the core BCPD algorithm, please refer to the [original repository](https://github.com/ohirose/bcpd).

