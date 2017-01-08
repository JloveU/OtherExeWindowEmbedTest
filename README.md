# OtherExeWindowEmbedTest

将其他exe界面嵌入自己的软件中。


**使用方法**

需要在软件相同目录放置一个Config.ini文件，里面的设置项如下：

```
[OtherExe]
filePathAndName=D:\\ProgramFiles\\PortablePrograms\\oCam_344.0_Portable\\oCam.exe
windowClassName=Hi! oCam
windowTitle=

[ThisWindow]
embedContainerWidth=520
embedContainerHeight=140
resizable=false
autoResizeEmbedContainer=false
title=

[Others]
millisecondsBetweenStartAndEmbed=3000
```

其中各项的含义如下：

```
[OtherExe]
filePathAndName: 待嵌入exe文件路径
windowClassName: 待嵌入exe窗口类名（可使用Visual Studio提供的工具Spy++获取该窗口的类名和标题。）
windowTitle: 待嵌入exe窗口标题

[ThisWindow]
embedContainerWidth: 内嵌容器的宽度
embedContainerHeight: 内嵌容器的高度
resizable: 是否可调整窗口大小
autoResizeEmbedContainer: 是否随窗口大小自动改变内嵌容器的大小
title: 本窗口标题

[Others]
millisecondsBetweenStartAndEmbed: 在待嵌入exe打开之后，嵌入之前的延迟时间（延时以保证该窗口已显示在顶层）
```


**参考：**

- <http://www.cnblogs.com/gaoxudong0704/p/5986730.html>
- <http://blog.csdn.net/jiaoyaziyang/article/details/49802993>
