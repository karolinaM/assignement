
#include <stddef.h>
#include <stdint.h>

#define GEO_INTERNAL_BUFFER_SIZE (int)664
#define GEO_MAX_KEY_LENGTH            11
#define GEO_MAX_VALUE_LENGTH          256

#define GEO_STRUCT_NOT_FULL  0
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

enum geoKeys{
  LNG,
  LAT,
  GEONAMEID,
  COUNTRYCODE,
  NAME,
  FCLNAME,
  TOPONYNAME,
  FCODENAME,
  WIKI,
  FCL,
  POPULATION,
  FCODE
};

/*
 * JSON file with only one object filled with max values has: identifiers 151 bytes + sizeof(geobuffertype_t) = 151+664 = 815 bytes
 * 151 char: {geonames: [{lng: ,geonameId: ,countrycode: "",name: "",fclName: "",toponymName: "",fcodeName: "",wikipedia: "",lat: ,fcl: "",population: ,fcode: ""}]}
 */

typedef struct {
	float lng;
	float lat;
	unsigned int geonameId;
	char countrycode[4];
	char name[64];
	char fclName[128];
	char toponyName[64];
	char fcodeName[256];
	char wiki[128];
	char fcl[4];
	unsigned int population;
	char fcode[4];
	char finished; // tells if struct is filled with parsed data
} geotype_t;

/*
 * Buffer is large enough so that at least one maximum large whole structure can
 * fit into it.
 */
typedef struct{
	char buffer[GEO_INTERNAL_BUFFER_SIZE];
	unsigned int index; // empty element index, not processed element
}geointernalbuffertype_t;

void geoInit(geotype_t *geo);
void geoInternalBufferInit(geointernalbuffertype_t *buf);
void geoPutInInternalBuffer(const char *stream, geointernalbuffertype_t *buf, unsigned int stream_start_index);

