#pragma once

#ifdef __cplusplus
#define TF_EXTERN extern "C"
#else
#define TF_EXTERN
#endif

#ifdef _WIN32
#define TF_API TF_EXTERN __declspec(dllimport)
#else
#define TF_API TF_EXTERN
#endif

#ifdef __cplusplus
struct TF_Cmd {
	int idx;
	float val;
};
#else
typedef struct {
	int idx;
	float val;
} TF_Cmd;
#endif

/*
Command IDs (TF_Cmd.idx):
0 - stop
1 - play
2 - toggle pause
3 - next
*/

TF_API int tf_threaded_main(int blocking);
TF_API int tf_thread_cmd(TF_Cmd cmd);
TF_API void tf_time_delay(int ms);
#ifdef __cplusplus
TF_API int tf_get_init_state();
#else
TF_API int tf_get_init_state(void);
#endif
