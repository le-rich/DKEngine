#pragma once

#include <vector>
#include <stdexcept>

enum VertexAttributeKeys
{
    DEFAULT = 0,
    POSITION,
    NORMAL,
    TEXCOORD_0,
    TANGENT
};

inline VertexAttributeKeys GetVertexAttributeFromString(std::string pVertexAttribute)
{
    if (pVertexAttribute.compare("POSITION") == 0) return VertexAttributeKeys::POSITION;
    if (pVertexAttribute.compare("NORMAL") == 0) return VertexAttributeKeys::NORMAL;
    if (pVertexAttribute.compare("TEXCOORD_0") == 0) return VertexAttributeKeys::TEXCOORD_0;
    if (pVertexAttribute.compare("TANGENT") == 0) return VertexAttributeKeys::TANGENT;
    return VertexAttributeKeys::DEFAULT;
}


struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    VertexBufferElement(unsigned int t, unsigned int c, bool n)
        : type(t), count(c), normalized(n)
    {}

    static unsigned int GetSizeOfType(unsigned int type)
    {
        switch (type)
        {
        case GL_FLOAT:			return 4;
        case GL_UNSIGNED_INT:	return 4;
        case GL_UNSIGNED_BYTE:	return 1;
        default:                return 0;
        }
    }
};

class VertexBufferLayout
{
private:
    std::vector<VertexBufferElement> mElements;
    unsigned int mStride;

public:
    VertexBufferLayout()
        : mStride(0)
    {}

    template<typename T>
    void Push(unsigned int count)
    {
        std::runtime_error(false);
    }

    template<>
    void Push<float>(unsigned int count)
    {
        mElements.push_back({ GL_FLOAT, count, GL_FALSE });
        mStride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
    }

    template<>
    void Push<unsigned int>(unsigned int count)
    {
        mElements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
        mStride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
    }

    template<>
    void Push<unsigned char>(unsigned int count)
    {
        mElements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
        mStride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
    }

    inline const std::vector<VertexBufferElement> GetElements() const& { return mElements; }
    inline unsigned int GetStride() const { return mStride; }
};
