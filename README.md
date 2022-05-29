## SuperMarioEx
基于learnopengl.com代码在iOS平台上重新实现超级马里奥

### 上手指南
以下指南将帮助你在本地机器上安装和运行该项目，进行开发和测试。

### 安装要求
1. 操作系统：Mac OS X 10.12及以上
2. Xcode 9.0及以上

### 安装步骤
1. $ git clone https://github.com/cabin5955/SuperMarioEx.git
2. 直接使用Xcode打开
3. Xcode点击运行

### 测试
game目录下包含5个测试类（场景）
1. GamePlatformer (game_platformer.hpp文件):简单的跳台演示
2. GameB2Platformer (game_b2platformer.hpp文件)：加入Box2D的简单的跳台演示
3. GameMario (game_mario.hpp文件)：马里奥第一关地图演示
4. GameB2Mario (game_b2mario.hpp文件):加入Box2D马里奥第一关地图演示
5. GameTilemap (game_tilemap.hpp)：加载瓦片地图测试

可实现自己的场景类。
在GameWrapper类(game_wrapper.h)方法- (void)InitWidth:(GLfloat) width Height:(GLfloat) height;中进行场景类初始化，
并设置当前要测试的场景。

### 使用到的框架
+	stb - 纹理图片加载
+	glfm - 为移动设备和web创建并管理窗口和OpenGL上下文，同时还提供了简单的输入功能。
+	freetype2 - 字体
+	CocosDenshion - 声音播放
+	libtmx - 解析tmx文件（tmx由Tiled地图编辑软件生成）
+	Box2D - 物理引擎
+	dear imgui - 测试使用

### 作者
作者：cabin5955

### 版权说明
该项目签署了MIT 授权许可，详情请参阅LICENSE.md

### 鸣谢
+	Joey de Vries - learnopengl.com
+	Brent Aureli -  SuperMario Bros YouTube Tutorials
