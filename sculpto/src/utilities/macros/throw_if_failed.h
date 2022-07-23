/*!****************************************************************//*!*
 * \file   throw_if_failed.h
 * \brief  Helper throw if failed inline function implementation function.
 *         Commonly used in DirectX module.
 * 
 * \author Sabitov Kirill
 * \date   22 June 2022
 *********************************************************************/

#pragma once

#include "sclpch.h"

namespace scl::dx
{
    /*! Helprer class for COM exeptions. */
    class com_exeption: public std::exception
    {
    private:
        /*! COM resuslt handle. */
        HRESULT Result;

    public:
        /*! Default constructor by COM HRESULT. */
        com_exeption(HRESULT hr) :Result(hr) {}

        const char *what() const override
        {
            static char str[64] {};
            sprintf_s(str, "Failure with HRESULT of %08X",
                      static_cast<unsigned int>(Result));
            return str;
        }
    };

    /*!*
     * Helper utility converts D3D API failures into exceptions.
     *
     * \param hr COM result handle.
     * \return None.
     */
    inline void ThrowIfFailed(HRESULT hr)
    {
        if (FAILED(hr))
        {
            throw com_exeption(hr);
        }
    }
}