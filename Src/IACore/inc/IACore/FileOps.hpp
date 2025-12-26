// IACore-OSS; The Core Library for All IA Open Source Projects
// Copyright (C) 2025 IAS (ias@iasoft.dev)
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <IACore/StreamReader.hpp>
#include <IACore/StreamWriter.hpp>

#if IA_PLATFORM_WINDOWS
using NativeFileHandle = HANDLE;
STATIC CONSTEXPR NativeFileHandle INVALID_FILE_HANDLE = INVALID_HANDLE_VALUE;
#else
using NativeFileHandle = int;
STATIC CONSTEXPR NativeFileHandle INVALID_FILE_HANDLE = -1;
#endif

namespace IACore
{
    class FileOps
    {
      public:
        class MemoryMappedRegion;

        enum class EFileAccess : UINT8
        {
            READ,      // Read-only
            WRITE,     // Write-only
            READ_WRITE // Read and Write
        };

        enum class EFileMode : UINT8
        {
            OPEN_EXISTING,    // Fails if file doesn't exist
            OPEN_ALWAYS,      // Opens if exists, creates if not
            CREATE_NEW,       // Fails if file exists
            CREATE_ALWAYS,    // Overwrites existing
            TRUNCATE_EXISTING // Opens existing and clears it
        };

        STATIC Expected<NativeFileHandle, String> NativeOpenFile(IN CONST FilePath &path, IN EFileAccess access,
                                                                 IN EFileMode mode, IN UINT32 permissions = 0644);
        STATIC VOID NativeCloseFile(IN NativeFileHandle handle);

      public:
        STATIC FilePath NormalizeExecutablePath(IN CONST FilePath &path);

      public:
        STATIC VOID UnmapFile(IN PCUINT8 mappedPtr);
        STATIC Expected<PCUINT8, String> MapFile(IN CONST FilePath &path, OUT SIZE_T &size);

        // @param `isOwner` TRUE to allocate/truncate. FALSE to just open.
        STATIC Expected<PUINT8, String> MapSharedMemory(IN CONST String &name, IN SIZE_T size, IN BOOL isOwner);
        STATIC VOID UnlinkSharedMemory(IN CONST String &name);

        STATIC Expected<StreamReader, String> StreamFromFile(IN CONST FilePath &path);
        STATIC Expected<StreamWriter, String> StreamToFile(IN CONST FilePath &path, IN BOOL overwrite = false);

        STATIC Expected<String, String> ReadTextFile(IN CONST FilePath &path);
        STATIC Expected<Vector<UINT8>, String> ReadBinaryFile(IN CONST FilePath &path);
        STATIC Expected<SIZE_T, String> WriteTextFile(IN CONST FilePath &path, IN CONST String &contents,
                                                      IN BOOL overwrite = false);
        STATIC Expected<SIZE_T, String> WriteBinaryFile(IN CONST FilePath &path, IN Span<UINT8> contents,
                                                        IN BOOL overwrite = false);

      private:
        STATIC UnorderedMap<PCUINT8, Tuple<PVOID, PVOID, PVOID>> s_mappedFiles;
    };

    class FileOps::MemoryMappedRegion
    {
      public:
        MemoryMappedRegion() = default;
        ~MemoryMappedRegion();

        MemoryMappedRegion(CONST MemoryMappedRegion &) = delete;
        MemoryMappedRegion &operator=(CONST MemoryMappedRegion &) = delete;

        MemoryMappedRegion(MemoryMappedRegion &&other) NOEXCEPT;
        MemoryMappedRegion &operator=(MemoryMappedRegion &&other) NOEXCEPT;

        Expected<VOID, String> Map(NativeFileHandle handle, UINT64 offset, SIZE_T size);

        VOID Unmap();
        VOID Flush();

        PUINT8 GetPtr() CONST
        {
            return m_ptr;
        }

        SIZE_T GetSize() CONST
        {
            return m_size;
        }

        BOOL IsValid() CONST
        {
            return m_ptr != nullptr;
        }

      private:
        PUINT8 m_ptr{nullptr};
        SIZE_T m_size{0};

#if IA_PLATFORM_WINDOWS
        HANDLE m_hMap{NULL};
#endif
    };
} // namespace IACore