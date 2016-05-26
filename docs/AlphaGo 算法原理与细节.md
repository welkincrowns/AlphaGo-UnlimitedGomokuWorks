# AlphaGo 算法原理与实现

## Overview

主要思想：使用Monte Carlo tree search的方法，通过神经网络来减少搜索树的深度(depth)和宽度(breadth)。

主要有两种神经网络：policy network和value network。policy network用来模拟行为(原文中是sample actions)，value network用来评估胜率。

有四个神经网络，分别是：

- *SL policy network $p_\sigma$*：直接用人类专家的棋谱来训练，功能是：给定一个棋局，预测人类会怎么走，目标是提高预测的准确率。

- *fast policy network $p_\pi$*：和上述的网络一样，只不过比上述网络训练预测地更快，但是准确度有所下降。

- *RL policy network $p_\rho$*：在$p_\sigma$的基础上训练，通过增强学习(Reinforcement Learning)，达到"尽可能地获胜"这个目标(注意这里的目标和$p_\sigma$的不同)。功能是：给定一个棋局，返回怎么走最后更有可能赢。

- *value network $p_\theta$*：在$p_\rho$的基础上训练，功能是：给定一个棋局，预测如果让$p_\rho$自己和自己下，谁更有可能获胜。

## Neural Network Overview

### SL policy network $p_\sigma$

假设函数$p_\sigma(a|s)$表示给定一个棋盘状态$s$，预测人类采取行为$a$的概率。神经网络的结构是卷积层和rectifier nonliearities层相互交替，最后使用sotfmax分类器算出所有合法走棋$a$的概率分布，一共13层。其中梯度满足：

$$\Delta\sigma \propto \frac{\partial\log p_\sigma(a|s)}{\partial \sigma}$$

最后的结果是达到了$57\%$的准确率，如果仅给定最原始的棋盘，有$55.7\%$的正确率，预测一步需要3ms。

### fast policy nework $p_\pi$

通过人工提取的特征训练，直接使用线性的softmax分类器训练，准确率是$24.2%$，预测一步需要2us

### RL policy network $p_\rho$

使用policy gradient reinforcement learning训练。神经网络的结构和$p_\sigma$相同并且在一开始使用$p_\sigma$的$\sigma$作为weight的初始值(一开始$\rho = \sigma$)。训练中，让当前的神经网络$p_\rho$和他之前的某个版本$p_{\rho'}$打。收益函数(reward function)$r(s)$在$s$为最终结局前($t < T$)都是0，让$z_t=\pm r(s_T)$为最后的回馈，如果这一步是最后赢的人走的，那么$z_t = 1$，否则$z_t = -1$，对于每一步$t$都用SGA(stochasitc gradient ascent)来更新，梯度满足：

$$\Delta\rho \propto \frac{\partial\log p_\rho(a_t|s_t)}{\partial \rho}z_t$$

评估的时候，直接用$p_\rho$和别的AI打，$p_\rho$相对于$p_\sigma$有$80\%$的胜率，相对于Pachi有$85\%$的胜率。

### value network $v_\theta$

假设函数$v_\theta(s)=v^p_\theta(s)$表示用policy network的策略，对于棋局$s$(这里原文中是position，不是特别理解他的意思)，预测赢的概率。结构和policy network一样仅仅把最后的概率分布改成一个单一的值，目标最小化均方差，从而梯度满足：

$$\Delta\theta \propto \frac{\partial v_\theta(s)}{\partial \theta}(z - v_\theta(s))$$

为了避免overfitting，使用人工数据集，随机了30,000,000万个不同的棋局，让*RL network*自己和自己下。

## SL policy network $p_\sigma$

### 准备工作

#### 数据集

KGS data set，选取了6-9段人类选手160,000句棋局中的$29.4 \times 10^6$步。

分成测试集($1 \times 10^6$步)和训练集($29.3 \times 10^6$)。

每个数据包含了输入(棋盘局势$s$)和标记(人类选手的走棋选择$a$)。

对数据进行了旋转/对称变换。

#### 特征提取

由于使用Convolutional Neural Network，特征是一个$19 \times 19 \times 48$的cube，每一个slice($19 \times 19$)都有各自的含义，在表格(Extend Data Table 2)中可以看到。

### 训练

每一步，随机选择从训练集中选择$m$个样本$\{s^i, a^i\}_{i=1}^m$，使用异步随机梯度下降法来训练，最大化log likelihood，令梯度为：

$$\Delta\sigma = \frac{\alpha}{m} \sum_{i=1}^{m} \frac{\partial\log p_\sigma(a^i|s^i)}{\partial \sigma}$$

学习速率$\alpha$一开始为$0.003$并且训练$80\times 10^6$步减少一半

mini-batch size $m = 16$

使用50个GPU训练，训练了3周，一共训练了$340\times10^6$步。

### 神经网络结构

一共13层hidden layer。

*Input Layer*: $19 \times 19 \times 48$ 的提取的特征

*Hidden Layer 1*: $S = 1$(Stride), $F = 5$(Receptive Field), $P = 2$(Zero Padding), filter: $5 \times 5 \times 48 \times k$，一层的cube是$19 \times 19 \times k$

*Hidden Layer 2~12*: $S = 1$, $F = 3$, $P = 2$, filter: $3 \times 3 \times k \times k$，一层的cube是$19 \times 19 \times k$

*Hidden Layer 13*: $S = 1$, $F = 1$, $P = 0$, filter: $1 \times 1 \times k \times 1$，一层的cube是$19 \times 19 \times 1$

*Output Layer*: 13层到Output Layer之间的连接用Softmax，$19 \times 19$ 的概率分布。

卷积后使用relu作为rectifier nonlinearity，其中$k = 192$的效果最好。

## rollout policy

### 准备工作

#### 训练集

$8 \times 10^6$个来自于Tygem server的数据。

#### 特征值

用人工提取的特征值，具体看Extend Data Table 4。这里详细描述两种特征值：

- *NonResponse Pattern*:对于每个位置，求和它他相临点的一个pattern：每个pattern是一个$3 \times 3$的0/1特征值，表示(这个位置的3种状态)$\times$(这个位置的自由度($=1$,$=2$,$\ge3$))。

- *Response Patterns*:描述前一步的特征(a binary feature matching the colour and liberty count in a a12-point diamond-shaped pattern centred aroud the previous one)。

- 还有一些其他的人工的特征

### 训练

使用线性softmax模型即可，是一个$features.size() \times 381$的矩阵。

## RL policy network $p_\rho$

### 准备工作

#### 初始值

即SL policy network $p_\sigma$

### 训练

对于每一步，mini-batch并行地进行了$n$场对决。每场对决都是在当前的参数$\rho$和在对手池中的之前的某个参数$\rho'$展开。一开始全部是$\sigma$，每训练500次，就把当前的参数$\rho$放到对手池里。

每次更新的梯度如下：

$$\Delta\rho ＝ \frac{\alpha}{n} \sum_{i=1}^n\sum_{t=1}^{T_i} \frac{\partial\log p_\rho(a_t^i|s_t^i)}{\partial \rho}(z_t^i-v(s_t^i))$$

一开始先训练一遍，默认baseline$v(s_t^i)=0$，之后用$v_\theta(s)$作为baseline再训练一遍，效果能够得到较小的提高。

训练了10,000个mini-batch，$n = 128$，训练了一天

### 神经网络结构

和SL policy network $p_\sigma$一样。

## value network $v_\theta$

### 准备工作

#### 训练集

来自于用$p_\sigma$和$p_\rho$创建的$30\times 10^6$组人工数据。

对于每个数据，执行一下几步

- 随机一个数$U$(1~450)，前$U-1$步都用$p_\sigma$走

- 第$U$步随机走(随机找一个合法的)

- 接下来都用$p_\rho$走，一直走到完，算出$z_T$。

- 把$(s_{U+1}, z_{U+1})$放入数据集

### 训练

梯度如下：

$$\Delta\theta = \frac{\alpha}{m} \sum_{i=1}^m\frac{\partial v_\theta(s^i)}{\partial \theta}(z^i - v_\theta(s^i))$$

训练了$50 \times 10^6$个mini-batch，mini-batch size $m = 32$，训练了一周。

### 神经网络结构

一共13层hidden layer。

*Input Layer*: $19 \times 19 \times 48$ 的提取的特征

*Hidden Layer 1*: $S = 1$(Stride), $F = 5$(Receptive Field), $P = 2$(Zero Padding), filter: $5 \times 5 \times 48 \times k$，一层的cube是$19 \times 19 \times k$

*Hidden Layer 2~12*: $S = 1$, $F = 3$, $P = 2$, filter: $3 \times 3 \times k \times k$，一层的cube是$19 \times 19 \times k$

*Hidden Layer 13*: $S = 1$, $F = 1$, $P = 0$, filter: $1 \times 1 \times k \times 1$，一层的cube是$19 \times 19$

*Hidden Layer 14*: 是一个普通的线性层，有$256$个神经元，与上一层用relu激励函数全连接。

*Output Layer*: 一个整数表示赢的概率，14层到Output Layer之间的连接用tanh作为激励函数。

卷积后使用relu作为rectifier nonlinearity，其中$k = 192$的效果最好。

## Monte Carlo tree search Algorithm

### Overview

考虑Monte Carlo tree上的每一个点，都表示一个棋局的状态$s$，每条边都可以用$(s, a)$来表示，其中$a$是一个合法的走棋，Monte Carlo tree search的算法分为4个部分。

***Selection***: 对于当前的点，如果不是叶子节点，考虑下一步的模拟(sampling)该选哪一步，基于UCT算法的改良，AlphaGo选择$Q(s, a) + u(s, a)$最大的那个，一直往下走到叶子节点$s_L$**(此时这个节点还不在树中，但是这个节点的父亲在树中)**为止。其中$u(s, a) = c_{puct}P(s, a)\frac{\sum_{b}N(s,b)}{1+N(s,a)}$，$P(s, a)=p_\sigma(a|s)$ 用SL Policy Network $p_\sigma$来算，$N(s,a)$表示$(s,a)$这条边参加sampling的次数。

***Evaluation***: 从叶子节点$s_L$开始，接下来用$p_\pi$来模拟双方落子行为，算出最后的输赢情况$z_L$，然后计算出叶子节点在这一轮模拟中的值$V(s_L^i)=(1-\lambda)v_\theta(s_L) + \lambda z_L$。

***Backup***: 向上更新所有的祖先的$Q$值和$N$值。其中$N(s, a) = \sum_{i=1}^n[(s,a,i)]$，$Q(s, a) = \frac{1}{N(s,a)}\sum_{i=1}^n{[(s,a,i)] \cdot V(s_L^i)}$，其中$[(s,a,i)]$为$1$当且仅当在第$i$轮模拟中经过了$(s, a)$这条边。

***Expansion***: 如果某条边$(s, a)$的访问次数超过了阀值，就把$s'= f(s,a)$加入到搜索树中来。

在搜索完成之后，AlphaGo会选择走$N(s, a)$最大的那个$a$，保留$s'=f(s, a)$子树的内容，然后释放其他所有节点的内存。在对方走的时候AlphaGo也会运算，并且会在胜率小于$10\%$的时候弃权。

### Implement Details

具体实现中为了加快速度使用了多线程，在MCTS中的每个节点$s$都包含由**所有**合法操作$a$组成的边$(s,a)$，对于每条边，都需要记录一下数据：

- $P(s, a)$ 即Overview中的$P(s, a)$
- $N_v(s, a)$ 即Overview中的$N(s, a)$，这里细化指的是在这棵子树中计算过的$v_\theta(s)$的次数。
- $N_r(s, a)$ 即Overview中的$N(s, a)$，这里细化指的是在这棵子树中计算过的rollout的次数。
- $W_v(s, a)$ 指的是在这棵子树中计算过的$v_\theta(s)$的和。
- $W_r(s, a)$ 指的是在这棵子树中计算过的rollout的$z_L$的和。

#### Selection

每次从根开始往下搜索，直到走到一个叶子节点为止(设这时候的时间为L)，对于所有的$t < L$，令$a_t = argmax_a(Q(s_t, a) + u(s_t, a))$，其中$u(s, a) = c_{puct}P(s, a)\frac{\sum_{b}N(s,b)}{1+N(s,a)}$，$c_{puct}$是一个参数，最后被定为$5$。

#### Evaluation

把叶子节点$s_L$放到队列中计算$v_\theta(s_L)$(这里采用多线程，并且可以加一个记忆化)。对于rollout，下棋双方都用$p_\pi$来模拟，也采用多线程放在队列里依次处理。

#### Backup

考虑这次模拟对所有树中边的影响，我们分别更新rollout的值$N_r(s, a), W_r(s, a)$和value network的值$N_v(s, a), W_v(s, a)$，最后更新$Q(s, a)$的值

$$Q(s, a) = (1 - \lambda)\frac{W_v(s,a)}{N_v(s,a)}+\lambda\frac{W_r(s,a)}{N_v(s,a)}$$

实践证明，$\lambda=0.5$的时候效果最好。

分别考虑rollout和value network的更新

- 对rollout更新的时候，每次把某个节点塞到队列里面计算的时候，就把所有相关的$N_r(s,a)$和$W_r(s,a)$进行修改：$N_r(s,a)=N_r(s,a)+n_{vl}$, $W_r(s,a)=W_r(s,a)-n_{vl}$，让他看起来输了$n_{vl}$场防止多次运算，最后计算出来的时候让$N_r(s,a)=N_r(s,a)-n_{vl}+1$, $W_r(s,a)=W_r(s,a)+n_{vl}+z_L$。

- 对value network进行更新的时候，算出来之后更新所有相关的边$N_v(s,a)=N_v(s,a)+1$,$W_v(s,a)=W_v(s,a)+v_\theta(s_L)$。

这里$n_{vl}$取$3$。

#### Expansion

当一条边$(s, a)$的访问次数超过一个阀值$n_{thr}$的时候，就把后续的状态$s'=f(s,a)$加入到树中，把这个节点的所有边都初始化: $N_v(s',a)=N_r(s',a)=0$,$W_v(s',a)=W_r(s',a)=0$, $P(s',a)=p_\sigma(a|s')$，具体计算的时候$P(s',a)=p_\sigma(a|s')$这步运算也采用多线程，一开始先用tree policy来计算$P(s',a)$(tree policy和rollout policy类似，但是拥有更多的特征值)，直到$p_\sigma(a|s')$计算完成之后再让$P(s',a)=p_\sigma^\beta(a|s')$，这里softmax temperature被设为$\beta$，$\beta$取$0.67$，$n_{thr}$需要与实际队列的情况相契合。
