// Copyright 2023 Ludeo. All rights reserved.

/**
 * @file
 * Types that don't belong to any specific interface.
 * 
 * This includes types used to initialize/shutdown the SDK, logging, errors, etc.
 */

#pragma once

#include "Ludeo/Common.h"

#pragma pack(push, 8)

/** \addtogroup Root
 * @{
 */

/** \addtogroup Logging
 * @{
 */

/**
 * Log level
 */
LUDEO_ENUM_START(LudeoLogLevel)

	/**
	* Value used to turn off logging
	*/
	LUDEO_ENUM_VALUE(LudeoLogLevel, Off, 0)

	/**
	 * Some fatal error or assert has happened. A log of this level means something critical has failed or an SDK assert has
	 * triggered. This should not occur at all. If it does, it's most likely a bug in the SDK.
	 */
	LUDEO_ENUM_VALUE(LudeoLogLevel, Fatal, 1)

	/**
	 * A non-fatal error has occurred.
	 * Logs of this level normally indicate why an API call has failed, or why some other non-fatal internal code has
	 * failed.
	 */
	LUDEO_ENUM_VALUE(LudeoLogLevel, Error, 2)

	/** Warning */
	LUDEO_ENUM_VALUE(LudeoLogLevel, Warning, 3)

	/** Normal log level */
	LUDEO_ENUM_VALUE(LudeoLogLevel, Log, 4)

	/** Detailed diagnostic information for troubleshooting */
	LUDEO_ENUM_VALUE(LudeoLogLevel, Verbose, 5)

	/** Extremely detailed output including low-level SDK internals */
	LUDEO_ENUM_VALUE_LAST(LudeoLogLevel, VeryVerbose, 6)
LUDEO_ENUM_END(LudeoLogLevel);

/**
 * Log category
 */
LUDEO_ENUM_START(LudeoLogCategory)
	/**
	 * Log category used for any logging not related to a specific interface or service
	 */
	LUDEO_ENUM_VALUE(LudeoLogCategory, Core, 0)

	/**
	 * Log category used for any logging related to the session interface
	 */
	LUDEO_ENUM_VALUE(LudeoLogCategory, Session, 1)

	/**
	 * Log category for Http communications
	 */
	LUDEO_ENUM_VALUE(LudeoLogCategory, Http, 2)

	/**
	 * Log category for writing and reading gameplay data
	 */
	LUDEO_ENUM_VALUE(LudeoLogCategory, Data, 3)

	/**
	 * Log category for any logging related to rooms 
	 */
	LUDEO_ENUM_VALUE(LudeoLogCategory, Room, 4)

	/**
	 * Log category for any logging related to the overlay
	 */
	LUDEO_ENUM_VALUE(LudeoLogCategory, Overlay, 5)

	/**
	 * Log category for the logging coming from Coherent's gameface
	 */
	LUDEO_ENUM_VALUE(LudeoLogCategory, Coherent, 6)

	/**
	 * Log category for the logging related to video encoding
	 */
	LUDEO_ENUM_VALUE(LudeoLogCategory, VideoEncoding, 7)


	/** Not a real category. This is a special value that can be passed to ludeo_setLoggingLevel */
	LUDEO_ENUM_VALUE_LAST(LudeoLogCategory, All, 0x7FFFFFFF)
LUDEO_ENUM_END(LudeoLogCategory);

/**
 * @}
 */

/** \addtogroup ludeo_Initialize
 * @{
 */

/** Signature for the memory allocation function */
EXTERN_C typedef void* (LUDEO_CALL * LudeoMemoryAllocFunc)(size_t size);
/** Signature for the memory reallocation function */
EXTERN_C typedef void* (LUDEO_CALL * LudeoMemoryReallocFunc)(void* ptr, size_t size);
/** Signature for the memory free function */
EXTERN_C typedef void  (LUDEO_CALL * LudeoMemoryFreeFunc)(void* ptr);

/**
 * @}
 */

/** \addtogroup Logging
 * @{
 */

/** The most recent version of ludeo_SetLoggingToFile */
#define LUDEO_SETLOGGINGTOFILE_API_LATEST 1

/**
 * Structure for parameters passed to ludeo_SetLoggingToFile
 */
LUDEO_STRUCT_START(LudeoSetLoggingToFileParams)

	/** Set this to LUDEO_SETLOGGINGTOFILE_API_LATEST */
	int32_t apiVersion;

	/**
	 * If set to true, then file logging is enabled
	 * If set to false, it disables file logging (if previously enabled)
	 */
	LudeoBool enable; 

	/**
	 * If enable is true, then this specifies in what directory should the log file be created.
	 * If nulled, then the current working directory will be used
	 */
	const char* directory;
LUDEO_STRUCT_END(LudeoSetLoggingToFileParams);

/**
 * Structure that represents a single log message
 * Any pointers in the struct will be temporary or internal to the SDK, and should therefore NOT be kept by the client
 */
LUDEO_STRUCT_START(LudeoLogMessage)
	/**
	 * Log level for this message
	 * Use ludeo_LogLevelToString/to_string to get the string equivalent
	 */
	LudeoLogLevel level;

	/**
	 * The log category
	 * Use ludeo_LogCategoryToString/to_string to get the string equivalent
	 */
	LudeoLogCategory category;

	/** String with UTC time. HH:mm: */
	const char* timestamp;

	/**
	 * UTF-8 log message
	 * This is intentionally just the log message without any prefixes, so the client can format the final output as desired.
	 * Recommended format is (in printf style):
	 *     printf("s%:%s:%s: %s", msg->timestamp, msg->categoryStr, msg->levelStr, msg->message)
	 *
	 * Note that the message doesn't include a \n . It's up to the client to add that if required.
	 */
	const char* message;
LUDEO_STRUCT_END(LudeoLogMessage);

/** Signature for the logging function */
EXTERN_C typedef void (LUDEO_CALL * LudeoLogCallback)(const LudeoLogMessage* data);

/**
 * @}
 */

/** \addtogroup ludeo_Initialize
 * @{
 */

/** The most recent version of the ludeo_Initialize API */
#define LUDEO_INITIALIZE_API_LATEST 2

/**
 * Parameters passed to ludeo_Initialize
 */
LUDEO_STRUCT_START(LudeoInitializeParams)

	/** Set this to LUDEO_INITIALIZE_API_LATEST */
	int32_t apiVersion;

	/**
	 * Memory allocation function. If nulled, it will use the default implementation.
	 * If specified, all other memory functions need to be specified.
	 */
	LudeoMemoryAllocFunc memoryAllocFunc;

	/**
	 * Memory reallocation function. If nulled, it will use the default implementation
	 * If specified, all other memory functions need to be specified.
	 */
	LudeoMemoryReallocFunc memoryReallocFunc;

	/**
	 * Memory release function. If nulled, it will use the default implementation
	 * If specified, all other memory functions need to be specified.
	 */
	LudeoMemoryFreeFunc memoryFreeFunc;

	/**
	 * Sets the logging callback function as part of initialization. If null, then it is going to be ignored.
	 * Any logging done by the SDK will be forwarded to the specified function.
	 * Avoid expensive operations from the callback, such as directly logging to a file.
	 */
	LudeoLogCallback loggingCallback;

	/**
	 * Sets up logging to a file as part of initialization. If null, then it is going to be ignored.
	 */
	LudeoSetLoggingToFileParams* loggingToFileParams;

LUDEO_STRUCT_END(LudeoInitializeParams);

/**
 * @}
 */

/** \addtogroup ludeo_AddNotifyOOBInfo
 * @{
 */

/** The most recent version of the #ludeo_AddNotifyOOBInfo API */
#define LUDEO_ADDNOTIFYOOBINFO_API_LATEST 1

/** Parameters to #ludeo_AddNotifyOOBInfo */
LUDEO_STRUCT_START(LudeoAddNotifyOOBInfoParams)
	/** Set this to LUDEO_ADDNOTIFYOOBINFO_API_LATEST */
	int32_t apiVersion;
LUDEO_STRUCT_END(LudeoAddNotifyOOBInfoParams)

/**
 * Structure with the parameters passed to the OOBInfo callback
 * See #ludeo_AddNotifyOOBInfo for more details.
 */
LUDEO_STRUCT_START(LudeoOOBInfoCallbackParams)
	/** Context that was passed to #ludeo_AddNotifyOOBInfo */
	void* clientData;
	/** The error code which describes what happened  */
	LudeoResult resultCode;
LUDEO_STRUCT_END(LudeoOOBInfoCallbackParams);

/** Signature to the #ludeo_AddNotifyOOBInfo callback */
LUDEO_DECLARE_CALLBACK(LudeoOOBInfoCallback, const LudeoOOBInfoCallbackParams* data);

/**
 * @}
 */

// General - END 
/**
 * @}
 */

#pragma pack(pop)


