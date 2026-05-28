// Copyright 2023 Ludeo. All rights reserved.

/**
 * @file
 * Types used by the Room interface
 */

#pragma once

#include "Ludeo/Common.h"
#include "Ludeo/GameplaySession.h"

#pragma pack(push, 8)

/** \addtogroup Room
 * @{
 */

/** Opaque Room handle */
EXTERN_C typedef struct LudeoRoomHandle* LudeoHRoom;

/**
 *\addtogroup ludeo_Room_GetInfo
 * @{
 */

/** The most recent version of #ludeo_Room_GetInfo */
#define LUDEO_ROOM_GETINFO_API_LATEST 1

/** Parameters to #ludeo_Room_GetInfo */
LUDEO_STRUCT_START(LudeoRoomGetInfoParams)
	/** Set this to #LUDEO_ROOM_GETINFO_API_LATEST */
	int32_t apiVersion;
LUDEO_STRUCT_END(LudeoRoomGetInfoParams);

/** Latest version of the #LudeoRoomInfo struct */
#define LUDEO_ROOMINFO_API_LATEST 1

/** 
 * Information that can be retrieved with the #ludeo_Room_GetInfo function.
 * Any pointers in this struct points to the SDK's internal memory, and therefore the client should not keep hold of them.
 * As soon as the client gets the information, it should copy what it needs before making any other calls to the SDK.
 */
LUDEO_STRUCT_START(LudeoRoomInfo)

	/**
	 * Set by the SDK when allocating the struct
	 */
	int32_t apiVersion;

	/**
	 * Room id.
	 * This is the id supplied by the client when opening the room with ludeo_Session_OpenRoom
	 */
	const char* roomId;

	/**
	 * If playing a ludeo, then this is set. Nulled otherwise
	 */
	const char* ludeoId;
LUDEO_STRUCT_END(LudeoRoomInfo);

/**
 * @}
 *
 *\addtogroup ludeo_Room_Close
 * @{
 */

/** The most recent version of #ludeo_Room_Close */
#define LUDEO_ROOM_CLOSE_API_LATEST 1

/**
 * Parameters for #ludeo_Room_Close
 */
LUDEO_STRUCT_START(LudeoRoomCloseParams)
	/** Set this to LUDEO_ROOM_CLOSE_API_LATEST */
	int32_t apiVersion;
LUDEO_STRUCT_END(LudeoRoomCloseParams);

/** Parameters to the #ludeo_Room_Close callback */
LUDEO_STRUCT_START(LudeoRoomCloseCallbackParams)
	/** Tells if the operation succeeded or failed. */
	LudeoResult resultCode;

	/** Context that was passed to the originating SDK call */
	void *clientData;
LUDEO_STRUCT_END(LudeoRoomCloseCallbackParams);

/** Signature for the #ludeo_Room_Close callback */
LUDEO_DECLARE_CALLBACK(LudeoRoomCloseCallback, const LudeoRoomCloseCallbackParams* data);

/**
 * @}
 *
 *\addtogroup ludeo_Room_AddPlayer
 * @{
 */

/** The most recent version of #ludeo_Room_AddPlayer */
#define LUDEO_ROOM_ADDPLAYER_API_LATEST 1

/**
 * Parameters for #ludeo_Room_AddPlayer
 */
LUDEO_STRUCT_START(LudeoRoomAddPlayerParams)
	/** Set this to #LUDEO_ROOM_ADDPLAYER_API_LATEST */
	int32_t apiVersion;

	/**
	 * Player Id to add.
	 * The SDK doesn't care what the actual string is. It is up to the client.
	 */
	const char* playerId;
LUDEO_STRUCT_END(LudeoRoomAddPlayerParams)

/** Parameters to the #ludeo_Room_AddPlayer callback */
LUDEO_STRUCT_START(LudeoRoomAddPlayerCallbackParams)

	/** Tells if the operation succeeded or failed. */
	LudeoResult resultCode;

	/** Context that was passed to the originating SDK call */
	void *clientData;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	// #LC3636 : I'm unsure if these should be here or somewhere else (e.g: when the room is ready). To be decided. Decision will
	// based on the expected lifetime of these. Maybe these should be given to the client as part of the "RoomReady" notification
#endif

	/**
	 * GameplaySession handle to use for the player.
	 * If any error occurred, then this will be null.
	 *
	 * The lifetime of this handle is tied to the lifetime of the owning Room.
	 * Any handles not destroyed when the room is destroyed will be destroyed and invalidated automatically.
	 */
	LudeoHGameplaySession gameplaySession;
LUDEO_STRUCT_END(LudeoRoomAddPlayerCallbackParams);

/** Signature for the #ludeo_Room_AddPlayer callback */
LUDEO_DECLARE_CALLBACK(LudeoRoomAddPlayerCallback, const LudeoRoomAddPlayerCallbackParams* data);

/**
 * @}
 *
 *\addtogroup ludeo_Room_RemovePlayer
 * @{
 */

/** The most recent version of #ludeo_Room_RemovePlayer */
#define LUDEO_ROOM_REMOVEPLAYER_API_LATEST 1

/**
 * Parameters for #ludeo_Room_RemovePlayer
 */
LUDEO_STRUCT_START(LudeoRoomRemovePlayerParams)
	/** Set this to #LUDEO_ROOM_REMOVEPLAYER_API_LATEST */
	int32_t apiVersion;

	/**
	 * Player to remove.
	 * This is what was passed to #ludeo_Room_AddPlayer
	 */
	const char* playerId;
LUDEO_STRUCT_END(LudeoRoomRemovePlayerParams);

/** Parameters to the #ludeo_Room_RemovePlayer callback */
LUDEO_STRUCT_START(LudeoRoomRemovePlayerCallbackParams)
	/** Tells if the operation succeeded or failed. */
	LudeoResult resultCode;

	/** Context that was passed to the originating SDK call */
	void *clientData;
LUDEO_STRUCT_END(LudeoRoomRemovePlayerCallbackParams);

/** Signature for the #ludeo_Room_RemovePlayer callback */
LUDEO_DECLARE_CALLBACK(LudeoRoomRemovePlayerCallback, const LudeoRoomRemovePlayerCallbackParams* data);

/**
 * @}
 */

/**
 * @}
 */

#pragma pack(pop)

