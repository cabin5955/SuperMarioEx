//
//  object.cpp
//  BreakoutEx
//
//  Created by chen caibin on 2021/12/29.
//

#include "object.hpp"

Object::Object(void)
: m_nLuaID(0)
, m_uReference(1) // when the object is created, the reference count of it is 1
, m_uAutoReleaseCount(0)
{
    static unsigned int uObjectCount = 0;

    m_uID = ++uObjectCount;
}

Object::~Object(void)
{
    
}
