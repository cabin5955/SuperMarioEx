//
//  object.hpp
//  BreakoutEx
//
//  Created by chen caibin on 2021/12/29.
//

#ifndef object_hpp
#define object_hpp

class Object
{
public:
    // object id, ScriptSupport need public m_uID
    unsigned int        m_uID;
    // Lua reference id
    int                 m_nLuaID;
protected:
    // count of references
    unsigned int        m_uReference;
    // count of autorelease
    unsigned int        m_uAutoReleaseCount;
public:
    Object(void);
    /**
     *  @lua NA
     */
    virtual ~Object(void);
};

#endif /* object_hpp */
