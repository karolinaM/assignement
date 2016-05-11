
#include <stddef.h>
#include <stdint.h>

#define GEO_INTERNAL_BUFFER_SIZE (int)5

#define GEO_EMPTY_LNG          0
#define GEO_EMPTY_LAT 		   0
#define GEO_EMPTY_POPULATION  -1
#define GEO_EMPY_FCODE 		  '\0'
#define GEO_EMPTY_COUNTRYCODE '\0'
#define GEO_EMPTY_NAME		  '\0'
#define GEO_EMPTY_FCLNAME	  '\0'
#define GEO_EMPTY_TOPONYNAME  '\0'
#define GEO_EMPTY_FCODENAME	  '\0'
#define GEO_EMPTY_WIKI        '\0'
#define GEO_EMPTY_FCL         '\0'
#define GEO_EMPTY_POPULATION  -1
#define GEO_EMPTY_FCODE       '\0'   

typedef struct {
	float lng;
	float lat;
	char countrycode[4];
	char name[64];
	char fclName[128];
	char toponyName[64];
	char fcodeName[256];
	char wiki[128];
	char fcl[4];
	unsigned int population;
	char fcode[4];
} geotype_t;

/*
 * Buffer is large enough so that at least one maximum large whole structure can
 * fit into it.
 */
typedef struct{
	char buffer[GEO_INTERNAL_BUFFER_SIZE];
	unsigned int index; // empty element index, not processed element
}geobuffertype_t;


