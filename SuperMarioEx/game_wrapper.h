//
//  game_wrapper.h
//  BreakOutES
//
//  Created by chen caibin on 2021/10/29.
//

#include "glad.h"
#include <UIKit/UIKit.h>

@interface GameWrapper : NSObject
- (void)InitWidth:(GLfloat) width Height:(GLfloat) height;
- (void)KeyboardInputWhithKey:(int) key Pressed:(char)pressed;
- (void)TouchMoveOffsetX:(double)x OffsetY:(double)y;
- (void)TouchBeganPosX:(double)x PosY:(double)y;
- (void)TouchEndedPosX:(double)x PosY:(double)y;
- (void)TouchMovedPosX:(double)x PosY:(double)y;
- (void)Update:(GLfloat)dt;
- (void)Render;
- (void)Realease;
@end

