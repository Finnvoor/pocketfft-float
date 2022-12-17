// swift-tools-version: 5.7

import PackageDescription

let package = Package(
    name: "swift-pocketfft-float",
    products: [
        .library(
            name: "PocketFFT",
            targets: ["PocketFFT"]),
    ],
    targets: [
        .target(
            name: "PocketFFT",
            dependencies: []),
        .testTarget(
            name: "PocketFFTTests",
            dependencies: ["PocketFFT"])
    ]
)
