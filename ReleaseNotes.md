# Next Release

## Breaking changes

## New features

## Improvements

## Fixes

# v4.2.3

## Improvements
* Removed the Gameface dependency and its leftovers (including OpenAL) from the SDK and third-party Deps, completing the migration to the ImGui-based overlay.

## Fixes
* Fixed the local Ludeo flow — the launcher-provided user id (e.g. the Steam id) is now sent as the `launcher_id` query parameter on the Echo WebSocket connections, captured at session activation.
* Fixed a deadlock during DX12 context shutdown (Steam overlay with `d3d11on12`) caused by the function-hook dispatch holding an exclusive lock across re-entrant calls into foreign code. Dispatch now takes a shared lock, and unhook drains in-flight dispatches before freeing the trampoline.
* Fixed false-positive `Error` logs during session-activation config parsing — the renamed `MarkHighlight` action, the deprecated `actionDataBufferSize` field, and the optional `PerformanceMonitor` endpoint no longer report spurious errors.
* Fixed a spurious `Json key 'url' not found` error logged on every mark-highlight against the Echo backend; the removed `url` field is no longer read from the response.
* Stopped the SDK from re-issuing the performance-monitor request every interval after the monitor endpoint returns a 404; monitoring is now disabled for the rest of the session, while transient failures still retry as before.

# v4.2.2

## Fixes
* Fixed the released version not being stamped into the binaries; the version is now stamped via the `LUDEOSDK_VERSION` override.

# v4.2.1

## Improvements
* ludeo_Tick log has now been moved to logging level Verbose.

## Fixes
* Fixed breaking bug in Echo - Ludeos using 4.2.0 couldn't run on the cloud. WS principal id is now 1:1 connection id.
* Fixed a bug where ImGui will appear alongside the platform's overlay on cloud.
* Fix for hardcoded ImGui overlay values.

# v4.2.0

## Breaking changes

* Migrated the realtime channel to the Echo WebSocket protocol — the session WebSocket is now split into separate binary and text channels with an enveloped wire format. Token and `principal_id` are passed as URL-encoded query parameters on the Echo endpoint. Game-event parsers (`NewLudeoSelected`, `CaptureVideoRequest`, etc.) were updated to the new Echo message shape.
* Replaced the `/highlights` HTTP endpoint with `/v1/moments`. `ludeo_DataWriter_MarkHighlight` now POSTs to `/v1/moments`, and backend-driven capture events use the matching `Capture_video_request` Echo event name.

## Improvements

* Replaced the in-process Game Face overlay with an ImGui-based implementation, removing the Game Face runtime dependency and reducing overlay startup cost and memory footprint.
* Hardened the new WebSocket client with reconnect-path coverage, frame-type enforcement, and a pong reply path in the Echo mock server used by tests.
* Added `MarkHighlightReason` plumbing so manual and backend-initiated highlights can be distinguished by call site rather than by a shared flag.

## Fixes

* Fixed a use-after-free during WebSocket reconnect when a pending IOCP read completed after the socket was torn down.
* Fixed `/api/v3` suffix handling in `SessionMarkHighlightTask` and removed the related magic number.
* Removed the dead FlatBuffer reply path on the session binary WebSocket and dropped the unused session lock that tolerated unexpected `connection_info` fields in `data.context`.

# v4.1.0

## New features

* **Thread-safe DataWriter** — added `threadSafe` and `maxConcurrentWrites` fields to `LudeoSessionOpenRoomParams` (API version 2). When `threadSafe` is `LUDEO_TRUE`, the following calls become fully concurrent-safe and may be called simultaneously from multiple game threads:
  - `ludeo_DataWriter_EnterObject` / `ludeo_DataWriter_LeaveObject`
  - `ludeo_DataWriter_EnterComponent` / `ludeo_DataWriter_LeaveComponent`
  - All `ludeo_DataWriter_SetXxx` functions
  - `ludeo_DataWriter_DestroyObject`
  - `ludeo_DataWriter_CreateObject`
  - `ludeo_DataWriter_SendAction`

  `maxConcurrentWrites` controls the maximum number of in-flight object writes before a game thread blocks (default: 1024). `ludeo_DataWriter_EnterObjectLate` is not supported in thread-safe mode.

## Fixes

* Fixed spurious SDK errors logged during session activation caused by a malformed log format string and stale endpoint configuration.

# v4.0.21

## New features

* Added `ludeo_Session_AddNotifySnapshotRequest` — a new notification API that triggers a callback when the SDK requests a snapshot of the game's initial state.
* Added `ludeo_DataWriter_SaveStorage` — saves an arbitrary binary payload to SDK-managed storage associated with the current run, allowing games to persist precomputed data alongside the recorded gameplay.
* Added `ludeo_DataReader_LoadStorage` and `ludeo_DataReader_GetStorageSize` — retrieve binary payloads previously saved via `SaveStorage` during playback, with `GetStorageSize` allowing callers to query the payload size upfront for pre-allocation.

## Improvements

* Added memory profiling with Tracy
* Added a dictionary for using the SDK with games that use OpenGL/AL
* Refactored CI/CD with Android builds and an on-demand test machine
* Enhanced WebSocket stability using reconnect strategy
* Updated Tracy to v0.12.2

## Fixes

* Removed redundant session activation related errors from SDK logs
* Fixed a bug when using Vulkan
* Fixed a bug when using with a game running OpenAL

# v4.0.20

## Improvements

* DirectX12 performance optimization

# v4.0.19

## Improvements

* Window handle is no longer required when calling the session activation API, to support headless game servers.
* For games that already depend on Gameface, the SDK can now use those Gameface DLLs.
* Video encoder now supports OpenGL
* Implementation-independent DirectX12 hooking (no need to rely on memory scanning)

## Fixes

* Fixed video encoder reset being triggered in a scenario where it should not have been
* Some improvements to shutdown code

# v4.0.18

## Improvements

* When the `video-localsave` ludeo command is set to `1`, it now saves a `LudeoVideoMessage-<TIMESTAMP>.txt` alongside the `h264` file, containing information to help debug video problems.
* Added an experimental internal data structure that considerably improves SDK performance for certain scenarios.
* Attribute registration is now split into multiple HTTP requests to reduce the load on the backend. The threshold for splitting can be controlled with the `data-attributes_per_request` command.
* `ludeo_DataWriter_EnterObject` can now be nested. This is allowed just for convenience — it does not make the objects related in any way.

## Fixes

* Fixed a heap corruption in DataWriter/DataReader
* Video encoder: the encoder no longer resets when a game is minimized unless the resolution or format changed.
* Video encoder: the video ring buffer now grows in capacity during an ongoing highlight, to help minimize cases where the video is slightly shorter than requested.
* The SDK now sends extra information to the backend (first frame information) to help solve video vs data sync issues.

# v4.0.17

## Fixes

* Failed `ludeo_Session_Activate` calls now automatically perform resource cleanup instead of leaving everything for the `ludeo_Session_Release` call. The client still needs to call `ludeo_Session_Release` for every session created, regardless of the `ludeo_Session_Activate` result.
* The callback for `ludeo_Session_Release` is now the last callback called for the session, as documented.
* Fixed some WebSocket related memory leaks.
* Fixed some asserts and WebSocket issues related to calling `ludeo_Session_Release` before `ludeo_Session_Activate` completes.

# v4.0.16

## Improvements

* Improved network timeout detection. Timeouts are now based on lack of traffic rather than total operation duration, preventing false timeouts on slow connections.
* `ludeo_DataWriter_EnterObject` and `ludeo_DataReader_EnterObject` can now be nested. See the headers for more information.
* If videos are being uploaded when `ludeo_Session_Release` is called, the call now completes only once all uploads finish.
* `ludeo_Tick` now also logs its own self-time, making it easier to identify if it is a bottleneck.
* Other logging improvements.

## Fixes

* Temporary workaround for a divide-by-zero exception caused by Coherent's Gameface.

# v4.0.15

## Fixes

* Fixed a crash in the Gallery.
* Fixed a bug in video recording where the SDK was not using the start and end times as requested by the backend.
* Fixed a bug in video recording where the SDK was hardcoding the video framerate to 30fps.

# v4.0.14

## Breaking changes

* `LudeoSessionActivateParams` now requires a `gameVersion` field (non-null, non-empty string) when `apiVersion` is 3 or higher. This field is used to identify which game versions are compatible with a given ludeo.

## New features

* Added `gameVersion` parameter to `LudeoSessionActivateParams`, used to identify compatible game versions for each recorded ludeo.

## Improvements

* Added the `activation-ludeoid` ludeo command, which allows specifying a ludeo ID to use on the next `ludeo_Session_Activate` call. Useful for testing a specific ludeo during development.

## Fixes

* Fixed a crash when using `ludeo_Shutdown`.

# v4.0.13.1

## Fixes

* Fixed PS5 controller input handling.

# v4.0.13

## New features

* Tracy profiler is now integrated and can be used to analyze SDK performance.

# v4.0.12

## New features

* The SDK can now upload logs to the backend. If this feature is enabled in the platform, calls to `ludeo_SetLoggingLevel` are ignored since the backend controls verbosity.
* WebSocket connections are now retried a few times before considering a failure.
* Added support for DirectInput, allowing the overlay to be navigated with a gamepad.

# v4.0.11

## Fixes

* Fixed blob attribute size being wrongly calculated.

# v4.0.10

## New features

* Added a `fullName` field to `LudeoDataReaderVisitAttributesCallbackParams`, representing the attribute's full name.
* Added `ludeo_DataWriter_EnterObjectLate` — allows sending data with an older timestamp.
* Added `ludeo_GetCurrentTimestamp` — returns the current SDK tick time.

## Improvements

* Blob attributes larger than 100KB are now automatically split into smaller chunks, transparently to the client.
* `ludeo_DataWriter_SetBlob` now accepts a null pointer when size is 0.

# v4.0.9

## New features

* The authentication type (cloud, Steam, etc.) used by the SDK for session activation is now included in `LudeoSessionActivateCallbackParams`.

## Improvements

* Improved video capture FPS stability for low-FPS gameplay.

## Fixes

* Fixed inputs polled from XInput not being properly forwarded to the foreground window.

# v4.0.8

## New features

* Added a notification API for requests to mute/unmute the game.

## Fixes

* Fixed overlay not being reset when session activation fails.
* Fixed a potential crash when the process exits immediately after the SDK library is loaded.

# v4.0.7

## Fixes

* Fixed a race condition which can happen during overlay initialization.
* Fixed hwinfo dependencies not properly restoring locale state after changing it.

# v4.0.6

## Fixes

* Replaced a DirectX12 call that is not supported in Proton.
* Fixed a memory leak in the blob API.

# v4.0.5

## New features

* Introduced a new Blob data type for storing arbitrary binary data within recorded gameplay.
  * `ludeo_DataWriter_SetBlob` — records a binary blob attribute. Accepts any arbitrary data; note that if even a single byte changes compared to the previous value, the entire blob is re-sent, so blobs should be used judiciously.
  * `ludeo_DataReader_GetBlob` — reads back a blob attribute. Call `ludeo_DataReader_GetSize` first to get the blob size and allocate a sufficiently large buffer.

## Fixes

* Fixed a multi-threaded crash in overlay input handling.

# 4.0.4

## Improvements

* Update Coherent library version to 1.59.0.1 for the overlay
* Stability improvements for gamepad navigation in overlay.
* Error and Fatal log messages are now logged to stderr if logging was not set up.

## Fixes

* Fixed DirectX12 hooking crash.

# 4.0.3

## Improvements

* Overlay localization support API.
* Gamepad navigation in overlay for XInput supported devices.

# 4.0.2

## Improvements

* Tutorial videos (accessible through the Gallery) now have audio.
* Further improvements to the C headers documentation.
* Some minor improvements to what gets logged.

## Fixes

* Temporarily disabling NvEnc (Nvidia) based video encoder while we chase down some bugs. Machines with an NVidia card will use the software encoder for now.

# 4.0.1

## Improvements

* New video encoding solution

# 4.0.0

First Core SDK release
