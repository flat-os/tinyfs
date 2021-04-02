# flat\_fs specification
(revision r04, 02/04/2021)

*Further revisions shall remain compatible to this one.*

Warning: This revision is NOT compatible with r03+1 or older.

## 1. Description

flat\_fs is a filesystem designed to be as simple to implement as possible and to allow easy and fast access without a virtual filesystem. It also provides features like easy resizing and 64-bit LBA addresses, allowing for up to 8 EiB per partition.

A flat\_fs image is composed of sectors, each 512 bytes in size. Every structure in a flat\_fs image has to be sector-aligned.

## 2. First sector

This sector is usually called the boot sector, as when the image contains an OS, it is loaded by the computer on boot time. To be able to easily boot from it, all the necessary information is stored at the last 36 bytes of this sector:

| Offset  | Size(bytes) | Description                              |
|---------|-------------|------------------------------------------|
| `0x1DC` | 16          | GUID of partition(if necessary)          |
| `0x1EC` | 8           | 64-bit LBA address of the root directory |
| `0x1F4` | 8           | Partition size in sectors                |
| `0x1FC` | 2           | flat\_fs r03 signature(`0x7B74`)         |
| `0x1FE` | 2           | Boot signature(`0xAA55`) if bootable     |

## 3. Structure

The rest of the image is divided in dynamically-sized blocks, but they cannot be resized after creation. These blocks are preceded with a 512-byte header that contains its size, type and other information depending on the type. To solve the resizing problem, they also have a pointer to a continuation block that points to NULL when it is not needed.

#### Header:

| Offset  | Size(bytes) | Description                                           |
|---------|-------------|-------------------------------------------------------|
| `0x000` | 2           | Block type(see table below)                           |
| `0x002` | 460         | Path(if file/dir), image name(if root dir.) or unused |
| `0x1CE` | 8           | Total size(in bytes, if file)                         |
| `0x1D6` | 8           | 64-bit atime(if file/dir)                             |
| `0x1DE` | 8           | 64-bit mtime(if file/dir)                             |
| `0x1E6` | 8           | 64-bit ctime(if file/dir)                             |
| `0x1EE` | 2           | Permissions(s----rwx-----rwx, if file/dir)            |
| `0x1F0` | 8           | Block size(in sectors)                                |
| `0x1F8` | 8           | Continuation block header offset, NULL if not needed  |

Note about permissions: They are NOT the same as in regular UNIX permissions as it flat\_fs is not designed with users in mind, but with a root/system level and a user level.

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

Files are the simplest type of entry in flat\_fs. They just store the file contents in the block and, if needed, in several continuation blocks.

### 3.2. Directories

Directories are special entries that are made of 64-bit offsets to more entries, with empty or deleted ones being NULL to avoid having to rearrange everything when a file is deleted.

### 3.3. Root directory

The root directory is the first entry on the image, and has its header on the second sector on the image, just after the bootsector. It works in exactly the same way as regular directories, the only difference being that they store the image/partition name instead of the path.

### 3.4. Links

In flat\_fs, there is only a single kind of links. They store the offset to the original file's header on the continuation header offset.

## 4. Credits & license

Designed and written by segfaultdev.

Just copy or mention this file on projects where you use it and do NOT do anything evil with it.
