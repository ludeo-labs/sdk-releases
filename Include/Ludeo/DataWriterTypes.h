// Copyright 2023 Ludeo. All rights reserved.

/**
 * @file
 * Types used by the DataWriter interface
 */

#pragma once

#include "Ludeo/Common.h"
#include "Ludeo/DataTypesCommon.h"

#pragma pack(push, 8)

/** \addtogroup DataWriter
 * @{
 */

/** Opaque DataWriter handle */
EXTERN_C typedef struct LudeoDataWriterHandle* LudeoHDataWriter;

/**
 *\addtogroup ludeo_DataWriter_SetSendSettings
 * @{
 */

/** The most recent version of the #ludeo_DataWriter_SetSendSettings API */
#define LUDEO_DATAWRITER_SETSENDSETTINGS_API_LATEST 1

/** Parameters to #ludeo_DataWriter_SetSendSettings */
LUDEO_STRUCT_START(LudeoDataWriterSetSendSettingsParams)
	/** Set this to LUDEO_DATAWRITER_SETSENDSETTINGS_API_LATEST */
	int32_t apiVersion;

	/** 
	 * Interval between sends, in milliseconds.
	 * In practice this won't be exact, because data sending only occurs as the result of a ludeo_Tick.
	 *
	 * For performance and bandwidth reasons, the SDK might impose a limit on how small this value can be.
	 */
	uint32_t sendIntervalMs;
LUDEO_STRUCT_END(LudeoDataWriterSetSendSettingsParams);

/**
 * @}
 *
 *\addtogroup ludeo_DataWriter_CreateObject
 * @{
 */

/** The most recent version of the #ludeo_DataWriter_CreateObject API */
#define LUDEO_DATAWRITER_CREATEOBJECT_API_LATEST 1

/** Parameters to #ludeo_DataWriter_CreateObject */
LUDEO_STRUCT_START(LudeoDataWriterCreateObjectParams)
	/** Set this to LUDEO_DATAWRITER_CREATEOBJECT_API_LATEST */
	int32_t apiVersion;

	/**
	 * String that that identifies the object type.
	 * The SDK doesn't impose any restrictions on this. Meaning is up to the client.
	 */
	 const char* objectType;
LUDEO_STRUCT_END(LudeoDataWriterCreateObjectParams);

/**
 * @}
 *
 *\addtogroup ludeo_DataWriter_DestroyObject
 * @{
 */

/** The most recent version of the #ludeo_DataWriter_DestroyObject API */
#define LUDEO_DATAWRITER_DESTROYOBJECT_API_LATEST 1


/** Parameters to #ludeo_DataWriter_DestroyObject */
LUDEO_STRUCT_START(LudeoDataWriterDestroyObjectParams)
	/** Set this to LUDEO_DATAWRITER_DESTROYOBJECT_API_LATEST */
	int32_t apiVersion;

	/**
	 * Object to destroy
	 */
	LudeoObjectId objectId;
LUDEO_STRUCT_END(LudeoDataWriterDestroyObjectParams)

/**
 * @}
 *
 *\addtogroup ludeo_DataWriter_SendAction
 * @{
 */

/** The most recent version of the #ludeo_DataWriter_DestroyObject API */
#define LUDEO_DATAWRITER_SENDACTION_API_LATEST 1

/** Parameters to #ludeo_DataWriter_SendAction */
LUDEO_STRUCT_START(LudeoDataWriterSendActionParams)
	/** Set this to LUDEO_DATAWRITER_SENDACTION_API_LATEST */
	int32_t apiVersion;

	/**
	 * Player id the action applies to.
	 *
	 * This is required.
	 */
	const char* playerId;

	/**
	 * The action to send.
	 *
	 * This is required.
	 */
	const char* action;
LUDEO_STRUCT_END(LudeoDataWriterSendActionParams);

/**
 * @}
 *
 *\addtogroup ludeo_DataWriter_SaveStorage
 * @{
 */

/** The most recent version of the #ludeo_DataWriter_SaveStorage API */
#define LUDEO_DATAWRITER_SAVESTORAGE_API_LATEST 1

/** Parameters to #ludeo_DataWriter_SaveStorage */
LUDEO_STRUCT_START(LudeoDataWriterSaveStorageParams)
	/** Set this to LUDEO_DATAWRITER_SAVESTORAGE_API_LATEST */
	int32_t apiVersion;
	
	/**
	 * The key which the binary data got saved with
	 */
	const char* key;
	
	/**
	 * Binary data to save in the storage.
	 */
	uint8_t* data;
	
	/**
	 * Size of the data.
	 */
	uint32_t size;

LUDEO_STRUCT_END(LudeoDataWriterSaveStorageParams);

/** The parameters for the #ludeo_DataWriter_SaveStorage callback. */
LUDEO_STRUCT_START(LudeoDataWriterSaveStorageCallbackParams)
	/** Context that was passed to #ludeo_DataWriter_SaveStorage */
	void* clientData;
	/** The error code which describes what happened  */
	LudeoResult resultCode;
	LUDEO_STRUCT_END(LudeoDataWriterSaveStorageCallbackParams);

/** Signature for the #ludeo_DataWriter_SaveStorage callback */
LUDEO_DECLARE_CALLBACK(LudeoDataWriterSaveStorageCallback, const LudeoDataWriterSaveStorageCallbackParams* data);
/**
 * @}
 */

/**
 * @}
 */

#pragma pack(pop)

