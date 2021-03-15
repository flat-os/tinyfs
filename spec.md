# flat\_fs specification

## 1. Description

flat\_fs is a filesystem designed to be as simple to implement as possible and to allow easy and fast access without a virtual filesystem. It also provides features like easy resizing and 64-bit LBA addresses, allowing for up to 8 EiB per partition.

A flat\_fs image is composed of sectors, each 512 bytes in size. Every structure in a flat\_fs image has to be sector-aligned.

## 2. First sector

This sector is usually called the boot sector, as when the image contains an OS, it is loaded by the computer on boot time. To be able to easily boot from it, all the necessary information is stored at the last 20 bytes of this sector:

| Offset  | Size(bytes) | Description                              |
|---------|-------------|------------------------------------------|
| `0x1EC` | 8           | 64-bit LBA address of the root directory |
| `0x1F4` | 8           | Partition size in sectors                |
| `0x1FC` | 2           | flat\_fs signature(`0xF147`)             |
| `0x1FE` | 2           | Boot signature(`0xAA55`) if bootable     |

## 3. Structure

The rest of the image is divided in dynamically-sized blocks, but they cannot be resized after creation. These blocks are preceded with a 512-byte header that contains its size, type and other information depending on the type. To solve the resizing problem, they also have a pointer to a continuation block that points to NULL when it is not needed.

#### Header:

| Offset  | Size(bytes) | Description                                          |
|---------|-------------|------------------------------------------------------|
| `0x000` | 2           | Block type(see table below)                          |
| `0x002` | 494         | Type-dependent data                                  |
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

#### File data:

| Offset  | Size(bytes) | Description                              |
|---------|-------------|------------------------------------------|
| `0x000` | 480         | Filename(NULL-terminated)                |
| `0x1E0` | 8           | File size(in bytes)                      |
| `0x1E8` | 4           | Creation date since year 2000 in seconds |
| `0x1EC` | 2           | Metadata(see table below)                |

#### File metadata:

| Offset(bits) | Size(bits) | Description                  |
|--------------|------------|------------------------------|
| `0x00`       | 1          | Read bit(1 if readable)      |
| `0x01`       | 1          | Write bit(1 if writable)     |
| `0x02`       | 1          | Execute bit(1 if executable) |
| `0x03`       | 1          | System bit(1 if system file) |
| `0x04`       | 12         | Unused                       |

### 3.2. Directories

Directories are special entries that are made of 64-bit offsets to more entries, with empty or deleted ones being NULL to avoid having to rearrange everything when a file is deleted.

**They share the same type-dependent data structure.**

### 3.3. Root directory

The root directory is the first entry on the image, and **has** its header in the sector sector on the image, just after the bootsector. It works in exactly the same way as regular directories, with the only difference being that they don't have any type-dependent data structure.

## 4. License

Just don't tell it's yours or do anything evil with it :).
