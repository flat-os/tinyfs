# flat\_fs specification
(revision r03, 24/03/2021)

Warning: This revision is NOT compatible with previous ones.

## 1. Description

flat\_fs is a filesystem designed to be as simple to implement as possible and to allow easy and fast access without a virtual filesystem. It also provides features like easy resizing and 64-bit LBA addresses, allowing for up to 8 EiB per partition.

A flat\_fs image is composed of sectors, each 512 bytes in size. Every structure in a flat\_fs image has to be sector-aligned.

## 2. First sector

This sector is usually called the boot sector, as when the image contains an OS, it is loaded by the computer on boot time. To be able to easily boot from it, all the necessary information is stored at the last 20 bytes of this sector:

| Offset  | Size(bytes) | Description                              |
|---------|-------------|------------------------------------------|
| `0x1EC` | 8           | 64-bit LBA address of the root directory |
| `0x1F4` | 8           | Partition size in sectors                |
| `0x1FC` | 2           | flat\_fs r03 signature(`0xF053`)         |
| `0x1FE` | 2           | Boot signature(`0xAA55`) if bootable     |

## 3. Structure

The rest of the image is divided in dynamically-sized blocks, but they cannot be resized after creation. These blocks are preceded with a 512-byte header that contains its size, type and other information depending on the type. To solve the resizing problem, they also have a pointer to a continuation block that points to NULL when it is not needed.

#### Header:

| Offset  | Size(bytes) | Description                                          |
|---------|-------------|------------------------------------------------------|
| `0x000` | 2           | Block type(see table below)                          |
| `0x002` | 494         | File/directory data                                  |
| `0x1F0` | 8           | Block size(in sectors)                               |
| `0x1F8` | 8           | Continuation block header offset, NULL if not needed |

#### Types:

| Type     | Description        |
|----------|--------------------|
| `0x0000` | Invalid block      |
| `0xFFFF` | Free block         |
| `0x0080` | File               |
| `0x0081` | Directory          |
| `0x0082` | File link          |
| `0x0083` | Directory link     |
| `0x00FF` | Root directory     |
| `0xFF00` | Continuation block |

### 3.1. Files

Files are the simplest type of entry in flat\_fs. They just store the file contents in the block and, if needed, in one or more continuation blocks.

#### File/dir. data:

| Offset  | Size(bytes) | Description                     |
|---------|-------------|---------------------------------|
| `0x000` | 460         | Filename(NULL-terminated)       |
| `0x1CC` | 8           | File size(in bytes)             |
| `0x1D4` | 8           | 64-bit atime(access time)       |
| `0x1DC` | 8           | 64-bit mtime(modification time) |
| `0x1E4` | 8           | 64-bit ctime(creation time)     |
| `0x1EC` | 2           | Permissions(s----rwx-----rwx)   |

Note about permissions: They are NOT the same as in regular UNIX permissions as it flat\_fs is not designed with users in mind, but with a root/system level and a user level(what flat_os uses).

### 3.2. Directories

Directories are special entries that are made of 64-bit offsets to more entries, with empty or deleted ones being NULL to avoid having to rearrange everything when a file is deleted. They share the same data structure as files.

### 3.3. Root directory

The root directory is the first entry on the image, and has its header on the second sector on the image, just after the bootsector. It works in exactly the same way as regular directories, with the only difference being that it doesn't use the directory data.

### 3.4. Links

In flat\_fs, there is only a single kind of links(which would act more like symbolic links). They act as a file on which they store a 64-bit pointer to the original file's header. That means their size is fixed to 8 bytes, and the link should be ignored if otherwise.

## 4. Credits & license

Designed and written by segfaultdev.

Just copy or mention this file on projects where you use it and do NOT do anything evil with it.
