// Copyright 2023 Ludeo. All rights reserved.

/**
 * @file
 * DataReader interface functions
 */

#pragma once

#include "Ludeo/Common.h"
#include "Ludeo/DataReaderTypes.h"

/**
 * \addtogroup DataReader
 * @{
 *
 * The DataReader interface encapsulates the concept of ludeo data. As-in, it contains all the information related to a
 * specific ludeo, such as the necessary data to restore the game state. 
 *
 * The lifetime of a DataReader instance is tied to the Session lifetime. When a session is destroyed, any DataReader handles
 * created as part of that Session are invalidated, although the client should call #ludeo_DataReader_Release when it has no
 * further need for the ludeo data.
 * 
 * A typical flow is the client gets hold of the DataReader it needs for a particular Ludeo, and keeps reusing while the player
 * keeps playing the same Ludeo. Once the player wishes to go back to the main menu, then a call to #ludeo_DataReader_Release
 * should be done to free up the resources.
 */

/**
 * Returns true or false depending if the handle is a valid handle.
 * This is useful for code that needs to quickly validate handles in a away that doesn't produce side effects. E.g: Debug code.
 */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataReader_IsValid(LudeoHDataReader handle);

/**
 * Releases the specified DataReader instance, freeing any resources.
 *
 * DataReader instances lifetime are tied to the Session they were created from and are automatically destroyed when the session
 * is destroy, but the client should call this when it has no further use for a Ludeo, in order to free the resources.
 *
 * @param handle
 *		The interface handle.
 *
 * Any attempt to use the handle after this call will fail.
 */
LUDEO_DECLARE_FUNC(LudeoResult) ludeo_DataReader_Release(LudeoHDataReader handle);

/** \defgroup ludeo_DataReader_GetInfo ludeo_DataReader_GetInfo
 * @{
 */

/**
 * Returns information related to the Ludeo the DataReader represents.
 *
 * Check the #LudeoDataReaderInfo struct for what information is available.
 *
 * @param handle
 *		The interface handle.
 *
 * @param params
 *		Parameters to the function
 *
 * @param out
 *		If #LudeoResult::Success is returned, then on exit this will contain a pointer to the information.
 * 
 * @return If LudeoResult::Success, then the "out" parameter will contain a pointer to the information.
 *
 * @important
 *		Once done with the information, the client should call #ludeo_DataReaderInfo_Release to release the struct.
 *
 */
LUDEO_DECLARE_FUNC(LudeoResult) ludeo_DataReader_GetInfo(LudeoHDataReader handle, const LudeoDataReaderGetInfoParams* params, LudeoDataReaderInfo** out);
LUDEO_DEFINE_FUNC_TRAITS_2(DataReader, DATAREADER, GetInfo, GETINFO);

/**
 * Release any resources hold by the specified structure.
 *
 * @param info
 *	Structure to release. If nulled, the call is a no-op.
 */
LUDEO_DECLARE_FUNC(void) ludeo_DataReaderInfo_Release(LudeoDataReaderInfo* info);

/**
 * @}
 *
 *\defgroup ludeo_DataReader_GetObjectsInfo ludeo_DataReader_GetObjectsInfo
 * @{
 */

/**
 * Retrieve information about all the existing objects in the ludeo data.
 *
 * The client can use this information to create all the in-game required objects.
 *
 * The client is required to maintain the right objectId/objectType mappings to its object instances.
 * E.g: if using the SDK from C++, when playing as a creator, the client will most likely create instances of several classes,
 * and for every relevant instance it will call #ludeo_DataWriter_CreateObject to register the object (with the right objectType).
 * When restoring the data to play a ludeo, the client is expected to create the right instances and set their objectId to the one supplied by this API.
 *
 * @param handle
 *		The interface handle.
 *
 * @param params
 *		Parameters to the function
 *
 * @param out
 *		If #LudeoResult::Success is returned, then on exit this will contain a pointer to the information.
 *
 * @important
 *		Once done with the information, the client should call #ludeo_ObjectsInfo_Release to release the struct.
 */
LUDEO_DECLARE_FUNC(LudeoResult) ludeo_DataReader_GetObjectsInfo(LudeoHDataReader handle, const LudeoDataReaderGetObjectsInfoParams* params, LudeoObjectsInfo** out);
LUDEO_DEFINE_FUNC_TRAITS_2(DataReader, DATAREADER, GetObjectsInfo, GETOBJECTSINFO);

/**
 * @}
 */

/**
 * Release the resources taken by the specified structure.
 */
LUDEO_DECLARE_FUNC(void) ludeo_ObjectsInfo_Release(LudeoObjectsInfo* info);

/**
 * Sets the DataReader instance to use for any further calls to DataReader API functions that don't explicitly ask for a handle.
 * This allows the client to have less verbose code when reading data.
 */
LUDEO_DECLARE_FUNC(LudeoResult) ludeo_DataReader_SetCurrent(LudeoHDataReader handle);

/**
 * Pushes an object to the context stack.
 *
 * Any ludeo_DataReader_XXX calls to read data will act on this object, until #ludeo_DataReader_LeaveObject is called.
 *
 * Typically a client will first make a call to #ludeo_DataReader_GetObjectsInfo to read back all the object ids and their types.
 * With that information the client instantiates all the game objects it needs (of the right type) and assigns the LudeoObjectId
 * to each instance to match the Ludeo data.
 * Once that is done the client can then query the data to deserialize every object by using this function as the starting point
 * for each object.
 *
 * @param objectId
 *		Object to use.
 *
 * @return
 *		True if successfully entered the object (the object exists), or False if the object doesn't exist.
 *
 * @important
 *		For every successful call to this function, there must be a counterpart #ludeo_DataReader_LeaveObject call to unwind the
 *		context stack.
 */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataReader_EnterObject(LudeoObjectId objectId);

/**
 * Pops the object from the context stack.
 *
 * This should only be called after a successful call to #ludeo_DataReader_EnterObject.
 *
 * @return
 *		True if there were no errors detected such as not being inside an object. The only reason this can return False is if
 *		the client has a mismatch in the EnterObject/LeaveObject calls.
 */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataReader_LeaveObject(void);

/**
 * Pushes a component to the context stack.
 *
 * @important
 *		For every successful call to this function, there most be a counterpart #ludeo_DataReader_LeaveComponent call to unwind the
 *		context stack.
 */ 
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataReader_EnterComponent(const char* name);

/**
 * Pops a component from the context stack
 *
 * @important
 *		For every successful call to this function, there must be a counterpart #ludeo_DataWriter_LeaveComponent call to unwind
 *		the context stack.
 *
 * @return
 *		The only reason this can return False is if the client has a mismatch in the EnterComponent/LeaveComponent calls, or is
 *		not inside an object.
 */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataReader_LeaveComponent(void);

/**
 * Checks if an attribute exists in the current context.
 *
 * @param attributeName
 *		Attribute to search for.
 *
 * @param outType
 *		If not null, on return it will contain the attribute type. If nulled, then the function simply checks if the attribute
 *		exists
 *
 * @return
 *		True if the attribute was found, in which case outType will have the type on exit, if not nulled.
 */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataReader_AttributeExists(const char* attributeName, LudeoDataType* outType);

/**
 * \defgroup ludeo_DataReader_VisitAttributes ludeo_DataReader_VisitAttributes
 * @{
 */

/**
 * Visits all attributes in the current context.
 *
 * @param params
 *		Struct with any parameters to the function
 *
 * @param clientData
 *		Context that will be passed to the callback. Can be nulled.
 *
 * @param callback
 *		The visit function. This will be called for every attribute found in the current context. It will be called for components
 *		too, but not for the component's attributes. Can be nulled, in which case it will just to count the number of attributes.
 *
 * @param outCount
 *		If the result is successful and this is not nulled, on exit it will contain the number of attributes for the current
 *		context
 *
 * @return True if the current context is valid to visit attributes, false otherwise
 *
 */
LUDEO_DECLARE_FUNC(LudeoResult) ludeo_DataReader_VisitAttributes(const LudeoDataReaderVisitAttributesParams* params, uint32_t* outCount, void* clientData, LudeoDataReaderVisitAttributesCallback callback);
LUDEO_DEFINE_FUNC_TRAITS_WITHCALLBACK_2(DataReader, DATAREADER, VisitAttributes, VISITATTRIBUTES);

/**
 * @}
 *
 *\defgroup ludeo_DataReader_LoadStorage ludeo_DataReader_LoadStorage
 * @{
 */

/**
 * Looks for the specified key in the binary storage.
 *
 * This allows the client to query the data which got stored into the binary storage
 *
 * @param handle
 *		The interface handle.
 *
 * @param params
 *		Struct with any parameters to the function
 *
 * @param clientData
 *		Context that will be passed to the callback. Can be nulled.
 *
 * @param callback
 *		This will be called for the binary data found in the storage behind the given key.
 */

LUDEO_DECLARE_FUNC(void) ludeo_DataReader_LoadStorage(LudeoHDataReader handle, const LudeoDataReaderLoadStorageParams* params, void* clientData, LudeoDataReaderLoadStorageCallback callback);
LUDEO_DEFINE_FUNC_TRAITS_WITHCALLBACK_2(DataReader, DATAREADER, LoadStorage, LOADSTORAGE);

/**
 * Looks for the specified key in the storage and returns its size in bytes.
 *
 * This allows the client to query the size of storages for which it needs to pre-allocate memory. E.g, when reading back a
 * dynamic string.
 *
 * @param params
 *		Struct with any parameters to the function
 *
 * @param outSize
 *		Size of the data in bytes.
 *
 * @return
 *		True if the attribute was found at the current level, in which case outSize will contain the size.
 */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataReader_GetStorageSize(const LudeoDataReaderGetStorageSizeParams* params, uint32_t* outSize);
LUDEO_DEFINE_FUNC_TRAITS_2(DataReader, DATAREADER, GetStorageSize, GETSTORAGESIZE);

/**
 * @}
 */

/**
 * Looks for the specified attribute in the current context and returns its size in bytes.
 *
 * This allows the client to query the size of attributes for which it needs to pre-allocate memory. E.g, when reading back a
 * dynamic string.
 *
 * @param attributeName
 *		Name of the attribute to query.
 *
 * @param outSize
 *		Size of the attribute in bytes.
 *		If the attribute is a string, this is the size of the string in bytes, excluding the null
 *		character. E.g: If the string is empty, this will be 0.
 *
 * @return
 *		True if the attribute was found at the current level, in which case outSize will contain the size.
 */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataReader_GetSize(const char* attributeName, uint32_t* outSize);


/**
 * \defgroup ludeo_DataReader_Get ludeo_DataReader_Get
 * @{
 *
 * Functions to get gameplay state take an `attributeName`, destination pointer, and return #LudeoBool to indicate success or
 * failure.<br>
 * The value is retrieved according to the current context stack ( see #ludeo_DataReader_EnterObject and
 * #ludeo_DataReader_EnterComponent )
 * 
 * Most primitive data types are supported, and also some complex ones. More will be added in the future as necessary.
 */

/** Read a boolean */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataReader_GetBool  (const char* attributeName, LudeoBool* outValue);

// If using C++, then overload to allow using "bool" directly
#ifdef __cplusplus
	/** C++ overload to read a `bool` */
	inline LudeoBool ludeo_DataReader_GetBool(const char* attributeName, bool* value)
	{
		LudeoBool tmpValue;
		LudeoBool ret = ludeo_DataReader_GetBool(attributeName, &tmpValue);
		if (ret == LUDEO_TRUE)
		{
			*value = (tmpValue == LUDEO_TRUE) ? true : false;
		}

		return ret;
	}
#endif
/** Get a int8_t */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataReader_GetInt8  (const char* attributeName, int8_t* outValue);
/** Get a uint8_t */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataReader_GetUInt8 (const char* attributeName, uint8_t* outValue);
/** Get a int16_t */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataReader_GetInt16 (const char* attributeName, int16_t* outValue);
/** Get a uint16_t */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataReader_GetUInt16(const char* attributeName, uint16_t* outValue);
/** Get a int32_t */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataReader_GetInt32 (const char* attributeName, int32_t* outValue);
/** Get a uint32_t */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataReader_GetUInt32 (const char* attributeName, uint32_t* outValue);
/** Get a int64_t */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataReader_GetInt64 (const char* attributeName, int64_t* outValue);
/** Get a uint64_t */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataReader_GetUInt64 (const char* attributeName, uint64_t* outValue);
/** Get a float (32 bits) */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataReader_GetFloat (const char* attributeName, float* outValue);
/** Get a double (64 bits) */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataReader_GetDouble(const char* attributeName, double* outValue);

/**
 * Gets a binary blob attribute.
 *
 * @important
 * When reading blobs, the client should first do a call to #ludeo_DataReader_GetSize to get the blob size and make sure
 * the buffer provided to this function is big enough.
 */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataReader_GetBlob(const char* attributeName, uint8_t* outValue);

/**
 * Gets a string attribute.
 * 
 * @important
 * The NULL character is **NOT** copied, and it's the client's responsibility to correctly terminate the string.
 * When reading strings, the client should first do a call to #ludeo_DataReader_GetSize to get the string size and make sure the
 * buffer provided to this function is big enough. After the call, the client can then null terminate the string since it knows
 * the size.
 */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataReader_GetString(const char* attributeName, char* outValue);
/**
 * Get an array of 3 floats (e.g x,y,z).
 * @param attributeName
 *		The attribute to read.
 *
 * @param outValue
 *		A to 3 consecutive floats.
 */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataReader_GetVec3Float(const char* attributeName, float* outValue);
/**
 * Get an array of 3 floats (e.g A quaternion).
 *
 * @param attributeName
 *		The attribute to read.
 *
 * @param outValue
 *		A to 4 consecutive floats.
 */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataReader_GetVec4Float(const char* attributeName, float* outValue);

// Some C++ helpers to make it easier to use from C++
#if defined(__cplusplus) && defined(__cpp_if_constexpr)

	#if __has_include(<type_traits>)
		#include <type_traits>
	#endif

	/**
	 * C++ template to make reading values easier from C++
	 *
	 * The client can create overrides for it's own types, like string classes.
	 */
	template<typename T>
	LudeoBool ludeo_DataReader_Get(const char* attributeName, T& value)
	{
		if constexpr(std::is_same_v<T, bool>)
		{
			return ludeo_DataReader_GetBool(attributeName, &value);
		}
		else if constexpr(std::is_same_v<T, int8_t>)
		{
			return ludeo_DataReader_GetInt8(attributeName, &value);
		}
		else if constexpr(std::is_same_v<T, uint8_t>)
		{
			return ludeo_DataReader_GetUInt8(attributeName, &value);
		}

		else if constexpr(std::is_same_v<T, int16_t>)
		{
			return ludeo_DataReader_GetInt16(attributeName, &value);
		}
		else if constexpr(std::is_same_v<T, uint16_t>)
		{
			return ludeo_DataReader_GetUInt16(attributeName, &value);
		}

		else if constexpr(std::is_same_v<T, int32_t>)
		{
			return ludeo_DataReader_GetInt32(attributeName, &value);
		}
		else if constexpr(std::is_same_v<T, uint32_t>)
		{
			return ludeo_DataReader_GetUInt32(attributeName, &value);
		}
		else if constexpr(std::is_same_v<T, int64_t>)
		{
			return ludeo_DataReader_GetInt64(attributeName, &value);
		}
		else if constexpr(std::is_same_v<T, uint64_t>)
		{
			return ludeo_DataReader_GetUInt64(attributeName, &value);
		}
		else if constexpr(std::is_same_v<T, float>)
		{
			return ludeo_DataReader_GetFloat(attributeName, &value);
		}
		else if constexpr(std::is_same_v<T, double>)
		{
			return ludeo_DataReader_GetDouble(attributeName, &value);
		}
		else
		{
			// This static_assert makes sure that if we call the function with a type that is not supported, it doesn't compile.
			static_assert(std::is_same_v<T, void> == true);

			return LUDEO_FALSE;
		}
	}

	inline LudeoBool ludeo_DataReader_Get(const char* attributeName, char* value)
	{
		return ludeo_DataReader_GetString(attributeName, value);
	}
#endif

/**
 * @}
 */

/**
 * @}
 */
