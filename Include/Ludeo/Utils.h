// Copyright 2023 Ludeo. All rights reserved.

/**
 * @file
 * Non essential utilities (such as C++ helpers)
 */

#pragma once

#include "Ludeo/Common.h"
#include "Ludeo/InitTypes.h"
#include "Ludeo/SessionTypes.h"


/**
 * \addtogroup Utilities
 * @{
 * Non-essential utilities.
 * 
 * To make the SDK easier to use from C++, some helper types and functions are provided.
 */

/**
 * Controls if the `to_cstring` and `to_string` functions are available.
 *
 * For every enum, there is a `const char* ludeo_<Enum>ToString()` function.
 * To make those functions easier to use from C++, #LUDEOSDK_TO_STRING and #LUDEOSDK_TO_STRING_TYPE can be set during the build or
 * before including `Utils.h` . These macros control if a respective `to_cstring` and `to_string` is available for every enum.
 *
 * #LUDEOSDK_TO_STRING controls if the overloads are defined, while #LUDEOSDK_TO_STRING_TYPE specifies what string class to use.
 *
 * For example, if before including `Utils.h` these two are set as:
 * ```
 * #define LUDEODSK_TO_STRING 1
 * #define LUDEOSDK_TO_STRING_TYPE String
 * ```
 * then the following functions will be defined for every enum (Using #LudeoResult as an example)
 * 
 * ```
 * const char* to_cstring(LudeoResult val);
 * String to_string(LudeoResult val);
 * ```
 * 
 * The purpose of #LUDEOSDK_TO_STRING_TYPE is so that the client can control if STL should be used or not.
 *
 */
#ifdef __cplusplus
	#if LUDEOSDK_BUILDING
		#define LUDEOSDK_TO_STRING 1
	#else
		#ifndef LUDEOSDK_TO_STRING
			#define LUDEOSDK_TO_STRING 0
		#endif
	#endif

#else
	#if defined(LUDEOSDK_TO_STRING) && LUDEOSDK_TO_STRING
		#pragma warning "Compiling as C. Forcing LUDEOSDK_TO_STRING to 0"
		#undef LUDEOSDK_TO_STRING
		#define LUDEOSDK_TO_STRING 0
	#else
		#define LUDEOSDK_TO_STRING 0
	#endif

#endif

// If building unit or integration tests, LUDEOSDK_TESTUTILS is set to 1, so we introduce Catch2 helpers
#if LUDEOSDK_TESTUTILS && LUDEOSDK_TO_STRING

	// This allows Catch2 tests to show the string representation of enums.
	// See https://github.com/catchorg/Catch2/blob/devel/docs/tostring.md#catchstringmaker-specialisation
	#define LUDEO_IMPL_UNITTEST_HELPERS(Enum)                                    \
		namespace Catch                                                          \
		{                                                                        \
		template <>                                                              \
		struct StringMaker<Ludeo##Enum>                                          \
		{                                                                        \
			static std::string convert(Ludeo##Enum const& value)                 \
			{                                                                    \
				return std::string("Ludeo") + #Enum + "::" + ::to_string(value); \
			}                                                                    \
		};                                                                       \
		}
#else
	#define LUDEO_IMPL_UNITTEST_HELPERS(Enum)
#endif

/**
 * Specifies what class to use as the return type of the `to_string` functions that are defined when #LUDEOSDK_TO_STRING is 1.
 */
#ifndef LUDEOSDK_TO_STRING_TYPE
	#define LUDEOSDK_TO_STRING_TYPE std::string
#endif

#if LUDEOSDK_TO_STRING
	#if defined (LUDEOSDK_TO_STRING_TYPE)
		// Using a custom type
		using LudeoString = LUDEOSDK_TO_STRING_TYPE;
	#else
		#include <string>
		using LudeoString = std::string;
	#endif

	#define LUDEO_IMPL_TO_STRING_CPP(Name) \
		inline const char* to_cstring(Ludeo##Name value) { return ludeo_##Name##ToString(value); } \
		inline LudeoString to_string(Ludeo##Name value) { return ludeo_##Name##ToString(value); }
		
#else
	#define LUDEO_IMPL_TO_STRING_CPP(Name) 
#endif

#define LUDEO_IMPL_ENUM_HELPERS(Name)                                          \
	LUDEO_DECLARE_FUNC(const char*) ludeo_##Name##ToString(Ludeo##Name value); \
	LUDEO_IMPL_TO_STRING_CPP(Name);                                            \
	LUDEO_IMPL_UNITTEST_HELPERS(Name);

/** Converts a #LudeoResult value to a string. */
LUDEO_IMPL_ENUM_HELPERS(Result)
/** Converts a #LudeoLogLevel value to a string. */
LUDEO_IMPL_ENUM_HELPERS(LogLevel)
/** Converts a #LudeoLogCategory value to a string. */
LUDEO_IMPL_ENUM_HELPERS(LogCategory)
/** Converts a #LudeoAuthType value to a string. */
LUDEO_IMPL_ENUM_HELPERS(AuthType)
/** Converts a #LudeoDataType value to a string. */
LUDEO_IMPL_ENUM_HELPERS(DataType)

#ifdef __cplusplus

/** */
namespace Ludeo
{
	/**
	 * Utility function to create a parameters struct and initialize apiVersion to the right value.
	 * In addition to setting apiVersion, it zeroes all other fields.
	 *
	 * This can avoid bugs such as using a wrong API_LATEST value (e.g: copy pasting bugs) or not zeroing fields
	 *
	 * Can be used as e.g:
	 * 
	 * ```
	 * auto params = Ludeo::create<LudeoSessionActivateParams>();
	 * // Set other fields as required...
	 * params.apiKey = ...;
	 * // ... call ludeo_Session_Activate with `params`
	 * ludeo_Session_Activate(sessionHandle, &params, ...);
	 * ```
	 *
	 */
	template<typename ParamsType>
	ParamsType create()
	{
		// Note the {}. That's for zero initialization: https://en.cppreference.com/w/cpp/language/zero_initialization
		ParamsType params{};
		params.apiVersion = Ludeo::ApiFuncTraits<ParamsType>::latestApiVersion;
		return params;
	}
} // namespace Ludeo
#endif


/**
 * 128-bit UUID
 */
LUDEO_STRUCT_START(LudeoUUID)
	uint8_t data[16]; 
LUDEO_STRUCT_END(LudeoUUID);

/**
 * Generates a UUID
 *
 * This is provided simply as a convenient cross-platform way of creating UUIDs.
 */
LUDEO_DECLARE_FUNC(LudeoUUID) ludeo_CreateUUID(void);

/**
 * Similar to ludeo_CreateUUID, but returns a string representation.
 *
 * The resulting string is a 36 characters long string (e.g: "550e8400-e29b-41d4-a716-446655440000" ).
 * The specified buffer needs to be at least 37 bytes (36 + null-terminating character)
 *
 * @param buf Buffer where to write the null-terminated string
 * @param bufSize size of the provided buffer in bytes. The function expects a buffer of size 37. This parameter is specified just
 * as a safety. If bufSize is <37, LudeoResult::InvalidParameters is returned.
 */
LUDEO_DECLARE_FUNC(LudeoResult) ludeo_CreateUUIDString(char* buf, int32_t bufSize);

/**
 * @}
 */

