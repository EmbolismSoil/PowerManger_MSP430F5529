#软件设计思路：
* 总线上有两种优化器，一种是主机，一种是从机
* 主机的作用是管理功率分配，和采样显示。
* 从机负责上报自己的状态和接受来自主机的命令。
* 主机对系统整体负责，从机除了响应主机的命令之外，还需要自己完成过流过压保护功能。
* I2C 通信采取主动让出总线的策略，初始化时，每个控制器都处于从机接收模式，
如果需要往总线上发送数据，再切换成主机发送模式，一旦数据发送完毕，立即切换回
从机接收模式。

##数据结构设计
###包头结构
控制器之间以包的形式进行通信，每个数据包都包含一个包头和一个数据段。包头如下：  
<pre>typedef struct {
    1 源地址  
    2 目标地址
    3 包类型
    4 标志位
}IICPackge_Header_t;
</pre>

###命令包结构
主机只给从机发送为命令包，命令包结构如下：  
<pre>typedef struct{  
    IIC_Header_t;  
	struct{  
	  1 命令名字  
	  2 命令参数1  
	  3 命令参数2  
	  4 命令参数3  
	}Cmd;  
}IICPackge_Command_t;
</pre>
###结果反馈包结构
<pre>从机反馈回来的执行结果包如下:  
typedef struct{  
    IIC_Header_t  
	struct{  
	  1 命令名字  
	  2 返回参数1  
	  3 返回参数2  
	  4 返回参数3  
	}  
}IICPackge_Result_t;
</pre>
###状态包结构
从机只会给主机发送状态包  
<pre>typedef struct{  
    IICH_eader_t Header  
    struct{
        1 最大输入电流
        2 最大输入电压  
        3 等效内阻
        4 当前输入电流
        5 当前输入电压
        7 当前输出电流
        8 当前输出电压
        9 当前PWM占空比
    }Status;  
}IICPackge_Status_t;
</pre>

##工作流程
* 一. 开机初始化时每一个状态置为空，主机平分电压，然后对从机广播调节命令,发送的命令包内容如下:  
<pre>IICPackge_Command_t Cmd =
{
	.Header.源地址 = 主机地址,
	.Header.目标地址 = 广播地址,
	.Header.包类型 = 命令包,  
	.Header.标志位 = 0,  
	.Cmd.命令名字 = 初始化测试，  
	.Cmd.命令参数 = 2V,  
} 
</pre>

* 二. 发送完成后主机进入等待模式，直到全部从机测量完成，并且向主机发送状态包。  主机接收到所有从机的状态包之后开始正式进入工作状态。
  主机开始进行功率分配并且下达命令。  从机开始工作并且实时测试自己的状态，包括：
* 1  从机地址
* 2  主机地址
* 3  输入电流
* 4  输入电压
* 5  输出电流
* 6  输出电压
* 7  最大输出电流
* 8  最大输入电流
* 9  等效内阻
* 10 总线地址 

当状态发生变化时，给主机发送状态，主机收到后将加入一个链表中<div><div id="highlighter_55656" class="syntaxhighlighter  php"><table border="0" cellpadding="0" cellspacing="0"><tbody><tr><td class="gutter"><div class="line number1 index0 alt2">1</div></td><td class="code"><div class="container"><div class="line number1 index0 alt2"><code class="php plain">export PATH=</code><code class="php variable">$PATH</code><code class="php plain">:/your_pi_gcc_dir/tools-master/arm-bcm2708/arm-bcm2708hardfp-linux-gnueabi/bin/:/your_stm_gcc_dir/gcc-arm-none-eabi-4_8-2013q4/bin</code></div></div></td></tr></tbody></table></div></div>
