// swift-tools-version: 5.9

import PackageDescription

let package = Package(
    name: "appaudio",
    platforms: [.iOS(.v14)],
    products: [
        .executable(name: "appaudio", targets: ["appaudio"])
    ],
    targets: [
        .target(
            name: "CAudio",
            path: "Sources/CAudio",
            publicHeadersPath: ".",
            linkerSettings: [
                .linkedFramework("CoreAudio", .when(platforms: [.iOS, .macOS])),
                .linkedFramework("CoreFoundation", .when(platforms: [.iOS, .macOS])),
                .linkedFramework("AudioToolbox", .when(platforms: [.iOS])),
                .linkedLibrary("m", .when(platforms: [.iOS])),
                .linkedLibrary("pthread", .when(platforms: [.iOS])),
            ]
        ),
        .executableTarget(
            name: "appaudio",
            dependencies: ["CAudio"],
            path: "Sources/appaudio"
        ),
    ]
)
