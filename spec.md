# flat\_fs specification

## 1. Description

flat\_fs is a filesystem designed to be as simple to implement as possible and to allow easy and fast access without a virtual filesystem. It also provides features like easy resizing and 64-bit LBA addresses, allowing for up to 8 EiB per partition.

A flat\_fs image is composed of sectors, each 512 bytes in size. Every structure in a flat\_fs image has to be sector-aligned.

## 2. First sector

This sector is usually called the boot sector, as when the image contains an OS, it is loaded by the computer on boot time. To be able to easily boot from it, all the necessary information is stored at the last 20 bytes of this sector:

| Offset | Size(bytes) | Description                              |
|--------|-------------|------------------------------------------|
|`0x1EC` | 8           | 64-bit LBA address of the root directory |
|`0x1F4` | 8           | Partition size in sectors                |
|`0x1FC` | 2           | flat\_fs signature(`0xF147`)             |
|`0x1FE` | 2           | Boot signature(`0xAA55`)                 |

## 3. Structure

The rest of the image is divided in dinamically-sized blocks, but they cannot be resized after creation. These blocks are preceded with a header that contains its size, type and other information depending on the type. To solve the resizing problem, they also have a pointer to a continuation block that points to NULL when it is not used.

(add table here)

### 3.1. Files

### 3.2. Directories

### 3.3. Root directory

## 4. License

Just don't tell it's yours or do anything evil with it :).
