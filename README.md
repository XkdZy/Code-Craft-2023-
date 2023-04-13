# Code-Craft-2023-
2023华为软件精英挑战赛（幺鸡杠上花）
====================================================================
* 核心知识：BFS、DFS（递归）、贪心、STL（优先队列）
* 初赛：25/651 复赛：12/32  

快速开始  
----------------------------
* 测试环境：Windows10、VS2019
* [视频](https://github.com/XkdZy/Code-Craft-2023-/blob/main/image/video.mp4)
* 1、克隆./demo文件夹到本地
* 2、双击运行./demo/demo.bat文件

初赛  
----------------------------
* 任务：  
>>选手程序操控 4 个机器人执行前进、后退、旋转、购买、出售等动作来完成物品递送任务，同时赚取差价获得利润。在运行结束时，选手拥有的资金数即为最终分数，所获得的资金越高越好。

![image](https://github.com/XkdZy/Code-Craft-2023-/blob/main/image/map.png)

* 解题思路：  
>>货物的利润不但取决于利润还受到到工作台的时间和碰撞，想要最终的利润最大可以使机器人完成当前对于其当前最优（或单位收益最大）的任务（局部最优）或者一开始就自适应求出所有的最优的任务（全局最优）。两者各有各的优势前者计算速度快但不一定最优，后者收益最大但实现困难。前者相当于贪心思想，后者相当于对于给定地图每一个机器人就可以唯一确定计算出一条很长的任务链。但是要计算出最优任务链不是易事并且控制也不一定是按照理论计算完全一致。  

* 策略
>>最终考虑对于每一帧计算最长三次买卖任务即：1(买)->4(卖)->4(买)->7(卖)->7(买)->8(卖)，并且认为买卖必须绑定，如果买了卖不出去一定是亏损的。这样遍历一遍地图就可以得到一个任务池，任务池中每一个就代表一条任务链表示当前机器人可以选择的有效任务。从工作台到工作台的有向无环图到任务池转换如下图所示：

![image](https://github.com/XkdZy/Code-Craft-2023-/blob/main/image/taskpool.png)  

* 任务池排序规则
>>对于每一条任务有一定的优先级，比如买卖1和买卖4带来的收益不同，且同样是4卖给7，7原材料的缺失情况不同带来收益也是不同，所以对不同任务进行排序是必须。分别将任务分为三个大级，买卖1、2、3；买卖4、5、6；买卖7。针对同一级优先级任务先按照缺失原材料个数进行排序，比如第10、11号工作台都是4类型工作台，10缺类型1原材料而11没有原材料，故1（买）->10（卖）优先级比1（买）->11（卖）高。然后再按照单位距离收益进行排序，比如1（买）->4（卖）->4（买）->7（卖）和4（买）->7（卖）根据单位距离收益进行排序。具体排序规则如图所示：  

![image](https://github.com/XkdZy/Code-Craft-2023-/blob/main/image/sort.png)   

* 控制策略（开环控制）  

* 防碰撞（针对两机器人正面撞击情况）  

复赛  
-----------------------------------
* 任务：  
在初赛的基础上，新增障碍物，运行时间由3分钟增加至5分钟其余不变

* 策略：  
采用Dijkstra方法将每一个网格转化为一个顶点（两种状态：0空置；1占用），然后从起始顶点到目标顶点采用广度优先搜索求出一条无碰撞路径。    

总结
----------------------------------
初赛无障碍工作台选择策略、控制更加优先，碰撞次之，复赛更加考虑路径、防碰撞，策略、控制次之。初赛代码Bug应该没有太多，都是按照预期实现，复赛加上Dijkstra后任务池运行出现问题并且运行时间出现剧烈增加，掉帧情况严重；并且初赛没有考虑防碰撞复赛最后也没时间考虑。
