# Parse command-line arguments
param (
    [switch]$Clean = $false
)

$buildDir = "./build"

if (Test-Path $buildDir) {
    if ($Clean) {
        Remove-Item -Recurse -Force "$buildDir/*"
    }
} else {
    New-Item -ItemType Directory -Path $buildDir | Out-Null
}

Set-Location $buildDir
cmake .. -G "Visual Studio 17 2022"
cmake --build .
Set-Location ..
