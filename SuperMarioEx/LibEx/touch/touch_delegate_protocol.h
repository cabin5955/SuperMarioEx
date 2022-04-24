//
//  TouchDelegateProtocol.h
//  BreakoutEx
//
//  Created by chen caibin on 2021/12/29.
//

#ifndef TouchDelegateProtocol_h
#define TouchDelegateProtocol_h

class TouchDelegate
{
public:

    TouchDelegate() {}

    virtual ~TouchDelegate()
    {
    }

    virtual bool TouchBegan(double x, double y) { return false;};
    // optional

    virtual void TouchMoved(double x, double y) { }
    virtual void TouchEnded(double x, double y) { }
    virtual void TouchCancelled(double x, double y) { }
    virtual void TouchMovedOffset(double x, double y) { }

    // optional
    virtual void TouchesMoved(double x, double y) { }
    virtual void TouchesEnded(double x, double y) { }
    virtual void TouchesCancelled(double x, double y) { }
};

#endif /* TouchDelegateProtocol_h */
