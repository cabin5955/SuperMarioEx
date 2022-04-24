//
//  touch_handler.hpp
//  BreakoutEx
//
//  Created by chen caibin on 2021/12/29.
//

#ifndef touch_handler_hpp
#define touch_handler_hpp

#include "touch_delegate_protocol.h"

class  TouchHandler
{
public:
    virtual ~TouchHandler(void);

    /** delegate */
    TouchDelegate* getDelegate();
    void setDelegate(TouchDelegate *pDelegate);

    /** priority */
    int getPriority(void);
    void setPriority(int nPriority);

    /** enabled selectors */
    int getEnabledSelectors(void);
    void setEnalbedSelectors(int nValue);

    /** initializes a TouchHandler with a delegate and a priority */
    virtual bool initWithDelegate(TouchDelegate *pDelegate, int nPriority);

public:
    /** allocates a TouchHandler with a delegate and a priority */
    static TouchHandler* handlerWithDelegate(TouchDelegate *pDelegate, int nPriority);

protected:
    TouchDelegate *m_pDelegate;
    int m_nPriority;
    int m_nEnabledSelectors;
};

#endif /* touch_handler_hpp */
