#pragma once 
#include "Types.h"
#include "ECS.h"

#include <bitset>

namespace xenon
{

    class registry;
    
    class SystemInterface
    {
    public:
        SystemInterface(XenonArchetype set)
            : m_Archetype(set)
        {}

        virtual void OnSystemUpdate(registry* reg, XenonID entity) = 0;

    protected:
        XenonArchetype m_Archetype;
    };

}