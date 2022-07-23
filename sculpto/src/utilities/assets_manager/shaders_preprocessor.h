/*!****************************************************************//*!*
 * \file   shaders_preprocessor.h
 * \brief  Assets manager shaders preprocessor class definition module.
 * 
 * \author Sabitov Kirill
 * \date   07 July 2022
 *********************************************************************/

#pragma once

#include "base.h"
#include "core/render/primitives/shader.h"

namespace scl::assets_manager
{
    /*! Static shader preprocessor class. */
    class shader_preprocessor
    {
    private:
        /*! Shader preprocessor lexems. */
        static constexpr const char *LexemBlockStart = "#shader-begin";
        static constexpr const char *LexemBlockEnd = "#shader-end";
        static constexpr const char *LexemInclude = "#include";

        /*!*
         * Read word from string, starting at Begin iterator function.
         * 
         * \param Begin - start point for reading word.
         * \param End - end of reading string.
         * \param SpacesCount - count of spkipped spaces on th left side of word.
         * \return readed word.
         */
        static std::string ReadWord(std::string::const_iterator Begin, std::string::const_iterator End, int *SpacesCount = nullptr);

        /*!*
         * Get shader type from string function.
         * 
         * \param ShaderTypeString - string, containing shader type.
         * \return shader type enum.
         */
        static shader_type ShaderTypeFromString(const std::string &ShaderTypeString);

    public:
        /*!*
         * Separate text to diffrent shader blocks function.
         *
         * \param ShaderDebugName - debug name to show if error occures.
         * \param ShaderText - inptu text to separate to blocks.
         * \param Out - output array of shaders text.
         */
        static void SeparateShaders(const std::string &ShaderDebugName, const std::string &ShaderText, std::vector<shader_props> &Out);

        /*!*
         * Process include directives in shader text.
         * 
         * \param ShaderDebugName - debug name to show if error occures.
         * \param ShaderFolderPath - path to folder where shalder is located.
         * \param ShaderText[in, out] - text of shader to process.
         * \return None.
         */
        static void ProcessIncludes(const std::string &ShaderDebugName, const std::string &ShaderFolderPath, std::string &ShaderText);
    };
}
