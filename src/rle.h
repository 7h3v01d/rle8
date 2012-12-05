typedef int (RLEproc)( const void *data, unsigned int size, void *arg );


typedef struct
{
	char data[128];
	unsigned int size;
	RLEproc *proc;
	void *arg;
	int ret;
} RLEenc;


void rleEncInit( RLEenc *ctx, const RLEproc *proc, const void *arg );

int rleEncFinal( RLEenc *ctx );

int rleEncUpdate( RLEenc *ctx, const void *data, unsigned int size );
