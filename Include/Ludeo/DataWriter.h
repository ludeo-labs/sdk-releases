// Copyright 2023 Ludeo. All rights reserved.

/**
 * @file
 * DataWriter interface functions
 */
#pragma once

#include "Ludeo/Common.h"
#include "Ludeo/DataWriterTypes.h"

/**
 * \addtogroup DataWriter
 * @{
 *
 * The DataWriter interface provides a way for the client to send gameplay state and player actions to the SDK.
 *
 * The client needs to send gameplay relevant data to the backend during both Creator and Ludeo runs:
 * - During a Creator run, the client needs to report gameplay data so the backend then has the required data to allow creating
 *   Ludeos
 * - During a Ludeo run, the client needs to report it so the backend calculates scoring for said Ludeo.
 *
 * A DataWriter instance is tied to the run that it originated from, and is invalidated as soon as the run finishes.
 * As-in, an instance is given to the client when starting a Creator or Player run so the client can report data. Once the run
 * finishes, the handle is invalidated automatically
 *
 * Data is not sent necessarily every tick. The real frequency will depend on the SDK internal logic, any relevant backend
 * settings, and settings specified by the client.
 */


/**
 * Returns true or false depending if the handle is a valid handle.
 * This is useful for code that needs to quickly validate handles in a away that doesn't produce side effects. E.g: Debug code.
 */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataWriter_IsValid(LudeoHDataWriter handle);

/**
 * Sets the DataWriter instance to use for any further calls to DataWriter API functions that don't explicitly ask for a handle.
 *
 * Since the client will be calling DataWriter possibly hundreds/thousands of times per frame, this function allows the SDK
 * to do the necessary API checks once (handle checking, internal lookups, etc), cache what it needs, and thus decrease the
 * overhead of the ludeo_DataWriter_SetXXX calls themselves.
 * 
 * If for whatever reason the client needs to call this function for every single ludeo_DataWriter_SetXXX call, then it should
 * cache what was the last handle passed to SetCurrent, and only make the call if necessary.
 *
 * E.g, considering the client has a generic write function that accepts a DataWriter handle and sets an attribute:
 *
 * ```
 *	template<typename T>
 *	bool genericWrite(LudeoHDataWriter dataWriter, const char* attributeName, const T& value)
 *	{
 *		static LudeoHDataWriter current = nullptr;
 * 
 *		if (current != dataWriter)
 *		{
 *			ludeo_DataWriter_SetCurrent(dataWriter);
 *			current = dataWriter;
 *		}
 * 
 *		return ludeo_DataWriter_Set(attributeName, value) == LUDEO_TRUE ? true : false;
 *	}
 * ```
 *
 * If using some a wrapper for the handle, make sure you compare the handle value itself and not the pointer to your wrapper.
 * For example, the following approach has a potential bug when your wrapper gets destroyed and created again:
 *
 * ```
 *	// Some wrapper the client has for the handle
 *	struct LudeoDataWriterWrapper
 *	{
 *		LudeoHDataWriter handle;
 *		... Other data and function members ...
 *	};
 *	
 *	template<typename T>
 *	bool genericWrite(LudeoDataWriterWrapper& dataWriter, const char* attributeName, const T& value)
 *	{
 *		static LudeoDataWriterWrapper* current = nullptr;
 *
 *		// Possible bug. "current" and "dataWriter" might be the same pointer, but different DataWriter handles.
 *		if (current != &dataWriter)
 *		{
 *			ludeo_DataWriter_SetCurrent(dataWriter.handle);
 *			current = &dataWriter;
 *		}
 *	
 *		return ludeo_DataWriter_Set(attributeName, value) == LUDEO_TRUE ? true : false;
 *	}
 * ```
 * 
 * @param handle
 *		Handle of the instance to set, or nullptr to unset.
 */
LUDEO_DECLARE_FUNC(LudeoResult) ludeo_DataWriter_SetCurrent(LudeoHDataWriter handle);


#ifndef DOXYGEN_SHOULD_SKIP_THIS
// #RVF I'm not sure specifying a global send frequency is a smart enough approach. What we end up doing might depend on if we
// implement the snapshot thing we discussed about. Some keys might need more frequent sends than others, and this might be game
// specific.
#endif

/** \defgroup ludeo_DataWriter_SetSendSettings ludeo_DataWriter_SetSendSettings
 * @{
 */

/**
 * Allows tweaking the data sending rules. E.g, how often to send data.
 * The SDK assumes sensible defaults.
 *
 * One thing to note is that data sending is triggered only as the result of a ludeo_Tick call, therefore the true frequency of
 * the sends doesn't depend solely on the frequency the client specifies.
 * When the client calls ludeo_Tick, the SDK checks the total time elapsed since the last send, and compares it against the desired
 * send frequency. If greater or equal, then it sends any pending data.
 *
 * @param handle
 *		The interface handle.
 *
 * @param params
 *		Parameters to the function
 *
 * @return
 *		LudeoResult::Success on success.
 */ 
LUDEO_DECLARE_FUNC(LudeoResult) ludeo_DataWriter_SetSendSettings(LudeoHDataWriter handle, const LudeoDataWriterSetSendSettingsParams* params);
LUDEO_DEFINE_FUNC_TRAITS_2(DataWriter, DATAWRITER, SetSendSettings, SETSENDSETTINGS);

/**
 * @}
 *
 *\defgroup ludeo_DataWriter_CreateObject ludeo_DataWriter_CreateObject
 * @{
 */

/**
 * Create a new object to track
 * 
 * This function can be used for two purposes:
 * - Create a new object
 * - Validate that the client is properly setup (when playing a ludeo).
 *
 * When playing a ludeo, the SDK will internally create any objects that exist in the ludeo data and the client is
 * expected to query the ludeo data for the existing object ids (and objecType) and assign those object ids to the right instances
 * it created. The output parameter "outObjectId" also functions as an input parameter to specify what behaviour to use.
 *
 * @param handle
 *		The interface handle
 *
 * @param params
 *		Function parameters
 *
 * @param outObjectId
 *	- On entry it should point to a LUDEO_INVALID_OBJECTID value or a valid object id. If LUDEO_INVALID_OBJECTID, the SDK will
 * create a new object, and on exit it will contain that new id.
 *	- If on entry it points to something else other than a LUDEO_INVALID_OBJECTID, the SDK validates if that is an existing object
 *	id and it matches the objectType specified in params. If it doesn't match it returns an error.
 *
 * @important
 *	For convenience , the SDK allows calling #ludeo_DataWriter_CreateObject while inside another object, but that by itself **does not** make the objects related in any way.
 *
 */
LUDEO_DECLARE_FUNC(LudeoResult) ludeo_DataWriter_CreateObject(LudeoHDataWriter handle, const LudeoDataWriterCreateObjectParams* params, LudeoObjectId* outObjectId);
LUDEO_DEFINE_FUNC_TRAITS_2(DataWriter, DATAWRITER, CreateObject, CREATEOBJECT);

/**
 * @}
 *
 *\defgroup ludeo_DataWriter_DestroyObject ludeo_DataWriter_DestroyObject
 * @{
 */

/**
 * Destroyed the specified object
 *
 * If the call succeeds, the client should not attempt to make use of this object id again
 *
 * @param handle
 *		The interface handle.
 *
 * @param params
 *		Function parameters
 */
LUDEO_DECLARE_FUNC(LudeoResult) ludeo_DataWriter_DestroyObject(LudeoHDataWriter handle, const LudeoDataWriterDestroyObjectParams* params);
LUDEO_DEFINE_FUNC_TRAITS_2(DataWriter, DATAWRITER, DestroyObject, DESTROYOBJECT);

/**
 * @}
 */


/**
 * Pushes an object to the context stack.
 * Any ludeo_DataWriter_XXX calls to write data will act on this object, until #ludeo_DataWriter_LeaveObject is called.
 *
 * @param objectId
 *		Object to use.
 *
 * @return
 *		True if successfully entered the object (the object exists), or False if the object doesn't exist.
 *
 * @important
 *		For every successful call to this function, there must be a counterpart #ludeo_DataWriter_LeaveObject call to unwind the
 *		context stack.
 *
 * @important
 *	For convenience , the SDK allows nesting #ludeo_DataWriter_EnterObject calls, but that by itself **does not** make the objects related in any way.
 */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataWriter_EnterObject(LudeoObjectId objectId);

/**
 * Pushes an object to the context stack with a delayed timestamp.
 * Any ludeo_DataWriter_XXX calls to write data will act on this object, until #ludeo_DataWriter_LeaveObject is called.
 *
 * This function is similar to #ludeo_DataWriter_EnterObject, but it allows the client to specify a timestamp for the object.
 * This is useful when the client has a mismatch between the time the object data was created and the time it is being reported.
 * The SDK ensures that the timestamp of all attributes set until the next #ludeo_DataWriter_LeaveObject will be associated with the specified timestamp.
 * 
 * @param objectId
 *		Object to use.
 *
 * @param timestamp
 *		Timestamp which will be associated with the object changed attributes.
 *		The timestamp parameter must be generated by the SDK using the #ludeo_GetCurrentTimestamp function.
 *		For any attribute the client sets with a delayed timestamp, the timestamp should be higher than the timestamp used in the previous call.
 *		In practice this also means an attribute that is being set as part of calling this function should not be set when using 
 *		#ludeo_DataWriter_EnterObject, and vice versa, since that means the timestamp used to set the attribute would be increasing and decreasing.
 *
 * @return
 *		True if successfully entered the object (the object exists), or False if the object doesn't exist.
 *
 * @important
 *		For every successful call to this function, there must be a counterpart #ludeo_DataWriter_LeaveObject call to unwind the
 *		context stack. 
 */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataWriter_EnterObjectLate(LudeoObjectId objectId, uint64_t timestamp);

/**
 * Pops the object from the context stack.
 *
 * This should only be called after a successful call to #ludeo_DataWriter_EnterObject.
 *
 * @return
 *		True if there were no errors detected such as not being inside an object. The only reason this can return False is if
 *		the client has a mismatch in the EnterObject/LeaveObject calls.
 */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataWriter_LeaveObject(void);

/**
 * Pushes a component to the context stack.
 *
 * @important
 *		For every successful call to this function, there must be a counterpart #ludeo_DataWriter_LeaveComponent call to unwind the
 *		context stack.
 */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataWriter_EnterComponent(const char* name);

/**
 * Pops a component from the context stack.
 *
 * @important
 *		For every successful call to this function, there must be a counterpart #ludeo_DataWriter_LeaveComponent call to unwind
 *		the context stack.
 *
 * @return
 *		The only reason this can return False is if the client has a mismatch in the EnterComponent/LeaveComponent calls, or is
 *		not inside an object.
 */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataWriter_LeaveComponent(void);

/**
 * Binds a player to the current object.
 *
 * This should only be called when inside an object. 
 * It applies to the object itself and is not affected by the actual full context stack level. As-in, once inside an object,
 * this can be called anywhere in the context stack.
 *
 * Repeatedly changing the player binding on a given object has the potential to increase bandwidth (and cpu) usage.
 * This is because any data written to the DataWriter is associated with the binding (either if set or not), so once a binding is
 * changed, the SDK needs to forcibly send any pending data that object has.
 *
 * @param playerId
 *	If specified and not an empty string, it will bind the current object to that player.
 *	If nullptr or an empty string it will remove the current binding.
 *	An object only allows one player to be bound to it at anytime, so if the client wishes to change the binding from one player
 *  to another, there is no need for an intermediate call with null (to unbind). It can simply specify the player to switch to.
 */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataWriter_SetPlayerBinding(const char* playerId);


/**
 *\defgroup ludeo_DataWriter_Set ludeo_DataWriter_Set
 * @{
 *
 * Functions to set gameplay state take an `attributeName` and value and return #LudeoBool to indicate success or failure.
 * The value is set according to the current context stack ( see #ludeo_DataWriter_EnterObject and
 * #ludeo_DataWriter_EnterComponent )
 * 
 * Most primitive data types are supported, and also some complex ones. More will be added in the future as necessary.
 */

/** Set a boolean */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataWriter_SetBool(const char* attributeName, LudeoBool value);

// If using C++, then overload to allow using "bool" directly
#ifdef __cplusplus
	/** C++ overload to set a `bool` */
	inline LudeoBool ludeo_DataWriter_SetBool(const char* attributeName, bool value)
	{
		return ludeo_DataWriter_SetBool(attributeName, LudeoBool(value));
	}
#endif

/** Set a int8_t */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataWriter_SetInt8(const char* attributeName, int8_t value);
/** Set a uint8_t */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataWriter_SetUInt8(const char* attributeName, uint8_t value);
/** Set a int16_t */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataWriter_SetInt16(const char* attributeName, int16_t value);
/** Set a uint16_t */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataWriter_SetUInt16(const char* attributeName, uint16_t value);
/** Set a int32_t */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataWriter_SetInt32(const char* attributeName, int32_t value);
/** Set a uint32_t */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataWriter_SetUInt32(const char* attributeName, uint32_t value);
/** Set a int64_t */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataWriter_SetInt64(const char* attributeName, int64_t value);
/** Set a uint64_t */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataWriter_SetUInt64(const char* attributeName, uint64_t value);
/** Set a float (32 bits) */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataWriter_SetFloat(const char* attributeName, float value);
/** Set a double (64 bits) */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataWriter_SetDouble(const char* attributeName, double value);
/** Set an array of 3 floats (e.g x,y,z) */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataWriter_SetVec3Float(const char* attributeName, const float* value);
/** Set an array of 4 floats (e.g A quaternion) */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataWriter_SetVec4Float(const char* attributeName, const float* value);
/** Set a string */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataWriter_SetString(const char* attributeName, const char* value);

/** 
 * Set a binary blob attribute.
 *
 * Blob attributes provide a way for the client to save any arbitrary data, which makes it a very flexible tool to leverage
 * serialization code the client might already have for game objects, but note that care must be taken not to over-rely on blobs,
 * since even if a single byte changes (compared to the previous value), the entire blob needs to be sent, which can cause bandwidth
 * problems.
 *
 * @param attributeName attribute name.
 * @param data pointer to the blob data. If the size parameter is 0, then this is allowed to be null.
 * @param size Size of the blob in bytes.
 *
 * @return LUDEO_TRUE on success, LUDEO_FALSE on failure.
 *
 * @remark
 * If the room that created this DataWriter was opened to play a ludeo (see #ludeo_Session_OpenRoom and #LudeoSessionOpenRoomParams),
 * this function performs no action and immediately returns LUDEO_TRUE. \n
 * Therefore, as an additional optimization at the client level, the client should avoid creating blobs when in ludeo playing mode,
 * as doing so results in unnecessary CPU usage for something that is thrown away.
 */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_DataWriter_SetBlob(const char* attributeName, const uint8_t* data, uint32_t size);

// Some C++ helpers to make it easier to use from C++
#if defined(__cplusplus) && defined(__cpp_if_constexpr)

	#if __has_include(<type_traits>)
		#include <type_traits>
	#endif


	/**
	 * C++ template to make setting values easier from C++.
	 *
	 * The client can create overrides for it's own types, like string classes.
	 */
	template<typename T>
	LudeoBool ludeo_DataWriter_Set(const char* attributeName, T value)
	{
		if constexpr(std::is_same_v<T, int8_t>)
		{
			return ludeo_DataWriter_SetInt8(attributeName, value);
		}
		else if constexpr(std::is_same_v<T, uint8_t>)
		{
			return ludeo_DataWriter_SetUInt8(attributeName, value);
		}

		else if constexpr(std::is_same_v<T, int16_t>)
		{
			return ludeo_DataWriter_SetInt16(attributeName, value);
		}
		else if constexpr(std::is_same_v<T, uint16_t>)
		{
			return ludeo_DataWriter_SetUInt16(attributeName, value);
		}

		else if constexpr(std::is_same_v<T, int32_t>)
		{
			return ludeo_DataWriter_SetInt32(attributeName, value);
		}
		else if constexpr(std::is_same_v<T, uint32_t>)
		{
			return ludeo_DataWriter_SetUInt32(attributeName, value);
		}

		else if constexpr(std::is_same_v<T, int64_t>)
		{
			return ludeo_DataWriter_SetInt64(attributeName, value);
		}
		else if constexpr(std::is_same_v<T, uint64_t>)
		{
			return ludeo_DataWriter_SetUInt64(attributeName, value);
		}

		else if constexpr(std::is_same_v<T, bool>)
		{
			return ludeo_DataWriter_SetBool(attributeName, value);
		}
		else if constexpr(std::is_same_v<T, float>)
		{
			return ludeo_DataWriter_SetFloat(attributeName, value);
		}
		else if constexpr(std::is_same_v<T, double>)
		{
			return ludeo_DataWriter_SetDouble(attributeName, value);
		}
		else if constexpr(std::is_same_v<T, const char*>)
		{
			return ludeo_DataWriter_SetString(attributeName, value);
		}
		else
		{
			static_assert(std::is_same_v<T, void> == true);
			return LUDEO_FALSE;
		}
	}
#endif

/**
 * @}
 *
 *
 *\defgroup ludeo_DataWriter_SendAction ludeo_DataWriter_SendAction
 * @{
 */

/**
 * Sends a player action to the backend
 *
 * Actions have no parameters. They represent a single thing such as "Kill", "HeadShot", etc.
 *
 * @param handle
 *		The interface handle.
 *
 * @param params
 *		Function parameters
 */
LUDEO_DECLARE_FUNC(LudeoResult) ludeo_DataWriter_SendAction(LudeoHDataWriter handle, const LudeoDataWriterSendActionParams* params);
LUDEO_DEFINE_FUNC_TRAITS_2(DataWriter, DATAWRITER, SendAction, SENDACTION);

/**
 * @}
 *
 *\defgroup ludeo_DataWriter_SaveStorage ludeo_DataWriter_SaveStorage
 * @{
 */

/**
 * Saves a binary payload to SDK-managed storage associated with the current run (Creator or Ludeo).
 *
 * This is a low-level, generic storage mechanism for arbitrary data (e.g., precomputed analytics,
 * snapshots, serialized game state, cache entries, thumbnails, etc.). Use this when you need to store data
 * that does not map to regular DataWriter attributes or when you want to batch/aggregate data
 * outside the per-tick attribute model.
 *
 * Typical use-cases:
 * - Persisting serialized object graphs or level state needed for later analysis.
 * - Saving binary assets (small thumbnails, metadata blobs) tied to a specific run.
 * - Exporting preprocessed stats or checkpoints that are consumed by backend scoring.
 *
 * Notes:
 * - Storage is tied to the DataWriter's run; data becomes invalid once the run lifecycle ends
 *   according to backend retention policies.
 * - Large payloads may impact bandwidth/CPU; prefer compact binary formats and avoid frequent writes.
 * - Completion is reported via the callback, which receives the result and any SDK-provided context.
 *
 * @param handle
 *      The interface handle.
 * @param params
 *      Parameters describing the storage key, payload pointer, size, and options.
 * @param clientData
 *      Opaque pointer forwarded to the callback for caller context (may be nullptr).
 * @param callback
 *      Completion callback invoked when the save operation finishes (success or failure).
 */
LUDEO_DECLARE_FUNC(void) ludeo_DataWriter_SaveStorage(LudeoHDataWriter handle, const LudeoDataWriterSaveStorageParams* params, void* clientData, LudeoDataWriterSaveStorageCallback callback);
LUDEO_DEFINE_FUNC_TRAITS_WITHCALLBACK_2(DataWriter, DATAWRITER, SaveStorage, SAVESTORAGE);

/**
 * @}
 */

/**
 * @}
 */
