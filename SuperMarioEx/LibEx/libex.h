//
//  libex.h
//  SuperMarioEx
//
//  Created by chen caibin on 2022/5/18.
//

#ifndef libex_h
#define libex_h

typedef enum {
    ExKeyEscape = 0x1b,
    ExKeyLeft = 0x25,
    ExKeyUp = 0x26,
    ExKeyRight = 0x27,
    ExKeyDown = 0x28
}   ExKeyCode;

typedef enum {
    ExActionPressed,
    ExActionRepeated,
    ExActionReleased,
} ExKeyAction;

#endif /* libex_h */
