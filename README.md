# Qt-WritingBoard
Qt实现画板，支持多点画线，线条平滑、无折线，重点体现书写效率。

支持两种效果：矢量和非矢量绘图方式，在wbcommondef.h中通过宏DRAW_VECTOR来控制模式切换

通过双缓冲方式实现绘制。

支持擦除效果。

在矢量模式下，如果线条很多，擦除会有些卡顿，这是在计算path相减的过程比较慢导致的，非矢量模式下不会慢。

效果如下：

<img src="https://github.com/luoyayun361/Qt-WritingBoard/blob/master/GIF.gif"/>


