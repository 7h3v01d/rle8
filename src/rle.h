typedef char (rle_proc_t)( const void *data, unsigned char size, void *arg );


typedef struct
{
	char data[128];
	unsigned char size;
	char *ptr;
	rle_proc_t *proc;
	void *arg;
	char ret;
} rle_enc_t;


void rle_init_enc( rle_enc_t *ctx, const rle_proc_t *proc, const void *arg );

char rle_final_enc( rle_enc_t *ctx );

char rle_update_enc( rle_enc_t *ctx, const void *data, unsigned char size );
