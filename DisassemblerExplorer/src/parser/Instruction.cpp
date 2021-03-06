/**************************************************************************************************
 * @addtogroup  Parser
 * {
 * @file        Instruction
 * @author      Pascal-Emmanuel Lachance
 * @date        2020/07/24  -  20:02
 * @brief       
 **************************************************************************************************
 *   Copyright (C) 2020  Samuel Martel - Pascal-Emmanuel Lachance
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
 *************************************************************************************************/
/* Includes ------------------------------------------------------------------------------------ */
#include "Instruction.h"

#include "utils/StringUtils.h"


namespace Parser
{


/*************************************************************************************************/
/* Member functions definitions ---------------------------------------------------------------- */
Instruction::~Instruction() = default;

void Instruction::CleanLineOfCode()
{
    m_lineOfCode = StringUtils::RemoveExtraWhitespaces(m_lineOfCode);
}

/*************************************************************************************************/
}    // namespace Parser
/**
 * @}
 */
/* ----- END OF FILE ----- */
