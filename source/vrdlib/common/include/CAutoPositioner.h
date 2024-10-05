#pragma once

#include "CDescriptor.h"

#include <cstdint>

namespace VRD
{
    namespace Utility
    {
        struct CStreamReader;
    }

    /** Moves to descriptor payload position on construction
        and moves to position + size on destruction.
    */
    struct CAutoPositioner
    {
        /** \param reader to be used
            \param descriptor for the block to be processed
        */
        CAutoPositioner(Utility::CStreamReader &reader, CDescriptor const &descriptor);

        /** Moves the reader to the end of the block using information
            from descriptor.
        */
        ~CAutoPositioner();

        /** Increments stored size value by given value.
            This can be useful when variable size blocks
            are found and the size of the block is bigger
            than expected initially.

            \param value to increment the internally stored size value.
        */
        void addSize(uint32_t value);

    private:
        Utility::CStreamReader &m_reader;
        CDescriptor const &m_descriptor;
        size_t m_variableSize;
    };
}
