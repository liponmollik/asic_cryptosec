/**
 * @file constraints.h
 * @brief Header for software-level constraints and validation functions.
 *
 * This file declares functions that can be used to validate system parameters,
 * perform runtime checks against defined operational limits, and handle
 * constraint violations. These are software-defined constraints, distinct
 * from hardware design constraints.
 */

#ifndef CONSTRAINTS_H
#define CONSTRAINTS_H

#include <stdbool.h>
#include <stdint.h>

// --- Public Function Declarations ---

/**
 * @brief Initializes the constraints module.
 *
 * This function should be called early in the system startup to load
 * and prepare any constraint-related data or configurations.
 *
 * @return True if initialization is successful, false otherwise.
 */
bool ConstraintsManager_Init(void); // Following Manager function naming convention

/**
 * @brief Validates a given clock frequency against acceptable bounds.
 *
 * Checks if the provided frequency is within the minimum and maximum
 * allowed system clock frequencies.
 *
 * @param freq_hz The clock frequency to validate, in Hz.
 * @return True if the frequency is valid, false otherwise.
 */
bool ConstraintsManager_ValidateClockFrequency(uint32_t freq_hz);

/**
 * @brief Checks if a given memory address falls within a valid RAM region.
 *
 * Useful for ensuring pointers or memory accesses are not out of bounds.
 *
 * @param address The memory address to check.
 * @return True if the address is within the defined RAM range, false otherwise.
 */
bool ConstraintsManager_ValidateRamAddress(uint32_t address);

/**
 * @brief Checks if a critical system resource is available and healthy.
 *
 * This could be used for checking things like communication buffers,
 * sensor status, or critical peripheral readiness.
 *
 * @param resource_id An identifier for the resource to check.
 * @return True if the resource is healthy and available, false otherwise.
 */
bool ConstraintsManager_CheckCriticalResource(uint32_t resource_id);

/**
 * @brief Reports a constraint violation.
 *
 * This function handles what happens when a constraint is violated.
 * It might log the error, trigger a system reset, or enter a safe state.
 *
 * @param constraint_id A unique identifier for the violated constraint.
 * @param error_message A descriptive string for the error.
 */
void ConstraintsManager_ReportViolation(uint32_t constraint_id, const char *error_message);

// --- Constraint Identifiers (Example) ---
// Define unique IDs for different constraints for better logging/reporting.
#define CONSTRAINT_ID_SYS_CLK_RANGE     0x01
#define CONSTRAINT_ID_RAM_ACCESS_OOB    0x02
#define CONSTRAINT_ID_SENSOR_TIMEOUT    0x03
#define CONSTRAINT_ID_COMM_BUFFER_FULL  0x04
// ... add more as needed

#endif // CONSTRAINTS_H
