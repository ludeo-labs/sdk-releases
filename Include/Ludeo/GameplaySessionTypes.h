// Copyright 2023 Ludeo. All rights reserved.

/**
 * @file
 * Types used by the GameplaySession interface
 */

#pragma once

#include "Ludeo/Common.h"

#pragma pack(push, 8)

/** \addtogroup GameplaySession
 * @{
 */

/** Opaque GameplaySession handle */
EXTERN_C typedef struct LudeoGameplaySessionHandle* LudeoHGameplaySession;

/**
 *\addtogroup ludeo_GameplaySession_GetInfo
 * @{
 */

/** The most recent version of #ludeo_GameplaySession_GetInfo */
#define LUDEO_GAMEPLAYSESSION_GETINFO_API_LATEST 1

/** Parameters to #ludeo_GameplaySession_GetInfo */
LUDEO_STRUCT_START(LudeoGameplaySessionGetInfoParams)
	/** Set this to LUDEO_GAMEPLAYSESSION_GETINFO_API_LATEST */
	int32_t apiVersion;
LUDEO_STRUCT_END(LudeoGameplaySessionGetInfoParams);

/** Latest version of the #LudeoGameplaySessionInfo struct */
#define LUDEO_GAMEPLAYSESSIONINFO_API_LATEST 1

/** 
 * Information that can be retrieved with the #ludeo_GameplaySession_GetInfo function
 *
 * The client should call #ludeo_GameplaySessionInfo_Release to release the structure.
 */
LUDEO_STRUCT_START(LudeoGameplaySessionInfo)

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
	 * Player id supplied to the ludeo_Room_AddPlayer
	 */
	const char* playerId;

	// #ROOMS : Add a "state" or similar field to indicate what's the state of things (Ready, Gameplay), etc.
LUDEO_STRUCT_END(LudeoGameplaySessionInfo);

/**
 * @}
 *
 *\addtogroup ludeo_GameplaySession_Begin
 * @{
 */

/** The most recent version of  #ludeo_GameplaySession_Begin */
#define LUDEO_GAMEPLAYSESSION_BEGIN_API_LATEST 1

/** Parameters to #ludeo_GameplaySession_Begin */
LUDEO_STRUCT_START(LudeoGameplaySessionBeginParams)
	/** Set this to LUDEO_GAMEPLAYSESSION_BEGIN_API_LATEST */
	int32_t apiVersion;
LUDEO_STRUCT_END(LudeoGameplaySessionBeginParams)

/** The parameters to the #ludeo_GameplaySession_Begin callback */
LUDEO_STRUCT_START(LudeoGameplaySessionBeginCallbackParams)
	/** Tells if the operation succeeded or failed. */
	LudeoResult resultCode;

	/** Context that was passed to the originating SDK call */
	void *clientData;
LUDEO_STRUCT_END(LudeoGameplaySessionBeginCallbackParams);

/** Signature for the #ludeo_GameplaySession_Begin callback */
LUDEO_DECLARE_CALLBACK(LudeoGameplaySessionBeginCallback, const LudeoGameplaySessionBeginCallbackParams* data);

/**
 * @}
 *
 *\addtogroup ludeo_GameplaySession_End
 * @{
 */

/** The most recent version of #ludeo_GameplaySession_End */
#define LUDEO_GAMEPLAYSESSION_END_API_LATEST 1

/** Parameters to #ludeo_GameplaySession_End */
LUDEO_STRUCT_START(LudeoGameplaySessionEndParams)
	/** Set this to LUDEO_GAMEPLAYSESSION_END_API_LATEST */
	int32_t apiVersion;

	/**
	 * This is only relevant when in a ludeo run.
	 * If the client wishes to programmatically end a ludeo run, the SDK needs to know what's the intent:
	 *
	 * - false (Not an abort) : The Ludeo summary overlay will be shown, so the ludeo can be replayed. A scenario for this is in a
	 *   last man standing game. If the player kills all other players or enemies, the client can end the ludeo without waiting
	 *   for the timer to end.
	 *
	 * - true - (abort) : The client wishes to exit the ludeo and NOT display the Ludeo Summary overlay.
	 */
	LudeoBool isAbort;
LUDEO_STRUCT_END(LudeoGameplaySessionEndParams);

/** The parameters to the #ludeo_GameplaySession_End callback */
LUDEO_STRUCT_START(LudeoGameplaySessionEndCallbackParams)
	/** Tells if the operation succeeded or failed. */
	LudeoResult resultCode;

	/** Context that was passed to the originating SDK call */
	void *clientData;
LUDEO_STRUCT_END(LudeoGameplaySessionEndCallbackParams);

/** Signature for the #ludeo_GameplaySession_End callback */
LUDEO_DECLARE_CALLBACK(LudeoGameplaySessionEndCallback, const LudeoGameplaySessionEndCallbackParams* data);

/**
 * @}
 */

/**
 * @}
 */

#pragma pack(pop)

