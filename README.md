### 2016亚马逊种子杯软件编程大赛代码

#### 说明
种子杯官网：[http://dian.hust.edu.cn/seedpk/?cat=15](http://dian.hust.edu.cn/seedpk/?cat=15)  
队伍名：有了金克拉，代码亩产一千八  
（试题见question文件夹）

#### 初赛代码
初赛题目要求是模拟程序运行，并且输出程序运行的行号，实际上就是要求实现一个简单的C语言编译器/解释器，所以最后的实现分为三个部分，词法分析、语法分析、解释执行。  
词法分析直接手写（不像某些大佬们自己造了个正则引擎），语法分析用的递归下降，最后生成一棵语法树交给解释器执行。


#### 决赛代码  
决赛题目是给出两份商品信息，判断是否为同一个商品。主要也是分为了三个部分：  
第一部分是分词系统，这里使用了[CppJieba](https://github.com/yanyiwu/cppjieba)库，将一段话的描述分割成数个单词，并且滤掉虚词，剩下的就是需要进行分析的关键词；  
第二部分是词语相似度的判断，例如判断Apple和apple，password和pa55word的相似度，用于判断两个词语是否是同一个；  
第三个是判断某一类商品信息（例如品牌，商品描述等）的相似度，一类商品信息中一般包括多个关键词，所以即判断两组关键词的相似度，这里可以使用多维余弦公式来计算相似度（然而比赛中并没有写完-_-||）。  
最后，根据每一类商品信息按照权重计算总的相似度，相似度大于特定值的就可以判断为相同商品了。  
