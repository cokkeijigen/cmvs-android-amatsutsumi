# Amatsutsumi | アマツツミ | 天津罪 Android 中文本地化
> 游戏详细：https://vndb.org/v18852  https://vndb.org/r100211
> 
> 准备工具：[Android Studio](https://developer.android.com/)、[MT管理器](https://mt2.cn/)

该项目编译后的`apk`内包含`arm64-v8a`, `armeabi-v7a`, `x86`, `x86_64`的`.so`文件，主要功能为让游戏支持GBK编码文本，需要手动修改游戏apk让其加载。

## 0x01 如何让游戏加载`libamatsutsumihook.so`
首先使用`Android Studio`编译成`apk`，然后使用`MT管理器`打开我们编译好的`apk`中`dex`，来到`Amatsutsumi`类中，将`loadLibrary`相关的`smali`代码复制。
![Image text](https://raw.githubusercontent.com/cokkeijigen/amatsutsumi_android/master/pictures/img_amatsutsumi_01.png)<br>
接着打开游戏的游戏`apk`中的`dex`，找到`amatsutsumi`类，将前面复制的代码粘贴进去。
![Image text](https://raw.githubusercontent.com/cokkeijigen/amatsutsumi_android/master/pictures/img_amatsutsumi_02.png)<br>
接着将我们自己编译的`apk`中的`lib`整个文件夹添加游戏的`apk`中
![Image text](https://raw.githubusercontent.com/cokkeijigen/amatsutsumi_android/master/pictures/img_amatsutsumi_03.png)<br>
最后使用`MT管理器`重新签名游戏`apk`即可。
