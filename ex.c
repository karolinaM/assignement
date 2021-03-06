#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "ex.h"

#define STREAM_DATA_SIZE  (unsigned int)2748

const char *JSON_STRING =
"{\"geonames\":[{\"lng\":-99.12766456604,\"geonameId\":3530597,\"countrycode\":\"MX\",\"name\":\"Mexico City\",\"fclName\":\"city, village,...\",\"toponymName\":\"Mexico City\",\"fcodeName\":\"capital of a political entity\",\"wikipedia\":\"en.wikipedia.org/wiki/Mexico_City\",\"lat\":19.428472427036,\"fcl\":\"P\",\"population\":12294193,\"fcode\":\"PPLC\"},{\"lng\":116.397228240967,\"geonameId\":1816670,\"countrycode\":\"CN\",\"name\":\"Beijing\",\"fclName\":\"city, village,...\",\"toponymName\":\"Beijing\",\"fcodeName\":\"capital of a political entity\",\"wikipedia\":\"en.wikipedia.org/wiki/Beijing\",\"lat\":39.9074977414405,\"fcl\":\"P\",\"population\":11716620,\"fcode\":\"PPLC\"},{\"lng\":90.40743827819824,\"geonameId\":1185241,\"countrycode\":\"BD\",\"name\":\"Dhaka\",\"fclName\":\"city, village,...\",\"toponymName\":\"Dhaka\",\"fcodeName\":\"capital of a political entity\",\"wikipedia\":\"en.wikipedia.org/wiki/Dhaka\",\"lat\":23.710395616597037,\"fcl\":\"P\",\"population\":10356500,\"fcode\":\"PPLC\"},{\"lng\":126.9784,\"geonameId\":1835848,\"countrycode\":\"KR\",\"name\":\"Seoul\",\"fclName\":\"city, village,...\",\"toponymName\":\"Seoul\",\"fcodeName\":\"capital of a political entity\",\"wikipedia\":\"en.wikipedia.org/wiki/Seoul\",\"lat\":37.566,\"fcl\":\"P\",\"population\":10349312,\"fcode\":\"PPLC\"},{\"lng\":106.84513092041016,\"geonameId\":1642911,\"countrycode\":\"ID\",\"name\":\"Jakarta\",\"fclName\":\"city, village,...\",\"toponymName\":\"Jakarta\",\"fcodeName\":\"capital of a political entity\",\"wikipedia\":\"en.wikipedia.org/wiki/Jakarta\",\"lat\":-6.214623197035775,\"fcl\":\"P\",\"population\":8540121,\"fcode\":\"PPLC\"},{\"lng\":139.69171,\"geonameId\":1850147,\"countrycode\":\"JP\",\"name\":\"Tokyo\",\"fclName\":\"city, village,...\",\"toponymName\":\"Tokyo\",\"fcodeName\":\"capital of a political entity\",\"wikipedia\":\"de.wikipedia.org/wiki/Tokyo\",\"lat\":35.6895,\"fcl\":\"P\",\"population\":8336599,\"fcode\":\"PPLC\"},{\"lng\":121.531846,\"geonameId\":1668341,\"countrycode\":\"TW\",\"name\":\"Taipei\",\"fclName\":\"city, village,...\",\"toponymName\":\"Taipei\",\"fcodeName\":\"capital of a political entity\",\"wikipedia\":\"de.wikipedia.org/wiki/Taipei\",\"lat\":25.047763,\"fcl\":\"P\",\"population\":7871900,\"fcode\":\"PPLC\"},{\"lng\":-74.08175468444824,\"geonameId\":3688689,\"countrycode\":\"CO\",\"name\":\"Bogotá\",\"fclName\":\"city, village,...\",\"toponymName\":\"Bogotá\",\"fcodeName\":\"capital of a political entity\",\"wikipedia\":\"en.wikipedia.org/wiki/Bogot%C3%A1\",\"lat\":4.609705849789108,\"fcl\":\"P\",\"population\":7674366,\"fcode\":\"PPLC\"},{\"lng\":114.157691001892,\"geonameId\":1819729,\"countrycode\":\"HK\",\"name\":\"Hong Kong\",\"fclName\":\"city, village,...\",\"toponymName\":\"Hong Kong\",\"fcodeName\":\"capital of a political entity\",\"wikipedia\":\"en.wikipedia.org/wiki/Hong_Kong\",\"lat\":22.2855225817732,\"fcl\":\"P\",\"population\":7012738,\"fcode\":\"PPLC\"},{\"lng\":100.501444,\"geonameId\":1609350,\"countrycode\":\"TH\",\"name\":\"Bangkok\",\"fclName\":\"city, village,...\",\"toponymName\":\"Bangkok\",\"fcodeName\":\"capital of a political entity\",\"wikipedia\":\"en.wikipedia.org/wiki/Bangkok\",\"lat\":13.753979,\"fcl\":\"P\",\"population\":5104476,\"fcode\":\"PPLC\"}]}";

void geoInit(geotype_t *geo){
  geo->finished = GEO_STRUCT_NOT_FULL;
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

void geoInternalBufferInit(geointernalbuffertype_t *buf){
  memset(buf->buffer, '\0', sizeof('\0'));
  buf->index = 0;
}

void geoPutInInternalBuffer(const char *stream, geointernalbuffertype_t *buf, unsigned int stream_start_index){
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

bool geoParseValue(geointernalbuffertype_t *buf, char *tmp_value, enum geoKeys key){
  int i = 0;
  int dot = 0;
  if(buf->buffer[buf->index] == ':'){
    ++buf->index;
  }
  switch(key){
   case COUNTRYCODE: 
   case NAME: 
   case FCLNAME: 
   case TOPONYNAME: 
   case FCODENAME:
   case WIKI: 
   case FCL: 
   case FCODE:
     if(buf->buffer[buf->index] != '\"'){
       printf("Not a string.\n");
       return NULL;
     }
     ++buf->index; // skipping the '"'
     while(buf->buffer[buf->index] != '\"'){
       *(tmp_value+i) = buf->buffer[buf->index];
       ++buf->index;
       ++i;
     }
     ++buf->index; // skipping the '"'
    break;
   case GEONAMEID:
   case LNG: 
   case LAT: 
   case POPULATION: 
     if(buf->buffer[buf->index] == '-'){
       *(tmp_value+i) = buf->buffer[buf->index];
       ++buf->index;
       ++i;
     }
     // implement for hex values
     while(buf->buffer[buf->index] >= '0' && buf->buffer[buf->index] <= '9' || buf->buffer[buf->index] == '.'){
       if(buf->buffer[buf->index] == '.')
         ++dot;
       if(dot > 1)
         return -1;
       *(tmp_value+i) = buf->buffer[buf->index];
       ++buf->index;
       ++i;
     }
     
   break;
    
  }
  *(tmp_value+i) = '\0';
  return 0;
}

bool geoParseKey(geointernalbuffertype_t *buf, geotype_t *geo){
  int i = 0;
  char tmp_key[GEO_MAX_KEY_LENGTH+1];
  char tmp_value[GEO_MAX_VALUE_LENGTH+1];
  while(buf->buffer[buf->index] != '\"'){
    tmp_key[i] = buf->buffer[buf->index];
    ++i;
    ++buf->index; // currently it points to ':'
  }
  ++buf->index;
  tmp_key[i] = '\0';
    
    if(strncmp(tmp_key, "lng", sizeof("lng")-1) == 0){
      geoParseValue(buf, &tmp_value[0], LNG);
      geo->lng = atof(tmp_value);
      printf("%s %s %f\n", tmp_key, "lng", geo->lng);
    }
    else if(strncmp(tmp_key, "lat", sizeof("lat")-1) == 0){
      geoParseValue(buf, &tmp_value[0], LAT);
      geo->lat = atof(tmp_value);
      printf("%s %s %f\n", tmp_key, "lat", geo->lat);
    }
    else if(strncmp(tmp_key, "geonameId", sizeof("geonameId")-1) == 0){
      geoParseValue(buf, &tmp_value[0], GEONAMEID);
      geo->geonameId = atoi(tmp_value);
      printf("%s %s %d\n", tmp_key, "geonameId", geo->geonameId);
    }
    else if(strncmp(tmp_key, "countrycode", sizeof("countrycode")-1) == 0){
      geoParseValue(buf, &tmp_value[0], COUNTRYCODE);
      memcpy(geo->countrycode, tmp_value, strlen(tmp_value));
      printf("%s %s %s\n", tmp_key, "countrycode", geo->countrycode);
    }
    else if(strncmp(tmp_key, "name", sizeof("name")-1) == 0){
      geoParseValue(buf, &tmp_value[0], NAME);
      memcpy(geo->name, tmp_value, strlen(tmp_value));
      printf("%s %s %s\n", tmp_key, "name", geo->name);
    }
    else if(strncmp(tmp_key, "fclName", sizeof("fclName")-1) == 0){
      geoParseValue(buf, &tmp_value[0], FCLNAME);
      memcpy(geo->fclName, tmp_value, strlen(tmp_value));
      printf("%s %s %s\n", tmp_key, "fclName", geo->fclName);
    }
    else if(strncmp(tmp_key, "toponymName", sizeof("toponymName")-1) == 0){
      geoParseValue(buf, &tmp_value[0], TOPONYNAME);
      memcpy(geo->toponyName, tmp_value, strlen(tmp_value));
      printf("%s %s %s\n", tmp_key, "toponymname", geo->toponyName);
    }
    else if(strncmp(tmp_key, "fcodeName", sizeof("fcodeName")-1) == 0){
      geoParseValue(buf, &tmp_value[0], FCODENAME);
      memcpy(geo->fcodeName, tmp_value, strlen(tmp_value));
      printf("%s %s %s\n", tmp_key, "fcodename", geo->fcodeName);
    }
    else if(strncmp(tmp_key, "wikipedia", sizeof("wikipedia")-1) == 0){
      geoParseValue(buf, &tmp_value[0], FCODENAME);
      memcpy(geo->wiki, tmp_value, strlen(tmp_value));
      printf("%s %s %s\n", tmp_key, "wikipedia", geo->wiki);
    }
    else if(strncmp(tmp_key, "fcl", sizeof("fcl")-1) == 0){
      geoParseValue(buf, &tmp_value[0], FCL);
      memcpy(geo->fcl, tmp_value, strlen(tmp_value));
      printf("%s %s %s\n", tmp_key, "fcl", geo->fcl);
    }    
    else if(strncmp(tmp_key, "population", sizeof("population")-1) == 0){
      geoParseValue(buf, &tmp_value[0], POPULATION);
      geo->population = atoi(tmp_value);
      printf("%s %s %d\n", tmp_key, "population", geo->population);
    }
    else if(strncmp(tmp_key, "fcode", sizeof("fcode")-1) == 0){
      geoParseValue(buf, &tmp_value[0], FCODE);
      memcpy(geo->fcode, tmp_value, strlen(tmp_value));
      printf("%s %s %s\n", tmp_key, "fcode", geo->fcode);
    }    
    else{
      printf("Not predicted value: %s\n", tmp_key);
      return -1;
    }
  return 0;
}

bool geoParseObject(geointernalbuffertype_t *buf, geotype_t *geo){
  while(buf->buffer[buf->index] != '}'){
  if(buf->buffer[buf->index] == '\"'){
    ++buf->index;
    geoParseKey(buf, geo); // it also parses the value and assigns it to the struct geo
  }
  if(buf->buffer[buf->index] == ','){
    ++buf->index;
  }
  
  }
}

void geoParseInternalBuffer(geointernalbuffertype_t *buf, geotype_t *geo){
  if(strncmp(buf->buffer, "{\"geonames\":[{", sizeof("{\"geonames\":[{")-1) == 0){
   buf->index = 14; /* Start of the file is found and nw first object will be parsed */
   geoParseObject(buf, geo); 
  }
  
}

int main(){
  int success;
  geointernalbuffertype_t buf;
  geotype_t geo[1024];
  int geo_index = 0;

  geoInternalBufferInit(&buf);
  geoPutInInternalBuffer(&JSON_STRING[0], &buf, 0);
  geoInit(&geo[geo_index]);
  geoParseInternalBuffer(&buf, &geo[geo_index]);
  return 0;
}
