#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ex.h"

#define STREAM_DATA_SIZE  9

const char *JSON_STRING =
"{geonames: [{lng: -99.12766456604,geonameId: 3530597,countrycode: \"MX\",name: \"Mexico City\",fclName: \"city, village,...\",toponymName:\"MexicoCity\",fcodeName: \"capital of a political entity\",wikipedia: \"en.wikipedia.org/wiki/Mexico_City\",lat: 19.428472427036,fcl: \"P\",population: 12294193,fcode: \"PPLC\"},{lng: 116.397228240967,geonameId: 1816670,countrycode: \"CN\",name: \"Beijing\",fclName: \"city,village,...\",toponymName: \"Beijing\",fcodeName: \"capital of a political entity\",wikipedia: \"en.wikipedia.org/wiki/Beijing\",lat:39.9074977414405,fcl: \"P\",population: 11716620,fcode: \"PPLC\"},{lng: 90.40743827819824,geonameId: 1185241,countrycode: \"BD\",name:\"Dhaka\",fclName: \"city, village,...\",toponymName: \"Dhaka\",fcodeName: \"capital of a political entity\",wikipedia: \"en.wikipedia.org/wikiDhaka\",lat: 23.710395616597037,fcl: \"P\",population: 10356500,fcode: \"PPLC\"},{lng: 126.9784,geonameId: 1835848,countrycode: \"KR\",name:\"Seoul\",fclName: \"city, village,...\",toponymName: \"Seoul\",fcodeName: \"capital of a political entity\",wikipedia: \"en.wikipedia.org/wikiSeoul\",lat: 37.566,fcl: \"P\",population: 10349312,fcode: \"PPLC\"},{lng: 106.84513092041016,geonameId: 1642911,countrycode: \"ID\",name:\"Jakarta\",fclName: \"city, village,...\",toponymName: \"Jakarta\",fcodeName: \"capital of a political entity\",wikipedia: \"en.wikipedia.orgwikiJakarta\",lat: -6.214623197035775,fcl: \"P\",population: 8540121,fcode: \"PPLC\"},{lng: 139.69171,geonameId: 1850147,countrycode: \"JP\",name: \"Tokyo\",fclName: \"city, village,...\",toponymName: \"Tokyo\",fcodeName: \"capital of a political entity\",wikipedia: \"de.wikipedia.org/wiki/Tokyo\",lat: 35.6895,fcl: \"P\",population: 8336599,fcode: \"PPLC\"},{lng: 121.531846,geonameId: 1668341,countrycode: \"TW\",name: \"Taipei\",fclName: \"city, village,...\",toponymName: \"Taipei\",fcodeName: \"capital of a political entity\",wikipedia: \"de.wikipedia.org/wiki/Taipei\",lat: 25.047763,fcl: \"P\",population: 7871900,fcode: \"PPLC\"},{lng: -74.08175468444824,geonameId:3688689,countrycode: \"CO\",name: \"Bogotá\",fclName: \"city, village,...\",toponymName: \"Bogotá\",fcodeName:\"capital of a political entity\",wikipedia: \"en.wikipedia.org/wiki/Bogot%C3%A1\",lat: 4.609705849789108,fcl: \"P\",population: 7674366,fcode:\"PPLC\"},{lng: 114.157691001892,geonameId: 1819729,countrycode: \"HK\",name: \"Hong Kong\",fclName: \"city, village,...\",toponymName: \"Hong Kon\",fcodeName: \"capital of a political entity\",wikipedia: \"en.wikipedia.org/wiki/Hong_Kong\",lat: 22.2855225817732,fcl: \"P\",population:7012738,fcode: \"PPLC\"},{lng: 100.501444,geonameId: 1609350,countrycode: \"TH\",name: \"Bangkok\",fclName: \"city, village,...\",toponymName: \"Bangkok\",fcodeName: \"capital of a political entity\",wikipedia: \"en.wikipedia.org/wiki/Bangkok\",lat: 13.753979,fcl: \"P\",population: 5104476,fcode: \"PPLC\"}]}";

const char *testing = "012345678";

void geoInit(geotype_t *geo){
  geo->lng = GEO_EMPTY_LNG;
  geo->lat = GEO_EMPTY_LAT;
  memset(geo->countrycode, GEO_EMPTY_COUNTRYCODE, sizeof(GEO_EMPTY_COUNTRYCODE));
  memset(geo->name, GEO_EMPTY_NAME, sizeof(GEO_EMPTY_NAME));
  memset(geo->fclName, GEO_EMPTY_FCLNAME, sizeof(GEO_EMPTY_FCLNAME));
  memset(geo->toponyName, GEO_EMPTY_TOPONYNAME, sizeof(GEO_EMPTY_TOPONYNAME));
  memset(geo->fcodeName, GEO_EMPTY_FCODENAME, sizeof(GEO_EMPTY_FCODENAME));
  memset(geo->wiki, GEO_EMPTY_WIKI, sizeof(GEO_EMPTY_WIKI));
  memset(geo->fcl, GEO_EMPTY_FCL, sizeof(GEO_EMPTY_FCL));
  geo->population = GEO_EMPTY_POPULATION;
  memset(geo->fcode, GEO_EMPTY_FCODE, sizeof(GEO_EMPTY_FCODE));
}

void  geoBufferInit(geobuffertype_t *buf){
  memset(buf->buffer, '\0', sizeof('\0'));
  buf->index = 0;
}

void geoPutInInternalBuffer(const char *stream, geobuffertype_t *buf, unsigned char stream_start_index){
 /*
  * Copies unparsed data from internal buffer to the beginning, freeing the 
  * space for new characters from the stream.
  */
  memcpy(buf->buffer, (buf->buffer)+(buf->index), GEO_INTERNAL_BUFFER_SIZE-(buf->index));
  /*
   * Internal buffer has less free space than neccessery for storing whole unbuffered stream.  
   */
  if(GEO_INTERNAL_BUFFER_SIZE-(buf->index) <= STREAM_DATA_SIZE-stream_start_index){
    memcpy((buf->buffer)+(buf->index), stream+stream_start_index, (GEO_INTERNAL_BUFFER_SIZE-(buf->index)));
    stream_start_index += (GEO_INTERNAL_BUFFER_SIZE-(buf->index));
    buf->index += ((GEO_INTERNAL_BUFFER_SIZE-(buf->index)));
  }
  /*
   * Internal buffer has more free space than neccessery for storing whole unbuffered stream.  
   */
  else{
    memcpy((buf->buffer)+(buf->index), stream+stream_start_index, STREAM_DATA_SIZE-stream_start_index);
    // Karolina odkomentiraj ovo (stream_start_index = 0;), jer kada pocne stream podataka, mora biti drugacije
    //stream_start_index = 0; 
    stream_start_index += (STREAM_DATA_SIZE-stream_start_index); // Karolina, ovo pobrisi u Eclipseu
    // Karolina: procitaj novi stream... i index postavi u 0, takoder spremi s memcpy opet...
    buf->index += (STREAM_DATA_SIZE-stream_start_index);
  }
}


int main(){
  int i;
  int success;
  geobuffertype_t buf;
  geotype_t geo;

  geoBufferInit(&buf);
  geoPutInInternalBuffer(&testing[0], &buf, 0);
  /*
  printf("|%.5s|, %d\n", buf.buffer, buf.index);
  buf.index = 2;
  geoPutInInternalBuffer(&testing[0], &buf, 5);
  printf("|%.5s|, %d\n", buf.buffer, buf.index);
  buf.index = 3;
  geoPutInInternalBuffer(testing, &buf, 6);
  printf("|%.5s|, %d\n", buf.buffer, buf.index);
  */
  geoInit(&geo);

  return 0;
}
