/*****************************************************************//**
 * \file   vertex.h
 * \brief  Vertex description and storage classes definition module.
 * 
 * \author Sabitov Kirill
 * \date   28 June 2022
 *********************************************************************/

#pragma once

#include "render_context.h"

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

    /* Abstract base vertex structure. */
    struct vertex_base abstract
    {
        /**
         * Vertex layout, describing all vertex atributes getter function.
         * 
         * \param None.
         * \return vertex layout.
         */
        static vertex_layout GetVertexLayout()
        {
            return vertex_layout({});
        };
    };

    /* Default vertex structure. */
    struct vertex
    {
        vec3 Position {};    /* Vertex position. */
        vec3 Normal {};      /* Normal of surface in vertex position. */
        vec2 TexCoords {};   /* Vertex texture cordinate. */
        vec4 Color {};       /* Vecrtex color. */

        /* Vertex default constructor. */
        vertex() = default;

        /**
         * Veretex contructor by color.
         * It is implied that the texture will not be used.
         * 
         * \param Position - vertex position.
         * \param Normal - vertex surface normal.
         * \param Color - vertex color.
         */
        vertex(vec3 Position, vec3 Normal, vec4 Color) :
            Position(Position), Normal(Normal), Color(Color) {}

        /**
         * Veretex contructor by texture coordinates.
         * It is implied that the color attribute will not be used.
         * 
         * \param Position - vertex position.
         * \param Normal - vertex surface normal.
         * \param TexCoords - vertex texture coordinates.
         */
        vertex(vec3 Position, vec3 Normal, vec2 TexCoords) :
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
                { shader_variable_type::FLOAT2, "TexCoord" },
                { shader_variable_type::FLOAT4, "Color" },
            });
        };
    };

    /**
     * Vertex for tangent space structure.
     * class vertex_tspace : public vertex_base
     * {
     * };
     * 
     * Vertex for tangent space layout:
     *
     * return vertex_layout({
     *     { shader_variable_type::FLOAT3, "Position" },
     *     { shader_variable_type::FLOAT3, "Normal" },
     *     { shader_variable_type::FLOAT2, "TexCoord" },
     *     { shader_variable_type::FLOAT3, "Tangent" },
     *     { shader_variable_type::FLOAT3, "Bitangent" },
     * });
     */


}
