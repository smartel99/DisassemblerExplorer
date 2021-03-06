﻿/******************************************************************************
 * @file ObjdumpConfigUI
 * @author Samuel Martel
 * @date 2020/07/27
 * @brief
 ******************************************************************************
 * Copyright (C) 2020  Samuel Martel - Pascal-Emmanuel Lachance
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *****************************************************************************/

#include "ObjdumpConfigUI.h"

#include "text/Text.h"

#include <ImGui/imgui.h>

/*****************************************************************************/
/* SECTION: FileTypeHandler                                                  */
/*****************************************************************************/
const char*                            FileTypeHandler::m_hint = TEXT_HINT_FILE_TYPE_HANDLER;
const std::map<FileTypes, const char*> FileTypeHandler::m_types =
  {{FileTypes::All, TEXT_FILE_TYPE_HANDLER_TYPE_ALL},
   {FileTypes::Elf, TEXT_FILE_TYPE_HANDLER_TYPE_ELF}};

void FileTypeHandler::Process(ObjdumpConfig& config)
{
    constexpr const char* label   = TEXT_FILE_TYPE_HANDLER_INPUT_LABEL;
    const char*           preview = m_types.at(config.fileType);

    ImGui::BeginGroup();
    if (ImGui::BeginCombo(label, preview))
    {
        for (const auto& type : m_types)
        {
            if (ImGui::Selectable(type.second))
            {
                config.fileType = type.first;
            }
        }
        ImGui::EndCombo();
    }
    ImGui::EndGroup();
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_hint);
    }
}

/************************************************************************/
/* SECTION: ProjectSettingsHandler                                      */
/************************************************************************/
const char* ProjectSettingsHandler::m_shouldSaveHint =
  TEXT_PROJECT_SETTINGS_HANDLER_SHOULD_SAVE_HINT;
const char* ProjectSettingsHandler::m_prjPathHint = TEXT_PROJECT_SETTINGS_HANDLER_PRJ_PATH_HINT;
const char* ProjectSettingsHandler::m_architectureHint = TEXT_PROJECT_SETTINGS_HANDLER_ARCH_HINT;
const char* ProjectSettingsHandler::m_filePathHint = TEXT_PROJECT_SETTINGS_HANDLER_FILE_PATH_HINT;
const std::map<ArchitectureEnum_t, const char*> ProjectSettingsHandler::m_architectures =
  {{ArchitectureEnum_t::Architecture_Auto, TEXT_ARCHITECTURE_AUTO},
   {ArchitectureEnum_t::Architecture_Arm, TEXT_ARCHITECTURE_ARM},
   {ArchitectureEnum_t::Architecture_Avr, TEXT_ARCHITECTURE_AVR},
   {ArchitectureEnum_t::Architecture_PowerPc, TEXT_ARCHITECTURE_POWER_PC},
   {ArchitectureEnum_t::Architecture_RiscV, TEXT_ARCHITECTURE_RISCV},
   {ArchitectureEnum_t::Architecture_x86, TEXT_ARCHITECTURE_X86},
   {ArchitectureEnum_t::Architecture_x86_64, TEXT_ARCHITECTURE_X86_64}};

void ProjectSettingsHandler::Process(ObjdumpConfig& config)
{
    constexpr const char* shouldSaveLabel    = TEXT_PROJECT_SETTINGS_HANDLER_SHOULD_SAVE_LABEL;
    constexpr const char* prjPathLabel       = TEXT_PROJECT_SETTINGS_HANDLER_PRJ_PATH_LABEL;
    constexpr const char* archLabel          = TEXT_PROJECT_SETTINGS_HANDLER_ARCH_LABEL;
    constexpr const char* filePathLabel      = TEXT_PROJECT_SETTINGS_HANDLER_FILE_PATH_LABEL;
    constexpr const char* openPrjButtonLabel = TEXT_PROJECT_SETTINGS_HANDLER_OPEN_PRJ_BUTTON_LABEL;
    constexpr const char* openBinButtonLabel = TEXT_PROJECT_SETTINGS_HANDLER_OPEN_BIN_BUTTON_LABEL;

    ImGui::Checkbox(shouldSaveLabel, &config.shouldSavePrj);
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_shouldSaveHint);
    }

    ImGui::BeginGroup();
    if (config.shouldSavePrj)
    {
        ImGui::Text(prjPathLabel, config.pathOfPrj.c_str());
        ImGui::SameLine();
        if (ImGui::Button(openPrjButtonLabel))
        {
            config.pathOfPrj = File::OpenFile(FileTypes::All, FileMode::Save);
        }
    }
    else
    {
        ImGui::TextDisabled(prjPathLabel, config.pathOfPrj.c_str());
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                              ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                              ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
        ImGui::Button(openPrjButtonLabel);
        ImGui::PopStyleColor(3);
    }
    ImGui::EndGroup();
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_prjPathHint);
    }

    //     ImGui::BeginGroup();
    //     const char* preview = m_architectures.at(config.disassemblerOptions.architecture);
    //     if (ImGui::BeginCombo(archLabel, preview))
    //     {
    //         for (const auto& type : m_architectures)
    //         {
    //             if (ImGui::Selectable(type.second))
    //             {
    //                 config.disassemblerOptions.architecture = type.first;
    //             }
    //         }
    //         ImGui::EndCombo();
    //     }
    //     ImGui::EndGroup();
    //     if (ImGui::IsItemHovered())
    //     {
    //         ImGui::SetTooltip(m_architectureHint);
    //     }

    ImGui::BeginGroup();
    ImGui::Text(filePathLabel, config.pathOfBin.c_str());
    ImGui::SameLine();
    if (ImGui::Button(openBinButtonLabel))
    {
        config.pathOfBin = File::OpenFile(FileTypes::All);
    }
    ImGui::EndGroup();
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_filePathHint);
    }
}

/************************************************************************/
/* SECTION: ShowArchiveHeadersHandler                                   */
/************************************************************************/
const char* ShowArchiveHeadersHandler::m_showArchiveHeadersHint = TEXT_SHOW_ARCHIVE_HEADERS_HINT;
const char* ShowArchiveHeadersHandler::m_objdumpFlag            = TEXT_SHOW_ARCHIVE_HEADERS_FLAG;
bool        ShowArchiveHeadersHandler::m_active                 = false;

void ShowArchiveHeadersHandler::Process(ObjdumpConfig& config)
{
    constexpr const char* label = TEXT_SHOW_ARCHIVE_HEADERS_LABEL;
    ImGui::Checkbox(label, &config.showArchiveHeader);
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_showArchiveHeadersHint);
    }

    m_active = config.showArchiveHeader;
}

std::ostream& ShowArchiveHeadersHandler::ToString(std::ostream& os)
{
    if (m_active)
    {
        os << " " << m_objdumpFlag;
    }

    return os;
}

/************************************************************************/
/* SECTION: AdjustVmaOffsetHandler                                      */
/************************************************************************/
const char* AdjustVmaOffsetHandler::m_hint        = TEXT_ADJUST_VMA_OFFSET_HINT;
const char* AdjustVmaOffsetHandler::m_objdumpFlag = TEXT_ADJUST_VMA_OFFSET_FLAG;
int         AdjustVmaOffsetHandler::m_offset      = 0;


void AdjustVmaOffsetHandler::Process(ObjdumpConfig& config)
{
    constexpr const char* label = TEXT_ADJUST_VMA_OFFSET_LABEL;
    ImGui::InputInt(label,
                    &config.adjustVmaOffset,
                    4,
                    0x0100,
                    ImGuiInputTextFlags_CharsHexadecimal);
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_hint);
    }
    m_offset = config.adjustVmaOffset;
}

std::ostream& AdjustVmaOffsetHandler::ToString(std::ostream& os)
{
    if (m_offset != 0)
    {
        os << " " << m_objdumpFlag << (unsigned int)m_offset;
    }

    return os;
}

/************************************************************************/
/* SECTION: DemangleHandler                                             */
/************************************************************************/
const char* DemangleHandler::m_hint       = TEXT_DEMANGLE_HINT;
const char* DemangleHandler::m_objdumFlag = TEXT_DEMANGLE_FLAG;
bool        DemangleHandler::m_active     = false;

void DemangleHandler::Process(ObjdumpConfig& config)
{
    constexpr const char* label = TEXT_DEMANGLE_LABEL;

    ImGui::Checkbox(label, &config.demangle);
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_hint);
    }
    m_active = config.demangle;
}

std::ostream& DemangleHandler::ToString(std::ostream& os)
{
    if (m_active)
    {
        os << " " << m_objdumFlag;
    }

    return os;
}

/************************************************************************/
/* SECTION: ShowDebugInfoHandler                                        */
/************************************************************************/
const char* ShowDebugInfoHandler::m_hint            = TEXT_SHOW_DEBUG_INFO_HINT;
const char* ShowDebugInfoHandler::m_ctagHint        = TEXT_SHOW_DEBUG_INFO_CTAG_HINT;
const char* ShowDebugInfoHandler::m_objdumpFlag     = TEXT_SHOW_DEBUG_INFO_FLAG;
const char* ShowDebugInfoHandler::m_objdumpFlagCtag = TEXT_SHOW_DEBUG_INFO_CTAG_FLAG;
bool        ShowDebugInfoHandler::m_debActive       = false;
bool        ShowDebugInfoHandler::m_debCtagActive   = false;

void ShowDebugInfoHandler::Process(ObjdumpConfig& config)
{
    constexpr const char* label     = TEXT_SHOW_DEBUG_INFO_LABEL;
    constexpr const char* ctagLabel = TEXT_SHOW_DEBUG_INFO_CTAG_LABEL;

    ImGui::Checkbox(label, &config.showDebuggingInfo);
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_hint);
    }
    ImGui::Checkbox(ctagLabel, &config.showDebuggingInfoForCtags);
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_ctagHint);
    }
    m_debActive     = config.showDebuggingInfo;
    m_debCtagActive = config.showDebuggingInfoForCtags;
}

std::ostream& ShowDebugInfoHandler::ToString(std::ostream& os)
{
    if (m_debActive)
    {
        os << " " << m_objdumpFlag;
    }
    if (m_debCtagActive)
    {
        os << " " << m_objdumpFlagCtag;
    }

    return os;
}

/************************************************************************/
/* SECTION: DisassembleHandler                                          */
/************************************************************************/
const char* DisassembleHandler::m_disassembleHint    = TEXT_DISASSEMBLE_HINT;
const char* DisassembleHandler::m_disassembleAllHint = TEXT_DISASSEMBLE_ALL_HINT;
const char* DisassembleHandler::m_disassembleFlag    = TEXT_DISASSEMBLE_FLAG;
const char* DisassembleHandler::m_disassembleAllFlag = TEXT_DISASSEMBLE_ALL_FLAG;
bool        DisassembleHandler::m_disActive          = false;
bool        DisassembleHandler::m_disAllActive       = false;

void DisassembleHandler::Process(ObjdumpConfig& config)
{
    constexpr const char* disLabel    = TEXT_DISASSEMBLE_LABEL;
    constexpr const char* disAllLabel = TEXT_DISASSEMBLE_ALL_LABEL;

    ImGui::Checkbox(disLabel, &config.disassemble);
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_disassembleHint);
    }

    ImGui::Checkbox(disAllLabel, &config.disassembleAll);
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_disassembleAllHint);
    }
    m_disActive    = config.disassemble;
    m_disAllActive = config.disassembleAll;
}

std::ostream& DisassembleHandler::ToString(std::ostream& os)
{
    if (m_disActive)
    {
        os << " " << m_disassembleFlag;
    }
    if (m_disAllActive)
    {
        os << " " << m_disassembleAllFlag;
    }
    return os;
}

/************************************************************************/
/* SECTION: PrefixAddressesHandler                                      */
/************************************************************************/
const char* PrefixAddressesHandler::m_hint   = TEXT_PREFIX_ADDRESSES_HINT;
const char* PrefixAddressesHandler::m_flag   = TEXT_PREFIX_ADDRESSES_FLAG;
bool        PrefixAddressesHandler::m_active = false;


void PrefixAddressesHandler::Process(ObjdumpConfig& config)
{
    constexpr const char* label = TEXT_PREFIX_ADDRESSES_LABEL;

    ImGui::Checkbox(label, &config.prefixAddresses);
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_hint);
    }
    m_active = config.prefixAddresses;
}

std::ostream& PrefixAddressesHandler::ToString(std::ostream& os)
{
    if (m_active == false)
    {
        return os;
    }
    else
    {
        os << " " << m_flag;
        return os;
    }
}

/************************************************************************/
/* SECTION: EndiannessHandler                                           */
/************************************************************************/
const char*                             EndiannessHandler::m_hint  = TEXT_ENDIANNESS_HINT;
std::map<EndiannessEnum_t, const char*> EndiannessHandler::m_flags = {{Endianness_Auto, ""},
                                                                      {Endianness_Big,
                                                                       TEXT_ENDIANNESS_FLAG_BIG},
                                                                      {Endianness_Small,
                                                                       TEXT_ENDIANNESS_FLAG_SMALL}};
std::map<EndiannessEnum_t, const char*> EndiannessHandler::m_labels =
  {{Endianness_Auto, TEXT_ENDIANNESS_LABELS_AUTO},
   {Endianness_Big, TEXT_ENDIANNESS_LABELS_BIG},
   {Endianness_Small, TEXT_ENDIANNESS_LABELS_SMALL}};
;
EndiannessEnum_t EndiannessHandler::m_currentSelection;

void EndiannessHandler::Process(ObjdumpConfig& config)
{
    constexpr const char* label = TEXT_ENDIANNESS_LABEL;

    ImGui::BeginGroup();
    if (ImGui::BeginCombo(label, m_labels[config.endianness]))
    {
        for (const auto& l : m_labels)
        {
            if (ImGui::Selectable(l.second))
            {
                config.endianness = l.first;
            }
        }
        ImGui::EndCombo();
    }
    ImGui::EndGroup();
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_hint);
    }
    m_currentSelection = config.endianness;
}

std::ostream& EndiannessHandler::ToString(std::ostream& os)
{
    if (m_currentSelection != Endianness_Auto)
    {
        os << " " << m_flags[m_currentSelection];
    }
    return os;
}

/************************************************************************/
/* SECTION: ShowFileOffsetHandler                                       */
/************************************************************************/
const char* ShowFileOffsetHandler::m_hint   = TEXT_SHOW_FILE_OFFSET_HINT;
const char* ShowFileOffsetHandler::m_flag   = TEXT_SHOW_FILE_OFFSET_FLAG;
bool        ShowFileOffsetHandler::m_active = false;

void ShowFileOffsetHandler::Process(ObjdumpConfig& config)
{
    constexpr const char* label = TEXT_SHOW_FILE_OFFSET_LABEL;
    ImGui::Checkbox(label, &config.showFileOffset);
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_hint);
    }
    m_active = config.showFileOffset;
}

std::ostream& ShowFileOffsetHandler::ToString(std::ostream& os)
{
    if (m_active)
    {
        os << " " << m_flag;
    }
    return os;
}

/************************************************************************/
/* SECTION: ShowFileStartContextHandler                                 */
/************************************************************************/
const char* ShowFileStartContextHandler::m_hint   = TEXT_SHOW_FILE_START_CONTEXT_HINT;
const char* ShowFileStartContextHandler::m_flag   = TEXT_SHOW_FILE_START_CONTEXT_FLAG;
bool        ShowFileStartContextHandler::m_active = false;

void ShowFileStartContextHandler::Process(ObjdumpConfig& config)
{
    constexpr const char* label = TEXT_SHOW_FILE_START_CONTEXT_LABEL;
    ImGui::Checkbox(label, &config.showFileStartContext);
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_hint);
    }
    m_active = config.showFileStartContext;
}

std::ostream& ShowFileStartContextHandler::ToString(std::ostream& os)
{
    if (m_active)
    {
        os << " " << m_flag;
    }
    return os;
}

/************************************************************************/
/* SECTION: ShowInfoFromHeaderHandler                                   */
/************************************************************************/
const char* ShowInfoFromHeaderHandler::m_hint   = TEXT_SHOW_INFO_FROM_HEADER_HINT;
const char* ShowInfoFromHeaderHandler::m_flag   = TEXT_SHOW_INFO_FROM_HEADER_FLAG;
bool        ShowInfoFromHeaderHandler::m_active = false;

void ShowInfoFromHeaderHandler::Process(ObjdumpConfig& config)
{
    constexpr const char* label = TEXT_SHOW_INFO_FROM_HEADER_LABEL;
    ImGui::Checkbox(label, &config.showInfoFromHeader);
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_hint);
    }
    m_active = config.showInfoFromHeader;
}

std::ostream& ShowInfoFromHeaderHandler::ToString(std::ostream& os)
{
    if (m_active)
    {
        os << " " << m_flag;
    }
    return os;
}

/************************************************************************/
/* SECTION: ShowInfoFromSectionHandler                                  */
/************************************************************************/
const char* ShowInfoFromSectionHandler::m_hint   = TEXT_SHOW_INFO_FROM_SECTION_HINT;
const char* ShowInfoFromSectionHandler::m_flag   = TEXT_SHOW_INFO_FROM_SECTION_FLAG;
bool        ShowInfoFromSectionHandler::m_active = false;

void ShowInfoFromSectionHandler::Process(ObjdumpConfig& config)
{
    constexpr const char* label = TEXT_SHOW_INFO_FROM_SECTION_LABEL;
    ImGui::Checkbox(label, &config.showInfoFromHeader);
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_hint);
    }
    m_active = config.showInfoFromHeader;
}

std::ostream& ShowInfoFromSectionHandler::ToString(std::ostream& os)
{
    if (m_active)
    {
        os << " " << m_flag;
    }
    return os;
}

/************************************************************************/
/* SECTION: DisassemblerOptionHandler                                   */
/************************************************************************/
const char*        DisassemblerOptionHandler::m_hint = TEXT_DISASSEMBLER_OPTION_HINT;
ArchitectureEnum_t DisassemblerOptionHandler::m_arch = Architecture_Auto;
std::map<ArchitectureEnum_t, const char*> DisassemblerOptionHandler::m_archLabels =
  {{Architecture_Auto, TEXT_ARCHITECTURE_AUTO},
   {Architecture_Arm, TEXT_ARCHITECTURE_ARM},
   {Architecture_Avr, TEXT_ARCHITECTURE_AVR},
   {Architecture_x86, TEXT_ARCHITECTURE_X86},
   {Architecture_x86_64, TEXT_ARCHITECTURE_X86_64},
   {Architecture_PowerPc, TEXT_ARCHITECTURE_POWER_PC},
   {Architecture_RiscV, TEXT_ARCHITECTURE_RISCV},
   {Architecture_Aarch64, TEXT_ARCHITECTURE_AARCH64},
   {Architecture_Mips, TEXT_ARCHITECTURE_MIPS}};


void DisassemblerOptionHandler::Process(ObjdumpConfig& config)
{
    constexpr const char* label = TEXT_DISASSEMBLER_OPTION_LABEL;

    ImGui::BeginGroup();
    if (ImGui::BeginCombo(label, m_archLabels[config.disassemblerOptions.architecture]))
    {
        for (const auto& l : m_archLabels)
        {
            if (ImGui::Selectable(l.second))
            {
                config.disassemblerOptions.architecture = l.first;
            }
        }
        ImGui::EndCombo();
    }
    m_arch = config.disassemblerOptions.architecture;
    ImGui::EndGroup();
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_hint);
    }

    if (m_arch != ArchitectureEnum_t::Architecture_Auto)
    {
        if (ImGui::TreeNode("##Disassembler options"))
        {
            switch (m_arch)
            {
                case Architecture_Aarch64:
                    ArcDisassemblerOptionsHandler::Process(config);
                    break;
                case Architecture_Arm:
                    ArmDisassemblerOptionsHandler::Process(config);
                    break;
                case Architecture_x86:
                case Architecture_x86_64:
                    x86DisassemblerOptionsHandler::Process(config);
                    break;
                case Architecture_PowerPc:
                    PowerPcDisassemberOptionsHandler::Process(config);
                    break;
                case Architecture_Mips:
                    MipsDisassemlerOptionsHandler::Process(config);
                    break;
                case Architecture_Auto:
                case Architecture_Avr:
                default:
                    break;
            }
            ImGui::TreePop();
        }
    }
}

std::ostream& DisassemblerOptionHandler::ToString(std::ostream& os)
{
    switch (m_arch)
    {
        case Architecture_Aarch64:
            return ArcDisassemblerOptionsHandler::ToString(os);
        case Architecture_Arm:
            return ArmDisassemblerOptionsHandler::ToString(os);
        case Architecture_x86:
        case Architecture_x86_64:
            return x86DisassemblerOptionsHandler::ToString(os);
        case Architecture_PowerPc:
            return PowerPcDisassemberOptionsHandler::ToString(os);
        case Architecture_Mips:
            return MipsDisassemlerOptionsHandler::ToString(os);
        case Architecture_Auto:
        case Architecture_Avr:
        default:
            return os;
    }
}

/************************************************************************/
/* SECTION: ArmDisassemblerOptionsHandler                               */
/************************************************************************/
const char* ArmDisassemblerOptionsHandler::m_registerNameSetHint =
  TEXT_ARM_DISASSEMBLER_OPTION_REG_NAME_HINT;
const char* ArmDisassemblerOptionsHandler::m_forceThumbHint =
  TEXT_ARM_DISASSEMBLER_OPTION_FORCE_THUMB_HINT;
const char* ArmDisassemblerOptionsHandler::m_cpuIsaHint = TEXT_ARM_DISASSEMBLER_OPTION_CPU_ISA_HINT;

const char* ArmDisassemblerOptionsHandler::m_registerNameSetFlag = TEXT_ARM_REG_SET_NAME_AUTO_FLAG;
const char* ArmDisassemblerOptionsHandler::m_forceThumbFlag =
  TEXT_ARM_DISASSEMBLER_OPTION_FORCE_THUMB_FLAG;
const char* ArmDisassemblerOptionsHandler::m_cpuIsaFlag = TEXT_ARM_DISASSEMBLER_OPTION_CPU_ISA_FLAG;

ArmRegisterNameSetEnum_t ArmDisassemblerOptionsHandler::m_registerNameSet = ARM_Auto;
bool                     ArmDisassemblerOptionsHandler::m_forceThumb      = false;
std::string              ArmDisassemblerOptionsHandler::m_cpuIsa          = std::string(64, '\0');

std::map<ArmRegisterNameSetEnum_t, const char*> ArmDisassemblerOptionsHandler::m_regNameSetLabels =
  {{ARM_Auto, TEXT_ARM_REG_SET_NAME_AUTO},
   {ARM_StdSet, TEXT_ARM_REG_SET_NAME_STD_SET},
   {ARM_ApcsSet, TEXT_ARM_REG_SET_NAME_APCS_SET},
   {ARM_ApcsSpecialSet, TEXT_ARM_REG_SET_NAME_APCS_SPECIAL_SET},
   {ARM_Raw, TEXT_ARM_REG_SET_NAME_RAW}};

std::map<ArmRegisterNameSetEnum_t, const char*> ArmDisassemblerOptionsHandler::m_regNameSetFlags =
  {{ARM_Auto, TEXT_ARM_REG_SET_NAME_AUTO_FLAG},
   {ARM_StdSet, TEXT_ARM_REG_SET_NAME_STD_SET_FLAG},
   {ARM_ApcsSet, TEXT_ARM_REG_SET_NAME_APCS_SET_FLAG},
   {ARM_ApcsSpecialSet, TEXT_ARM_REG_SET_NAME_APCS_SPECIAL_SET_FLAG},
   {ARM_Raw, TEXT_ARM_REG_SET_NAME_RAW_FLAG}};

void ArmDisassemblerOptionsHandler::Process(ObjdumpConfig& config)
{
    constexpr const char* regNameLabel    = TEXT_ARM_DISASSEBLER_OPTION_REG_NAME_LABEL;
    constexpr const char* forceThumbLabel = TEXT_ARM_DISASSEBLER_OPTION_FORCE_THUMB_LABEL;
    constexpr const char* cpuIsaLabel     = TEXT_ARM_DISASSEBLER_OPTION_CPU_ISA_LABEL;

    ImGui::BeginGroup();
    if (ImGui::BeginCombo(regNameLabel,
                          m_regNameSetLabels[config.disassemblerOptions.armDisassemblerOptions
                                               .armRegisterNameSet]))
    {
        for (const auto& l : m_regNameSetLabels)
        {
            if (ImGui::Selectable(l.second))
            {
                config.disassemblerOptions.armDisassemblerOptions.armRegisterNameSet = l.first;
            }
        }
        ImGui::EndCombo();
    }
    m_registerNameSet = config.disassemblerOptions.armDisassemblerOptions.armRegisterNameSet;
    ImGui::EndGroup();
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_registerNameSetHint);
    }

    ImGui::Checkbox(forceThumbLabel, &config.disassemblerOptions.armDisassemblerOptions.forceThumb);
    m_forceThumb = config.disassemblerOptions.armDisassemblerOptions.forceThumb;
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_forceThumbHint);
    }

    char*  buf    = config.disassemblerOptions.armDisassemblerOptions.cpuIsa.data();
    size_t bufLen = config.disassemblerOptions.armDisassemblerOptions.cpuIsa.capacity();
    ImGui::InputText(cpuIsaLabel, buf, bufLen);
    config.disassemblerOptions.armDisassemblerOptions.cpuIsa =
      config.disassemblerOptions.armDisassemblerOptions.cpuIsa.c_str();
    m_cpuIsa = config.disassemblerOptions.armDisassemblerOptions.cpuIsa;
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_cpuIsaHint);
    }
}

std::ostream& ArmDisassemblerOptionsHandler::ToString(std::ostream& os)
{
    os << " " << m_regNameSetFlags[m_registerNameSet];
    if (m_forceThumb)
    {
        os << " " << m_forceThumbFlag;
    }
    if (m_cpuIsa.empty() == false && m_cpuIsa[0] != '\0')
    {
        os << " " << m_cpuIsaFlag << m_cpuIsa;
    }

    return os;
}

/************************************************************************/
/* SECTION: Aarch64DisassemblerOptionsHandler                           */
/************************************************************************/
const char* Aarch64DisassemblerOptionsHandler::m_disassembleIntoGenericInstructionHint =
  TEXT_AARCH64_DIS_OPTION_DIS_INTO_GENERIC_INST_HINT;
const char* Aarch64DisassemblerOptionsHandler::m_disassembleIntoGenericInstructionFlag =
  TEXT_AARCH64_DIS_OPTION_DIS_INTO_GENERIC_INST_FLAG;
bool Aarch64DisassemblerOptionsHandler::m_disassembleIntoGenericInstruction = false;

void Aarch64DisassemblerOptionsHandler::Process(ObjdumpConfig& config)
{
    constexpr const char* label = TEXT_AARCH64_DIS_OPTION_DIS_INTO_GENERIC_INST_LABEL;

    ImGui::Checkbox(label,
                    &config.disassemblerOptions.aArch64DisassemblerOptions
                       .disassembleIntoGenericInstruction);

    m_disassembleIntoGenericInstruction =
      config.disassemblerOptions.aArch64DisassemblerOptions.disassembleIntoGenericInstruction;
}

std::ostream& Aarch64DisassemblerOptionsHandler::ToString(std::ostream& os)
{
    if (m_disassembleIntoGenericInstruction)
    {
        os << " " << m_disassembleIntoGenericInstructionFlag;
    }

    return os;
}

/************************************************************************/
/* SECTION: x86DisassemblerOptionsHandler                               */
/************************************************************************/
const char* x86DisassemblerOptionsHandler::m_architectureHint = TEXT_X86_OPTIONS_ARCHITECTURE_HINT;
const char* x86DisassemblerOptionsHandler::m_syntaxHint       = TEXT_X86_OPTIONS_SYNTAX_HINT;
const char* x86DisassemblerOptionsHandler::m_isaHint          = TEXT_X86_OPTIONS_ISA_HINT;
const char* x86DisassemblerOptionsHandler::m_addrSizeHint     = TEXT_X86_OPTIONS_ADDR_SIZE_HINT;
const char* x86DisassemblerOptionsHandler::m_dataSizeHint     = TEXT_X86_OPTIONS_DATA_SIZE_HINT;
const char* x86DisassemblerOptionsHandler::m_displaySuffixHint =
  TEXT_X86_OPTIONS_DISPLAY_SUFFIX_HINT;

const char* x86DisassemblerOptionsHandler::m_displaySuffixFlag =
  TEXT_X86_OPTIONS_DISPLAY_SUFFIX_FLAG;

x86DisassemblyArchitectureEnum_t x86DisassemblerOptionsHandler::m_architecture  = x86Arch_Auto;
x86SyntaxModeEnum_t              x86DisassemblerOptionsHandler::m_syntax        = x86Syntax_Auto;
x86IsaEnum_t                     x86DisassemblerOptionsHandler::m_isa           = x86Isa_Auto;
x86AddrSizeEnum_t                x86DisassemblerOptionsHandler::m_addrSize      = x86AddrSize_Auto;
x86DataSizeEnum_t                x86DisassemblerOptionsHandler::m_dataSize      = x86DataSize_Auto;
bool                             x86DisassemblerOptionsHandler::m_displaySuffix = false;

std::map<x86DisassemblyArchitectureEnum_t, const char*>
                                           x86DisassemblerOptionsHandler::m_archLabels = {{x86Arch_Auto, TEXT_X86_ARCH_AUTO_LABEL},
                                                 {x86Arch_i386, TEXT_X86_ARCH_I386_LABEL},
                                                 {x86Arch_i8086, TEXT_X86_ARCH_I8086_LABEL},
                                                 {x86Arch_x86_64, TEXT_X86_ARCH_X86_64_LABEL}};
std::map<x86SyntaxModeEnum_t, const char*> x86DisassemblerOptionsHandler::m_syntaxLabels =
  {{x86Syntax_Auto, TEXT_X86_SYNTAX_AUTOMATIC_LABEL},
   {x86Syntax_intel, TEXT_X86_SYNTAX_INTEL_LABEL},
   {x86Syntax_att, TEXT_X86_SYNTAX_ATT_LABEL}};
std::map<x86IsaEnum_t, const char*>
                                         x86DisassemblerOptionsHandler::m_isaLabels{{x86Isa_Auto, TEXT_X86_ISA_AUTOMATIC_LABEL},
                                             {x86Isa_amd64, TEXT_X86_ISA_AMD64_LABEL},
                                             {x86Isa_intel64, TEXT_X86_ISA_INTEL64_LABEL}};
std::map<x86AddrSizeEnum_t, const char*> x86DisassemblerOptionsHandler::m_addrSizeLabels =
  {{x86AddrSize_Auto, TEXT_X86_ADDR_SIZE_AUTO_LABEL},
   {x86AddrSize_64, TEXT_X86_ADDR_SIZE_64_LABEL},
   {x86AddrSize_32, TEXT_X86_ADDR_SIZE_32_LABEL},
   {x86AddrSize_16, TEXT_X86_ADDR_SIZE_16_LABEL}};
std::map<x86DataSizeEnum_t, const char*> x86DisassemblerOptionsHandler::m_dataSizeLabels =
  {{x86DataSize_Auto, TEXT_X86_DATA_SIZE_AUTO_LABEL},
   {x86DataSize_32, TEXT_X86_DATA_SIZE_32_LABEL},
   {x86DataSize_16, TEXT_X86_DATA_SIZE_16_LABEL}};

std::map<x86DisassemblyArchitectureEnum_t, const char*> x86DisassemblerOptionsHandler::m_archFlags =
  {{x86Arch_Auto, TEXT_X86_ARCH_AUTO_FLAG},
   {x86Arch_i386, TEXT_X86_ARCH_I386_FLAG},
   {x86Arch_i8086, TEXT_X86_ARCH_I8086_FLAG},
   {x86Arch_x86_64, TEXT_X86_ARCH_X86_64_FLAG}};
std::map<x86SyntaxModeEnum_t, const char*> x86DisassemblerOptionsHandler::m_syntaxFlags =
  {{x86Syntax_Auto, TEXT_X86_SYNTAX_AUTOMATIC_FLAG},
   {x86Syntax_intel, TEXT_X86_SYNTAX_INTEL_FLAG},
   {x86Syntax_att, TEXT_X86_SYNTAX_ATT_FLAG}};
std::map<x86IsaEnum_t, const char*>
                                         x86DisassemblerOptionsHandler::m_isaFlags{{x86Isa_Auto, TEXT_X86_ISA_AUTOMATIC_FLAG},
                                            {x86Isa_amd64, TEXT_X86_ISA_AMD64_FLAG},
                                            {x86Isa_intel64, TEXT_X86_ISA_INTEL64_FLAG}};
std::map<x86AddrSizeEnum_t, const char*> x86DisassemblerOptionsHandler::m_addrSizeFlags =
  {{x86AddrSize_Auto, TEXT_X86_ADDR_SIZE_AUTO_FLAG},
   {x86AddrSize_64, TEXT_X86_ADDR_SIZE_64_FLAG},
   {x86AddrSize_32, TEXT_X86_ADDR_SIZE_32_FLAG},
   {x86AddrSize_16, TEXT_X86_ADDR_SIZE_16_FLAG}};
std::map<x86DataSizeEnum_t, const char*> x86DisassemblerOptionsHandler::m_dataSizeFlags =
  {{x86DataSize_Auto, TEXT_X86_DATA_SIZE_AUTO_FLAG},
   {x86DataSize_32, TEXT_X86_DATA_SIZE_32_FLAG},
   {x86DataSize_16, TEXT_X86_DATA_SIZE_16_FLAG}};

void x86DisassemblerOptionsHandler::Process(ObjdumpConfig& config)
{
    constexpr const char* archLabel          = TEXT_X86_OPTIONS_ARCHITECTURE_LABEL;
    constexpr const char* syntaxLabel        = TEXT_X86_OPTIONS_SYNTAX_LABEL;
    constexpr const char* isaLabel           = TEXT_X86_OPTIONS_ISA_LABEL;
    constexpr const char* addrSizeLabel      = TEXT_X86_OPTIONS_ADDR_SIZE_LABEL;
    constexpr const char* dataSizeLabel      = TEXT_X86_OPTIONS_DATA_SIZE_LABEL;
    constexpr const char* displaySuffixLabel = TEXT_X86_OPTIONS_DISPLAY_SUFFIX_LABEL;

    ImGui::BeginGroup();
    if (ImGui::BeginCombo(archLabel,
                          m_archLabels[config.disassemblerOptions.x86DisassemblerOptions
                                         .architecture]))
    {
        for (const auto& l : m_archLabels)
        {
            if (ImGui::Selectable(l.second))
            {
                config.disassemblerOptions.x86DisassemblerOptions.architecture = l.first;
            }
        }
        ImGui::EndCombo();
    }
    m_architecture = config.disassemblerOptions.x86DisassemblerOptions.architecture;
    ImGui::EndGroup();
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_architectureHint);
    }

    ImGui::BeginGroup();
    if (ImGui::BeginCombo(syntaxLabel,
                          m_syntaxLabels[config.disassemblerOptions.x86DisassemblerOptions.syntax]))
    {
        for (const auto& l : m_syntaxLabels)
        {
            if (ImGui::Selectable(l.second))
            {
                config.disassemblerOptions.x86DisassemblerOptions.syntax = l.first;
            }
        }
        ImGui::EndCombo();
    }
    m_syntax = config.disassemblerOptions.x86DisassemblerOptions.syntax;
    ImGui::EndGroup();
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_syntaxHint);
    }

    ImGui::BeginGroup();
    if (ImGui::BeginCombo(isaLabel,
                          m_isaLabels[config.disassemblerOptions.x86DisassemblerOptions.isa]))
    {
        for (const auto& l : m_isaLabels)
        {
            if (ImGui::Selectable(l.second))
            {
                config.disassemblerOptions.x86DisassemblerOptions.isa = l.first;
            }
        }
        ImGui::EndCombo();
    }
    m_isa = config.disassemblerOptions.x86DisassemblerOptions.isa;
    ImGui::EndGroup();
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_isaHint);
    }

    ImGui::BeginGroup();
    if (ImGui::BeginCombo(addrSizeLabel,
                          m_addrSizeLabels[config.disassemblerOptions.x86DisassemblerOptions
                                             .addrSize]))
    {
        for (const auto& l : m_addrSizeLabels)
        {
            if (ImGui::Selectable(l.second))
            {
                config.disassemblerOptions.x86DisassemblerOptions.addrSize = l.first;
            }
        }
        ImGui::EndCombo();
    }
    m_addrSize = config.disassemblerOptions.x86DisassemblerOptions.addrSize;
    ImGui::EndGroup();
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_addrSizeHint);
    }

    ImGui::BeginGroup();
    if (ImGui::BeginCombo(dataSizeLabel,
                          m_dataSizeLabels[config.disassemblerOptions.x86DisassemblerOptions
                                             .dataSize]))
    {
        for (const auto& l : m_dataSizeLabels)
        {
            if (ImGui::Selectable(l.second))
            {
                config.disassemblerOptions.x86DisassemblerOptions.dataSize = l.first;
            }
        }
        ImGui::EndCombo();
    }
    m_dataSize = config.disassemblerOptions.x86DisassemblerOptions.dataSize;
    ImGui::EndGroup();
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_dataSizeHint);
    }

    if (config.disassemblerOptions.x86DisassemblerOptions.syntax == x86Syntax_att)
    {
        ImGui::Checkbox(displaySuffixLabel,
                        &config.disassemblerOptions.x86DisassemblerOptions.displaySuffix);
        m_displaySuffix = config.disassemblerOptions.x86DisassemblerOptions.displaySuffix;
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip(m_displaySuffixHint);
        }
    }
}

std::ostream& x86DisassemblerOptionsHandler::ToString(std::ostream& os)
{
    if (m_architecture != x86DisassemblyArchitectureEnum_t::x86Arch_Auto)
    {
        os << " " << m_archFlags[m_architecture];
    }

    if (m_syntax != x86SyntaxModeEnum_t::x86Syntax_Auto)
    {
        os << " " << m_syntaxFlags[m_syntax];
    }

    if (m_isa != x86IsaEnum_t::x86Isa_Auto)
    {
        os << " " << m_isaFlags[m_isa];
    }

    if (m_addrSize != x86AddrSizeEnum_t::x86AddrSize_Auto)
    {
        os << " " << m_addrSizeFlags[m_addrSize];
    }

    if (m_dataSize != x86DataSizeEnum_t::x86DataSize_Auto)
    {
        os << " " << m_dataSizeFlags[m_dataSize];
    }

    if (m_displaySuffix)
    {
        os << " " << m_displaySuffixFlag;
    }

    return os;
}

/************************************************************************/
/* SECTION: PowerPcDisassemberOptionsHandler                            */
/************************************************************************/
const char* PowerPcDisassemberOptionsHandler::m_useRawHint = TEXT_PPC_OPTIONS_USE_RAW_HINT;
const char* PowerPcDisassemberOptionsHandler::m_cpuIsaHint = TEXT_PPC_OPTIONS_CPU_ISA_HINT;
const char* PowerPcDisassemberOptionsHandler::m_enable64BitsHint =
  TEXT_PPC_OPTIONS_ENABLE_64_BITS_HINT;
const char* PowerPcDisassemberOptionsHandler::m_useAltivecHint = TEXT_PPC_OPTIONS_USE_ALTIVEC_HINT;
const char* PowerPcDisassemberOptionsHandler::m_useAnyHint     = TEXT_PPC_OPTIONS_USE_ANY_HINT;
const char* PowerPcDisassemberOptionsHandler::m_useHtmHint     = TEXT_PPC_OPTIONS_USE_HTM_HINT;
const char* PowerPcDisassemberOptionsHandler::m_useVsxHint     = TEXT_PPC_OPTIONS_USE_VSX_HINT;
const char* PowerPcDisassemberOptionsHandler::m_useSpeHint     = TEXT_PPC_OPTIONS_USE_SPE_HINT;

const char* PowerPcDisassemberOptionsHandler::m_useRawFlag = TEXT_PPC_OPTIONS_USE_RAW_FLAG;
const char* PowerPcDisassemberOptionsHandler::m_enable64BitsFlag =
  TEXT_PPC_OPTIONS_ENABLE_64_BITS_FLAG;
const char* PowerPcDisassemberOptionsHandler::m_disable64BitsFlag =
  TEXT_PPC_OPTIONS_DISABLE_64_BITS_FLAG;
const char* PowerPcDisassemberOptionsHandler::m_useAltivecFlag = TEXT_PPC_OPTIONS_USE_ALTIVEC_FLAG;
const char* PowerPcDisassemberOptionsHandler::m_useAnyFlag     = TEXT_PPC_OPTIONS_USE_ANY_FLAG;
const char* PowerPcDisassemberOptionsHandler::m_useHtmFlag     = TEXT_PPC_OPTIONS_USE_HTM_FLAG;
const char* PowerPcDisassemberOptionsHandler::m_useVsxFlag     = TEXT_PPC_OPTIONS_USE_VSX_FLAG;
const char* PowerPcDisassemberOptionsHandler::m_useSpeFlag     = TEXT_PPC_OPTIONS_USE_SPE_FLAG;

bool         PowerPcDisassemberOptionsHandler::m_useRaw       = false;
PpcCpuEnum_t PowerPcDisassemberOptionsHandler::m_cpuIsa       = PpcCpuEnum_t::PpcCpu_Auto;
bool         PowerPcDisassemberOptionsHandler::m_enable64Bits = false;
bool         PowerPcDisassemberOptionsHandler::m_useAltivec   = false;
bool         PowerPcDisassemberOptionsHandler::m_useAny       = false;
bool         PowerPcDisassemberOptionsHandler::m_useHtm       = false;
bool         PowerPcDisassemberOptionsHandler::m_useVsx       = false;
bool         PowerPcDisassemberOptionsHandler::m_useSpe       = false;

std::map<PpcCpuEnum_t, const char*> PowerPcDisassemberOptionsHandler::m_cpuIsaLabels =
  {{PpcCpu_Auto, TEXT_PPC_CPU_LABEL_Auto},
   {PpcCpu_403, TEXT_PPC_CPU_LABEL_403},
   {PpcCpu_405, TEXT_PPC_CPU_LABEL_405},
   {PpcCpu_440, TEXT_PPC_CPU_LABEL_440},
   {PpcCpu_464, TEXT_PPC_CPU_LABEL_464},
   {PpcCpu_476, TEXT_PPC_CPU_LABEL_476},
   {PpcCpu_601, TEXT_PPC_CPU_LABEL_601},
   {PpcCpu_603, TEXT_PPC_CPU_LABEL_603},
   {PpcCpu_604, TEXT_PPC_CPU_LABEL_604},
   {PpcCpu_620, TEXT_PPC_CPU_LABEL_620},
   {PpcCpu_7400, TEXT_PPC_CPU_LABEL_7400},
   {PpcCpu_7410, TEXT_PPC_CPU_LABEL_7410},
   {PpcCpu_7450, TEXT_PPC_CPU_LABEL_7450},
   {PpcCpu_7455, TEXT_PPC_CPU_LABEL_7455},
   {PpcCpu_750cl, TEXT_PPC_CPU_LABEL_750cl},
   {PpcCpu_821, TEXT_PPC_CPU_LABEL_821},
   {PpcCpu_850, TEXT_PPC_CPU_LABEL_850},
   {PpcCpu_860, TEXT_PPC_CPU_LABEL_860},
   {PpcCpu_a2, TEXT_PPC_CPU_LABEL_a2},
   {PpcCpu_booke, TEXT_PPC_CPU_LABEL_booke},
   {PpcCpu_booke32, TEXT_PPC_CPU_LABEL_booke32},
   {PpcCpu_cell, TEXT_PPC_CPU_LABEL_cell},
   {PpcCpu_com, TEXT_PPC_CPU_LABEL_com},
   {PpcCpu_e200z4, TEXT_PPC_CPU_LABEL_e200z4},
   {PpcCpu_e300, TEXT_PPC_CPU_LABEL_e300},
   {PpcCpu_e500, TEXT_PPC_CPU_LABEL_e500},
   {PpcCpu_e500mc, TEXT_PPC_CPU_LABEL_e500mc},
   {PpcCpu_e500mc64, TEXT_PPC_CPU_LABEL_e500mc64},
   {PpcCpu_e500x2, TEXT_PPC_CPU_LABEL_e500x2},
   {PpcCpu_e5500, TEXT_PPC_CPU_LABEL_e5500},
   {PpcCpu_e6500, TEXT_PPC_CPU_LABEL_e6500},
   {PpcCpu_efs, TEXT_PPC_CPU_LABEL_efs},
   {PpcCpu_power4, TEXT_PPC_CPU_LABEL_power4},
   {PpcCpu_power5, TEXT_PPC_CPU_LABEL_power5},
   {PpcCpu_power6, TEXT_PPC_CPU_LABEL_power6},
   {PpcCpu_power7, TEXT_PPC_CPU_LABEL_power7},
   {PpcCpu_power8, TEXT_PPC_CPU_LABEL_power8},
   {PpcCpu_power9, TEXT_PPC_CPU_LABEL_power9},
   {PpcCpu_ppc, TEXT_PPC_CPU_LABEL_ppc},
   {PpcCpu_ppc32, TEXT_PPC_CPU_LABEL_ppc32},
   {PpcCpu_ppc64, TEXT_PPC_CPU_LABEL_ppc64},
   {PpcCpu_ppc64bridge, TEXT_PPC_CPU_LABEL_ppc64bridge},
   {PpcCpu_ppcps, TEXT_PPC_CPU_LABEL_ppcps},
   {PpcCpu_pwr, TEXT_PPC_CPU_LABEL_pwr},
   {PpcCpu_pwr2, TEXT_PPC_CPU_LABEL_pwr2},
   {PpcCpu_pwr4, TEXT_PPC_CPU_LABEL_pwr4},
   {PpcCpu_pwr5, TEXT_PPC_CPU_LABEL_pwr5},
   {PpcCpu_pwr5x, TEXT_PPC_CPU_LABEL_pwr5x},
   {PpcCpu_pwr6, TEXT_PPC_CPU_LABEL_pwr6},
   {PpcCpu_pwr7, TEXT_PPC_CPU_LABEL_pwr7},
   {PpcCpu_pwr8, TEXT_PPC_CPU_LABEL_pwr8},
   {PpcCpu_pwr9, TEXT_PPC_CPU_LABEL_pwr9},
   {PpcCpu_pwrx, TEXT_PPC_CPU_LABEL_pwrx},
   {PpcCpu_titan, TEXT_PPC_CPU_LABEL_titan},
   {PpcCpu_vle, TEXT_PPC_CPU_LABEL_vle}};

void PowerPcDisassemberOptionsHandler::Process(ObjdumpConfig& config)
{
    constexpr const char* useRawLabel       = TEXT_PPC_OPTIONS_USE_RAW_LABEL;
    constexpr const char* cpuIsaLabel       = TEXT_PPC_OPTIONS_CPU_ISA_LABEL;
    constexpr const char* enable64BitsLabel = TEXT_PPC_OPTIONS_ENABLE_64_BITS_LABEL;
    constexpr const char* useAltivecLabel   = TEXT_PPC_OPTIONS_USE_ALTIVEC_LABEL;
    constexpr const char* useAnyLabel       = TEXT_PPC_OPTIONS_USE_ANY_LABEL;
    constexpr const char* useHtmLabel       = TEXT_PPC_OPTIONS_USE_HTM_LABEL;
    constexpr const char* useVsxLabel       = TEXT_PPC_OPTIONS_USE_VSX_LABEL;
    constexpr const char* useSpeLabel       = TEXT_PPC_OPTIONS_USE_SPE_LABEL;

    ImGui::Checkbox(useRawLabel, &m_useRaw);
    config.disassemblerOptions.powerPcDisassemblerOptions.useRaw = m_useRaw;
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_useRawHint);
    }

    ImGui::BeginGroup();
    if (ImGui::BeginCombo(cpuIsaLabel,
                          m_cpuIsaLabels[config.disassemblerOptions.powerPcDisassemblerOptions
                                           .cpuIsa]))
    {
        for (const auto& l : m_cpuIsaLabels)
        {
            if (ImGui::Selectable(l.second))
            {
                config.disassemblerOptions.powerPcDisassemblerOptions.cpuIsa = l.first;
            }
        }
        ImGui::EndCombo();
    }
    m_cpuIsa = config.disassemblerOptions.powerPcDisassemblerOptions.cpuIsa;
    ImGui::EndGroup();
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_cpuIsaHint);
    }

    if (config.disassemblerOptions.powerPcDisassemblerOptions.cpuIsa != PpcCpuEnum_t::PpcCpu_Auto)
    {
        ImGui::Checkbox(enable64BitsLabel,
                        &config.disassemblerOptions.powerPcDisassemblerOptions.enable64Bits);
        m_enable64Bits = config.disassemblerOptions.powerPcDisassemblerOptions.enable64Bits;
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip(m_enable64BitsHint);
        }

        ImGui::Checkbox(useAltivecLabel,
                        &config.disassemblerOptions.powerPcDisassemblerOptions.useAltivec);
        m_useAltivec = config.disassemblerOptions.powerPcDisassemblerOptions.useAltivec;
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip(m_useAltivecHint);
        }

        ImGui::Checkbox(useAnyLabel, &config.disassemblerOptions.powerPcDisassemblerOptions.useAny);
        m_useAny = config.disassemblerOptions.powerPcDisassemblerOptions.useAny;
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip(m_useAnyHint);
        }

        ImGui::Checkbox(useHtmLabel, &config.disassemblerOptions.powerPcDisassemblerOptions.useHtm);
        m_useHtm = config.disassemblerOptions.powerPcDisassemblerOptions.useHtm;
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip(m_useHtmHint);
        }

        ImGui::Checkbox(useVsxLabel, &config.disassemblerOptions.powerPcDisassemblerOptions.useVsx);
        m_useVsx = config.disassemblerOptions.powerPcDisassemblerOptions.enable64Bits;
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip(m_useVsxHint);
        }

        ImGui::Checkbox(useSpeLabel, &config.disassemblerOptions.powerPcDisassemblerOptions.useSpe);
        m_useSpe = config.disassemblerOptions.powerPcDisassemblerOptions.useSpe;
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip(m_useSpeHint);
        }
    }
}

std::ostream& PowerPcDisassemberOptionsHandler::ToString(std::ostream& os)
{
    if (m_useRaw)
    {
        os << " " << m_useRawFlag;
    }
    if (m_cpuIsa != PpcCpuEnum_t::PpcCpu_Auto)
    {
        os << " -m " << m_cpuIsaLabels[m_cpuIsa];
    }

    os << " " << (m_enable64Bits ? m_enable64BitsFlag : m_disable64BitsFlag);

    if (m_useAltivec)
    {
        os << " " << m_useAltivecFlag;
    }
    if (m_useHtm)
    {
        os << " " << m_useHtmFlag;
    }
    if (m_useVsx)
    {
        os << " " << m_useVsxFlag;
    }
    if (m_useSpe)
    {
        os << " " << m_useSpeFlag;
    }

    return os;
}

/************************************************************************/
/* SECTION: MipsDisassemlerOptionsHandler                               */
/************************************************************************/
const char* MipsDisassemlerOptionsHandler::m_useRawMnemonicHint =
  TEXT_MIPS_OPTIONS_USE_RAW_MNEMONIC_HINT;
const char* MipsDisassemlerOptionsHandler::m_disassembleMsaHint =
  TEXT_MIPS_OPTIONS_DISASSEMBLE_MSA_HINT;
const char* MipsDisassemlerOptionsHandler::m_disassembleVirtHint =
  TEXT_MIPS_OPTIONS_DISASSEMBLE_VIRT_HINT;
const char* MipsDisassemlerOptionsHandler::m_disassembleXpaHint =
  TEXT_MIPS_OPTIONS_DISASSEMBLE_XPA_HINT;
const char* MipsDisassemlerOptionsHandler::m_gprNamesAbiHint = TEXT_MIPS_OPTIONS_GPR_NAMES_ABI_HINT;
const char* MipsDisassemlerOptionsHandler::m_fprNamesAbiHint = TEXT_MIPS_OPTIONS_FPR_NAMES_ABI_HINT;
const char* MipsDisassemlerOptionsHandler::m_cp0ArchNameHint = TEXT_MIPS_OPTIONS_CP0_ARCH_NAME_HINT;
const char* MipsDisassemlerOptionsHandler::m_hwrArchNameHint = TEXT_MIPS_OPTIONS_HWR_ARCH_NAME_HINT;
const char* MipsDisassemlerOptionsHandler::m_regNamesAbiHint = TEXT_MIPS_OPTIONS_REG_NAMES_ABI_HINT;
const char* MipsDisassemlerOptionsHandler::m_regArchNameHint = TEXT_MIPS_OPTIONS_REG_ARCH_NAME_HINT;

const char* MipsDisassemlerOptionsHandler::m_useRawMnemonicFlag =
  TEXT_MIPS_OPTIONS_USE_RAW_MNEMONIC_FLAG;
const char* MipsDisassemlerOptionsHandler::m_disassembleMsaFlag =
  TEXT_MIPS_OPTIONS_DISASSEMBLE_MSA_FLAG;
const char* MipsDisassemlerOptionsHandler::m_disassembleVirtFlag =
  TEXT_MIPS_OPTIONS_DISASSEMBLE_VIRT_FLAG;
const char* MipsDisassemlerOptionsHandler::m_disassembleXpaFlag =
  TEXT_MIPS_OPTIONS_DISASSEMBLE_XPA_FLAG;
const char* MipsDisassemlerOptionsHandler::m_gprNamesAbiFlag = TEXT_MIPS_OPTIONS_GPR_NAMES_ABI_FLAG;
const char* MipsDisassemlerOptionsHandler::m_fprNamesAbiFlag = TEXT_MIPS_OPTIONS_FPR_NAMES_ABI_FLAG;
const char* MipsDisassemlerOptionsHandler::m_cp0ArchNameFlag = TEXT_MIPS_OPTIONS_CP0_ARCH_NAME_FLAG;
const char* MipsDisassemlerOptionsHandler::m_hwrArchNameFlag = TEXT_MIPS_OPTIONS_HWR_ARCH_NAME_FLAG;
const char* MipsDisassemlerOptionsHandler::m_regNamesAbiFlag = TEXT_MIPS_OPTIONS_REG_NAMES_ABI_FLAG;
const char* MipsDisassemlerOptionsHandler::m_regArchNameFlag = TEXT_MIPS_OPTIONS_REG_ARCH_NAME_FLAG;

bool        MipsDisassemlerOptionsHandler::m_useRawMnemonic  = false;
bool        MipsDisassemlerOptionsHandler::m_disassembleMsa  = false;
bool        MipsDisassemlerOptionsHandler::m_disassembleVirt = false;
bool        MipsDisassemlerOptionsHandler::m_disassembleXpa  = false;
std::string MipsDisassemlerOptionsHandler::m_gprNamesAbi     = std::string(128, '\0');
std::string MipsDisassemlerOptionsHandler::m_fprNamesAbi     = std::string(128, '\0');
std::string MipsDisassemlerOptionsHandler::m_cp0ArchName     = std::string(128, '\0');
std::string MipsDisassemlerOptionsHandler::m_hwrArchName     = std::string(128, '\0');
std::string MipsDisassemlerOptionsHandler::m_regNamesAbi     = std::string(128, '\0');
std::string MipsDisassemlerOptionsHandler::m_regArchName     = std::string(128, '\0');

void MipsDisassemlerOptionsHandler::Process(ObjdumpConfig& config)
{
    constexpr const char* useRawMnemonicLabel  = TEXT_MIPS_OPTIONS_USE_RAW_MNEMONIC_LABEL;
    constexpr const char* disassembleMsaLabel  = TEXT_MIPS_OPTIONS_DISASSEMBLE_MSA_LABEL;
    constexpr const char* disassembleVirtLabel = TEXT_MIPS_OPTIONS_DISASSEMBLE_VIRT_LABEL;
    constexpr const char* disassembleXpaLabel  = TEXT_MIPS_OPTIONS_DISASSEMBLE_XPA_LABEL;
    constexpr const char* gprNamesAbiLabel     = TEXT_MIPS_OPTIONS_GPR_NAMES_ABI_LABEL;
    constexpr const char* fprNamesAbiLabel     = TEXT_MIPS_OPTIONS_FPR_NAMES_ABI_LABEL;
    constexpr const char* cp0ArchNameLabel     = TEXT_MIPS_OPTIONS_CP0_ARCH_NAME_LABEL;
    constexpr const char* hwrArchNameLabel     = TEXT_MIPS_OPTIONS_HWR_ARCH_NAME_LABEL;
    constexpr const char* regNamesAbiLabel     = TEXT_MIPS_OPTIONS_REG_NAMES_ABI_LABEL;
    constexpr const char* regArchLabel         = TEXT_MIPS_OPTIONS_REG_ARCH_NAME_LABEL;

    ImGui::Checkbox(useRawMnemonicLabel,
                    &config.disassemblerOptions.mipsDisassemblerOptions.useRawMnemonic);
    m_useRawMnemonic = config.disassemblerOptions.mipsDisassemblerOptions.useRawMnemonic;
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_useRawMnemonicHint);
    }

    ImGui::Checkbox(disassembleMsaLabel,
                    &config.disassemblerOptions.mipsDisassemblerOptions.disassembleMsa);
    m_disassembleMsa = config.disassemblerOptions.mipsDisassemblerOptions.disassembleMsa;
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_disassembleMsaHint);
    }

    ImGui::Checkbox(disassembleVirtLabel,
                    &config.disassemblerOptions.mipsDisassemblerOptions.disassembleVirt);
    m_disassembleVirt = config.disassemblerOptions.mipsDisassemblerOptions.disassembleVirt;
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_disassembleVirtHint);
    }

    ImGui::Checkbox(disassembleXpaLabel,
                    &config.disassemblerOptions.mipsDisassemblerOptions.disassembleXpa);
    m_disassembleXpa = config.disassemblerOptions.mipsDisassemblerOptions.disassembleXpa;
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_disassembleXpaHint);
    }

    char*  gprBuf    = config.disassemblerOptions.mipsDisassemblerOptions.gprNamesAbi.data();
    size_t gprBufLen = config.disassemblerOptions.mipsDisassemblerOptions.gprNamesAbi.capacity();
    if (ImGui::InputText(gprNamesAbiLabel, gprBuf, gprBufLen))
    {
        config.disassemblerOptions.mipsDisassemblerOptions.gprNamesAbi =
          config.disassemblerOptions.mipsDisassemblerOptions.gprNamesAbi.c_str();
    }
    m_gprNamesAbi = config.disassemblerOptions.mipsDisassemblerOptions.gprNamesAbi;
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_gprNamesAbiHint);
    }

    char*  fprBuf    = config.disassemblerOptions.mipsDisassemblerOptions.fprNamesAbi.data();
    size_t fprBufLen = config.disassemblerOptions.mipsDisassemblerOptions.fprNamesAbi.capacity();
    if (ImGui::InputText(fprNamesAbiLabel, fprBuf, fprBufLen))
    {
        config.disassemblerOptions.mipsDisassemblerOptions.fprNamesAbi =
          config.disassemblerOptions.mipsDisassemblerOptions.fprNamesAbi.c_str();
    }
    m_fprNamesAbi = config.disassemblerOptions.mipsDisassemblerOptions.fprNamesAbi;
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_fprNamesAbiHint);
    }

    char*  cp0Buf    = config.disassemblerOptions.mipsDisassemblerOptions.cp0ArchName.data();
    size_t cp0BufLen = config.disassemblerOptions.mipsDisassemblerOptions.cp0ArchName.capacity();
    if (ImGui::InputText(cp0ArchNameLabel, cp0Buf, cp0BufLen))
    {
        config.disassemblerOptions.mipsDisassemblerOptions.cp0ArchName =
          config.disassemblerOptions.mipsDisassemblerOptions.cp0ArchName.c_str();
    }
    m_cp0ArchName = config.disassemblerOptions.mipsDisassemblerOptions.cp0ArchName;
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_cp0ArchNameHint);
    }

    char*  hwrBuf    = config.disassemblerOptions.mipsDisassemblerOptions.hwrArchName.data();
    size_t hwrBufLen = config.disassemblerOptions.mipsDisassemblerOptions.hwrArchName.capacity();
    if (ImGui::InputText(hwrArchNameLabel, hwrBuf, hwrBufLen))
    {
        config.disassemblerOptions.mipsDisassemblerOptions.hwrArchName =
          config.disassemblerOptions.mipsDisassemblerOptions.hwrArchName.c_str();
    }

    m_hwrArchName = config.disassemblerOptions.mipsDisassemblerOptions.hwrArchName;
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_hwrArchNameHint);
    }

    char*  regNameBuf = config.disassemblerOptions.mipsDisassemblerOptions.regNamesAbi.data();
    size_t regNameBufLen =
      config.disassemblerOptions.mipsDisassemblerOptions.regNamesAbi.capacity();
    if (ImGui::InputText(regNamesAbiLabel, regNameBuf, regNameBufLen))
    {
        config.disassemblerOptions.mipsDisassemblerOptions.regNamesAbi =
          config.disassemblerOptions.mipsDisassemblerOptions.regNamesAbi.c_str();
    }
    m_regNamesAbi = config.disassemblerOptions.mipsDisassemblerOptions.regNamesAbi;
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_regNamesAbiHint);
    }

    char*  regArchBuf = config.disassemblerOptions.mipsDisassemblerOptions.regArchName.data();
    size_t regArchBufLen =
      config.disassemblerOptions.mipsDisassemblerOptions.regArchName.capacity();
    if (ImGui::InputText(regArchLabel, regArchBuf, regArchBufLen))
    {
        config.disassemblerOptions.mipsDisassemblerOptions.regArchName =
          config.disassemblerOptions.mipsDisassemblerOptions.regArchName.c_str();
    }
    m_regArchName = config.disassemblerOptions.mipsDisassemblerOptions.regArchName;
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_regArchNameHint);
    }
}

std::ostream& MipsDisassemlerOptionsHandler::ToString(std::ostream& os)
{
    if (m_useRawMnemonic)
    {
        os << " " << m_useRawMnemonicFlag;
    }
    if (m_disassembleMsa)
    {
        os << " " << m_disassembleMsaFlag;
    }
    if (m_disassembleVirt)
    {
        os << " " << m_disassembleVirtFlag;
    }
    if (m_disassembleXpa)
    {
        os << " " << m_disassembleXpaFlag;
    }
    if (m_gprNamesAbi.empty() == false && m_gprNamesAbi[0] != '\0')
    {
        os << " " << m_gprNamesAbiFlag << m_gprNamesAbi;
    }
    if (m_fprNamesAbi.empty() == false && m_fprNamesAbi[0] != '\0')
    {
        os << " " << m_fprNamesAbiFlag << m_fprNamesAbi;
    }
    if (m_cp0ArchName.empty() == false && m_cp0ArchName[0] != '\0')
    {
        os << " " << m_cp0ArchNameFlag << m_cp0ArchName;
    }
    if (m_hwrArchName.empty() == false && m_hwrArchName[0] != '\0')
    {
        os << " " << m_hwrArchNameFlag << m_hwrArchName;
    }
    if (m_regNamesAbi.empty() == false && m_regNamesAbi[0] != '\0')
    {
        os << " " << m_regNamesAbiFlag << m_regNamesAbi;
    }
    if (m_regArchName.empty() == false && m_regArchName[0] != '\0')
    {
        os << " " << m_regArchNameFlag << m_regArchName;
    }

    return os;
}

/************************************************************************/
/* SECTION: ArcDisassemblerOptionsHandler                               */
/************************************************************************/
const char* ArcDisassemblerOptionsHandler::m_dspHint        = TEXT_ARC_DIS_OPTION_DSP_HINT;
const char* ArcDisassemblerOptionsHandler::m_spfpHint       = TEXT_ARC_DIS_OPTION_SPFP_HINT;
const char* ArcDisassemblerOptionsHandler::m_dpfpHint       = TEXT_ARC_DIS_OPTION_DPFP_HINT;
const char* ArcDisassemblerOptionsHandler::m_quarkse_emHint = TEXT_ARC_DIS_OPTION_QUARKSE_EM_HINT;
const char* ArcDisassemblerOptionsHandler::m_fpudaHint      = TEXT_ARC_DIS_OPTION_FPUDA_HINT;
const char* ArcDisassemblerOptionsHandler::m_fpusHint       = TEXT_ARC_DIS_OPTION_FPUS_HINT;
const char* ArcDisassemblerOptionsHandler::m_fpudHint       = TEXT_ARC_DIS_OPTION_FPUD_HINT;
const char* ArcDisassemblerOptionsHandler::m_displayAsHexHint =
  TEXT_ARC_DIS_OPTION_DISPLAY_AS_HEX_HINT;

const char* ArcDisassemblerOptionsHandler::m_dspFlag        = TEXT_ARC_DIS_OPTION_DSP_FLAG;
const char* ArcDisassemblerOptionsHandler::m_spfpFlag       = TEXT_ARC_DIS_OPTION_SPFP_FLAG;
const char* ArcDisassemblerOptionsHandler::m_dpfpFlag       = TEXT_ARC_DIS_OPTION_DPFP_FLAG;
const char* ArcDisassemblerOptionsHandler::m_quarkse_emFlag = TEXT_ARC_DIS_OPTION_QUARKSE_EM_FLAG;
const char* ArcDisassemblerOptionsHandler::m_fpudaFlag      = TEXT_ARC_DIS_OPTION_FPUDA_FLAG;
const char* ArcDisassemblerOptionsHandler::m_fpusFlag       = TEXT_ARC_DIS_OPTION_FPUS_FLAG;
const char* ArcDisassemblerOptionsHandler::m_fpudFlag       = TEXT_ARC_DIS_OPTION_FPUD_FLAG;
const char* ArcDisassemblerOptionsHandler::m_displayAsHexFlag =
  TEXT_ARC_DIS_OPTION_DISPLAY_AS_HEX_FLAG;

bool ArcDisassemblerOptionsHandler::m_dspActive          = false;
bool ArcDisassemblerOptionsHandler::m_spfpActive         = false;
bool ArcDisassemblerOptionsHandler::m_dpfpActive         = false;
bool ArcDisassemblerOptionsHandler::m_quarkse_emActive   = false;
bool ArcDisassemblerOptionsHandler::m_fpudaActive        = false;
bool ArcDisassemblerOptionsHandler::m_fpusActive         = false;
bool ArcDisassemblerOptionsHandler::m_fpudActive         = false;
bool ArcDisassemblerOptionsHandler::m_displayAsHexActive = false;

void ArcDisassemblerOptionsHandler::Process(ObjdumpConfig& config)
{
    constexpr const char* dspLabel          = TEXT_ARC_DIS_OPTION_DSP_LABEL;
    constexpr const char* spfpLabel         = TEXT_ARC_DIS_OPTION_SPFP_LABEL;
    constexpr const char* dpfpLabel         = TEXT_ARC_DIS_OPTION_DPFP_LABEL;
    constexpr const char* quarkseEmLabel    = TEXT_ARC_DIS_OPTION_QUARKSE_EM_LABEL;
    constexpr const char* fpudaLabel        = TEXT_ARC_DIS_OPTION_FPUDA_LABEL;
    constexpr const char* fpusLabel         = TEXT_ARC_DIS_OPTION_FPUS_LABEL;
    constexpr const char* fpudLabel         = TEXT_ARC_DIS_OPTION_FPUD_LABEL;
    constexpr const char* displayAsHexLabel = TEXT_ARC_DIS_OPTION_DISPLAY_AS_HEX_LABEL;

    ImGui::Checkbox(dspLabel, &config.disassemblerOptions.arcDisassemblerOptions.dsp);
    m_dspActive = config.disassemblerOptions.arcDisassemblerOptions.dsp;
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_dspHint);
    }

    ImGui::Checkbox(spfpLabel, &config.disassemblerOptions.arcDisassemblerOptions.spfp);
    m_spfpActive = config.disassemblerOptions.arcDisassemblerOptions.spfp;
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_spfpHint);
    }

    ImGui::Checkbox(dpfpLabel, &config.disassemblerOptions.arcDisassemblerOptions.dpfp);
    m_dpfpActive = config.disassemblerOptions.arcDisassemblerOptions.dpfp;
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_dpfpHint);
    }

    ImGui::Checkbox(quarkseEmLabel, &config.disassemblerOptions.arcDisassemblerOptions.quarkse_em);
    m_quarkse_emActive = config.disassemblerOptions.arcDisassemblerOptions.quarkse_em;
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_quarkse_emHint);
    }

    ImGui::Checkbox(fpudaLabel, &config.disassemblerOptions.arcDisassemblerOptions.fpuda);
    m_fpudaActive = config.disassemblerOptions.arcDisassemblerOptions.fpuda;
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_fpudaHint);
    }

    ImGui::Checkbox(fpusLabel, &config.disassemblerOptions.arcDisassemblerOptions.fpus);
    m_fpusActive = config.disassemblerOptions.arcDisassemblerOptions.fpus;
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_fpusHint);
    }

    ImGui::Checkbox(fpudLabel, &config.disassemblerOptions.arcDisassemblerOptions.fpud);
    m_fpudActive = config.disassemblerOptions.arcDisassemblerOptions.fpud;
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_fpudHint);
    }

    ImGui::Checkbox(displayAsHexLabel,
                    &config.disassemblerOptions.arcDisassemblerOptions.displayAsHex);
    m_displayAsHexActive = config.disassemblerOptions.arcDisassemblerOptions.displayAsHex;
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_displayAsHexHint);
    }
}

std::ostream& ArcDisassemblerOptionsHandler::ToString(std::ostream& os)
{
    if (m_dspActive)
    {
        os << " " << m_dspFlag;
    }
    if (m_spfpActive)
    {
        os << " " << m_spfpFlag;
    }
    if (m_dpfpActive)
    {
        os << " " << m_dpfpFlag;
    }
    if (m_quarkse_emActive)
    {
        os << " " << m_quarkse_emFlag;
    }
    if (m_fpudaActive)
    {
        os << " " << m_fpudaFlag;
    }
    if (m_fpusActive)
    {
        os << " " << m_fpusFlag;
    }
    if (m_fpudActive)
    {
        os << " " << m_fpudFlag;
    }
    if (m_displayAsHexActive)
    {
        os << " " << m_displayAsHexFlag;
    }

    return os;
}

/************************************************************************/
/* SECTION: ShowFormatSpecificInfo                                      */
/************************************************************************/
const char* ShowFormatSpecificInfoHandler::m_hint      = TEXT_SHOW_FORMAT_SPECIFIC_HINT;
const char* ShowFormatSpecificInfoHandler::m_paramHint = TEXT_SHOW_FORMAT_SPECIFIC_PARAM_HINT;
const char* ShowFormatSpecificInfoHandler::m_flag      = TEXT_SHOW_FORMAT_SPECIFIC_FLAG;
const char* ShowFormatSpecificInfoHandler::m_paramFlag = TEXT_SHOW_FORMAT_SPECIFIC_PARAM_FLAG;
bool        ShowFormatSpecificInfoHandler::m_active    = false;
std::string ShowFormatSpecificInfoHandler::m_param     = std::string(128, '\0');


void ShowFormatSpecificInfoHandler::Process(ObjdumpConfig& config)
{
    constexpr const char* label      = TEXT_SHOW_FORMAT_SPECIFIC_LABEL;
    constexpr const char* paramLabel = TEXT_SHOW_FORMAT_SPECIFIC_PARAM_LABEL;

    ImGui::Checkbox(label, &config.showFormatSpecificInfo);
    m_active = config.showFormatSpecificInfo;
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_hint);
    }

    if (m_active)
    {
        char*  buf = config.showFormatSpecificInfoParam.data();
        size_t len = config.showFormatSpecificInfoParam.capacity();
        if (ImGui::InputText(paramLabel, buf, len))
        {
            // Recreate the string using the raw data because we have invalidated the actual string
            // object.
            config.showFormatSpecificInfoParam = config.showFormatSpecificInfoParam.c_str();
        }
        m_param = config.showFormatSpecificInfoParam;
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip(m_paramHint);
        }
    }
}

std::ostream& ShowFormatSpecificInfoHandler::ToString(std::ostream& os)
{
    if (m_active)
    {
        os << " " << m_flag;
    }

    if (m_param.empty() == false && m_param[0] != '\0')
    {
        os << " " << m_paramFlag << m_param;
    }

    return os;
}


/************************************************************************/
/* SECTION: ShowRelocEntriesHandler                                     */
/************************************************************************/
const char* ShowRelocEntriesHandler::m_relocHint        = TEXT_SHOW_RELOC_ENTRIES_HINT;
const char* ShowRelocEntriesHandler::m_dynamicRelocHint = TEXT_SHOW_DYNAMIC_RELOC_ENTRIES_HINT;
const char* ShowRelocEntriesHandler::m_relocFlag        = TEXT_SHOW_RELOC_ENTRIES_FLAG;
const char* ShowRelocEntriesHandler::m_dynamicRelocFlag = TEXT_SHOW_DYNAMIC_RELOC_ENTRIES_FLAG;
bool        ShowRelocEntriesHandler::m_reloc            = false;
bool        ShowRelocEntriesHandler::m_dynamicReloc     = false;

void ShowRelocEntriesHandler::Process(ObjdumpConfig& config)
{
    constexpr const char* relocLabel        = TEXT_SHOW_RELOC_ENTRIES_LABEL;
    constexpr const char* dynamicRelocLabel = TEXT_SHOW_DYNAMIC_RELOC_ENTRIES_LABEL;

    ImGui::Checkbox(relocLabel, &config.showRelocEntries);
    m_reloc = config.showRelocEntries;
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_relocHint);
    }

    ImGui::Checkbox(dynamicRelocLabel, &config.showDynamicRelocEntries);
    m_dynamicReloc = config.showDynamicRelocEntries;
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_dynamicRelocHint);
    }
}

std::ostream& ShowRelocEntriesHandler::ToString(std::ostream& os)
{
    if (m_reloc)
    {
        os << " " << m_relocFlag;
    }
    if (m_reloc)
    {
        os << " " << m_dynamicRelocFlag;
    }

    return os;
}

/************************************************************************/
/* SECTION: ShowAllContentHandler                                       */
/************************************************************************/
const char* ShowAllContentHandler::m_hint   = TEXT_SHOW_ALL_CONTENT_HINT;
const char* ShowAllContentHandler::m_flag   = TEXT_SHOW_ALL_CONTENT_FLAG;
const char* ShowAllContentHandler::m_label  = TEXT_SHOW_ALL_CONTENT_LABEL;
bool        ShowAllContentHandler::m_active = false;

void ShowAllContentHandler::Process(ObjdumpConfig& config)
{
    ImGui::Checkbox(m_label, &config.showAllContent);
    m_active = config.showAllContent;
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_hint);
    }
}

std::ostream& ShowAllContentHandler::ToString(std::ostream& os)
{
    if (m_active)
    {
        os << " " << m_flag;
    }

    return os;
}

/************************************************************************/
/* SECTION: ShowSourceHandler                                           */
/************************************************************************/
const char* ShowSourceHandler::m_showSourceLabel = TEXT_SHOW_SOURCE_LABEL;
const char* ShowSourceHandler::m_prefixLabel     = TEXT_SHOW_SOURCE_PREFIX_LABEL;
const char* ShowSourceHandler::m_showSourceHint  = TEXT_SHOW_SOURCE_HINT;
const char* ShowSourceHandler::m_prefixHint      = TEXT_SHOW_SOURCE_PREFIX_HINT;
const char* ShowSourceHandler::m_showSourceFlag  = TEXT_SHOW_SOURCE_FLAG;
const char* ShowSourceHandler::m_prefixFlag      = TEXT_SHOW_SOURCE_PREFIX_FLAG;
bool        ShowSourceHandler::m_showSource      = false;
std::string ShowSourceHandler::m_prefix          = std::string(128, '\0');


void ShowSourceHandler::Process(ObjdumpConfig& config)
{
    if (ImGui::Checkbox(m_showSourceLabel, &config.showSource))
    {
        if (config.showSource == true)
        {
            config.disassemble = true;
        }
    }
    m_showSource = config.showSource;
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_showSourceHint);
    }

    char*  buf = config.sourcePrefix.data();
    size_t len = config.sourcePrefix.capacity();
    if (ImGui::InputText(m_prefixLabel, buf, len))
    {
        config.sourcePrefix = buf;
    }
    m_prefix = config.sourcePrefix;
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_prefixHint);
    }
}

std::ostream& ShowSourceHandler::ToString(std::ostream& os)
{
    if (m_prefix.empty() == false && m_prefix[0] != '\0')
    {
        os << " " << m_prefixFlag << m_prefix;
    }
    else if (m_showSource)
    {
        // We don't want --source-comment *and* -S at the same time.
        os << " " << m_showSourceFlag;
    }
    return os;
}

/************************************************************************/
/* SECTION: PathHandler                                                 */
/************************************************************************/
const char* PathHandler::m_prefixLabel = TEXT_PATH_PREFIX_LABEL;
const char* PathHandler::m_stripLabel  = TEXT_PATH_STRIP_LEVEL_LABEL;
const char* PathHandler::m_prefixHint  = TEXT_PATH_PREFIX_HINT;
const char* PathHandler::m_stripHint   = TEXT_PATH_STRIP_LEVEL_HINT;
const char* PathHandler::m_prefixFlag  = TEXT_PATH_PREFIX_FLAG;
const char* PathHandler::m_stripFlag   = TEXT_PATH_STRIP_LEVEL_FLAG;
std::string PathHandler::m_prefix      = "";
int         PathHandler::m_strip       = 0;
bool        PathHandler::m_showSource  = false;

void PathHandler::Process(ObjdumpConfig& config)
{
    char*  buf        = config.pathPrefix.data();
    size_t len        = config.pathPrefix.capacity();
    int*   stripLevel = &config.pathStripLevel;
    int    dummy      = config.pathStripLevel;
    m_showSource      = config.showSource;
    if (m_showSource == false)
    {
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive,
                              ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered,
                              ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));

        // Clear len so that user can't input anything in the field.
        len        = 0;
        stripLevel = &dummy;
    }

    if (ImGui::InputText(m_prefixLabel, buf, len))
    {
        config.pathPrefix = buf;
    }
    m_prefix = config.pathPrefix;
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_prefixHint);
    }

    if (m_prefix.empty() == false)
    {
        ImGui::InputInt(m_stripLabel, stripLevel);
        // Don't allow negative numbers.
        *stripLevel = std::max(*stripLevel, 0);
        m_strip     = config.pathStripLevel;
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip(m_stripHint);
        }
    }

    if (m_showSource == false)
    {
        ImGui::PopStyleColor(3);
    }
}

std::ostream& PathHandler::ToString(std::ostream& os)
{
    if ((m_prefix.empty() == false && m_prefix[0] != '\0') && m_showSource)
    {
        os << " " << m_prefixFlag << m_prefix;
        if (m_strip != 0)
        {
            os << " " << m_stripFlag << m_strip;
        }
    }
    return os;
}

/************************************************************************/
/* SECTION: ShowRawInstructionHandler                                   */
/************************************************************************/
const char* ShowRawInstructionHandler::m_label      = TEXT_SHOW_RAW_INSTRUCTION_LABEL;
const char* ShowRawInstructionHandler::m_widthLabel = TEXT_SHOW_INSTRUCTION_WIDTH_LABEL;
const char* ShowRawInstructionHandler::m_hint       = TEXT_SHOW_RAW_INSTRUCTION_HINT;
const char* ShowRawInstructionHandler::m_widthHint  = TEXT_SHOW_INSTRUCTION_WIDTH_HINT;
const char* ShowRawInstructionHandler::m_flag       = TEXT_SHOW_RAW_INSTRUCTION_FLAG;
const char* ShowRawInstructionHandler::m_widthFlag  = TEXT_SHOW_INSTRUCTION_WIDTH_FLAG;

bool ShowRawInstructionHandler::m_active = false;
int  ShowRawInstructionHandler::m_width  = 0;

void ShowRawInstructionHandler::Process(ObjdumpConfig& config)
{
    ImGui::Checkbox(m_label, &config.showRawInstruction);
    m_active = config.showRawInstruction;
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_hint);
    }

    ImGui::InputInt(m_widthLabel, &config.insnWidth);
    // Don't allow negative numbers.
    config.insnWidth = std::max(config.insnWidth, 0);
    m_width          = config.insnWidth;
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_widthHint);
    }
}

std::ostream& ShowRawInstructionHandler::ToString(std::ostream& os)
{
    if (m_active)
    {
        os << " " << m_flag;
    }

    if (m_width != 0)
    {
        os << " " << m_widthFlag << m_width;
    }

    return os;
}

/************************************************************************/
/* SECTION: VisualizeJumpsHandler                                       */
/************************************************************************/
const char* VisualizeJumpsHandler::m_label  = TEXT_VISUALIZE_JUMPS_LABEL;
const char* VisualizeJumpsHandler::m_hint   = TEXT_VISUALIZE_JUMPS_HINT;
const char* VisualizeJumpsHandler::m_flag   = TEXT_VISUALIZE_JUMPS_FLAG;
bool        VisualizeJumpsHandler::m_active = false;

void VisualizeJumpsHandler::Process(ObjdumpConfig& config)
{
    ImGui::Checkbox(m_label, &config.visualizeJumps);
    m_active = config.visualizeJumps;
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_hint);
    }
}

std::ostream& VisualizeJumpsHandler::ToString(std::ostream& os)
{
    os << " " << m_flag;
    if (m_active == false)
    {
        os << "=off";
    }
    return os;
}

/************************************************************************/
/* SECTION: DwarfOptionsHandler                                         */
/************************************************************************/
const char* DwarfOptionsHandler::m_label      = TEXT_DWARF_LABEL;
const char* DwarfOptionsHandler::m_hint       = TEXT_DWARF_HINT;
const char* DwarfOptionsHandler::m_flag       = TEXT_DWARF_FLAG;
const char* DwarfOptionsHandler::m_depthLabel = TEXT_DWARF_DEPTH_LABEL;
const char* DwarfOptionsHandler::m_depthHint  = TEXT_DWARF_DEPTH_HINT;
const char* DwarfOptionsHandler::m_depthFlag  = TEXT_DWARF_DEPTH_FLAG;
const char* DwarfOptionsHandler::m_startLabel = TEXT_DWARF_START_LABEL;
const char* DwarfOptionsHandler::m_startHint  = TEXT_DWARF_START_HINT;
const char* DwarfOptionsHandler::m_startFlag  = TEXT_DWARF_START_FLAG;
const char* DwarfOptionsHandler::m_checkLabel = TEXT_DWARF_CHECK_LABEL;
const char* DwarfOptionsHandler::m_checkHint  = TEXT_DWARF_CHECK_HINT;
const char* DwarfOptionsHandler::m_checkFlag  = TEXT_DWARF_CHECK_FLAG;

DwarfDisplayOptions*                        DwarfOptionsHandler::m_config = nullptr;
std::array<DwarfOptionsHandler::Option, 21> DwarfOptionsHandler::m_options =
  {Option(TEXT_DWARF_ABBREV_LABEL, TEXT_DWARF_ABBREV_HINT, TEXT_DWARF_ABBREV_FLAG),
   Option(TEXT_DWARF_ADDR_LABEL, TEXT_DWARF_ADDR_HINT, TEXT_DWARF_ADDR_FLAG),
   Option(TEXT_DWARF_CU_INDEX_LABEL, TEXT_DWARF_CU_INDEX_HINT, TEXT_DWARF_CU_INDEX_FLAG),
   Option(TEXT_DWARF_FRAMES_LABEL, TEXT_DWARF_FRAMES_HINT, TEXT_DWARF_FRAMES_FLAG),
   Option(TEXT_DWARF_FRAME_INTERP_LABEL,
          TEXT_DWARF_FRAME_INTERP_HINT,
          TEXT_DWARF_FRAME_INTERP_FLAG),
   Option(TEXT_DWARF_GDB_INDEX_LABEL, TEXT_DWARF_GDB_INDEX_HINT, TEXT_DWARF_GDB_INDEX_FLAG),
   Option(TEXT_DWARF_INFO_LABEL, TEXT_DWARF_INFO_HINT, TEXT_DWARF_INFO_FLAG),
   Option(TEXT_DWARF_LINKS_LABEL, TEXT_DWARF_LINKS_HINT, TEXT_DWARF_LINKS_FLAG),
   Option(TEXT_DWARF_FOLLOW_LINKS_LABEL,
          TEXT_DWARF_FOLLOW_LINKS_HINT,
          TEXT_DWARF_FOLLOW_LINKS_FLAG),
   Option(TEXT_DWARF_RAW_LINE_LABEL, TEXT_DWARF_RAW_LINE_HINT, TEXT_DWARF_RAW_LINE_FLAG),
   Option(TEXT_DWARF_DECODED_LINE_LABEL,
          TEXT_DWARF_DECODED_LINE_HINT,
          TEXT_DWARF_DECODED_LINE_FLAG),
   Option(TEXT_DWARF_MACRO_LABEL, TEXT_DWARF_MACRO_HINT, TEXT_DWARF_MACRO_FLAG),
   Option(TEXT_DWARF_LOC_LABEL, TEXT_DWARF_LOC_HINT, TEXT_DWARF_LOC_FLAG),
   Option(TEXT_DWARF_PUBNAMES_LABEL, TEXT_DWARF_PUBNAMES_HINT, TEXT_DWARF_PUBNAMES_FLAG),
   Option(TEXT_DWARF_ARANGES_LABEL, TEXT_DWARF_ARANGES_HINT, TEXT_DWARF_ARANGES_FLAG),
   Option(TEXT_DWARF_RANGES_LABEL, TEXT_DWARF_RANGES_HINT, TEXT_DWARF_RANGES_FLAG),
   Option(TEXT_DWARF_STR_LABEL, TEXT_DWARF_STR_HINT, TEXT_DWARF_STR_FLAG),
   Option(TEXT_DWARF_PUBTYPE_LABEL, TEXT_DWARF_PUBTYPE_HINT, TEXT_DWARF_PUBTYPE_FLAG),
   Option(TEXT_DWARF_TRACE_ARANGES_LABEL,
          TEXT_DWARF_TRACE_ARANGES_HINT,
          TEXT_DWARF_TRACE_ARANGES_FLAG),
   Option(TEXT_DWARF_TRACE_ABBREV_LABEL,
          TEXT_DWARF_TRACE_ABBREV_HINT,
          TEXT_DWARF_TRACE_ABBREV_FLAG),
   Option(TEXT_DWARF_TRACE_INFO_LABEL, TEXT_DWARF_TRACE_INFO_HINT, TEXT_DWARF_TRACE_INFO_FLAG)};

void DwarfOptionsHandler::Process(ObjdumpConfig& config)
{
    m_config = &config.dwarfOptions;
    AssignPtrs(config);

    //     ImGui::BeginGroup();
    if (ImGui::TreeNode(m_label))
    {
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip(m_hint);
        }
        for (auto& o : m_options)
        {
            ImGui::Checkbox(o.label, o.active);
            if (ImGui::IsItemHovered())
            {
                ImGui::SetTooltip(o.hint);
            }
        }

        ImGui::InputInt(m_startLabel, &(m_config->debugInfoDepthStart));
        m_config->debugInfoDepthStart = std::max(m_config->debugInfoDepthStart, 0);
        m_config->debugInfoDepthStart =
          std::min(m_config->debugInfoDepthStart, m_config->debugInfoDepthLimit);
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip(m_startHint);
        }

        ImGui::InputInt(m_depthLabel, &(m_config->debugInfoDepthLimit));
        m_config->debugInfoDepthLimit = std::max(m_config->debugInfoDepthLimit, 0);
        m_config->debugInfoDepthLimit =
          std::max(m_config->debugInfoDepthLimit, m_config->debugInfoDepthStart);
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip(m_depthHint);
        }

        ImGui::Checkbox(m_checkLabel, &(m_config->enableAdditionChecks));
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip(m_checkHint);
        }
        ImGui::TreePop();
    }
    // We just want to check that is the node isn't open.
    else if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_hint);
    }
}

std::ostream& DwarfOptionsHandler::ToString(std::ostream& os)
{
    bool isAnyActive = false;

    for (const auto& o : m_options)
    {
        if (*o.active)
        {
            isAnyActive = true;
            break;
        }
    }

    if (isAnyActive)
    {
        os << " " << m_flag;
    }

    for (const auto& o : m_options)
    {
        if (*o.active)
        {
            os << o.flag;
        }
    }

    if (m_config->debugInfoDepthStart != 0)
    {
        os << " " << m_startFlag << m_config->debugInfoDepthStart;
    }
    if (m_config->debugInfoDepthLimit != 0)
    {
        os << " " << m_depthFlag << m_config->debugInfoDepthLimit;
    }
    if (m_config->enableAdditionChecks)
    {
        os << " " << m_checkFlag;
    }

    return os;
}

void DwarfOptionsHandler::AssignPtrs(ObjdumpConfig& config)
{
    m_options[0].active  = &config.dwarfOptions.showDebugAbbrev;
    m_options[1].active  = &config.dwarfOptions.showDebugAddr;
    m_options[2].active  = &config.dwarfOptions.showDebugCuTuIndex;
    m_options[3].active  = &config.dwarfOptions.showRawDebugFrame;
    m_options[4].active  = &config.dwarfOptions.showInterpretedDebugFrame;
    m_options[5].active  = &config.dwarfOptions.showGdbIndex;
    m_options[6].active  = &config.dwarfOptions.showDebugInfo;
    m_options[7].active  = &config.dwarfOptions.showGnuDebugLink;
    m_options[8].active  = &config.dwarfOptions.followLinks;
    m_options[9].active  = &config.dwarfOptions.showDebugLine;
    m_options[10].active = &config.dwarfOptions.showInterpretedDebugLine;
    m_options[11].active = &config.dwarfOptions.showDebugMacro;
    m_options[12].active = &config.dwarfOptions.showDebugLoc;
    m_options[13].active = &config.dwarfOptions.showDebugPubNames;
    m_options[14].active = &config.dwarfOptions.showDebugAranges;
    m_options[15].active = &config.dwarfOptions.showDebugRanges;
    m_options[16].active = &config.dwarfOptions.showDebugStr;
    m_options[17].active = &config.dwarfOptions.showDebugPubTypes;
    m_options[18].active = &config.dwarfOptions.showTraceAranges;
    m_options[19].active = &config.dwarfOptions.showTraceAbbrev;
    m_options[20].active = &config.dwarfOptions.showTraceInfo;
}

/************************************************************************/
/* SECTION: CtfSectionHandler                                           */
/************************************************************************/
const char* CtfSectionHandler::m_sectionNameLabel   = TEXT_CTF_SECTION_NAME_LABEL;
const char* CtfSectionHandler::m_sectionNameHint    = TEXT_CTF_SECTION_NAME_HINT;
const char* CtfSectionHandler::m_sectionNameFlag    = TEXT_CTF_SECTION_NAME_FLAG;
const char* CtfSectionHandler::m_sectionParentLabel = TEXT_CTF_PARENT_SECTION_NAME_LABEL;
const char* CtfSectionHandler::m_sectionParentHint  = TEXT_CTF_PARENT_SECTION_NAME_HINT;
const char* CtfSectionHandler::m_sectionParentFlag  = TEXT_CTF_PARENT_SECTION_NAME_FLAG;
std::string CtfSectionHandler::m_sectionName        = "";
std::string CtfSectionHandler::m_sectionParent      = "";

void CtfSectionHandler::Process(ObjdumpConfig& config)
{
    char*  buf = config.ctfSectionName.data();
    size_t len = config.ctfSectionName.capacity();
    if (ImGui::InputText(m_sectionNameLabel, buf, len))
    {
        config.ctfSectionName = buf;
    }
    m_sectionName = config.ctfSectionName;
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_sectionNameHint);
    }

    buf = config.ctfParentSectionName.data();
    len = config.ctfParentSectionName.capacity();
    if (ImGui::InputText(m_sectionParentLabel, buf, len))
    {
        config.ctfParentSectionName = buf;
    }
    m_sectionParent = config.ctfParentSectionName;
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_sectionParentHint);
    }
}

std::ostream& CtfSectionHandler::ToString(std::ostream& os)
{
    if (m_sectionName.empty() == false && m_sectionName[0] != '\0')
    {
        os << " " << m_sectionNameFlag << m_sectionName;
    }
    if (m_sectionParent.empty() == false && m_sectionParent[0] != '\0')
    {
        os << " " << m_sectionParentFlag << m_sectionParent;
    }

    return os;
}

/************************************************************************/
/* SECTION: ShowFullSectionContentHandler                               */
/************************************************************************/
const char* ShowFullSectionContentHandler::m_label  = TEXT_SHOW_FULL_SECTION_CONTENT_LABEL;
const char* ShowFullSectionContentHandler::m_hint   = TEXT_SHOW_FULL_SECTION_CONTENT_HINT;
const char* ShowFullSectionContentHandler::m_flag   = TEXT_SHOW_FULL_SECTION_CONTENT_FLAG;
bool        ShowFullSectionContentHandler::m_active = false;

void ShowFullSectionContentHandler::Process(ObjdumpConfig& config)
{
    ImGui::Checkbox(m_label, &config.showFullSectionContent);
    m_active = config.showFullSectionContent;
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_hint);
    }
}

std::ostream& ShowFullSectionContentHandler::ToString(std::ostream& os)
{
    if (m_active)
    {
        os << " " << m_flag;
    }

    return os;
}

/************************************************************************/
/* SECTION: AddressRangeHandler                                         */
/************************************************************************/
const char* AddressRangeHandler::m_startAddrLabel = TEXT_ADDRESS_RANGE_HANDLER_START_LABEL;
const char* AddressRangeHandler::m_startAddrHint  = TEXT_ADDRESS_RANGE_HANDLER_START_HINT;
const char* AddressRangeHandler::m_startAddrFlag  = TEXT_ADDRESS_RANGE_HANDLER_START_FLAG;
const char* AddressRangeHandler::m_stopAddrLabel  = TEXT_ADDRESS_RANGE_HANDLER_STOP_LABEL;
const char* AddressRangeHandler::m_stopAddrHint   = TEXT_ADDRESS_RANGE_HANDLER_STOP_HINT;
const char* AddressRangeHandler::m_stopAddrFlag   = TEXT_ADDRESS_RANGE_HANDLER_STOP_FLAG;
int         AddressRangeHandler::m_startAddr      = 0;
int         AddressRangeHandler::m_stopAddr       = 0;

void AddressRangeHandler::Process(ObjdumpConfig& config)
{
    ImGui::InputInt(m_startAddrLabel,
                    &config.startAddress,
                    4,
                    0x100,
                    ImGuiInputTextFlags_CharsHexadecimal);
    config.startAddress = std::max(config.startAddress, 0);
    config.startAddress = std::min(config.startAddress, config.stopAddress);
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_startAddrHint);
    }
    ImGui::InputInt(m_stopAddrLabel,
                    &config.stopAddress,
                    4,
                    0x100,
                    ImGuiInputTextFlags_CharsHexadecimal);
    config.stopAddress = std::max(config.stopAddress, 0);
    config.stopAddress = std::max(config.stopAddress, config.startAddress);
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_stopAddrHint);
    }
}

std::ostream& AddressRangeHandler::ToString(std::ostream& os)
{
    if (m_startAddr != 0)
    {
        os << " " << m_startAddrFlag << "0x" << std::hex << m_startAddr;
    }
    if (m_stopAddr != 0)
    {
        os << " " << m_stopAddrFlag << "0x" << std::hex << m_stopAddr;
    }

    return os;
}

/************************************************************************/
/* SECTION: ShowSymbolTableAsSymsHandler                                */
/************************************************************************/
const char* ShowSymbolTableAsSymsHandler::m_symsLabel    = TEXT_SHOW_SYMBOL_TABLE_AS_SYMS_LABEL;
const char* ShowSymbolTableAsSymsHandler::m_symsHint     = TEXT_SHOW_SYMBOL_TABLE_AS_SYMS_HINT;
const char* ShowSymbolTableAsSymsHandler::m_symsFlag     = TEXT_SHOW_SYMBOL_TABLE_AS_SYMS_FLAG;
const char* ShowSymbolTableAsSymsHandler::m_dynSymsLabel = TEXT_SHOW_DYN_SYMBOL_TABLE_AS_SYMS_LABEL;
const char* ShowSymbolTableAsSymsHandler::m_dynSymsHint  = TEXT_SHOW_DYN_SYMBOL_TABLE_AS_SYMS_HINT;
const char* ShowSymbolTableAsSymsHandler::m_dynSymsFlag  = TEXT_SHOW_DYN_SYMBOL_TABLE_AS_SYMS_FLAG;
const char* ShowSymbolTableAsSymsHandler::m_specSymsLabel =
  TEXT_SHOW_SPE_SYMBOL_TABLE_AS_SYMS_LABEL;
const char* ShowSymbolTableAsSymsHandler::m_specSymsHint = TEXT_SHOW_SPE_SYMBOL_TABLE_AS_SYMS_HINT;
const char* ShowSymbolTableAsSymsHandler::m_specSymsFlag = TEXT_SHOW_SPE_SYMBOL_TABLE_AS_SYMS_FLAG;
bool        ShowSymbolTableAsSymsHandler::m_syms         = false;
bool        ShowSymbolTableAsSymsHandler::m_dynSyms      = false;
bool        ShowSymbolTableAsSymsHandler::m_specSyms     = false;

void ShowSymbolTableAsSymsHandler::Process(ObjdumpConfig& config)
{
    ImGui::Checkbox(m_symsLabel, &config.showSymbolTableAsSyms);
    m_syms = config.showSymbolTableAsSyms;
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_symsHint);
    }

    ImGui::Checkbox(m_dynSymsLabel, &config.showDynamicSymbolTableAsSyms);
    m_dynSyms = config.showDynamicSymbolTableAsSyms;
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_dynSymsHint);
    }

    ImGui::Checkbox(m_specSymsLabel, &config.includeSpecialSymbols);
    m_specSyms = config.includeSpecialSymbols;
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_specSymsHint);
    }
}

std::ostream& ShowSymbolTableAsSymsHandler::ToString(std::ostream& os)
{
    if (m_syms)
    {
        os << " " << m_symsFlag;
    }
    if (m_dynSyms)
    {
        os << " " << m_dynSymsFlag;
    }
    if (m_specSyms)
    {
        os << " " << m_specSymsFlag;
    }
    return os;
}

/************************************************************************/
/* SECTION: ExtraFormatOptionsHandler                                   */
/************************************************************************/
const char* ExtraFormatOptionsHandler::m_fmtForWideLabel = TEXT_FORMAT_FOR_WIDE_DEVICES_LABEL;
const char* ExtraFormatOptionsHandler::m_fmtForWideHint  = TEXT_FORMAT_FOR_WIDE_DEVICES_HINT;
const char* ExtraFormatOptionsHandler::m_fmtForWideFlag  = TEXT_FORMAT_FOR_WIDE_DEVICES_FLAG;
bool        ExtraFormatOptionsHandler::m_fmtForWide      = false;

void ExtraFormatOptionsHandler::Process(ObjdumpConfig& config)
{
    ImGui::Checkbox(m_fmtForWideLabel, &config.formatForWideDevices);
    m_fmtForWide = config.formatForWideDevices;
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_fmtForWideHint);
    }
}

std::ostream& ExtraFormatOptionsHandler::ToString(std::ostream& os)
{
    if (m_fmtForWide)
    {
        os << " " << m_fmtForWideFlag;
    }
    return os;
}

/************************************************************************/
/* SECTION: ShowAllHeadersHandler                                       */
/************************************************************************/
const char* ShowAllHeadersHandler::m_label  = TEXT_SHOW_ALL_HEADERS_LABEL;
const char* ShowAllHeadersHandler::m_hint   = TEXT_SHOW_ALL_HEADERS_HINT;
const char* ShowAllHeadersHandler::m_flag   = TEXT_SHOW_ALL_HEADERS_FLAG;
bool        ShowAllHeadersHandler::m_active = false;

void ShowAllHeadersHandler::Process(ObjdumpConfig& config)
{
    ImGui::Checkbox(m_label, &config.showAllHeaders);
    m_active = config.showAllHeaders;
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_hint);
    }
}

std::ostream& ShowAllHeadersHandler::ToString(std::ostream& os)
{
    if (m_active)
    {
        os << " " << m_flag;
    }
    return os;
}

/************************************************************************/
/* SECTION: DisassembleZerosHandler                                     */
/************************************************************************/
const char* DisassembleZerosHandler::m_label  = TEXT_DISASSEMBLE_ZEROS_LABEL;
const char* DisassembleZerosHandler::m_hint   = TEXT_DISASSEMBLE_ZEROS_HINT;
const char* DisassembleZerosHandler::m_flag   = TEXT_DISASSEMBLE_ZEROS_FLAG;
bool        DisassembleZerosHandler::m_active = false;

void DisassembleZerosHandler::Process(ObjdumpConfig& config)
{
    ImGui::Checkbox(m_label, &config.disassembleZeroes);
    m_active = config.disassembleZeroes;
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(m_hint);
    }
}

std::ostream& DisassembleZerosHandler::ToString(std::ostream& os)
{
    if (m_active)
    {
        os << " " << m_flag;
    }
    return os;
}
