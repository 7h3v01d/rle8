#include "rle.h"


#ifndef NULL
#define NULL ( (void *)0 )
#endif /* NULL */
#define SIZE ( sizeof(char) )
#define BITS ( SIZE*8 )
#define SIGN ( 1<<(BITS-1) )
#define MAX  ( SIGN-1 )
#define MIN  ( SIZE+2 )


void rle_init_enc( rle_enc_t *ctx, const rle_proc_t *proc, const void *arg )
{
	if( NULL == ctx ||
	    NULL == ctx->proc ) {
		return;
	}

	*ctx->data = SIGN;
	ctx->next = ctx->data+1;
	ctx->ptr = 0;
	ctx->len = 0;
	ctx->proc = proc;
	ctx->arg = (void *)arg;
	ctx->ret = 0;
}

int rle_final_enc( rle_enc_t *ctx )
{
	if( NULL == ctx ||
	    NULL == ctx->proc ) {
		return 0;
	}

	if( SIGN == (*ctx->data & SIGN) ) {
		ctx->ret = ctx->proc( ctx->data, (*ctx->data & MAX)+1, ctx->arg );
	}
	else {
		ctx->ret = ctx->proc( ctx->data, 2, ctx->arg );
	}

	return ctx->ret;
}

int rle_update_enc( rle_enc_t *ctx, const void *data, unsigned int size )
{
	if( NULL == ctx ||
	    NULL == ctx->proc ||
	    NULL == data ||
	    0 == size ) {
		return 0;
	}

	ctx->ptr = (char *)data;
	ctx->len = size;

	do {
		if( MAX == (*ctx->data & MAX) ) {
			if( SIGN == (*ctx->data & SIGN) ) {
				ctx->ret = ctx->proc( ctx->data, (*ctx->data & MAX)+1, ctx->arg );
			}
			else {
				ctx->ret = ctx->proc( ctx->data, 2, ctx->arg );
			}
			if( 0 != ctx->ret ) {
				return ctx->ret;
			}

			*ctx->data = SIGN;
			ctx->next = ctx->data+1;

			ctx->len++;
		}
		else
		if( MIN > (*ctx->data & MAX) ) {
			*(ctx->next++) = *(ctx->ptr++);
			(*ctx->data)++;
		}
		else
		if( MIN == (*ctx->data & MAX) ) {
			if( ctx->data[1] == ctx->data[2] &&
			    ctx->data[2] == ctx->data[3] ) {
			    *ctx->data &= MAX;
			}

			*(ctx->next++) = *(ctx->ptr++);
			(*ctx->data)++;

			if( SIGN != (*ctx->data & SIGN) ) {
				if( ctx->next[-1] != ctx->next[-2] ) {
					(*ctx->data)--;

					ctx->ret = ctx->proc( ctx->data, 2, ctx->arg );
					if( 0 != ctx->ret ) {
						return ctx->ret;
					}

					ctx->data[1] = ctx->next[-1];
					*ctx->data = 1 | SIGN;
					ctx->next = ctx->data+1+1;
				}
			}
		}
		else {
			*(ctx->next++) = *(ctx->ptr++);
			(*ctx->data)++;

			if( SIGN == (*ctx->data & SIGN) ) {
				if( ctx->next[-1] == ctx->next[-2] &&
				    ctx->next[-2] == ctx->next[-3] ) {
					*ctx->data -= 3;

					ctx->ret = ctx->proc( ctx->data, (*ctx->data & MAX)+1, ctx->arg );
					if( 0 != ctx->ret ) {
						return ctx->ret;
					}

					ctx->data[1] = ctx->next[-3];
					ctx->data[2] = ctx->next[-2];
					ctx->data[3] = ctx->next[-1];
					*ctx->data = 3;
					ctx->next = ctx->data+1+3;
				}
			}
			else {
				if( ctx->next[-1] != ctx->next[-2] ) {
					(*ctx->data)--;

					ctx->ret = ctx->proc( ctx->data, 2, ctx->arg );
					if( 0 != ctx->ret ) {
						return ctx->ret;
					}

					ctx->data[1] = ctx->next[-1];
					*ctx->data = 1 | SIGN;
					ctx->next = ctx->data+1+1;
				}
			}
		}
	}
	while( 0 != --ctx->len );

	return 0;
}
