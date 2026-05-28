// Copyright 2023 Ludeo. All rights reserved.

/**
 * @file
 * Common definitions used by all other header files.
 */

#pragma once

#include <stdint.h>
#include <uchar.h> 

/**
 * \addtogroup Common
 * @{
 * Preprocessor macros, functions and types used by all the other headers.
 */

// Set the LUDEOSDK_<PLATFORM_NAME> macro
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	#define LUDEOSDK_WINDOWS 1
#elif defined(__APPLE__)
	#include "TargetConditionals.h"
	#if TARGET_OS_IPHONE && TARGET_OS_SIMULATOR
		// define something for simulator
		// (although, checking for TARGET_OS_IPHONE should not be required).
		#error Unknown/unsupported platform
	#elif TARGET_OS_IPHONE && TARGET_OS_MACCATALYST
		// define something for Mac's Catalyst
		#error Unknown/unsupported platform
	#elif TARGET_OS_IPHONE
		// define something for iphone  
		#error Unknown/unsupported platform
	#else
		#define LUDEOSDK_MACOSX 1
	#endif
#elif defined(__ANDROID__) // NOTE: This needs to be before __linux__, because __linux__ is also set on Android
	#define LUDEOSDK_ANDROID 1
#elif defined(__linux__)
	#define LUDEOSDK_LINUX 1
#else
	#error Unknown/unsupported platform
#endif

#ifndef LUDEOSDK_WINDOWS
	#define LUDEOSDK_WINDOWS 0
#endif

#ifndef LUDEOSDK_MACOSX
	#define LUDEOSDK_MACOSX 0
#endif

#ifndef LUDEOSDK_LINUX
	#define LUDEOSDK_LINUX 0
#endif

#ifndef LUDEOSDK_ANDROID
	#define LUDEOSDK_ANDROID 0
#endif

#if defined(__arm__) || defined(__aarch64__) || defined(_M_ARM)
	#define LUDEOSDK_ARCH_ARM 1
#elif defined(__x86_64__) || defined(__i386__) || defined(_M_AMD64)
	#define LUDEOSDK_ARCH_X86 1
#endif

#ifndef LUDEOSDK_ARCH_ARM
	#define LUDEOSDK_ARCH_ARM 0
#endif

#ifndef LUDEOSDK_ARCH_X86
	#define LUDEOSDK_ARCH_X86 0
#endif

#if defined(_WIN32) || defined(_WIN64)
	#if defined(_WIN64)
		#define LUDEOSDK_64BITS 1
	#else
		#define LUDEOSDK_32BITS 1
	#endif
#elif defined(__ANDROID__)
	// https://developer.android.com/ndk/guides/cpu-features
	#if defined(__aarch64__) || defined(__x86_64__)
		#define LUDEOSDK_64BITS 1
	#elif defined(__arm__) || defined(__i386__)
		#define LUDEOSDK_32BITS 1
	#else
		#error Unknown/unsupported Android platform
	#endif
#elif defined(__GNUC__)
	#if defined(__x86_64__) || defined(__ppc64__) || defined(__arm64__)
		#define LUDEOSDK_64BITS 1
	#else
		#define LUDEOSDK_32BITS 1
	#endif
#else
	#error Unknown compiler
#endif

#ifndef LUDEOSDK_64BITS
	#define LUDEOSDK_64BITS 0
#endif

#ifndef LUDEOSDK_32BITS
	#define LUDEOSDK_32BITS 0
#endif

#if LUDEOSDK_WINDOWS || LUDEOSDK_LINUX || LUDEOSDK_MACOSX
	#define LUDEOSDK_HAS_STEAM 1
#else
	#define LUDEOSDK_HAS_STEAM 0
#endif

#ifdef __cplusplus
	#include <functional>
#endif

#ifndef LUDEOSDK_BUILDING
	#define LUDEOSDK_BUILDING 0
#endif

#ifndef LUDEOSDK_TESTUTILS
	#define LUDEOSDK_TESTUTILS 0
#endif

#ifndef LUDEOSDK_INTEGRATIONTESTS
	#define LUDEOSDK_INTEGRATIONTESTS 0
#endif

#ifndef LUDEOSDK_UNITTESTS
	#define LUDEOSDK_UNITTESTS 0
#endif

#ifndef LUDEOSDK_BUILDING_FOR_UNITTESTS
	#define LUDEOSDK_BUILDING_FOR_UNITTESTS 0
#endif

#if LUDEOSDK_BUILDING_FOR_UNITTESTS
	#if defined(_WIN32)
		#define LUDEOSDK_API
	#elif defined(__GNUC__) && (__GNUC__ >= 4)
		#define LUDEOSDK_API __attribute__((__visibility__("default")))
	#else
		#error Unexpected platform?
	#endif
#elif LUDEOSDK_BUILDING
	#if defined(_WIN32)
		#define LUDEOSDK_API __declspec(dllexport)
	#elif defined(__GNUC__) && (__GNUC__ >= 4)
		#define LUDEOSDK_API __attribute__((__visibility__("default")))
	#else
		#error Unexpected platform?
	#endif
#else
	#if defined(_WIN32)
		#define LUDEOSDK_API __declspec(dllimport)
	#elif defined(__GNUC__) && (__GNUC__ >= 4)
		#define LUDEOSDK_API __attribute__((__visibility__("default")))
	#else
		#error Unexpected platform?
	#endif
#endif

#if defined(_WIN32) && (defined(__i386) || defined(_M_IX86))
	#define LUDEO_CALL __stdcall
#else
	#define LUDEO_CALL
#endif

#ifdef __cplusplus
	#define EXTERN_C extern "C"
#else
	#define EXTERN_C
#endif

#define LUDEO_DECLARE_FUNC(ReturnType) EXTERN_C LUDEOSDK_API ReturnType LUDEO_CALL
#define LUDEO_IMPL_FUNC(ReturnType) EXTERN_C ReturnType LUDEO_CALL

#ifdef __cplusplus
	// std::function style signature for a callback
	#define LUDEO_DECLARE_CALLBACK_CPP(ReturnType, CallbackName, ...) \
		using LUDEO_CONCATENATE(CallbackName, Cpp) = std::function<ReturnType(__VA_ARGS__)>;
#else
	#define LUDEO_DECLARE_CALLBACK_CPP(ReturnType, CallbackName, ...)
#endif


#define LUDEO_DECLARE_CALLBACK(CallbackName, ...)                  \
	EXTERN_C typedef void(LUDEO_CALL * CallbackName)(__VA_ARGS__); \
	LUDEO_DECLARE_CALLBACK_CPP(void, CallbackName, __VA_ARGS__)

#define LUDEO_DECLARE_CALLBACK_RETVALUE(ReturnType, CallbackName, ...)  \
	EXTERN_C typedef ReturnType(LUDEO_CALL* CallbackName)(__VA_ARGS__); \
	LUDEO_DECLARE_CALLBACK_CPP(ReturnType, CallbackName, __VA_ARGS__)

#define LUDEO_PASTE(...) __VA_ARGS__

// Macros to define structs. If compiling with C, it uses a typedef. If compiling with C++, it skips the typedef to reduce noise
// in code completion.
#ifdef __cplusplus
	#define LUDEO_FORWARDDECLARE_STRUCT(StructName) struct StructName;
	#define LUDEO_STRUCT_START(StructName) EXTERN_C struct StructName {
	#define LUDEO_STRUCT_END(StructName) };
#else
	#define LUDEO_FORWARDDECLARE_STRUCT(StructName) EXTERN_C typedef struct _tag##StructName StructName;
	#define LUDEO_STRUCT_START(StructName) EXTERN_C typedef struct _tag##StructName {
	#define LUDEO_STRUCT_END(StructName) } StructName;
#endif


//
// Utility macros to concatenate things
//
#define LUDEO_CONCATENATE_PASTER(s1,s2) s1 ## s2
#define LUDEO_CONCATENATE(s1,s2) LUDEO_CONCATENATE_PASTER(s1,s2)

//
// Helpers to make things easier to use from C++
//
#ifdef __cplusplus

	namespace Ludeo
	{
		template<typename T>
		struct ApiFuncTraits
		{
		};
	}

	// Defines traits for a function that doesn't belong to an interface
	#define LUDEO_DEFINE_FUNC_TRAITS_1(inFuncName, inFuncNameU)                             \
	    namespace Ludeo                                                                   \
	    {                                                                                 \
	    template <>                                                                       \
	    struct ApiFuncTraits<Ludeo##inFuncName##Params>                                   \
	    {                                                                                 \
		    static constexpr int32_t latestApiVersion = LUDEO_##inFuncNameU##_API_LATEST; \
		    using ParamsType = Ludeo##inFuncName##Params;                                 \
		    using FuncType = decltype(&ludeo_##inFuncName);                               \
		    static FuncType func()                                                        \
		    {                                                                             \
			    return &ludeo_##inFuncName;                                               \
		    }                                                                             \
	    };                                                                                \
	    }

	// Defines traits for a function that belong to an interface
	#define LUDEO_DEFINE_FUNC_TRAITS_2(inInterfaceName, inInterfaceNameU, inFuncName, inFuncNameU)             \
	    namespace Ludeo                                                                                        \
	    {                                                                                                      \
	    template <>                                                                                            \
	    struct ApiFuncTraits<Ludeo##inInterfaceName##inFuncName##Params>                                       \
	    {                                                                                                      \
		    static constexpr int32_t latestApiVersion = LUDEO_##inInterfaceNameU##_##inFuncNameU##_API_LATEST; \
		    using ParamsType = Ludeo##inInterfaceName##inFuncName##Params;                                     \
		    using FuncType = decltype(&ludeo_##inInterfaceName##_##inFuncName);                                \
			static constexpr const char* funcName()                                                            \
			{                                                                                                  \
				return "ludeo_" #inInterfaceName "_" #inFuncName;                                              \
			}                                                                                                  \
		    static FuncType func()                                                                             \
		    {                                                                                                  \
			    return &ludeo_##inInterfaceName##_##inFuncName;                                                \
		    }                                                                                                  \
	    };                                                                                                     \
	    }

	// Defines traits for a function that belongs to an interface and that has a callback
	#define LUDEO_DEFINE_FUNC_TRAITS_WITHCALLBACK_2(inInterfaceName, inInterfaceNameU, inFuncName, inFuncNameU)      \
		namespace Ludeo                                                                                              \
		{                                                                                                            \
			template <>                                                                                              \
			struct ApiFuncTraits<Ludeo##inInterfaceName##inFuncName##Params>                                         \
			{                                                                                                        \
				static constexpr int32_t latestApiVersion = LUDEO_##inInterfaceNameU##_##inFuncNameU##_##API_LATEST; \
				using ParamsType = Ludeo##inInterfaceName##inFuncName##Params;                                       \
				static constexpr const char* funcName()                                                              \
				{                                                                                                    \
					return "ludeo_" #inInterfaceName "_" #inFuncName;                                                \
				}                                                                                                    \
				using FuncType = decltype(&ludeo_##inInterfaceName##_##inFuncName);                                  \
				static FuncType func()                                                                               \
				{                                                                                                    \
					return &ludeo_##inInterfaceName##_##inFuncName;                                                  \
				}                                                                                                    \
				using CallbackType = Ludeo##inInterfaceName##inFuncName##Callback;                                   \
				using CallbackTypeCpp = Ludeo##inInterfaceName##inFuncName##CallbackCpp;                             \
				using CallbackParamsType = Ludeo##inInterfaceName##inFuncName##CallbackParams;                       \
			};                                                                                                       \
																													 \
			template <>                                                                                              \
			struct ApiFuncTraits<Ludeo##inInterfaceName##inFuncName##CallbackParams>                                 \
				: public ApiFuncTraits<Ludeo##inInterfaceName##inFuncName##Params>                                   \
			{                                                                                                        \
			};                                                                                                       \
		}

	// Defines traits for a "AddNotify" function
	#define LUDEO_DEFINE_FUNC_TRAITS_ADDNOTIFY(inInterfaceName, inInterfaceNameU, CallbackName, CallbackNameU)								\
		namespace Ludeo																														\
		{																																	\
			template <>																														\
			struct ApiFuncTraits<Ludeo##inInterfaceName##AddNotify##CallbackName##Params>													\
			{																																\
				static constexpr int32_t latestApiVersion = LUDEO_##inInterfaceNameU##_##ADDNOTIFY##CallbackNameU##_##API_LATEST;           \
				using ParamsType = Ludeo##inInterfaceName##AddNotify##CallbackName##Params;													\
				static constexpr const char* funcName()																						\
				{																															\
					return "ludeo_" #inInterfaceName "_AddNotify" #CallbackName;															\
				}																															\
				static constexpr const char* callbackName()																					\
				{																															\
					return #CallbackName;																									\
				}																															\
				static constexpr const char* tracyName()																					\
				{																															\
					return #CallbackName " notification callback";																			\
				}																															\
				using FuncType = decltype(&ludeo_##inInterfaceName##_##AddNotify##CallbackName);											\
				static FuncType func()																										\
				{																															\
					return &ludeo_##inInterfaceName##_##AddNotify##CallbackName;															\
				}																															\
				using CallbackType = Ludeo##inInterfaceName##CallbackName##Callback;														\
				using CallbackTypeCpp = Ludeo##inInterfaceName##CallbackName##CallbackCpp;													\
				using CallbackParamsType = Ludeo##inInterfaceName##CallbackName##CallbackParams;											\
			};																																\
																																			\
			template <>																														\
			struct ApiFuncTraits<Ludeo##inInterfaceName##CallbackName##CallbackParams>														\
				: public ApiFuncTraits<Ludeo##inInterfaceName##AddNotify##CallbackName##Params>												\
			{																																\
			};																																\
		}

	// Defines traits for a "AddNotify" function
	#define LUDEO_DEFINE_FUNC_TRAITS_ADDNOTIFY_NO_INTERFACE(CallbackName, CallbackNameU)									  \
		namespace Ludeo                                                                                                       \
		{                                                                                                                     \
			template <>                                                                                                       \
			struct ApiFuncTraits<LudeoAddNotify##CallbackName##Params>														  \
			{                                                                                                                 \
				static constexpr int32_t latestApiVersion = LUDEO_ADDNOTIFY##CallbackNameU##_##API_LATEST;					  \
				using ParamsType = LudeoAddNotify##CallbackName##Params;				                                      \
				static constexpr const char* funcName()                                                                       \
				{                                                                                                             \
					return "ludeo_AddNotify" #CallbackName;						                                              \
				}                                                                                                             \
				static constexpr const char* callbackName()                                                                   \
				{                                                                                                             \
					return #CallbackName;                                                                                     \
				}                                                                                                             \
				static constexpr const char* tracyName()                                                                      \
				{                                                                                                             \
					return #CallbackName " notification callback";                                                            \
				}                                                                                                             \
				using FuncType = decltype(&ludeo_AddNotify##CallbackName);					                                  \
				static FuncType func()                                                                                        \
				{                                                                                                             \
					return &ludeo_AddNotify##CallbackName;																	  \
				}                                                                                                             \
				using CallbackType = Ludeo##CallbackName##Callback;															  \
				using CallbackTypeCpp = Ludeo##CallbackName##CallbackCpp;													  \
				using CallbackParamsType = Ludeo##CallbackName##CallbackParams;												  \
			};                                                                                                                \
																													          \
			template <>                                                                                                       \
			struct ApiFuncTraits<Ludeo##CallbackName##CallbackParams>														  \
				: public ApiFuncTraits<LudeoAddNotify##CallbackName##Params>												  \
			{                                                                                                                 \
			};                                                                                                                \
		}
#else
	#define LUDEO_DEFINE_FUNC_TRAITS_1(...)
	#define LUDEO_DEFINE_FUNC_TRAITS_2(...)
	#define LUDEO_DEFINE_FUNC_TRAITS_WITHCALLBACK_1(...)
	#define LUDEO_DEFINE_FUNC_TRAITS_WITHCALLBACK_2(...)
	#define LUDEO_DEFINE_FUNC_TRAITS_ADDNOTIFY(...)
	#define LUDEO_DEFINE_FUNC_TRAITS_ADDNOTIFY_NO_INTERFACE(...)
#endif

//
// - An enum value called "_LASTVALUE" is added. This is not to be used by the clients. It's used internally
// - For C, a value _PADDING is added. This is to force the enum to be 32-bits
//

#ifdef __cplusplus
	//
	// If using C++ we use enum classes
	//
	#define LUDEO_ENUM_START(Name) enum class Name : int32_t {
	#define LUDEO_ENUM_END(Name) }
	#define LUDEO_ENUM_VALUE(EnumType, Name, Value) Name = Value,
	#define LUDEO_ENUM_VALUE_LAST(EnumType, Name, Value) \
		Name = Value, \
		_LASTVALUE = Value
#else
	//
	// If using C then:
	// - We use a typedef
	// - We add a last entry to force the compiler to use 32 bits
	#define LUDEO_ENUM_START(Name) typedef enum Name {
	#define LUDEO_ENUM_END(Name) , __##Name##_PADDING = 0x7FFFFFFF } Name

	#define LUDEO_ENUM_VALUE(EnumType, Name, Value) EnumType ## _ ## Name = Value,
	#define LUDEO_ENUM_VALUE_LAST(EnumType, Name, Value) EnumType ## _ ## Name = Value
#endif

/**
 * The return type of most SDK functions.
 *
 * To retrieve a string representation, use #ludeo_ResultToString
 */
LUDEO_ENUM_START(LudeoResult)

	/** Success */
	LUDEO_ENUM_VALUE(LudeoResult, Success, 0)

	/** An invalid apiVersion was passed to a function call */
	LUDEO_ENUM_VALUE(LudeoResult, InvalidVersion, 1)

	/** An invalid parameter was passed to a function */
	LUDEO_ENUM_VALUE(LudeoResult, InvalidParameters, 2)

	/**
	 * Invalid authentication details 
	 * E.g: The wrong API key was specified, or authentication failed (either the one explicitly specified or automatic auth)
	 */
	LUDEO_ENUM_VALUE(LudeoResult, InvalidAuth, 3)

	/**
	 * A resource was not found. 
	 * E.g: The client passed an invalid ludeo Id to ludeo_Session_GetLudeo
	 */
	LUDEO_ENUM_VALUE(LudeoResult, NotFound, 4)

	/**
	 * Operation timed out.
	 * E.g, a network operation timed out.
	 */
	LUDEO_ENUM_VALUE(LudeoResult, TimedOut, 5)

	/** 
	 * An error that we cannot identify has occurred.
	 * Check the logs for more info.
	 */
	LUDEO_ENUM_VALUE(LudeoResult, Unknown, 6)

	/**
	 * The operation failed because the relevant instance was not in a state valid to perform the request operation.
	 * This can be for example trying to initialize a system which is already initialized, shutting down when it's not initialized,
	 * or the SDK is not initialized.
	 */
	LUDEO_ENUM_VALUE(LudeoResult, WrongState, 7)

	/**
	 *
	 * The SDK was disabled by the backend, or the specific feature the client is trying to use is not permitted due to user consent
	 * restrictions.
	 * 
	 */
	LUDEO_ENUM_VALUE(LudeoResult, SDKDisabled, 8)

	/**
	 * An operation failed because something went wrong establishing a network connection
	 */
	LUDEO_ENUM_VALUE(LudeoResult, NetworkError, 9)

	/**
	 * An operation was canceled.
	 * One example of when this can happen is when pending operation are canceled because the SDK is shutting down
	 */
	LUDEO_ENUM_VALUE(LudeoResult, Canceled, 10)

	/**
	 * Indicates the SDK received invalid configuration from the backend.
	 */
	LUDEO_ENUM_VALUE(LudeoResult, InvalidConfiguration, 11)

	/**
	 * Something the client requested or provided is of the wrong type.
	 */
	LUDEO_ENUM_VALUE(LudeoResult, WrongType, 12)

	/**
	 * The call failed because the SDK encountered invalid data.
	 * A situation like this can happen if the SDK receives ludeo data from the backend that is invalid somehow. E.g, problems related
	 * to the data and object types the client previously registered.
	 */
	LUDEO_ENUM_VALUE_LAST(LudeoResult, InvalidData, 13)

LUDEO_ENUM_END(LudeoResult);

/**
* Boolean type used by the SDK.
* The SDK uses this even for C++ to avoid any ABI problems.
*/
typedef int32_t LudeoBool;
#define LUDEO_TRUE 1
#define LUDEO_FALSE 0

/**
 * Handle to an existing notification the client registered.
 * The value #LUDEO_INVALID_NOTIFICATIONID is used to represent an invalid notification id.
 */
EXTERN_C typedef uint64_t LudeoNotificationId;

/**
 * Invalid notification Id. "AddNotifyXXX" functions return this if they failed to register a notification
 */
#define LUDEO_INVALID_NOTIFICATIONID ((LudeoNotificationId)0)

/**
 * @}
 */

