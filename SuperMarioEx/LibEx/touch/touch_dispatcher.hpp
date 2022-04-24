//
//  TouchDispatcher.hpp
//  BreakoutEx
//
//  Created by chen caibin on 2021/12/29.
//

#ifndef TouchDispatcher_hpp
#define TouchDispatcher_hpp

#include "object.hpp"
#include "touch_handler.hpp"
#include <vector>
#include <map>

typedef enum
{
    TouchTouchSelectorBeganBit = 1 << 0,
    TouchTouchSelectorMovedBit = 1 << 1,
    TouchTouchSelectorEndedBit = 1 << 2,
    TouchTouchSelectorCancelledBit = 1 << 3,
    TouchTouchSelectorAllBits = ( TouchTouchSelectorBeganBit | TouchTouchSelectorMovedBit | TouchTouchSelectorEndedBit | TouchTouchSelectorCancelledBit),
} TouchTouchSelectorFlag;


enum {
    TOUCHBEGAN,
    TOUCHMOVED,
    TOUCHENDED,
    TOUCHCANCELLED,
    
    TouchTouchMax,
};


struct TouchTouchHandlerHelperData {
    int  m_type;
};

/**
 * @js NA
 */
class  EGLTouchDelegate
{
public:
    /**
     * @lua NA
     */
    virtual void touchesBegan(double x, double y) = 0;
    /**
     * @lua NA
     */
    virtual void touchesMoved(double x, double y) = 0;
    /**
     * @lua NA
     */
    virtual void touchesEnded(double x, double y) = 0;
    /**
     * @lua NA
     */
    virtual void touchesCancelled(double x, double y) = 0;
    
    
    virtual void touchesMovedOffset(double x, double y) = 0;
    /**
     * @lua NA
     */
    virtual ~EGLTouchDelegate() {}
};

class  TouchDispatcher : public Object, public EGLTouchDelegate
{
private:
    TouchDispatcher(){
        dispatchEvents = true;
    }
    TouchDispatcher(TouchDispatcher&)=delete;
    TouchDispatcher& operator=(const TouchDispatcher&)=delete;
    static TouchDispatcher* m_instance_ptr;
    
public:
   
    ~TouchDispatcher();
    
    bool init(void);
    
    static TouchDispatcher* get_instance(){
            if(m_instance_ptr==nullptr){
                  m_instance_ptr = new TouchDispatcher;
            }
            return m_instance_ptr;
        }

public:
    /** Whether or not the events are going to be dispatched. Default: true */
    bool isDispatchEvents(void);
    void setDispatchEvents(bool bDispatchEvents);

    /** Adds a targeted touch delegate to the dispatcher's list.
     * See TargetedTouchDelegate description.
     * IMPORTANT: The delegate will be retained.
     * @lua NA
     */
    void addTargetedDelegate(int scene, TouchDelegate *pDelegate, int nPriority);

    /** Removes a touch delegate.
     * The delegate will be released
     * @lua NA
     */
    void removeDelegate(TouchDelegate *pDelegate);

    /** Removes all touch delegates, releasing all the delegates
     * @lua NA
     */
    void removeAllDelegates(void);

    /** Changes the priority of a previously added delegate. The lower the number,
     * the higher the priority
     * @lua NA
     */
    void setPriority(int nPriority, TouchDelegate *pDelegate);
    
    virtual void touchesBegan(double x, double y);
    /**
     * @lua NA
     */
    virtual void touchesMoved(double x, double y);
    /**
     * @lua NA
     */
    virtual void touchesEnded(double x, double y);
    /**
     * @lua NA
     */
    virtual void touchesCancelled(double x, double y);
    
    virtual void touchesMovedOffset(double x, double y);

protected:
    std::map<int,std::vector<TouchHandler*>> touchHandlers;
    bool dispatchEvents;
    
private:
    void addHandler(int sceneIndex,TouchHandler* handler);
    
};

#endif /* TouchDispatcher_hpp */
