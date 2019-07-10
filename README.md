# Qt-WritingBoard
Qt实现画板，支持多点画线，线条平滑、无折线，重点体现书写效率。

支持两种效果：矢量和非矢量绘图方式，在wbcommondef.h中通过宏DRAW_VECTOR来控制模式切换

通过双缓冲方式实现绘制。

支持擦除效果。

在矢量模式下，如果线条很多，擦除会有些卡顿，这是在计算path相减的过程比较慢导致的，非矢量模式下不会慢。

快捷键控制：

- 按键1：画线模式
- 按键2：擦除模式
- 按键3：清空画布
- 按键+：放大
- 按键-：缩小

矢量和非矢量最大的区别在于：
1.矢量图缩放后不会模糊，而非矢量图缩放后会模糊。

2.线条多的情况下，矢量图擦除会比较慢，非矢量图不会影响。

效果如下：

<img src="https://github.com/luoyayun361/Qt-WritingBoard/blob/master/GIF.gif"/>


博客介绍：https://blog.csdn.net/luoyayun361/article/details/93650003


