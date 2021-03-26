#ifndef __FS__FLAT_FS_H__
#define __FS__FLAT_FS_H__

#include <stdint.h>
#include <stdbool.h>
#include <lib/part.h>
#include <lib/blib.h>

// flat_fs r03+1 signature
#define FLAT_SIGNATURE (0xF053)

#define FLAT_TYPE_NULL      (0x0000)
#define FLAT_TYPE_FREE      (0xFFFF)
#define FLAT_TYPE_FILE      (0x0080)
#define FLAT_TYPE_DIR       (0x0081)
#define FLAT_TYPE_FILE_LINK (0x0082)
#define FLAT_TYPE_DIR_LINK  (0x0083)
#define FLAT_TYPE_ROOT_DIR  (0x00FF)
#define FLAT_TYPE_DATA      (0xFF00)

// Used for checking if it has extra data on it
#define FLAT_TYPE_MASK (0b1111111101110000)

#define FLAT_META_READ   (0b0000000000000001)
#define FLAT_META_WRITE  (0b0000000000000010)
#define FLAT_META_EXEC   (0b0000000000000100)
#define FLAT_META_SYSTEM (0b0000000000001000)

#define NULL_64 (0x0000000000000000)

typedef struct flatfs_t flatfs_t;

typedef struct flatfs_data_t flatfs_data_t;
typedef struct flatfs_header_t flatfs_header_t;

struct flatfs_t {
  uint64_t guid[2];

  uint64_t root_ptr;
  uint64_t img_size;

  uint16_t flat_sig;
  uint16_t boot_sig;
} __attribute__((packed));

struct flatfs_data_t {
  char name[460];

  uint64_t size;
  uint64_t atime, mtime, ctime;
  uint16_t attr;
} __attribute__((packed));

struct flatfs_header_t {
  uint16_t type;
  flatfs_data_t data;

  uint64_t block_cnt;
  uint64_t next_ptr;
} __attribute__((packed));

struct flatfs_file_handle {
  struct volume *part;

  uint64_t header_ptr;
  uint64_t size;
};

int flatfs_open(struct flatfs_file_handle *ret, struct volume *part, const char *filename);
int flatfs_read(struct flatfs_file_handle *file, void *buf, uint64_t loc, uint64_t count);

#endif
