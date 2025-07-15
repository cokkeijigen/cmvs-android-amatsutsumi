# Amatsutsumi | アマツツミ | 天津罪 Android 中文本地化
![Image text](https://raw.githubusercontent.com/cokkeijigen/amatsutsumi_android/master/pictures/img_amatsutsumi_00.png)<br>
> 游戏详细：https://vndb.org/v18852  https://vndb.org/r100211 <br>
> 使用的库：https://github.com/jmpews/Dobby <br>
> 所需工具：[Android Studio](https://developer.android.com/) [MT管理器](https://mt2.cn/) [RxCMVS](https://github.com/ZQF-ReVN/RxCMVS) <br>

该项目编译后的`apk`内包含`arm64-v8a`, `armeabi-v7a`, `x86`, `x86_64`版本的`.so`文件，主要功能为让游戏支持GBK编码文本，需要手动修改游戏apk让其加载。
![Image text](https://raw.githubusercontent.com/cokkeijigen/amatsutsumi_android/master/pictures/img_amatsutsumi_01.png)<br>
> 主要实现原理不难，hook游戏函数`mbstowcs_one` 和`ApiSjisCheck`即可。
> ```cpp
> static auto call_type mbstowcs_one_hook(uint16_t chars) -> wchar_t
> {
>     auto u16char { gbk2utf16::query((chars & 0xFF) << 8 | (chars >> 8)) };
>     if(u16char != 0xFFFF)
>     {
>         return u16char;
>     }
>     return hooker::call<mbstowcs_one_hook>(chars);
> }
>
> static auto call_type api_sjis_check_hook(uint8_t achar) -> int
> {
>     return static_cast<int>(achar > 0x7F);
> }
> ```
>
> 完整代码：[native-lib.cpp](https://github.com/cokkeijigen/Amatsutsumi-Android/blob/master/amatsutsumi/app/src/main/cpp/native-lib.cpp)
## 0x01 如何让游戏加载`libamatsutsumihook.so`
首先使用`Android Studio`编译成`apk`，然后使用`MT管理器`打开我们编译好的`apk`中`dex`，来到`Amatsutsumi`类中，将`loadLibrary`相关的`smali`代码复制。
![Image text](https://raw.githubusercontent.com/cokkeijigen/amatsutsumi_android/master/pictures/img_amatsutsumi_02.png)<br>
接着打开游戏的游戏`apk`中的`dex`，找到`amatsutsumi`类，将前面复制的代码粘贴进去。
![Image text](https://raw.githubusercontent.com/cokkeijigen/amatsutsumi_android/master/pictures/img_amatsutsumi_03.png)<br>
接着将我们自己编译的`apk`中的`lib`整个文件夹添加游戏的`apk`中
![Image text](https://raw.githubusercontent.com/cokkeijigen/amatsutsumi_android/master/pictures/img_amatsutsumi_04.png)<br>
最后使用`MT管理器`重新签名游戏`apk`即可。
