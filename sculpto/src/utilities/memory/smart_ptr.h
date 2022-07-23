/*!****************************************************************//*!*
 * \file   shared.h
 * \brief  Smart pointers implementatino module.
 * 
 * \author Sabitov Kirill
 * \date   22 June 2022
 *********************************************************************/

#pragma once

namespace scl
{
    /*! Shared smart pointer. */
    template<typename T>
    using shared = std::shared_ptr<T>;

    template<typename T, typename... Args>
    constexpr shared<T> CreateShared(Args &&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    /*! Unique smart pointer. */
    template<typename T>
    using unique = std::unique_ptr<T>;

    template<typename T, typename... Args>
    constexpr unique<T> CreateUnique(Args &&... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    /*! Weak smart pointer (do not increases counter). */
    template<typename T>
    using weak = std::weak_ptr<T>;
}
