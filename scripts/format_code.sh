#!/bin/bash

#===============================================================================
#                           BorASM Code Formatter Script
#===============================================================================
# Script to format C++ code according to project style guide
# Uses clang-format with Google C++ Style Guide (customized)
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

# Default settings
DRY_RUN=false
VERBOSE=false
RECURSIVE=true
BACKUP=false

# File extensions to format
EXTENSIONS=("cpp" "cc" "cxx" "c++" "c" "h" "hpp" "hxx" "h++")

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

# Function to check if clang-format is available
check_clang_format() {
    if ! command -v clang-format >/dev/null 2>&1; then
        print_error "clang-format is not installed!"
        echo "To install on Ubuntu/Debian: sudo apt-get install clang-format"
        exit 1
    fi
    
    local version=$(clang-format --version | grep -oE '[0-9]+\.[0-9]+' | head -1)
    print_info "Using clang-format version: $version"
}

# Function to build file pattern for find
build_file_pattern() {
    local pattern=""
    for ext in "${EXTENSIONS[@]}"; do
        if [ -n "$pattern" ]; then
            pattern="$pattern -o"
        fi
        pattern="$pattern -name '*.${ext}'"
    done
    echo "$pattern"
}

# Function to find source files
find_source_files() {
    local search_path="$1"
    local pattern=$(build_file_pattern)
    
    if [ "$RECURSIVE" = true ]; then
        eval "find '$search_path' -type f \( $pattern \)" | sort
    else
        eval "find '$search_path' -maxdepth 1 -type f \( $pattern \)" | sort
    fi
}

# Function to format a single file
format_file() {
    local file="$1"
    local relative_path="${file#$PROJECT_ROOT/}"
    
    if [ "$VERBOSE" = true ]; then
        print_info "Processing: $relative_path"
    fi
    
    # Create backup if requested
    if [ "$BACKUP" = true ]; then
        cp "$file" "$file.bak"
        if [ "$VERBOSE" = true ]; then
            print_info "Backup created: $relative_path.bak"
        fi
    fi
    
    # Format the file
    if [ "$DRY_RUN" = true ]; then
        # Dry run - show what would be changed
        clang-format --style=file "$file" > /tmp/formatted_output
        if ! diff -q "$file" /tmp/formatted_output >/dev/null; then
            echo -e "${YELLOW}[WOULD FORMAT]${NC} $relative_path"
        elif [ "$VERBOSE" = true ]; then
            echo -e "${GREEN}[NO CHANGE]${NC} $relative_path"
        fi
        rm -f /tmp/formatted_output
    else
        # Actually format the file
        local original_size=$(stat -f%z "$file" 2>/dev/null || stat -c%s "$file")
        clang-format -i --style=file "$file"
        local new_size=$(stat -f%z "$file" 2>/dev/null || stat -c%s "$file")
        
        if [ "$original_size" != "$new_size" ] || [ "$VERBOSE" = true ]; then
            echo -e "${GREEN}[FORMATTED]${NC} $relative_path"
        fi
    fi
}

# Function to format all files in a directory
format_directory() {
    local target_dir="$1"
    
    if [ ! -d "$target_dir" ]; then
        print_error "Directory does not exist: $target_dir"
        return 1
    fi
    
    print_info "Searching for source files in: $target_dir"
    
    local files=($(find_source_files "$target_dir"))
    local file_count=${#files[@]}
    
    if [ $file_count -eq 0 ]; then
        print_warning "No source files found in: $target_dir"
        return 0
    fi
    
    print_info "Found $file_count source files"
    
    if [ "$DRY_RUN" = true ]; then
        print_info "Running in DRY RUN mode - no files will be modified"
    fi
    
    local formatted_count=0
    for file in "${files[@]}"; do
        format_file "$file"
        ((formatted_count++))
    done
    
    print_success "Processed $formatted_count files"
}

# Function to format specific files
format_files() {
    local files=("$@")
    local file_count=${#files[@]}
    
    print_info "Formatting $file_count specific files"
    
    if [ "$DRY_RUN" = true ]; then
        print_info "Running in DRY RUN mode - no files will be modified"
    fi
    
    for file in "${files[@]}"; do
        if [ ! -f "$file" ]; then
            print_error "File does not exist: $file"
            continue
        fi
        format_file "$file"
    done
}

# Function to show project formatting status
show_status() {
    print_section "BorASM Project Formatting Status"
    
    # Check if .clang-format exists
    if [ -f "$PROJECT_ROOT/.clang-format" ]; then
        print_success ".clang-format configuration found"
    else
        print_error ".clang-format configuration missing!"
        return 1
    fi
    
    # Count source files
    local src_files=($(find_source_files "$PROJECT_ROOT/src"))
    local inc_files=($(find_source_files "$PROJECT_ROOT/include"))
    local test_files=($(find_source_files "$PROJECT_ROOT/tests"))
    
    echo ""
    echo "Source files summary:"
    echo "  src/: ${#src_files[@]} files"
    echo "  include/: ${#inc_files[@]} files"
    echo "  tests/: ${#test_files[@]} files"
    echo "  Total: $((${#src_files[@]} + ${#inc_files[@]} + ${#test_files[@]})) files"
    
    # Check for files that need formatting (dry run)
    echo ""
    print_info "Checking for files that need formatting..."
    local old_dry_run=$DRY_RUN
    local old_verbose=$VERBOSE
    DRY_RUN=true
    VERBOSE=false
    
    local needs_formatting=0
    for file in "${src_files[@]}" "${inc_files[@]}" "${test_files[@]}"; do
        clang-format --style=file "$file" > /tmp/formatted_output
        if ! diff -q "$file" /tmp/formatted_output >/dev/null; then
            ((needs_formatting++))
        fi
        rm -f /tmp/formatted_output
    done
    
    DRY_RUN=$old_dry_run
    VERBOSE=$old_verbose
    
    if [ $needs_formatting -eq 0 ]; then
        print_success "All files are properly formatted!"
    else
        print_warning "$needs_formatting files need formatting"
        echo "Run: $0 --all to format all files"
    fi
}

# Function to show usage
show_usage() {
    echo "Usage: $0 [OPTIONS] [FILES/DIRECTORIES]"
    echo ""
    echo "BorASM Code Formatter - Formats C++ code according to project style guide"
    echo ""
    echo "Options:"
    echo "  -a, --all           Format all source files in the project"
    echo "  -s, --src           Format only src/ directory"
    echo "  -i, --include       Format only include/ directory"
    echo "  -t, --tests         Format only tests/ directory"
    echo "  -d, --dry-run       Show what would be formatted without making changes"
    echo "  -v, --verbose       Enable verbose output"
    echo "  -b, --backup        Create backup files (.bak) before formatting"
    echo "  -r, --recursive     Format recursively (default: true)"
    echo "  --no-recursive      Format only specified directory (not subdirectories)"
    echo "  --status            Show project formatting status"
    echo "  -h, --help          Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0 --all                    # Format all project files"
    echo "  $0 --src --verbose          # Format src/ with verbose output"
    echo "  $0 --dry-run --all          # Check what would be formatted"
    echo "  $0 file1.cpp file2.h        # Format specific files"
    echo "  $0 --status                 # Show formatting status"
    echo ""
    echo "Configuration:"
    echo "  Uses .clang-format in project root"
    echo "  Based on Google C++ Style Guide with customizations"
    echo "  Line limit: 160 characters"
    echo "  Indentation: 4 spaces"
    echo "  Include sorting: Enabled"
}

# Main function
main() {
    local format_all=false
    local format_src=false
    local format_include=false
    local format_tests=false
    local show_status_only=false
    local files_to_format=()
    
    # Parse command line arguments
    while [[ $# -gt 0 ]]; do
        case $1 in
            -a|--all)
                format_all=true
                shift
                ;;
            -s|--src)
                format_src=true
                shift
                ;;
            -i|--include)
                format_include=true
                shift
                ;;
            -t|--tests)
                format_tests=true
                shift
                ;;
            -d|--dry-run)
                DRY_RUN=true
                shift
                ;;
            -v|--verbose)
                VERBOSE=true
                shift
                ;;
            -b|--backup)
                BACKUP=true
                shift
                ;;
            -r|--recursive)
                RECURSIVE=true
                shift
                ;;
            --no-recursive)
                RECURSIVE=false
                shift
                ;;
            --status)
                show_status_only=true
                shift
                ;;
            -h|--help)
                show_usage
                exit 0
                ;;
            -*)
                print_error "Unknown option: $1"
                show_usage
                exit 1
                ;;
            *)
                files_to_format+=("$1")
                shift
                ;;
        esac
    done
    
    print_section "BorASM Code Formatter"
    
    # Check prerequisites
    check_clang_format
    
    # Show status only if requested
    if [ "$show_status_only" = true ]; then
        show_status
        exit 0
    fi
    
    # Determine what to format
    if [ "$format_all" = true ]; then
        format_directory "$PROJECT_ROOT/src"
        format_directory "$PROJECT_ROOT/include"
        format_directory "$PROJECT_ROOT/tests"
    elif [ "$format_src" = true ] || [ "$format_include" = true ] || [ "$format_tests" = true ]; then
        [ "$format_src" = true ] && format_directory "$PROJECT_ROOT/src"
        [ "$format_include" = true ] && format_directory "$PROJECT_ROOT/include"
        [ "$format_tests" = true ] && format_directory "$PROJECT_ROOT/tests"
    elif [ ${#files_to_format[@]} -gt 0 ]; then
        format_files "${files_to_format[@]}"
    else
        print_warning "No files or directories specified"
        show_usage
        exit 1
    fi
    
    print_section "FORMATTING COMPLETED"
    if [ "$DRY_RUN" = true ]; then
        print_info "Dry run completed - no files were modified"
        print_info "Remove --dry-run flag to apply formatting"
    else
        print_success "Code formatting completed successfully!"
        if [ "$BACKUP" = true ]; then
            print_info "Backup files (.bak) were created"
        fi
    fi
}

# Run main function with all arguments
main "$@"