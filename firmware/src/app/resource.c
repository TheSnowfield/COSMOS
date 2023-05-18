#define RES_INTERNAL
#include "resource.h"
#undef RES_INTERNAL

#include <rucfs.h>
#include <rucfs.img.h>

const uint8_t* RUCFSIMG = rucfs_img;

typedef struct {
  const char*     file;
  rucfs_file_t*   handle;
  const uint8_t** symbol;
} resource_cache_t;

static const resource_cache_t _caches[] = {
  { "/font/pixel_3x5",                     NULL, &RES_FONT_PIXEL3X5              },
  { "/font/pixel_5x7",                     NULL, &RES_FONT_PIXEL5X7              },
  { "/texture/splash",                     NULL, &RES_TEXTURE_COSMOS_SPLASH      },
  { NULL, NULL, NULL }
};

bool resource_init(const uint8_t* address) {

  rucfs_ctx_t context;

  // load a rucfs binary
  if(!rucfs_ok(rucfs_load((uint8_t *)address, &context))) {
    resource_uninit();
    return false;
  }

  // cache the resources
  const resource_cache_t *_list = _caches;
  while(_list->file != NULL) {

    // open the file
    rucfs_file_t* file;
    if(!rucfs_ok(rucfs_fopen(&context, _list->file, &file))) {
      resource_uninit();
      return false;
    }

    // cache the pointer
    *_list->symbol = file->data;
    ++_list;
  }

  return true;
}

void resource_uninit() {
  
}

const uint8_t* resource_fetch(const char* path) {
  return NULL;
}
