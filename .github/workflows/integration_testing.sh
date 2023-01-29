name: "Build, Analyse, and Run Integration Tests"

on:
  pull_request:
    branches: [ "main" ]
  schedule:
    - cron: '21 8 * * 0'

env:
  # Customize the CMake build type here (Release, Debug, RelWithDebInfo, etc.)
  BUILD_TYPE: Release

jobs:
  IntegrationTesting:
    runs-on: ubuntu-latest
    permissions:
      actions: read
      contents: read
      security-events: write
    
    strategy:
      fail-fast: false
      matrix:
        language: [ 'cpp' ]
        os: [ubuntu-latest, windows-latest]
    runs-on: ${{ matrix.os }}

    steps:
    - name: Checkout repository
      uses: actions/checkout@v3
    
    - name: Initialize CodeQL
      if: matrix.os == ubuntu-latest
      uses: github/codeql-action/init@v2
      with:
        languages: 'cpp'

    - name: Configure CMake
      run: cmake -B ${{ github.workspace }}/build -DCMAKE_BUILD_TYPE=${{ env.BUILD_TYPE }}

    - name: Build Application
      run: cmake --build ${{ github.workspace }}/build --config ${{ env.BUILD_TYPE }} --target dndmanager

    - name: Run Integration Test - Imperial
      working-directory: ${{ github.workspace }}
      run: ./build/dndmanager -c example_campaign -d ${{ github.workspace }}/content_imperial

    - name: Run Integration Test - Metric
      working-directory: ${{ github.workspace }}
      run: ./build/dndmanager -c example_campaign -d ${{ github.workspace }}/content_metric

    - name: Perform CodeQL Analysis
      if: matrix.os == ubuntu-latest
      uses: github/codeql-action/analyze@v2
      with:
        category: "/language:'cpp'"

    
