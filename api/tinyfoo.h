#pragma once

#ifdef __cplusplus
#define TF_EXTERN extern "C"
#define TF_NO_FUNC_ARGS

struct TF_Cmd {
	int idx;
	float val;
};
#else
#define TF_EXTERN
#define TF_NO_FUNC_ARGS void

typedef struct {
	int idx;
	float val;
} TF_Cmd;
#endif

#ifdef _WIN32
#define TF_API TF_EXTERN __declspec(dllimport)
#else
#define TF_API TF_EXTERN
#endif

/*
Command IDs (TF_Cmd.idx):
0 - stop
1 - play
2 - toggle pause
3 - next
4 - set volume (normally in [0.f; 1.f] range)
5 - change volume
6 - set position (seconds)
7 - change position
8 - app stop (val >= 1.f for rage quit)
*/

// Returns 1 on success, 0 on failure
TF_API int tf_threaded_main(int blocking);
// Returns 1 on success, 0 on failure
TF_API int tf_thread_cmd(TF_Cmd cmd);
// Helper function
TF_API void tf_time_delay(int ms);
// Returns 1 when inited, 0 when not inited, -1 on error
TF_API int tf_get_init_state(TF_NO_FUNC_ARGS);
// Set status callback
TF_API void tf_set_status_callback(void (*status_cb)(const char*));
