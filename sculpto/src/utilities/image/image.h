/*****************************************************************//**
 * \file   image.h
 * \brief  Image container class implementation module.
 * 
 * \author Sabitov Kirill
 * \date   28 June 2022
 *********************************************************************/

#pragma once

#include <stb_image.h>

#include "base.h"
#include "utilities/memory/memory_swap.h"

namespace scl
{
    /* Image container class. */
    class image
    {
    private: /* Image data. */
        int Width {}, Height {};
        int ComponentsCount {};
        u8 *Data {};

    public: /* Image getter/setter functions. */
        /* Image width getter function. */
        int GetWidth() const { return Width; }
        /* Image height getter function. */
        int GetHeight() const { return Height; }
        /* Image dimentions (components per pixel) count getter function. */
        int GetComponentsCount() const { return ComponentsCount; }
        /* Image raw data pointer getter function. */
        const u8 *GetRawData() const { return Data; }

        /**
         * Check if image container filled with pixels function.
         * 
         * \param None.
         * \return is image empty flag.
         */
        bool IsEmpty() const { return Data == nullptr; }

    public:
        /* Imaeg default constructor. */
        image() = default;

        /* Image copy constructor. */
        image(const image &Other)
        {
            if (Other.Data == nullptr) return;

            Width = Other.Width;
            Height = Other.Height;
            ComponentsCount = Other.ComponentsCount;

            size_t size = (size_t)Width * Height * ComponentsCount;
            Data = new u8[size];
            memcpy(Data, Other.Data, size);
        }

        /* Image copy assigment operator. */
        image &operator= (const image &Other)
        {
            if (this == &Other || Other.Data == nullptr) return *this;
            if (Data != nullptr) delete[] Data;

            Width = Other.Width;
            Height = Other.Height;
            ComponentsCount = Other.ComponentsCount;

            size_t size = (size_t)Width * Height * ComponentsCount;
            Data = new u8[size];
            memcpy(Data, Other.Data, size);
            return *this;
        }

        /**
         * Empty image specified size constructor.
         * 
         * \param Width - width of creating image.
         * \param Height - height of creating image.
         * \param ComponentsCount - dimentions (components per pixel) count.
         */
        image(int Width, int Height, int ComponentsCount) :
            Width(Width), Height(Height), ComponentsCount(ComponentsCount)
        {
            Data = new u8[(u64)Width * Height * ComponentsCount];
        }

        /**
         * Image by pixels array constructor.
         * 
         * \param Width - width of creating image.
         * \param Height - height of creating image.
         * \param ComponentsCount - dimentions (components per pixel) count.
         * \param Data - image pixel array pointer with byte size [Width * Height * ComponentsCount].
         */
        image(int Width, int Height, int ComponentsCount, void *Data) :
            Width(Width), Height(Height), ComponentsCount(ComponentsCount)
        {
            size_t size = (size_t)Width * Height * ComponentsCount;
            this->Data = new u8[size];
            memcpy(this->Data, Data, size);
        }

        /**
         * Image constructor loading data from file.
         * 
         * \param FileName - image file name.
         */
        image(const std::string &FileName)
        {
            this->Load(FileName);
        }

        /* Image default destructor. */
        ~image()
        {
            Width = Height = ComponentsCount = 0;
            if (Data != nullptr) delete[] Data;
        }

        /**
         * Image load from file function.
         * 
         * \param FileName - file name to load image from.
         * \return success flag.
         */
        bool Load(const std::string &FileName)
        {
            // За каждым малоком должен быть free.... (CGSG FOREVER!!!)
            if (Data != nullptr) delete[] Data;

            // Load image, check if successful
            int w, h, c;
            Data = stbi_load(FileName.c_str(), &w, &h, &c, 0);
            if (Data == nullptr)
            {
                SCL_CORE_ERROR("Error during loading image from file \"{}\"!", FileName);
                Width = Height = ComponentsCount = 0;
                return false;
            }

            // If image loaded successful, set image data
            Width = w, Height = h, ComponentsCount = c;
            return true;
        }

        /**
         * Flip image along X axis function.
         * 
         * \param None.
         * \return None.
         */
        void FlipHorizontaly()
        {
            u32 row_size = Width * ComponentsCount;
            u8 *tmp = new u8[row_size];
            for (u64 i = 0; i < Height / 2; ++i)
            {
                std::memmove(tmp, &Data[i * row_size], row_size);
                std::memmove(&Data[i * row_size], &Data[(Height - i - 1) * row_size], row_size);
                std::memmove(&Data[(Height - i - 1) * row_size], tmp, row_size);
            }
            delete[] tmp;
        }
    };
}
