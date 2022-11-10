// [AsmJit]
// Complete x86/x64 JIT and Remote Assembler for C++.
//
// [License]
// Zlib - See LICENSE.md file in the package.

// [Guard]
#ifndef _ASMJIT_BASE_RUNTIME_H
#define _ASMJIT_BASE_RUNTIME_H

// [Dependencies - AsmJit]
#include "../base/cpuinfo.h"
#include "../base/vmem.h"

// [Api-Begin]
#include "../apibegin.h"

namespace asmjit {

// ============================================================================
// [Forward Declarations]
// ============================================================================

struct Assembler;
struct CpuInfo;

//! \addtogroup asmjit_base
//! \{

// ============================================================================
// [asmjit::Runtime]
// ============================================================================

//! Base runtime.
struct ASMJIT_VIRTAPI Runtime {
  ASMJIT_NO_COPY(Runtime)

  // --------------------------------------------------------------------------
  // [asmjit::RuntimeType]
  // --------------------------------------------------------------------------

  ASMJIT_ENUM(Type) {
    kTypeNone = 0,
    kTypeJit = 1,
    kTypeRemote = 2
  };

  // --------------------------------------------------------------------------
  // [Construction / Destruction]
  // --------------------------------------------------------------------------

  //! Create a `Runtime` instance.
  ASMJIT_API Runtime() noexcept;
  //! Destroy the `Runtime` instance.
  ASMJIT_API virtual ~Runtime() noexcept;

  // --------------------------------------------------------------------------
  // [Accessors]
  // --------------------------------------------------------------------------

  //! Get the runtime type, see \ref Type.
  ASMJIT_INLINE uint32_t getRuntimeType() const noexcept { return _runtimeType; }

  //! Get stack alignment of the target.
  ASMJIT_INLINE uint32_t getStackAlignment() const noexcept { return _stackAlignment; }

  //! Get the CDECL calling convention conforming to the runtime's ABI.
  //!
  //! NOTE: This is a default calling convention used by the runtime's target.
  ASMJIT_INLINE uint32_t getCdeclConv() const noexcept { return _cdeclConv; }
  //! Get the STDCALL calling convention conforming to the runtime's ABI.
  //!
  //! NOTE: STDCALL calling convention is only used by 32-bit x86 target. On
  //! all other targets it's mapped to CDECL and calling `getStdcallConv()` will
  //! return the same as `getCdeclConv()`.
  ASMJIT_INLINE uint32_t getStdCallConv() const noexcept { return _stdCallConv; }

  //! Get CPU information.
  ASMJIT_INLINE const CpuInfo& getCpuInfo() const noexcept { return _cpuInfo; }
  //! Set CPU information.
  ASMJIT_INLINE void setCpuInfo(const CpuInfo& ci) noexcept { _cpuInfo = ci; }

  //! Get whether the runtime has a base address.
  ASMJIT_INLINE bool hasBaseAddress() const noexcept { return _baseAddress != kNoBaseAddress; }
  //! Get the base address.
  ASMJIT_INLINE Ptr getBaseAddress() const noexcept { return _baseAddress; }

  // --------------------------------------------------------------------------
  // [Interface]
  // --------------------------------------------------------------------------

  //! Allocate a memory needed for a code generated by `assembler` and
  //! relocate it to the target location.
  //!
  //! The beginning of the memory allocated for the function is returned in
  //! `dst`. Returns Status code as \ref ErrorCode, on failure `dst` is set to
  //! `nullptr`.
  virtual Error add(void** dst, Assembler* assembler) noexcept = 0;

  //! Release memory allocated by `add`.
  virtual Error release(void* p) noexcept = 0;

  // --------------------------------------------------------------------------
  // [Members]
  // --------------------------------------------------------------------------

  //! Type of the runtime.
  uint8_t _runtimeType;
  //! Type of the allocation.
  uint8_t _allocType;

  //! Runtime's stack alignment.
  uint8_t _stackAlignment;
  //! CDECL calling convention conforming to runtime ABI.
  uint8_t _cdeclConv;
  //! STDCALL calling convention conforming to runtime ABI.
  uint8_t _stdCallConv;
  //! \internal
  uint8_t _reserved[3];

  //! Runtime CPU information.
  CpuInfo _cpuInfo;

  //! Base address (-1 means no base address).
  Ptr _baseAddress;
  //! Maximum size of the code that can be added to the runtime (0=unlimited).
  size_t _sizeLimit;
};

// ============================================================================
// [asmjit::HostRuntime]
// ============================================================================

//! Base runtime for JIT code generation.
struct ASMJIT_VIRTAPI HostRuntime : public Runtime {
  ASMJIT_NO_COPY(HostRuntime)

  // --------------------------------------------------------------------------
  // [Construction / Destruction]
  // --------------------------------------------------------------------------

  //! Create a `HostRuntime` instance.
  ASMJIT_API HostRuntime() noexcept;
  //! Destroy the `HostRuntime` instance.
  ASMJIT_API virtual ~HostRuntime() noexcept;

  // --------------------------------------------------------------------------
  // [Interface]
  // --------------------------------------------------------------------------

  //! Flush an instruction cache.
  //!
  //! This member function is called after the code has been copied to the
  //! destination buffer. It is only useful for JIT code generation as it
  //! causes a flush of the processor cache.
  //!
  //! Flushing is basically a NOP under X86/X64, but is needed by architectures
  //! that do not have a transparent instruction cache.
  //!
  //! This function can also be overridden to improve compatibility with tools
  //! such as Valgrind, however, it's not an official part of AsmJit.
  ASMJIT_API virtual void flush(void* p, size_t size) noexcept;
};

// ============================================================================
// [asmjit::StaticRuntime]
// ============================================================================

//! JIT static runtime.
//!
//! JIT static runtime can be used to generate code to a memory location that
//! is known.
struct ASMJIT_VIRTAPI StaticRuntime : public HostRuntime {
  ASMJIT_NO_COPY(StaticRuntime)

  // --------------------------------------------------------------------------
  // [Construction / Destruction]
  // --------------------------------------------------------------------------

  //! Create a `StaticRuntime` instance.
  //!
  //! The `address` specifies a fixed target address, which will be used as a
  //! base address for relocation, and `sizeLimit` specifies the maximum size
  //! of a code that can be copied to it. If there is no limit `sizeLimit`
  //! should be zero.
  ASMJIT_API StaticRuntime(void* baseAddress, size_t sizeLimit = 0) noexcept;
  //! Destroy the `StaticRuntime` instance.
  ASMJIT_API virtual ~StaticRuntime() noexcept;

  // --------------------------------------------------------------------------
  // [Accessors]
  // --------------------------------------------------------------------------

  //! Get the base address.
  ASMJIT_INLINE Ptr getBaseAddress() const noexcept { return _baseAddress; }

  //! Get the maximum size of the code that can be relocated/stored in the target.
  //!
  //! Returns zero if unlimited.
  ASMJIT_INLINE size_t getSizeLimit() const noexcept { return _sizeLimit; }

  // --------------------------------------------------------------------------
  // [Interface]
  // --------------------------------------------------------------------------

  ASMJIT_API virtual Error add(void** dst, Assembler* assembler) noexcept;
  ASMJIT_API virtual Error release(void* p) noexcept;
};

// ============================================================================
// [asmjit::JitRuntime]
// ============================================================================

//! JIT runtime.
struct ASMJIT_VIRTAPI JitRuntime : public HostRuntime {
  ASMJIT_NO_COPY(JitRuntime)

  // --------------------------------------------------------------------------
  // [Construction / Destruction]
  // --------------------------------------------------------------------------

  //! Create a `JitRuntime` instance.
  ASMJIT_API JitRuntime() noexcept;
  //! Destroy the `JitRuntime` instance.
  ASMJIT_API virtual ~JitRuntime() noexcept;

  // --------------------------------------------------------------------------
  // [Accessors]
  // --------------------------------------------------------------------------

  //! Get the type of allocation.
  ASMJIT_INLINE uint32_t getAllocType() const noexcept { return _allocType; }
  //! Set the type of allocation.
  ASMJIT_INLINE void setAllocType(uint32_t allocType) noexcept { _allocType = allocType; }

  //! Get the virtual memory manager.
  ASMJIT_INLINE VMemMgr* getMemMgr() const noexcept { return const_cast<VMemMgr*>(&_memMgr); }

  // --------------------------------------------------------------------------
  // [Interface]
  // --------------------------------------------------------------------------

  ASMJIT_API virtual Error add(void** dst, Assembler* assembler) noexcept;
  ASMJIT_API virtual Error release(void* p) noexcept;

  // --------------------------------------------------------------------------
  // [Members]
  // --------------------------------------------------------------------------

  //! Virtual memory manager.
  VMemMgr _memMgr;
};

//! \}

} // asmjit namespace

// [Api-End]
#include "../apiend.h"

// [Guard]
#endif // _ASMJIT_BASE_RUNTIME_H
