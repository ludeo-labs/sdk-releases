// Copyright 2023 Ludeo. All rights reserved.

/**
 * @file
 * Types used by the DataReader interface
 */

#pragma once

#include "Ludeo/Common.h"
#include "Ludeo/DataTypesCommon.h"

#pragma pack(push, 8)

/** \addtogroup DataReader
 * @{
 */

/** Opaque DataWriter handle */
EXTERN_C typedef struct LudeoDataReaderHandle* LudeoHDataReader;

/**
 *\addtogroup ludeo_DataReader_GetInfo
 * @{
 */

/** The most recent version of the #ludeo_DataReader_GetInfo API */
#define LUDEO_DATAREADER_GETINFO_API_LATEST 1

/** The parameters to the function */
LUDEO_STRUCT_START(LudeoDataReaderGetInfoParams)
	/** Set this to LUDEO_DATAREADER_GETINFO_API_LATEST */
	int32_t apiVersion;
LUDEO_STRUCT_END(LudeoDataReaderGetInfoParams);

/** Latest version of the LudeoDataReaderInfo struct */
#define LUDEO_DATAREADERINFO_API_LATEST 1

/**
 * Structure used as an out parameter for the #ludeo_DataReader_GetInfo.
 */
LUDEO_STRUCT_START(LudeoDataReaderInfo)
	/**
	 * Set by the SDK when allocating the struct
	 */
	int32_t apiVersion;

	/**
	 * The id of the ludeo the DataReader refers to.
	 * The client should NOT keep hold of this pointer. It should copy the string instead of keeping the pointer.
	 */
	const char* ludeoId;

	/**
	 * Id of the player that created the ludeo.
	 */
	const char* playerId;
LUDEO_STRUCT_END(LudeoDataReaderInfo);

/**
 * @}
 *
 *\addtogroup ludeo_DataReader_GetObjectsInfo
 * @{
 */

/** Latest version of the #LudeoObjectInfo struct */
#define LUDEO_OBJECTINFO_API_LATEST 1

/** Information about a single object */
LUDEO_STRUCT_START(LudeoObjectInfo)
	/**
	 * Set by the SDK when allocating the struct
	 */
	int32_t apiVersion;

	/**
	 * Object Id.
	 */
	LudeoObjectId objectId;

	/**
	 * String that identifies the object type.
	 * This is what the client used when creating this object during the gameplay that created this ludeo data.
	 */
	const char* objectType;
LUDEO_STRUCT_END(LudeoObjectInfo);

/** Latest version of the LudeoObjectsInfo struct */
#define LUDEO_OBJECTSINFO_API_LATEST 1

/** Information about a group of objects */
LUDEO_STRUCT_START(LudeoObjectsInfo)
	/**
	 * Set by the SDK when allocating the struct
	 */
	int32_t apiVersion;

	/**
	 * Array with the information about all the objects.
	 * @note The objects's order is unspecified, and thus the client should not rely on any specific ordering.
	 */
	LudeoObjectInfo* objects;

	/** How many objects in the array */
	uint32_t objectsCount;
LUDEO_STRUCT_END(LudeoObjectsInfo);

/** The most recent version of the #ludeo_DataReader_GetObjectsInfo API */
#define LUDEO_DATAREADER_GETOBJECTSINFO_API_LATEST 1

/** Parameters to the function */
LUDEO_STRUCT_START(LudeoDataReaderGetObjectsInfoParams)
	int32_t apiVersion;
LUDEO_STRUCT_END(LudeoDataReaderGetObjectsInfoParams);

/**
 * @}
 *
 *\addtogroup ludeo_DataReader_VisitAttributes
 * @{
 */

/** The most recent version of the #ludeo_DataReader_VisitAttributes API */
#define LUDEO_DATAREADER_VISITATTRIBUTES_API_LATEST 1

/** Parameters to the function */
LUDEO_STRUCT_START(LudeoDataReaderVisitAttributesParams)
	/** Set this to LUDEO_DATAREADER_VISITATTRIBUTES_API_LATEST */
	int32_t apiVersion;
LUDEO_STRUCT_END(LudeoDataReaderVisitAttributesParams);

/** The parameters for the #ludeo_DataReader_VisitAttributes callback. */
LUDEO_STRUCT_START(LudeoDataReaderVisitAttributesCallbackParams)
	/** Context that was passed to the originating SDK call */
	void* clientData;
	
	/** Attribute name */
	const char* name;

	/**
	 * Type of attribute.
	 * If type is #LudeoDataType::Component, the client can do a call to #ludeo_DataReader_EnterComponent, visit the attributes
	 * inside that component, and afterwards call #ludeo_DataReader_LeaveComponent. This allows the client to recursively visit
	 * all attributes.
	 */
	LudeoDataType type;

	/**
	 * Attribute full name.
	 * The client has no use for this. It's only for debugging issues when working together with Ludeo's support.
	 *
	 */
	const char* fullname;
LUDEO_STRUCT_END(LudeoDataReaderVisitAttributesCallbackParams);

/** Signature for the #ludeo_DataReader_VisitAttributes callback */
LUDEO_DECLARE_CALLBACK_RETVALUE(LudeoBool, LudeoDataReaderVisitAttributesCallback, const LudeoDataReaderVisitAttributesCallbackParams* data);

/**
 * @}
 *
 *\addtogroup ludeo_DataReader_LoadStorage
 * @{
 */

/** Parameters to #ludeo_DataReader_LoadStorage */
LUDEO_STRUCT_START(LudeoDataReaderLoadStorageParams)
	/** Set this to LUDEO_DATAREADER_LOADSTORAGE_API_LATEST */
	int32_t apiVersion;
	/**
	 * The key which the binary data got saved with
	 */
	const char* key;
	/**
	 * Binary container which is allocated by the client.
	 */
	uint8_t* data;
	/**
	 * Capacity of the give binary container.
	 */
	uint32_t size;

LUDEO_STRUCT_END(LudeoDataReaderLoadStorageParams);

/** The parameters for the #ludeo_DataReader_LoadStorage callback. */
LUDEO_STRUCT_START(LudeoDataReaderLoadStorageCallbackParams)

	/** Tells if the operation succeeded or failed. */
	LudeoResult resultCode;
	
	/** Context that was passed to the originating SDK call */
	void* clientData;
	
	/**
	 * The key which the binary data got saved with
	 */
	const char* key;
	
	/**
	 * Size of the given binary container.
	 */
	size_t size;
	
	/**
	 * Binary container which is allocated by the client and contains the queried content.
	 */
	uint8_t* data;

LUDEO_STRUCT_END(LudeoDataReaderLoadStorageCallbackParams);

/** Signature for the #ludeo_DataReader_LoadStorage callback */
LUDEO_DECLARE_CALLBACK(LudeoDataReaderLoadStorageCallback, const LudeoDataReaderLoadStorageCallbackParams* data);

/** The most recent version of the #ludeo_DataReader_LoadStorage API */
#define LUDEO_DATAREADER_LOADSTORAGE_API_LATEST 1

/** Parameters to #ludeo_DataReader_GetStorageSize */
LUDEO_STRUCT_START(LudeoDataReaderGetStorageSizeParams)
	/** Set this to LUDEO_DATAREADER_GETSTORAGESIZE_API_LATEST */
	int32_t apiVersion;
	/**
	 * The key which the binary data got saved with
	 */
	const char* key;

LUDEO_STRUCT_END(LudeoDataReaderGetStorageSizeParams);

/** The most recent version of the #ludeo_DataReader_GetStorageSize API */
#define LUDEO_DATAREADER_GETSTORAGESIZE_API_LATEST 1

/**
 * @}
 */

/**
 * @}
 */
#pragma pack(pop)

