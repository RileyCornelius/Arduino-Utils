
/**--------------------------------------------------------------------------------------
 * Settings
 *-------------------------------------------------------------------------------------*/

// Log level variable
#define LOG_LEVEL LOG_LEVEL_ERROR

// Time getter
#define LOG_TICK_MS_CALLBACK millis()

#define LOG_PRINT_CALLBACK Serial.printf

// Print time in message
#define LOG_USE_TIMESTAMP 0

// Print file and line number
#define LOG_USE_FILE_LINE 1

// Print file name
#define LOG_USE_FILE_NAME 1

// Print function name
#define LOG_USE_FUNCTION_NAME 1

// Format time in message
#define LOG_TIME_FORMATTING 0

// Print log types full name or single letter
#define LOG_FORMAT_LONG_NAME 0