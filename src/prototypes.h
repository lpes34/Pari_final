/*File generated automatically in luis by luis on Seg Jan  8 10:47:55 WET 2018*/
#ifdef __cplusplus
extern "C" {
#endif
/* client.c */
int main (int argc, char *argv[]);
/* myf.c */
int CreateNewMessage (int a, char *Dir1, char *Dir2, int *left, int *right, char *message);
int pari_AskDataFromServer (char *ip, int port, char *message, char *server_reply, int maxChars);
srv_status pari_TestServer (char *addr, int port, struct timeval * tv);
#ifdef __cplusplus
}
#endif
