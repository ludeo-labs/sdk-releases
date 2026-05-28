// Copyright 2023 Ludeo. All rights reserved.

/**
 * @file
 * GameplaySession interface functions
 */


#pragma once

#include "Ludeo/Common.h"
#include "Ludeo/GameplaySessionTypes.h"

/**
 * \addtogroup GameplaySession
 * @{
 *
 * The GameplaySession interface encapsulates the concept of a player in a Room.
 * These interfaces are handed out to the client as the result of calling #ludeo_Room_AddPlayer.
 * 
 * The interface has 3 lifetime stages:
 * - Initial state (as created by the Room)
 * - Active gameplay. This is triggered with #ludeo_GameplaySession_Begin
 * - Ended. This is triggered with #ludeo_GameplaySession_End
 *
 * This flow is one way only. As-in, switching between gameplay and ended multiple times per interface is not allowed.
 * 
 * The lifetime of these interfaces are tied to the owning Room, and no explicit "Release" call exists. The interface handle
 * is invalidated when calling ludeo_Room_RemovePlayer, or closing the room.
 * 
 */

/**
 * Returns true or false depending if the handle is a valid handle.
 * This is useful for code that needs to quickly validate handles in a away that doesn't produce side effects. E.g: Debug code.
 */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_GameplaySession_IsValid(LudeoHGameplaySession handle);

/**
 *\defgroup ludeo_GameplaySession_GetInfo ludeo_GameplaySession_GetInfo
 * @{
 */

/**
 * Returns information about the specified GameplaySession handle.
 *
 * @param handle
 *		The interface handle.
 *
 * @param params
 *		Parameters to the functions
 *
 * @param out
 *		If #LudeoResult::Success is returned, then on exit this will contain a pointer to the information.
 *
 * @return
 *		If LudeoResult::Success, then the "out" parameter contains a pointer to the information.
 *
 * @important
 *		Once done with the information, the client should call #ludeo_GameplaySessionInfo_Release to release the struct.
 */
LUDEO_DECLARE_FUNC(LudeoResult) ludeo_GameplaySession_GetInfo(LudeoHGameplaySession handle, const LudeoGameplaySessionGetInfoParams* params, LudeoGameplaySessionInfo** out);
LUDEO_DEFINE_FUNC_TRAITS_2(GameplaySession, GAMEPLAYSESSION, GetInfo, GETINFO);

/**
 * Release any resources hold by the specified structure
 *
 * @param info structure to release. If nulled, the call is a no-op.
 */
LUDEO_DECLARE_FUNC(void) ludeo_GameplaySessionInfo_Release(LudeoGameplaySessionInfo* info);

/**
 * @}
 *
 *\defgroup ludeo_GameplaySession_Begin ludeo_GameplaySession_Begin
 * @{
 */

/**
 * Starts the player's gameplay session.
 *
 * The client should call this once it wants to start actual gameplay.
 *
 * Calling this when a previous call already succeed OR it is still in flight will fail with LudeoResult::WrongState.
 *
 * @param handle
 *		The interface handle.
 *
 * @param params
 *		Parameters to the function
 *
 * @param clientData
 *		Context that will be passed to the callback
 *
 * @param callback
 *		Callback to call once the operation completes
 */
LUDEO_DECLARE_FUNC(void) ludeo_GameplaySession_Begin(LudeoHGameplaySession handle, LudeoGameplaySessionBeginParams* params, void* clientData, LudeoGameplaySessionBeginCallback callback);
LUDEO_DEFINE_FUNC_TRAITS_WITHCALLBACK_2(GameplaySession, GAMEPLAYSESSION, Begin, BEGIN);

/**
 * @}
 *
 *\defgroup ludeo_GameplaySession_End ludeo_GameplaySession_End
 * @{
 */

/**
 * Ends the player's gameplay session.
 *
 * This is a valid call only after a successful call to ludeo_GameplaySession_Begin. Doing otherwise will return return a
 * LudeoResult::WrongState error.
 *
 * No further calls to #ludeo_GameplaySession_Begin or #ludeo_GameplaySession_End are allowed after this call. See the params struct for more details.
 * 
 * @param handle
 *		The interface handle.
 *
 * @param params
 *		Parameters to the function.
 *
 * @param clientData
 *		Context that will be passed to callback.
 *
 * @param callback
 *		Callback to call once the operation completes.
 */
LUDEO_DECLARE_FUNC(void) ludeo_GameplaySession_End(LudeoHGameplaySession handle, LudeoGameplaySessionEndParams* params, void* clientData, LudeoGameplaySessionEndCallback callback);
LUDEO_DEFINE_FUNC_TRAITS_WITHCALLBACK_2(GameplaySession, GAMEPLAYSESSION, End, END);

/**
 * @}
 */

/**
 * @}
 */

