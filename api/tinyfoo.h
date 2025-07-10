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
	int cmd;
	float val;
};
#else
typedef struct {
	int cmd;
	float val;
} TF_Cmd;
#endif

TF_API int tf_threaded_main(int blocking);
TF_API int tf_thread_cmd(TF_Cmd cmd);
