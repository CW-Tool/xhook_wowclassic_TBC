// Copyright (C) 2010-2015 Joshua Boyce
// See the file COPYING for copying permission.

#include "headers.hpp"

#include <iostream>

#include <hadesmem/config.hpp>
#include <hadesmem/detail/str_conv.hpp>
#include <hadesmem/pelib/dos_header.hpp>
#include <hadesmem/pelib/nt_headers.hpp>
#include <hadesmem/pelib/pe_file.hpp>
#include <hadesmem/process.hpp>

#include "disassemble.hpp"
#include "main.hpp"
#include "print.hpp"
#include "warning.hpp"

// TODO: Dump rich header.

namespace
{
void DumpDosHeader(hadesmem::Process const& process,
                   hadesmem::PeFile const& pe_file)
{
  std::wostream& out = GetOutputStreamW();

  WriteNewline(out);
  WriteNormal(out, L"DOS Header:", 1);

  hadesmem::DosHeader const dos_hdr(process, pe_file);
  WriteNewline(out);
  WriteNamedHex(out, L"Magic", dos_hdr.GetMagic(), 2);
  WriteNamedHex(out, L"BytesOnLastPage", dos_hdr.GetBytesOnLastPage(), 2);
  WriteNamedHex(out, L"PagesInFile", dos_hdr.GetPagesInFile(), 2);
  WriteNamedHex(out, L"Relocations", dos_hdr.GetRelocations(), 2);
  WriteNamedHex(
    out, L"SizeOfHeaderInParagraphs", dos_hdr.GetSizeOfHeaderInParagraphs(), 2);
  WriteNamedHex(out, L"MinExtraParagraphs", dos_hdr.GetMinExtraParagraphs(), 2);
  WriteNamedHex(out, L"MaxExtraParagraphs", dos_hdr.GetMaxExtraParagraphs(), 2);
  WriteNamedHex(out, L"InitialSS", dos_hdr.GetInitialSS(), 2);
  WriteNamedHex(out, L"InitialSP", dos_hdr.GetInitialSP(), 2);
  WriteNamedHex(out, L"Checksum", dos_hdr.GetChecksum(), 2);
  WriteNamedHex(out, L"InitialIP", dos_hdr.GetInitialIP(), 2);
  WriteNamedHex(out, L"InitialCS", dos_hdr.GetInitialCS(), 2);
  WriteNamedHex(out, L"RelocTableFileAddr", dos_hdr.GetRelocTableFileAddr(), 2);
  WriteNamedHex(out, L"OverlayNum", dos_hdr.GetOverlayNum(), 2);
  // Officially reserved. No known uses by the PE loader.
  // Used by some to mark binaries. E.g. Detours uses it to mark an in-memory
  // image with "Detours!", Nvidia marks some of their binaries with "NV00",
  // "Go" tools (the assembler for Windows, not the language by Google) embed
  // the string "Win32 Program!" which spans across the
  // OEMID/OEMInfo/ReservedWords2 fields also.
  // Also used by some packers as scratch space (or simply filled with garbage
  // to confuse reversers).
  auto const reserved_words_1 = dos_hdr.GetReservedWords1();
  WriteNamedHexContainer(out, L"ReservedWords1", reserved_words_1, 2);
  if (std::find_if(std::begin(reserved_words_1),
                   std::end(reserved_words_1),
                   [](WORD w)
                   {
                     return !!w;
                   }) != std::end(reserved_words_1))
  {
    WriteNormal(out, L"WARNING! Detected non-zero data in ReservedWords1.", 2);
    WarnForCurrentFile(WarningType::kSuspicious);
  }
  WriteNamedHex(out, L"OEMID", dos_hdr.GetOEMID(), 2);
  WriteNamedHex(out, L"OEMInfo", dos_hdr.GetOEMInfo(), 2);
  auto const reserved_words_2 = dos_hdr.GetReservedWords2();
  WriteNamedHexContainer(out, L"ReservedWords2", reserved_words_2, 2);
  if (std::find_if(std::begin(reserved_words_2),
                   std::end(reserved_words_2),
                   [](WORD w)
                   {
                     return !!w;
                   }) != std::end(reserved_words_2))
  {
    WriteNormal(out, L"WARNING! Detected non-zero data in ReservedWords1.", 2);
    WarnForCurrentFile(WarningType::kSuspicious);
  }
  WriteNamedHex(out, L"NewHeaderOffset", dos_hdr.GetNewHeaderOffset(), 2);
}

std::wstring GetDataDirName(DWORD num)
{
  switch (static_cast<hadesmem::PeDataDir>(num))
  {
  case hadesmem::PeDataDir::Export:
    return L"Export";

  case hadesmem::PeDataDir::Import:
    return L"Import";

  case hadesmem::PeDataDir::Resource:
    return L"Resource";

  case hadesmem::PeDataDir::Exception:
    return L"Exception";

  case hadesmem::PeDataDir::Security:
    return L"Security";

  case hadesmem::PeDataDir::BaseReloc:
    return L"BaseReloc";

  case hadesmem::PeDataDir::Debug:
    return L"Debug";

  case hadesmem::PeDataDir::Architecture:
    return L"Architecture";

  case hadesmem::PeDataDir::GlobalPTR:
    return L"GlobalPTR";

  case hadesmem::PeDataDir::TLS:
    return L"TLS";

  case hadesmem::PeDataDir::LoadConfig:
    return L"LoadConfig";

  case hadesmem::PeDataDir::BoundImport:
    return L"BoundImport";

  case hadesmem::PeDataDir::IAT:
    return L"IAT";

  case hadesmem::PeDataDir::DelayImport:
    return L"DelayImport";

  case hadesmem::PeDataDir::COMDescriptor:
    return L"COMDescriptor";

  case hadesmem::PeDataDir::Reserved:
    return L"Reserved";

  default:
    HADESMEM_DETAIL_ASSERT(false);
    return L"UKNOWN";
  }
}

bool IsSupportedDataDir(DWORD num)
{
  switch (static_cast<hadesmem::PeDataDir>(num))
  {
  case hadesmem::PeDataDir::Export:
  case hadesmem::PeDataDir::Import:
  case hadesmem::PeDataDir::TLS:
  case hadesmem::PeDataDir::BoundImport:
  case hadesmem::PeDataDir::BaseReloc:
  case hadesmem::PeDataDir::IAT:
    return true;

  case hadesmem::PeDataDir::Resource:
  case hadesmem::PeDataDir::Exception:
  case hadesmem::PeDataDir::Security:
  case hadesmem::PeDataDir::Debug:
  case hadesmem::PeDataDir::Architecture:
  case hadesmem::PeDataDir::GlobalPTR:
  case hadesmem::PeDataDir::LoadConfig:
  case hadesmem::PeDataDir::DelayImport:
  case hadesmem::PeDataDir::COMDescriptor:
  case hadesmem::PeDataDir::Reserved:
    return false;

  default:
    HADESMEM_DETAIL_ASSERT(false);
    return false;
  }
}

void DumpNtHeaders(hadesmem::Process const& process,
                   hadesmem::PeFile const& pe_file)
{
  std::wostream& out = GetOutputStreamW();

  WriteNewline(out);
  WriteNormal(out, L"NT Headers:", 1);

  hadesmem::NtHeaders const nt_hdrs(process, pe_file);
  WriteNewline(out);
  WriteNamedHex(out, L"Signature", nt_hdrs.GetSignature(), 2);
  WriteNamedHex(out, L"Machine", nt_hdrs.GetMachine(), 2);
  auto const num_sections = nt_hdrs.GetNumberOfSections();
  WriteNamedHex(out, L"NumberOfSections", num_sections, 2);
  if (!num_sections)
  {
    WriteNormal(out, L"WARNING! No sections.", 2);
    WarnForCurrentFile(WarningType::kSuspicious);
  }
  if (num_sections > 96U)
  {
    WriteNormal(out, L"WARNING! More than 96 sections.", 2);
    WarnForCurrentFile(WarningType::kSuspicious);
  }
  // TODO: Warn for unusual time stamps (very old, in the future, etc.). Both
  // here and elsewehre (e.g. export dir, import dir, etc.).
  DWORD const time_date_stamp = nt_hdrs.GetTimeDateStamp();
  std::wstring time_date_stamp_str;
  if (!ConvertTimeStamp(time_date_stamp, time_date_stamp_str))
  {
    WriteNormal(out, L"WARNING! Invalid timestamp.", 2);
    WarnForCurrentFile(WarningType::kSuspicious);
  }
  WriteNamedHexSuffix(
    out, L"TimeDateStamp", time_date_stamp, time_date_stamp_str, 2);
  WriteNamedHex(
    out, L"PointerToSymbolTable", nt_hdrs.GetPointerToSymbolTable(), 2);
  WriteNamedHex(out, L"NumberOfSymbols", nt_hdrs.GetNumberOfSymbols(), 2);
  // TODO: Detect when SizeOfOptionalHeader has been set to put the section
  // table in unmapped space (e.g. the overlay).
  WriteNamedHex(
    out, L"SizeOfOptionalHeader", nt_hdrs.GetSizeOfOptionalHeader(), 2);
  WriteNamedHex(out, L"Characteristics", nt_hdrs.GetCharacteristics(), 2);
  WriteNamedHex(out, L"Magic", nt_hdrs.GetMagic(), 2);
  WriteNamedHex(out, L"MajorLinkerVersion", nt_hdrs.GetMajorLinkerVersion(), 2);
  WriteNamedHex(out, L"MinorLinkerVersion", nt_hdrs.GetMinorLinkerVersion(), 2);
  WriteNamedHex(out, L"SizeOfCode", nt_hdrs.GetSizeOfCode(), 2);
  WriteNamedHex(
    out, L"SizeOfInitializedData", nt_hdrs.GetSizeOfInitializedData(), 2);
  WriteNamedHex(
    out, L"SizeOfUninitializedData", nt_hdrs.GetSizeOfUninitializedData(), 2);
  // TODO: For valid EPs inside the file, dump the section that it is in.
  DWORD const addr_of_ep = nt_hdrs.GetAddressOfEntryPoint();
  WriteNamedHex(out, L"AddressOfEntryPoint", addr_of_ep, 2);
  // Entry point can be null. For DLLs this is fine, because it simply means the
  // EP is not called, but for non-DLLs it means that execution starts at
  // ImageBase, executing 'MZ' as 'dec ebp/pop edx'.
  // Sample: nullEP.exe (Corkami PE Corpus).
  // The EP can also be null in the case where it is 'patched' via TLS (although
  // it doesn't actually have to be null, it can be anything, even a decoy value
  // that seems correct).
  // It seems it can be null for some .NET assemblies. This needs to be
  // investigated further however as I'm not yet sure how it works...
  // http://bit.ly/1M95tzh
  // TODO: Investigate null EP RVA for .NET assemblies and re-add the warning
  // if appropriate.
  if (!addr_of_ep && !(nt_hdrs.GetCharacteristics() & IMAGE_FILE_DLL))
  {
    WriteNormal(out, L"WARNING! Detected zero EP in non-DLL PE.", 2);
    WarnForCurrentFile(WarningType::kSuspicious);
  }
  auto const ep_va = hadesmem::RvaToVa(process, pe_file, addr_of_ep);
  if (addr_of_ep && !ep_va)
  {
    WriteNormal(out, L"WARNING! Unable to resolve EP to file offset.", 2);
    WarnForCurrentFile(WarningType::kSuspicious);
  }
  auto const image_base = nt_hdrs.GetImageBase();
  if (image_base + addr_of_ep < image_base)
  {
    WriteNormal(out, L"WARNING! EP is at a negative offset.", 2);
    WarnForCurrentFile(WarningType::kSuspicious);
  }
  DisassembleEp(process, pe_file, addr_of_ep, ep_va, 3);
  WriteNamedHex(out, L"BaseOfCode", nt_hdrs.GetBaseOfCode(), 2);
  if (!pe_file.Is64())
  {
    WriteNamedHex(out, L"BaseOfData", nt_hdrs.GetBaseOfData(), 2);
  }
  WriteNamedHex(out, L"ImageBase", image_base, 2);
  // ImageBase can be null under XP. In this case the binary is relocated to
  // 0x10000.
  // Sample: ibnullXP.exe (Corkami PE corpus).
  if (!image_base)
  {
    WriteNormal(out, L"WARNING! Detected zero ImageBase.", 2);
    WarnForCurrentFile(WarningType::kSuspicious);
  }
  // If ImageBase is in the kernel address range it's relocated to 0x1000.
  // Sample: ibkernel.exe (Corkami PE corpus).
  if (nt_hdrs.GetMachine() == IMAGE_FILE_MACHINE_I386 &&
      image_base >= (1UL << 31))
  {
    WriteNormal(out, L"WARNING! Detected kernel space ImageBase.", 2);
    WarnForCurrentFile(WarningType::kSuspicious);
  }
  // Not sure if this is actually possible under x64.
  // TODO: Check if this is allowed.
  else if (nt_hdrs.GetMachine() == IMAGE_FILE_MACHINE_AMD64 &&
           image_base >= (0xFFFFULL << 48))
  {
    // User space is 0x00000000`00000000 - 0x0000FFFF`FFFFFFFF
    // Kernel space is 0xFFFF0000`00000000 - 0xFFFFFFFF`FFFFFFFF
    // TODO: What about the gap?
    WriteNormal(out, L"WARNING! Detected kernel space ImageBase.", 2);
    WarnForCurrentFile(WarningType::kSuspicious);
  }
  // ImageBase must be a multiple of 0x10000
  if (!!(image_base & 0xFFFF))
  {
    WriteNormal(
      out,
      L"WARNING! Detected invalid ImageBase (not a multiple of 0x10000).",
      2);
    WarnForCurrentFile(WarningType::kSuspicious);
  }
  DWORD const section_alignment = nt_hdrs.GetSectionAlignment();
  WriteNamedHex(out, L"SectionAlignment", section_alignment, 2);
  // Some system DLLs (and normal .NET PE files) have large section alignment
  // (e.g. WoW64 kernel32.dll on Windows 8.1 has section alignment of 0x10000).
  // Instead of simply flagging all large alignments, look simply for ones which
  // are not a multiple of the page size.
  SYSTEM_INFO system_info{};
  ::GetNativeSystemInfo(&system_info);
  bool const is_page_size_multiple =
    !(section_alignment % system_info.dwPageSize);
  // Sample: bigalign.exe (Corkami PE corpus).
  // Sample: nosection*.exe (Corkami PE corpus).
  if (section_alignment < 0x200 ||
      (section_alignment > 0x1000 && !is_page_size_multiple))
  {
    WriteNormal(out, L"WARNING! Unusual section alignment.", 2);
    WarnForCurrentFile(WarningType::kSuspicious);
  }
  DWORD const file_alignment = nt_hdrs.GetFileAlignment();
  WriteNamedHex(out, L"FileAlignment", file_alignment, 2);
  // TODO: Better file alignment checks. Should always be a power of 2 between
  // 512 and 65535?
  // Sample: bigalign.exe (Corkami PE corpus).
  // Sample: nosection*.exe (Corkami PE corpus).
  if (file_alignment < 0x200 || file_alignment > 0x1000)
  {
    WriteNormal(out, L"WARNING! Unusual file alignment.", 2);
    WarnForCurrentFile(WarningType::kSuspicious);

    if (file_alignment < 0x200 && num_sections != 0)
    {
      // TODO: Find out if it's legal to have a file alignment < 0x200, but
      // still have sections. This may impact whether or not part of RvaToVa is
      // correct (explicitly using ~0x1FF instead of taking the min of 0x200 and
      // the FileAlignment). Need to reverse what the loader does in this
      // situations.
      // Sample: maxsecXP.exe.
      // Sample: maxsec_lowaligW7.exe.
      // Sample: nullSOH-XP.exe.
      WriteNormal(
        out, L"WARNING! Low file alignment with non-zero section count.", 2);
      WarnForCurrentFile(WarningType::kSuspicious);
    }
  }
  if (section_alignment < 0x800 && section_alignment != file_alignment)
  {
    WriteNormal(out, L"WARNING! Unusual alignment.", 2);
    WarnForCurrentFile(WarningType::kSuspicious);
  }
  if (file_alignment > section_alignment)
  {
    WriteNormal(out, L"WARNING! Invalid alignment.", 2);
    WarnForCurrentFile(WarningType::kSuspicious);
  }
  WriteNamedHex(out,
                L"MajorOperatingSystemVersion",
                nt_hdrs.GetMajorOperatingSystemVersion(),
                2);
  WriteNamedHex(out,
                L"MinorOperatingSystemVersion",
                nt_hdrs.GetMinorOperatingSystemVersion(),
                2);
  WriteNamedHex(out, L"MajorImageVersion", nt_hdrs.GetMajorImageVersion(), 2);
  WriteNamedHex(out, L"MinorImageVersion", nt_hdrs.GetMinorImageVersion(), 2);
  WriteNamedHex(
    out, L"MajorSubsystemVersion", nt_hdrs.GetMajorSubsystemVersion(), 2);
  WriteNamedHex(
    out, L"MinorSubsystemVersion", nt_hdrs.GetMinorSubsystemVersion(), 2);
  if (nt_hdrs.GetMajorSubsystemVersion() < 3 ||
      (nt_hdrs.GetMajorSubsystemVersion() == 3 &&
       nt_hdrs.GetMinorSubsystemVersion() < 10))
  {
    WriteNormal(out, L"WARNING! Invalid subsystem version.", 2);
    WarnForCurrentFile(WarningType::kSuspicious);
  }
  auto const win32_version_value = nt_hdrs.GetWin32VersionValue();
  WriteNamedHex(out, L"Win32VersionValue", win32_version_value, 2);
  // Officially defined as reserved and should be null.
  // However, if non-null it overrides OS version values in the PEB after
  // loading.
  // Sample: winver.exe (Corkami PE Corpus)
  if (win32_version_value)
  {
    WriteNormal(
      out, L"WARNING! Detected non-zero data in Win32VersionValue.", 2);
    WarnForCurrentFile(WarningType::kSuspicious);
  }
  WriteNamedHex(out, L"SizeOfImage", nt_hdrs.GetSizeOfImage(), 2);
  WriteNamedHex(out, L"SizeOfHeaders", nt_hdrs.GetSizeOfHeaders(), 2);
  WriteNamedHex(out, L"CheckSum", nt_hdrs.GetCheckSum(), 2);
  WriteNamedHex(out, L"Subsystem", nt_hdrs.GetSubsystem(), 2);
  WriteNamedHex(out, L"DllCharacteristics", nt_hdrs.GetDllCharacteristics(), 2);
  WriteNamedHex(out, L"SizeOfStackReserve", nt_hdrs.GetSizeOfStackReserve(), 2);
  WriteNamedHex(out, L"SizeOfStackCommit", nt_hdrs.GetSizeOfStackCommit(), 2);
  WriteNamedHex(out, L"SizeOfHeapReserve", nt_hdrs.GetSizeOfHeapReserve(), 2);
  WriteNamedHex(out, L"SizeOfHeapCommit", nt_hdrs.GetSizeOfHeapCommit(), 2);
  WriteNamedHex(out, L"LoaderFlags", nt_hdrs.GetLoaderFlags(), 2);
  DWORD const num_dirs = nt_hdrs.GetNumberOfRvaAndSizes();
  WriteNamedHex(out, L"NumberOfRvaAndSizes", num_dirs, 2);
  DWORD const num_dirs_clamped = nt_hdrs.GetNumberOfRvaAndSizesClamped();
  WriteNamedHex(out, L"NumberOfRvaAndSizes (Clamped)", num_dirs_clamped, 2);
  if (num_dirs > num_dirs_clamped)
  {
    WriteNormal(
      out, L"WARNING! Detected an invalid number of data directories.", 2);
    WarnForCurrentFile(WarningType::kSuspicious);
  }
  for (DWORD i = 0; i < num_dirs_clamped; ++i)
  {
    auto const data_dir_va = nt_hdrs.GetDataDirectoryVirtualAddress(
      static_cast<hadesmem::PeDataDir>(i));
    auto const data_dir_size =
      nt_hdrs.GetDataDirectorySize(static_cast<hadesmem::PeDataDir>(i));
    auto const data_dir_name = GetDataDirName(i);
    WriteNamedHexSuffix(
      out, L"Data Directory VA", data_dir_va, data_dir_name, 2);
    WriteNamedHexSuffix(
      out, L"Data Directory Size", data_dir_size, data_dir_name, 2);
    if (data_dir_va && !IsSupportedDataDir(i))
    {
      WriteNormal(out,
                  L"WARNING! " + data_dir_name +
                    L" data directory is unsupported.",
                  2);
      // TODO: Re-enable this once we support all of the more common data dirs.
      // WarnForCurrentFile(WarningType::kUnsupported);
    }
    // Used by packers etc for scratch storage.
    if (static_cast<hadesmem::PeDataDir>(i) == hadesmem::PeDataDir::Reserved &&
        (data_dir_va || data_dir_size))
    {
      WriteNormal(
        out, L"WARNING! Detected usage of reserved data directory.", 2);
      WarnForCurrentFile(WarningType::kSuspicious);
    }
  }
}
}

void DumpHeaders(hadesmem::Process const& process,
                 hadesmem::PeFile const& pe_file)
{
  DumpDosHeader(process, pe_file);

  DumpNtHeaders(process, pe_file);
}
