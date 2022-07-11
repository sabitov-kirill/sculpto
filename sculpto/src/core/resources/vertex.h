/*****************************************************************//**
 * \file   vertex.h
 * \brief  Vertex description and storage classes definition module.
 * 
 * \author Sabitov Kirill
 * \date   28 June 2022
 *********************************************************************/

#pragma once

#include "core/render/render_context.h"

namespace scl
{
    /* Vertex element structure. */
    struct vertex_element
    {
        /* Vertex element data. */
        shader_variable_type  Type;   /* Vertex element data type. */
        u32                   Size;   /* Vertex element data type size. */
        u32                   Index;  /* Vertex element index in vertex layer. */
        u32                   Offset; /* Vertex element offset by vertex layer start pointer. */

        /**
         * Vertex element default constrictor.
         *
         * \param Type - vertex element data type.
         * \param Index - vertex element index in vertex layer.
         * \param Offset - vertex element offset by vertex layer start pointer.
         */
        vertex_element(shader_variable_type Type, const std::string &Name) :
            Type(Type), Size(GetShaderVariableTypeSize(Type)), Index(-1), Offset(-1) {}
    };

    /* Vertex layout structure. Represent data for one single vertex. */
    class vertex_layout
    {
    protected: /* Vertex layout data. */
        std::vector<vertex_element> Elements {};
        u32 Size {};

    public: /* Vertex layout getter/setter functions. */
        /* Vertex layout elements getter function. */
        const std::vector<vertex_element> &GetElements() const { return Elements; }
        /* Vertex layout size (vertex size) getter function. */
        u32 GetSize() const { return Size; }
        /* Vertex layout elements count getter function. */
        size_t GetCount() const { return Elements.size(); }

        std::vector<vertex_element>::iterator begin() { return Elements.begin(); }
        std::vector<vertex_element>::iterator end() { return Elements.end(); }
        std::vector<vertex_element>::const_iterator begin() const { return Elements.begin(); }
        std::vector<vertex_element>::const_iterator end() const { return Elements.end(); }

    public:
        /* Vertex layout default constructor. */
        vertex_layout() = default;

        /**
         * Vertex layout default constructor.
         *
         * \param Elements - array of vertex layout elements.
         */
        vertex_layout(const std::initializer_list<vertex_element> &Elements) :
            Elements(Elements)
        {
            // Calculating index and offset of each element
            u32 offset = 0;
            for (int i = 0; i < Elements.size(); i++)
            {
                this->Elements[i].Index = i;
                this->Elements[i].Offset = offset;
                offset += this->Elements[i].Size;
            }
            Size = offset;
        }
    };

    /* Default vertex structure. */
    struct vertex
    {
        vec3 Position {};    /* Vertex position. */
        vec3 Normal {};      /* Normal of surface in vertex position. */
        vec3 Tangent {};     /* Vertex tangent space first vector. */
        vec3 Bitangent {};   /* Vertex tangent space second vector. */
        vec2 TexCoords {};   /* Vertex texture cordinate. */

        /* Vertex default constructor. */
        vertex() = default;

        /**
         * Veretex contructor by texture coordinates.
         * It is implied that the color attribute will not be used.
         * 
         * \param Position - vertex position.
         * \param Normal - vertex surface normal.
         * \param Tangent, Bitangent - vertex surface tangent space basis vectors.
         * \param TexCoords - vertex texture coordinates.
         */
        vertex(const vec3 &Position, const vec3 &Normal, const vec3 &Tangent, const vec3 &Bitangent, const vec2 &TexCoords) :
            Position(Position), Normal(Normal), TexCoords(TexCoords), Tangent(Tangent), Bitangent(Bitangent) {}

        /**
         * Veretex contructor by texture coordinates.
         * It is implied that the color attribute will not be used.
         *
         * \param Position - vertex position.
         * \param Normal - vertex surface normal.
         * \param TexCoords - vertex texture coordinates.
         */
        vertex(const vec3 &Position, const vec3 &Normal, const vec2 &TexCoords) :
            Position(Position), Normal(Normal), TexCoords(TexCoords) {}

        /**
         * Vertex layout, describing all vertex atributes getter function.
         *
         * \param None.
         * \return vertex layout.
         */
        static vertex_layout GetVertexLayout()
        {
            return vertex_layout({
                { shader_variable_type::FLOAT3, "Position" },
                { shader_variable_type::FLOAT3, "Normal" },
                { shader_variable_type::FLOAT3, "Tangent" },
                { shader_variable_type::FLOAT3, "Bitangent" },
                { shader_variable_type::FLOAT2, "TexCoord" },
            });
        };
    };

    /* Default vertex structure. */
    struct vertex_point
    {
        vec3 Position {};    /* Vertex position. */
        vec3 Color {};       /* Vertex color. */

        /* Vertex default constructor. */
        vertex_point() = default;

        /**
         * Veretex contructor by texture coordinates.
         * It is implied that the color attribute will not be used.
         *
         * \param Position - vertex position.
         * \param Normal - vertex surface normal.
         * \param TexCoords - vertex texture coordinates.
         */
        vertex_point(const vec3 &Position, const vec3 &Color) :
            Position(Position), Color(Color) {}

        /**
         * Vertex layout, describing all vertex atributes getter function.
         *
         * \param None.
         * \return vertex layout.
         */
        static vertex_layout GetVertexLayout()
        {
            return vertex_layout({
                { shader_variable_type::FLOAT3, "Position" },
                { shader_variable_type::FLOAT3, "Color" },
            });
        };
    };
}
