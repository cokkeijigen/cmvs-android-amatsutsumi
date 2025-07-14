# Amatsutsumi | アマツツミ | 天津罪 Android 中文本地化
> 游戏详细：https://vndb.org/v18852  https://vndb.org/r100211
>
> 使用到的工具：Android Studio、MT管理器

## 0x01 如何让游戏加载`libamatsutsumihook.so`
首先使用`Android Studio`编译成`apk`，然后使用`MT管理器`打开我们编译好的`apk`中`dex`，来到`Amatsutsumi`类中，将`loadLibrary`相关的`smali`代码复制。
![Image text](https://raw.githubusercontent.com/cokkeijigen/amatsutsumi_android/master/pictures/img_amatsutsumi_01.png)<br>
接着打开游戏的游戏`apk`中的`dex`，找到`amatsutsumi`类，将前面复制的代码粘贴进去。
![Image text](https://raw.githubusercontent.com/cokkeijigen/amatsutsumi_android/master/pictures/img_amatsutsumi_02.png)<br>