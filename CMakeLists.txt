# CMakeLists.txt for an ASIC Firmware Project
# This file defines how to build the firmware for a single ASIC (e.g., ASIC_0001)

# Minimum required CMake version
# Ensure this is a version supported by your development environment.
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

# Define the project name. This should match your ASIC's identifier (e.g., ASIC_0001)
project(ASIC_Firmware_Project_Name C ASM) # Specify C and Assembly languages

# Define the target architecture and toolchain if cross-compiling
# This is crucial for embedded systems. You will need to customize these paths.
# set(CMAKE_SYSTEM_NAME Generic) # Or 'BareMetal', 'FreeRTOS', etc.
# set(CMAKE_SYSTEM_PROCESSOR arm) # Or 'riscv', 'mips', 'etc.'

# Example of a toolchain file (recommended for cross-compilation)
# You would create a file like 'toolchain-arm-none-eabi.cmake'
# set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/conf/toolchain-arm-none-eabi.cmake")

# --- Compiler Options ---
# Common C flags for embedded development. Adjust as needed for your specific compiler (e.g., GCC, Clang).
# -mcpu: Specify the CPU architecture (e.g., cortex-m4, arm7tdmi)
# -mthumb: Generate Thumb instructions (for ARM Cortex-M)
# -mfloat-abi=softfp/hard: Floating-point ABI (soft-float, hard-float, softfp)
# -fno-common: Place global variables in the data section (important for embedded)
# -fmessage-length=0: Prevent messages from wrapping
# -ffunction-sections -fdata-sections: Allows linker to remove unused functions/data (for optimization)
# -Wall -Wextra -Wpedantic: Enable various warnings for robust code
# -g: Include debug information
# -Os: Optimize for size (common in embedded)
set(CMAKE_C_FLAGS_INIT "-mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -fno-common -fmessage-length=0 -ffunction-sections -fdata-sections -Wall -Wextra -Wpedantic -g -Os")
set(CMAKE_ASM_FLAGS_INIT "-mcpu=cortex-m4 -mthumb -g") # Assembly flags

# Add definitions (e.g., for target-specific features, debug flags)
# For example, to define a macro for your ASIC version:
add_definitions(-DPRODUCT_VERSION="1.0.0")
add_definitions(-DASIC_ID="ASIC_0001") # Define based on the specific ASIC's build

# --- Include Directories ---
# Specify where CMake should look for header files (.h)
# inc/: Contains project-specific headers
# src/: Often includes headers for local translation units
# common/inc: If you have shared headers from a common library
include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/inc"
    "${CMAKE_CURRENT_SOURCE_DIR}/inc/drivers"
    "${CMAKE_CURRENT_SOURCE_DIR}/inc/app"
    "${CMAKE_CURRENT_SOURCE_DIR}/inc/utils"
    # Example for common library includes (if your project uses the common/ directory)
    # "${ASICS_ROOT_DIR}/common/inc" # Assuming ASICS_ROOT_DIR is defined higher up
)

# --- Source Files ---
# Collect all source files (.c and .s/.S) that make up your firmware.
# Use globbing for convenience, but explicit listing is safer for large projects.
# main.c is the entry point.
# List all driver, application, and utility source files.
file(GLOB_RECURSE SRC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/app/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/*.c"
    # If you have startup assembly code, include it here:
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/startup/startup_cortex_m.s"
    # Example for common library sources (if your project uses the common/ directory)
    # "${ASICS_ROOT_DIR}/common/src/*.c"
)

# --- Linker Settings ---
# Specify the linker script. This is essential for embedded systems to define
# memory regions, stack/heap, and interrupt vector table placement.
# The linker script is typically located in the 'conf/' directory.
set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/conf/linker.ld")

# Add linker flags
# -nostdlib: Don't link standard libraries (you'll often provide your own minimal libc)
# -T: Specify the linker script
# -Wl,--gc-sections: Garbage collect unused sections (works with -ffunction-sections)
set(CMAKE_EXE_LINKER_FLAGS_INIT "-nostdlib -T\"${LINKER_SCRIPT}\" -Wl,--gc-sections")

# --- Define the Firmware Executable Target ---
# This creates an executable target named 'asic_firmware_ASIC_0001' (or similar).
# You can change the target name to something specific to your ASIC.
add_executable(asic_firmware_ASIC_0001 ${SRC_FILES})

# --- Post-Build Steps (Optional but common) ---
# Commands to run after the executable is built, e.g., generating .bin, .hex files
# or running a size analysis tool.
add_custom_command(
    TARGET asic_firmware_ASIC_0001 POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $< ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $< ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.bin
    COMMAND ${CMAKE_SIZE} $<
    COMMENT "Generating .hex, .bin files and printing size information."
)

# You might also want to add rules for flashing the firmware to the ASIC.
# For example:
# add_custom_target(flash_asic
#    COMMAND ${JLINK_TOOL} -device YOUR_DEVICE -if SWD -speed 4000 -autoexit -CommanderScript ${CMAKE_CURRENT_SOURCE_DIR}/conf/flash_script.jlink
#    DEPENDS asic_firmware_ASIC_0001
# )

# Set the output directory for the executable (e.g., build/debug or build/release)
# Default is CMAKE_CURRENT_BINARY_DIR, which usually points to your build/ subdirectory.
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}) # For the executable itself
