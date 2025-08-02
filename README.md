# Amatsutsumi | アマツツミ | 天津罪 Android 中文本地化
![Image text](https://raw.githubusercontent.com/cokkeijigen/amatsutsumi_android/master/pictures/img_amatsutsumi_00.png)<br>
> 游戏详细：https://vndb.org/v18852  https://vndb.org/r100211 <br>
> 使用的库：https://github.com/jmpews/Dobby <br>
> 所需工具：[Android Studio](https://developer.android.com/studio) [MT管理器](https://mt2.cn/) [RxCMVS](https://github.com/ZQF-ReVN/RxCMVS) <br>

该项目编译后的`apk`内包含`arm64-v8a`, `armeabi-v7a`, `x86`, `x86_64`版本的`.so`文件，主要功能为让游戏支持`GBK编码文本`以及`可指定默认字体（修复Android15+字体无法加载的BUG）`，需要手动修改游戏apk让其加载。
![Image text](https://raw.githubusercontent.com/cokkeijigen/amatsutsumi_android/master/pictures/img_amatsutsumi_01.png)<br>

> 让游戏支持GBK其实很简单，只需要hook游戏函数`mbstowcs_one` 和`ApiSjisCheck`即可。
> ```cpp
> auto mbstowcs_one::func(uint16_t chars) -> wchar_t
> {
>     bool is_sjis_chars
>     {
>         /* ♪・～－ 在GBK中没有的SJIS特殊符号 */
>         chars == 0xF481 || chars == 0x4581 ||
>         chars == 0x6081 || chars == 0x7C81
>     };
>     if(!is_sjis_chars)
>     {
>         auto u16char { gbk2utf16::query((chars & 0xFF) << 8 | (chars >> 8)) };
>         if(u16char != 0xFFFF)
>         {
>             return u16char;
>         }
>     }
>     return hooker::call<mbstowcs_one::func>(chars);
> }
> 
> auto api_sjis_check::func(uint8_t achar) -> int
> {
>     return static_cast<int>(achar > 0x7F);
> }
> ```
>
> 完整代码：[amatsutsumi.cpp](https://github.com/cokkeijigen/Amatsutsumi-Android/blob/master/amatsutsumi/app/src/main/cpp/src/amatsutsumi.cpp)
## 0x01 如何让游戏加载`libamatsutsumihook.so`
首先使用`Android Studio`编译成`apk`，然后使用`MT管理器`打开我们编译好的`apk`中`dex`，来到`Amatsutsumi`类中，将`loadLibrary`相关的`smali`代码复制。
![Image text](https://raw.githubusercontent.com/cokkeijigen/amatsutsumi_android/master/pictures/img_amatsutsumi_02.png)<br>
接着打开游戏的游戏`apk`中的`dex`，找到`amatsutsumi`类，将前面复制的代码粘贴进去。
![Image text](https://raw.githubusercontent.com/cokkeijigen/amatsutsumi_android/master/pictures/img_amatsutsumi_03.png)<br>
接着将我们自己编译的`apk`中的`lib`整个文件夹添加游戏的`apk`中
![Image text](https://raw.githubusercontent.com/cokkeijigen/amatsutsumi_android/master/pictures/img_amatsutsumi_04.png)<br>
最后使用`MT管理器`重新签名游戏`apk`即可。

## 0x02 更改默认字体
打开游戏数据目录下的`cmvs.cfg` 编辑即可。
```cfg
[USE_DEFAULT_FONT]
PATH=./font/SourceHanSerifCN-SemiBold.ttf
```
![Image text](https://raw.githubusercontent.com/cokkeijigen/amatsutsumi_android/master/pictures/img_amatsutsumi_05.png)<br>
![Image text](https://raw.githubusercontent.com/cokkeijigen/amatsutsumi_android/master/pictures/img_amatsutsumi_06.png)<br>
这里更改为你要替换字体的路径![Image text](https://raw.githubusercontent.com/cokkeijigen/amatsutsumi_android/master/pictures/img_amatsutsumi_07.png)<br>
