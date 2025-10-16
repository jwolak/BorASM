#!/bin/bash

#===============================================================================
#                    BorASM - Universal Build Script (Debug & Release)
#===============================================================================
# This script builds the BorASM project in Debug and/or Release mode
# Creates separate binary directories for each build type
# Author: Auto-generated
# Version: 1.0
#===============================================================================

# Color definitions for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
MAGENTA='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Script configuration
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
BUILD_BASE_DIR="$PROJECT_ROOT/build"
BINARIES_DIR="$PROJECT_ROOT/binaries"

# Build configurations
DEBUG_BUILD_DIR="$BUILD_BASE_DIR/debug"
RELEASE_BUILD_DIR="$BUILD_BASE_DIR/release"
DEBUG_BIN_DIR="$BINARIES_DIR/debug"
RELEASE_BIN_DIR="$BINARIES_DIR/release"

# Function to print colored messages
print_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

print_section() {
    echo -e "\n${MAGENTA}===============================================================================${NC}"
    echo -e "${MAGENTA} $1${NC}"
    echo -e "${MAGENTA}===============================================================================${NC}"
}

print_subsection() {
    echo -e "\n${CYAN}--- $1 ---${NC}"
}

# Function to check if command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Function to clean build directories
clean_builds() {
    local build_type="$1"
    
    case "$build_type" in
        "debug")
            if [ -d "$DEBUG_BUILD_DIR" ]; then
                print_info "Cleaning Debug build directory: $DEBUG_BUILD_DIR"
                rm -rf "$DEBUG_BUILD_DIR"
            fi
            if [ -d "$DEBUG_BIN_DIR" ]; then
                print_info "Cleaning Debug binaries directory: $DEBUG_BIN_DIR"
                rm -rf "$DEBUG_BIN_DIR"
            fi
            ;;
        "release")
            if [ -d "$RELEASE_BUILD_DIR" ]; then
                print_info "Cleaning Release build directory: $RELEASE_BUILD_DIR"
                rm -rf "$RELEASE_BUILD_DIR"
            fi
            if [ -d "$RELEASE_BIN_DIR" ]; then
                print_info "Cleaning Release binaries directory: $RELEASE_BIN_DIR"
                rm -rf "$RELEASE_BIN_DIR"
            fi
            ;;
        "all")
            print_info "Cleaning all build directories"
            [ -d "$BUILD_BASE_DIR" ] && rm -rf "$BUILD_BASE_DIR"
            [ -d "$BINARIES_DIR" ] && rm -rf "$BINARIES_DIR"
            ;;
    esac
    print_success "Cleaning completed"
}

# Function to create directories
create_directories() {
    local build_type="$1"
    
    case "$build_type" in
        "debug")
            mkdir -p "$DEBUG_BUILD_DIR"
            mkdir -p "$DEBUG_BIN_DIR"
            print_success "Debug directories created"
            ;;
        "release")
            mkdir -p "$RELEASE_BUILD_DIR"
            mkdir -p "$RELEASE_BIN_DIR"
            print_success "Release directories created"
            ;;
        "all")
            mkdir -p "$DEBUG_BUILD_DIR"
            mkdir -p "$RELEASE_BUILD_DIR"
            mkdir -p "$DEBUG_BIN_DIR"
            mkdir -p "$RELEASE_BIN_DIR"
            print_success "All directories created"
            ;;
    esac
}

# Function to configure cmake
configure_cmake() {
    local build_type="$1"
    local build_dir=""
    
    case "$build_type" in
        "Debug")
            build_dir="$DEBUG_BUILD_DIR"
            ;;
        "Release")
            build_dir="$RELEASE_BUILD_DIR"
            ;;
    esac
    
    print_info "Configuring CMake for $build_type build..."
    cd "$build_dir"
    
    cmake \
        -DCMAKE_BUILD_TYPE="$build_type" \
        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
        -DPROJECT_TESTS=OFF \
        "$PROJECT_ROOT"
    
    if [ $? -eq 0 ]; then
        print_success "CMake configuration for $build_type completed successfully"
        return 0
    else
        print_error "CMake configuration for $build_type failed"
        return 1
    fi
}

# Function to build project
build_project() {
    local build_type="$1"
    local build_dir=""
    local bin_dir=""
    
    case "$build_type" in
        "Debug")
            build_dir="$DEBUG_BUILD_DIR"
            bin_dir="$DEBUG_BIN_DIR"
            ;;
        "Release")
            build_dir="$RELEASE_BUILD_DIR"
            bin_dir="$RELEASE_BIN_DIR"
            ;;
    esac
    
    print_info "Building BorASM in $build_type mode..."
    
    cd "$build_dir"
    
    # Use all available CPU cores for compilation
    NPROC=$(nproc)
    print_info "Using $NPROC CPU cores for compilation"
    
    make -j$NPROC
    
    if [ $? -eq 0 ]; then
        print_success "$build_type build completed successfully!"
        
        # Copy binary to binaries directory
        # Binary is now built in the build directory itself
        if [ -f "$build_dir/BorASM" ]; then
            cp "$build_dir/BorASM" "$bin_dir/BorASM"
            print_success "Binary copied to: $bin_dir/BorASM"
            
            # Create version info file
            create_version_info "$build_type" "$bin_dir"
        else
            print_warning "Binary not found at: $build_dir/BorASM"
            # Try alternative locations
            if [ -f "$PROJECT_ROOT/BorASM" ]; then
                cp "$PROJECT_ROOT/BorASM" "$bin_dir/BorASM"
                print_success "Binary found and copied from: $PROJECT_ROOT/BorASM"
                create_version_info "$build_type" "$bin_dir"
            else
                print_error "Binary not found in any expected location"
            fi
        fi
        
        return 0
    else
        print_error "$build_type build failed!"
        return 1
    fi
}

# Function to create version info file
create_version_info() {
    local build_type="$1"
    local bin_dir="$2"
    local info_file="$bin_dir/build_info.txt"
    
    cat > "$info_file" << EOF
BorASM Build Information
=======================
Build Type: $build_type
Build Date: $(date)
Build Host: $(hostname)
Compiler: $(gcc --version | head -n1)
CMake Version: $(cmake --version | head -n1)
Git Branch: $(git rev-parse --abbrev-ref HEAD 2>/dev/null || echo "Unknown")
Git Commit: $(git rev-parse --short HEAD 2>/dev/null || echo "Unknown")
Built by: $(whoami)

Binary Location: $bin_dir/BorASM
EOF
    
    print_info "Build info saved to: $info_file"
}

# Function to show build summary
show_build_summary() {
    local debug_built="$1"
    local release_built="$2"
    
    print_section "BUILD SUMMARY"
    
    if [ "$debug_built" = true ]; then
        echo -e "${GREEN}✓${NC} Debug build:   ${YELLOW}$DEBUG_BIN_DIR/BorASM${NC}"
        if [ -f "$DEBUG_BIN_DIR/BorASM" ]; then
            local debug_size=$(du -h "$DEBUG_BIN_DIR/BorASM" | cut -f1)
            echo -e "  Size: $debug_size"
        fi
    fi
    
    if [ "$release_built" = true ]; then
        echo -e "${GREEN}✓${NC} Release build: ${YELLOW}$RELEASE_BIN_DIR/BorASM${NC}"
        if [ -f "$RELEASE_BIN_DIR/BorASM" ]; then
            local release_size=$(du -h "$RELEASE_BIN_DIR/BorASM" | cut -f1)
            echo -e "  Size: $release_size"
        fi
    fi
    
    echo ""
    echo -e "To run Debug:   ${CYAN}$DEBUG_BIN_DIR/BorASM${NC}"
    echo -e "To run Release: ${CYAN}$RELEASE_BIN_DIR/BorASM${NC}"
}

# Function to show build info
show_build_info() {
    print_section "BUILD CONFIGURATION"
    echo -e "Project Root:        ${YELLOW}$PROJECT_ROOT${NC}"
    echo -e "Build Base Dir:      ${YELLOW}$BUILD_BASE_DIR${NC}"
    echo -e "Binaries Dir:        ${YELLOW}$BINARIES_DIR${NC}"
    echo -e "Debug Build Dir:     ${YELLOW}$DEBUG_BUILD_DIR${NC}"
    echo -e "Release Build Dir:   ${YELLOW}$RELEASE_BUILD_DIR${NC}"
    echo -e "Debug Binaries:      ${YELLOW}$DEBUG_BIN_DIR${NC}"
    echo -e "Release Binaries:    ${YELLOW}$RELEASE_BIN_DIR${NC}"
    echo -e "Compiler:            ${YELLOW}$(gcc --version | head -n1)${NC}"
}

# Function to show usage
show_usage() {
    echo "Usage: $0 [OPTIONS] [BUILD_TYPE]"
    echo ""
    echo "Build Types:"
    echo "  debug       Build only Debug version"
    echo "  release     Build only Release version"
    echo "  all         Build both Debug and Release versions (default)"
    echo ""
    echo "Options:"
    echo "  -c, --clean     Clean build directories before building"
    echo "  -h, --help      Show this help message"
    echo "  -v, --verbose   Enable verbose output"
    echo "  --info          Show build configuration and exit"
    echo ""
    echo "Examples:"
    echo "  $0                    # Build both Debug and Release"
    echo "  $0 debug              # Build only Debug version"
    echo "  $0 release            # Build only Release version"
    echo "  $0 --clean all        # Clean and build both versions"
    echo "  $0 --verbose debug    # Build Debug with verbose output"
}

# Main execution
main() {
    local BUILD_TYPE="all"
    local CLEAN_BUILD=false
    local VERBOSE=false
    local SHOW_INFO=false
    
    # Parse command line arguments
    while [[ $# -gt 0 ]]; do
        case $1 in
            debug|release|all)
                BUILD_TYPE="$1"
                shift
                ;;
            -c|--clean)
                CLEAN_BUILD=true
                shift
                ;;
            -v|--verbose)
                VERBOSE=true
                shift
                ;;
            --info)
                SHOW_INFO=true
                shift
                ;;
            -h|--help)
                show_usage
                exit 0
                ;;
            *)
                print_error "Unknown option: $1"
                show_usage
                exit 1
                ;;
        esac
    done
    
    # Show info and exit if requested
    if [ "$SHOW_INFO" = true ]; then
        show_build_info
        exit 0
    fi
    
    # Enable verbose mode if requested
    if [ "$VERBOSE" = true ]; then
        set -x
    fi
    
    print_section "BorASM Universal Build Script"
    
    # Check prerequisites
    if ! command_exists cmake; then
        print_error "CMake is not installed. Please install CMake first."
        exit 1
    fi
    
    if ! command_exists make; then
        print_error "Make is not installed. Please install build-essential."
        exit 1
    fi
    
    if ! command_exists gcc; then
        print_error "GCC compiler is not installed. Please install build-essential."
        exit 1
    fi
    
    # Show build information
    show_build_info
    
    # Variables to track what was built
    local debug_built=false
    local release_built=false
    
    # Clean if requested
    if [ "$CLEAN_BUILD" = true ]; then
        clean_builds "$BUILD_TYPE"
    fi
    
    # Build based on selected type
    case "$BUILD_TYPE" in
        "debug")
            print_subsection "Building Debug Version"
            create_directories "debug"
            if configure_cmake "Debug" && build_project "Debug"; then
                debug_built=true
            else
                exit 1
            fi
            ;;
        "release")
            print_subsection "Building Release Version"
            create_directories "release"
            if configure_cmake "Release" && build_project "Release"; then
                release_built=true
            else
                exit 1
            fi
            ;;
        "all")
            create_directories "all"
            
            print_subsection "Building Debug Version"
            if configure_cmake "Debug" && build_project "Debug"; then
                debug_built=true
            else
                print_error "Debug build failed, stopping..."
                exit 1
            fi
            
            print_subsection "Building Release Version"
            if configure_cmake "Release" && build_project "Release"; then
                release_built=true
            else
                print_error "Release build failed, but Debug succeeded"
                exit 1
            fi
            ;;
    esac
    
    # Show final summary
    show_build_summary "$debug_built" "$release_built"
    
    print_section "BUILD COMPLETED SUCCESSFULLY"
    print_success "All requested builds completed successfully!"
}

# Run main function with all arguments
main "$@"
