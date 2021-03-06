/*
 * Copyright(c): 2017 - 2019 ,NJU
 * File name:	MyCalculator
 * Description:	 Calculate a mathematical expression, variance, one-dollar definite integral, and solving equation.
 * Author: Liu Zechao
 * Version: 2.9.1
 * Date: 2017/6/10
 * History:
 * version 1.0   2017.4.11 18:35
 * version 1.1   2017 4.16 0:11 改进了OprandType String_to_Oprand函数， 增加了乘方 ^ 功能 和 根式 r 功能 前两天已改进只能算一位数的问题
 * version 1.1.1 2017 4.18 20:46 改进了 乘方 ^ 与括号() 的先后运算关系
 * version 1.2   2017 4.20 23:02 加入了基本三角函数运算
 * version 1.2.1 2017 4.23 17:47 改进了三角函数运算时可能出现的默认运算问题  如 9cos0
 * version 1.3   2017 4.26 20:17 改进输入格式，使之允许出现space及tab，还有括号等问题，下一步可以考虑使之筛选错误字符，留下正确表达式
 * version 1.3.1 2017 5.4  20:22 加入 PI ，改进 String_Handle 函数，减少了在其他函数中转化大小写的累赘
 * version 1.4   2017 5.4  21:29 定义了_stack_initial和_stack1_initial 消除野指针隐患
 * version 1.5   2017 5.4  23.52 定义了 E , 加入了 lg 和 ln 运算(注意真数不能为负)。改进了 字符存入时的一些小问题，例如 使得ln(e)可以直接写为lne.
 * version 1.6   2017 5.5  19:48 改进了 String_Handle 函数，使能处理诸如4*(-2), cos(+1),这样的 '+' '-'作为正负号使用的情况，改进方法是补零
 * version 1.7   2017 5.8  16:00 加入了绝对值符号 |x|  改进了部分变量的命名， 但是引入了一个全局变量flag（分辨绝对值符号的前后，如果不想要全局变量，可以用[x] 来代替|x|）.对于cos|2| 2^|2|这样的表达式，默认为错误 
 * version 1.8   2017 5.8  16:55 加入了阶乘符号 ！，对于可能存在的溢出现象，还没想好用什么方式来提示
 * verison 1.9   2017 5.8  20:40 将绝对值符合改为 [x] 使之能够嵌套。若用原有的格式进行嵌套，代码不容易写。
 * version 2.0   2017 5.9  18:26 加入了整除符合 x % y ，注意此符合不限于操作整数
 * version 2.1   2017 5.9  18:53 加入了三个反三角函数 asin, acos, atan.
 * version 2.1.1 2017 5.9  23:31 输出格式改为 %g ,看起来比较厉害！
 * version 2.2   2017 5.10 0:08  加入了记忆功能，把stack的声明和定义分开，比较规范。
 * version 2.2.1 2017 5.10 1:03  修改界面，显示基本信息，待完善，加入积分之后，要显示菜单界面
 * version 2.3   2017 5.10 23:44 加入菜单界面，主函数中增强了用户体验，每次可以停留在积分或计算界面直到键入空行。将主要中文修改为英文，比较厉害。
 * version 2.3.1 2017 5.11 1:14  加入了ShowSample函数，显示部分可能的输入及其理论上的输出
 * version 2.3.2 2017 5.11 23:22 将各个函数分放在不同的cpp文件中。改进了一点点输入和各个show函数的内容
 * version 2.3.3 2017 5.17 23:14 移植到 vs2017 ，将 main 函数里的 gets 函数都改为 gets_s 后，其余部分不变，可以运行
 * version 2.4   2017 5.19 21:00 加入了计算方差和标准差的功能，并随之修改了一些 ShowHelp 函数的输出。但需要在末尾加入@或$，不是很方便。
 * version 2.5   2017 5.27 16:42 加入了进制转换， 在输入的整数后加入h、o、b表示输入为相应的进制数。
 * version 2.6   2017 6.1  17:05 加入了解方程功能，然而并没有什么卵用，精度不够、无法处理某些可能存在的逻辑。 由于没有合适的算法，因此无法找到合适的ans的遍历范围及精度，另外double类型的数要追求精度也不容易
 * version 2.7   2017 6.2  18:21 修改了所有报错机制（通过EvaluateExpression等函数的flag参数），使之不会陷入死循环，并随之修改了main函数中相关的显示界面，且对show系列函数做了相应的修改，在menu中可以输入表达式来选择功能（这个虽然没什么用，但却导致可以输入q和quit来退出程序，还是挺好的）。解方程的功能仍需进一步完善。界面颜色由F1莫名跳回F9，相当妙。
 * version 2.7.1 2017 6.9  0:04  加入了rand，范围是1-6，便于掷骰子，是不是很厉害。
 * version 2.7.2 2017 6.9  0:21  修改了string_handle函数，使之能够处理诸如 3 ** 4 ,这样的输入，并有报错。
 * version 2.8   2017 6.9  1:31  修改了string_handle函数，使之能够处理诸如 3*-4 , 5 % -3sin(pi/2) 这样的输入。
 * version 2.8.1 2017 6.10 0:43  修改了阶乘的判断，能够计算 0！，规范了部分函数的注释，修改了EvaluateExpression()中部分多余参数，并在calculator.h中宏定义了Element（char），比较规范
 * version 2.8.2 2017 6.10 1:19  把 main 函数用到的函数的声明转移到 calculator.h 中（不知是否正确？），规范了 main.cpp文件中部分注释的格式 
 * version 2.9   2017 6.10 10:03 修改了variance相关函数和功能，使之能够处理大部分特殊情况，规范了除stack.cpp外所有函数的外注释
 * version 2.9.1 2017 6.10 14:34 规范了所有头文件和源文件的注释，具体函数的注释需进一步完善，新建了readme.text,把程序说明移到此文件中，把功能的标号宏定义了，把 char 换成了 OpcharType ，修改了main函数中的部分变量名,把版本号宏定义了
 * version 2.9.2 2017 6.10 21:42 把函数外注释改为英文，修改了一些源文件里函数的声明位置
 * version 3.0   2017 6.11 0:11  完善了大部分报错机制，为此修改了precede()和stringhandle()函数，并由此修改了引用它们的其他函数。修改了阶乘的部分内容
 * version 3.0.1 2017 6.11 14:47 给 calculator.h 中的宏定义增加了部分注释，利用c++重载的特性把两种栈的操作函数名改为一致
 * version 3.1   2017 6.11 15:12 加入了求商符号 '\', 在 ShowSample() 函数里增加了 order 变量，便于修改序号
 */

//历史遗留问题
//好像按 右方向键 和 上方向键 能重复上一次的输入？什么原理？ 加入 help 里
//最好再完整地测试一遍 stack.cpp
//考虑建立一个新文本，保存每次计算的信息（使用时间、输入的表达式、计算结果、版本号等），这就要用文件的知识了，还得建立链表啥的
//考虑加入复数
//考虑加入矩阵
//在使用 ctype.h 里的函数 isdigit时 有时在某些地方会报错  expression:(unsigned)(c+1) <= 256 具体原因未知

