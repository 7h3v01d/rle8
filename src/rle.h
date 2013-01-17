typedef int (rle_proc_t)( const void *data, unsigned int size, void *arg );


typedef struct
{
	char data[128];
	unsigned char size;
	char *next;
	char *ptr;
	unsigned int len;
	rle_proc_t *proc;
	void *arg;
	int ret;
} rle_enc_t;


void rle_init_enc( rle_enc_t *ctx, const rle_proc_t *proc, const void *arg );

int rle_final_enc( rle_enc_t *ctx );

int rle_update_enc( rle_enc_t *ctx, const void *data, unsigned int size );
