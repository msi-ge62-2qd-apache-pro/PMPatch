/* Patch Internal Header 

  Copyright (c) 2012, Nikolaj Schlej. All rights reserved.
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

*/

#ifndef __PATCH_INT_H__
#define __PATCH_INT_H__

#include <stdio.h>
#include <stdlib.h>
#include "Common/BaseTypes.h"
#include "Tiano/TianoCompress.h"
#include "Tiano/TianoDecompress.h"
#include "LZMA/LzmaCompress.h"
#include "LZMA/LzmaDecompress.h"
#include "patch.h"

#define UUID_LENGTH             16
#define MODULE_ALLIGNMENT       8

// Data structures
#pragma pack(push, 1)
// Common UEFI module header 
typedef struct {
    UINT8 guid[UUID_LENGTH];
    UINT8 header_checksum;
    UINT8 data_checksum;
    UINT8 type;
    UINT8 attributes;
    UINT8 size[3];
    UINT8 state;
} module_header;
// Gap module type, attributes and state
#define TYPE_GAP                0xF0
#define ATTRIBUTES_GAP          0x00
#define STATE_STD               0xF8

// Common section header 
typedef struct {
    UINT8 size[3];
    UINT8 type;
} common_section_header;
// Compressed section type for nest module
#define SECTION_COMPRESSED      0x01
// GUID-defined section type for PowerManagement2.efi and SmmPlatform modules
#define SECTION_GUID_DEFINED    0x02
// DXE driver section type for PowerManagement and SmmPlatform modules 
#define SECTION_DXE_DEPEX       0x13

// GUID-defined section header
typedef struct {    
    UINT8 size[3];
    UINT8 type;
    UINT8 guid[16];
    UINT16 data_offset;
    UINT16 attributes;
} guid_section_header;

// Compressed section header 
typedef struct {
    UINT8 size[3];
    UINT8 type;
    UINT32 decompressed_size;
    UINT8 compression_type;
} compressed_section_header;
// Uncompressed data type 
#define COMPRESSION_NONE        0x00
// Tiano compressed data type 
#define COMPRESSION_TIANO       0x01
// LZMA compressed data type 
#define COMPRESSION_LZMA        0x02
#pragma pack(pop)

// Error codes 
#define ERR_PATCHED                         0x00
#define ERR_INVALID_ARGUMENT                0x01
#define ERR_NOT_MODULE						0x02
#define ERR_UNKNOWN_MODULE                  0x03
#define ERR_UNKNOWN_COMPRESSION_TYPE        0x04
#define ERR_TIANO_DECOMPRESSION_FAILED      0x05
#define ERR_LZMA_DECOMPRESSION_FAILED       0x06
#define ERR_PATCH_STRING_NOT_FOUND          0x07
#define ERR_TIANO_COMPRESSION_FAILED        0x08
#define ERR_LZMA_COMPRESSION_FAILED         0x09
#define ERR_PATCHED_MODULE_INSERTION_FAILED 0x0A
#define ERR_MODULE_NOT_FOUND                0x0B
#define ERR_MEMORY_ALLOCATION_FAILED        0x0C

// UUIDs 
CONST UINT8 AMI_NEST_UUID[] = 
{0x2F,0x7C,0x71,0xAE,0x42,0x1A,0x2B,0x4F,0x88,0x61,0x78,0xB7,0x9C,0xA0,0x7E,0x07};
CONST UINT8 PHOENIX_NEST_UUID[] =
{0x18,0x88,0x53,0x4A,0xE0,0x5A,0xB2,0x4E,0xB2,0xEB,0x48,0x8B,0x23,0x65,0x70,0x22};
CONST UINT8 POWERMANAGEMENT_UUID[] =
{0x70,0x39,0x78,0x8C,0x2A,0xF0,0x4D,0x4A,0xAF,0x09,0x87,0x97,0xA5,0x1E,0xEC,0x8D};
CONST UINT8 POWERMANAGEMENT2_UUID[] =
{0x4C,0x1B,0x73,0xF7,0xA2,0x58,0xF4,0x4D,0x89,0x80,0x56,0x45,0xD3,0x9E,0xCE,0x58};
CONST UINT8 SMMPLATFORM_UUID[] = 
{0x37,0x0A,0xC2,0x99,0x2A,0x04,0xE2,0x46,0x80,0xF4,0xE4,0x02,0x7F,0xDB,0xC8,0x6F};
CONST UINT8 CPUPEI_UUID[] = 
{0xA9,0xAF,0xB5,0x2B,0x33,0xFF,0x7B,0x41,0x84,0x97,0xCB,0x77,0x3C,0x2B,0x93,0xBF};
CONST UINT8 PLATFORMSETUPADVANCED_UUID[] =
{0xC4,0x94,0xEF,0xCF,0x67,0x41,0x6A,0x46,0x88,0x93,0x87,0x79,0x45,0x9D,0xFA,0x86};
CONST UINT8 DELL_RAW_FILE_UUID[] =
{0x7F,0x36,0x3E,0xF3,0xD2,0x41,0x01,0x42,0x9C,0xB7,0xAF,0xA6,0x3D,0xCC,0xEE,0xC9};
CONST UINT8 GAP_UUID[] = 
{0x85,0x65,0x53,0xE4,0x09,0x79,0x60,0x4A,0xB5,0xC6,0xEC,0xDE,0xA6,0xEB,0xFB,0x54};

// PowerManagement patch 
CONST UINT8 POWERMANAGEMENT_PATCH_PATTERN[] =
{0x75,0x08,0x0F,0xBA,0xE8,0x0F,0x89,0x44,0x24,0x30};
CONST UINT8 POWERMANAGEMENT_PATCH_PATTERN_80FB01[] =
{0x80,0xFB,0x01,0x75,0x08,0x0F,0xBA,0xE8,0x0F,0x89,0x44,0x24,0x30};
CONST UINT8 POWERMANAGEMENT_PATCHED_PATTERNS[][10] =  {
    {0xEB,0x08,0x0F,0xBA,0xE8,0x0F,0x89,0x44,0x24,0x30},
    {0xEB,0x08,0x0F,0xBA,0xE8,0x0F,0x90,0x90,0x90,0x90},
    {0xEB,0x08,0x90,0x90,0x90,0x90,0x89,0x44,0x24,0x30},
    {0xEB,0x08,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90},
    {0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90},
};
CONST UINT8 PATCHED_PATTERNS_COUNT = 
    sizeof(POWERMANAGEMENT_PATCHED_PATTERNS)/sizeof(POWERMANAGEMENT_PATCHED_PATTERNS[0]);

// SmmPlatform patch
CONST UINT8 SMMPLATFORM_PATCH_PATTERN[] =
{0x48,0x0F,0xBA,0xE0,0x0F,0x72,0x12,0x48,0x0F,0xBA,0xE8,0x0F,0xB9,0xE2,0x00,0x00,0x00};
CONST UINT8 SMMPLATFORM_PATCHED_PATTERN[] =
{0x48,0x0F,0xBA,0xE0,0x0F,0xEB,0x12,0x48,0x0F,0xBA,0xE8,0x0F,0xB9,0xE2,0x00,0x00,0x00};

// CpuPei patch 
CONST UINT8 CPUPEI_PATCH_PATTERN[] =      
{0x80,0x00,0x18,0xEB,0x05,0x0D,0x00,0x80};
CONST UINT8 CPUPEI_PATCHED_PATTERN[] =    
{0x00,0x00,0x18,0xEB,0x05,0x0D,0x00,0x00};

// Phoenix SCT 2.0 PlatformSetupAdvancedDxe patch to unlock additional BIOS settings
CONST UINT8 PLATFORMSETUPADVANCED_PATCH_PATTERN[] =
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x45,0x0A};
CONST UINT8 PLATFORMSETUPADVANCED_PATCHED_PATTERN[] =
{0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x45,0x0A};
CONST UINT8 PLATFORMSETUPADVANCED_UNICODE_PATCH_PATTERN[] =
{0x00,0x14,0x42,0x00,0x65,0x00,0x6C,0x00,0x6F,0x00,0x77,0x00,0x20,0x00,0x69,0x00,
 0x73,0x00,0x20,0x00,0x72,0x00,0x65,0x00,0x73,0x00,0x65,0x00,0x72,0x00,0x76,0x00,
 0x65,0x00,0x64,0x00,0x20,0x00,0x66,0x00,0x6F,0x00,0x72,0x00,0x20,0x00,0x52,0x00,
 0x44,0x00,0x2C,0x00,0x20,0x00,0x6E,0x00,0x6F,0x00,0x74,0x00,0x20,0x00,0x44,0x00,
 0x45,0x00,0x4C,0x00,0x4C,0x00,0x4F,0x00,0x49,0x00,0x4C,0x00,0x20,0x00,0x72,0x00,
 0x65,0x00,0x71,0x00,0x75,0x00,0x65,0x00,0x73,0x00,0x74,0x00,0x2E,0x00,0x00,0x00};
CONST UINT8 PLATFORMSETUPADVANCED_UNICODE_PATCHED_PATTERN[] =
{0x00,0x14,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,
 0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,
 0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,
 0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,
 0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,
 0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x00,0x00};

#endif