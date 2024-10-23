# Parse command-line arguments
param (
    [switch]$Clean = $false,
    [string]$Type
)

$buildDir = "./build"

if (Test-Path $buildDir) {
    if ($Clean) {
        Remove-Item -Recurse -Force "$buildDir/*"
    }
} else {
    New-Item -ItemType Directory -Path $buildDir | Out-Null
}

$cmakeArgs = @()

$buildType = "Release"
if ($Type) {
    if ($Type -eq "Debug") {
        $cmakeArgs += "-DCMAKE_BUILD_TYPE=Debug"
        $buildType = "Debug"
    }
    elseif ($Type -eq "Release") {
        $cmakeArgs += "-DCMAKE_BUILD_TYPE=Release"
    }
    else {
        Write-Error "Received build type that was not 'Debug' or 'Release'; exiting."
        exit 1
    }
}

$cmakeArgs += "-G"
$cmakeArgs += "Visual Studio 17 2022"

Set-Location $buildDir

Write-Host "Generating project using CMake with arguments: " @cmakeArgs
cmake .. @cmakeArgs
cmake --build . --config $buildType

Set-Location ..
