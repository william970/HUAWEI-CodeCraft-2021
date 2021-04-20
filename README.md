# HUAWEI-CodeCraft-2021
 华为软件精英挑战赛
## 写在前面
距离华为软件精英挑战赛结束也有一段时间了
我是浙工大投降战队的队长，第一次参加这种比赛打到赛区前二十我还是比较满意的
这次比赛我最大的收获就是认识了好多厉害的大佬
希望他们总决赛能拿到好成绩
也希望比赛结束了能交流一下的思路，扩展一下思维。
## 成绩
杭厦赛区，复赛第20名
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210420160654895.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl8zODYxNjAxOA==,size_16,color_FFFFFF,t_70)
## 大致思路
这个比赛，能优化的地方一共就三个地方，一个是购买服务器的策略，还有一个是插入虚拟机到服务器的策略，最后是迁移策略
## 代码结构
![在这里插入图片描述](https://img-blog.csdnimg.cn/2021042014175162.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl8zODYxNjAxOA==,size_16,color_FFFFFF,t_70)


这个是整体的代码结构，最后因为编译问题，全部整合到了一个cpp里面，这样看着清晰一点

 - GlobalMessage：单例模式，这里面存放了所有全局的信息，读进来的信息以及根据该信息做解析之后的信息都存放在这里
 - fuwuqi，xuniji，logicalOperation：一些基础的数据结构还有添加删除操作
 - Buy里面写了服务器购买逻辑
 - poccessOperation：流程函数
 - Printer：输出类
 - MachineLibarary：单例模式，里面放了所有的机器的操作函数，比如添加服务器，增加虚拟机，删除虚拟机，以及为虚拟机分配一台服务器

## 算法
算法分三块 

 1. 购买服务器（参考buy.cpp）

```cpp
FUWUQI buyOptimalMachine2(const XUNIJI &xuniji);
```

 3. 为虚拟机分配服务器（参考MachineLibarary里）

```cpp
pair<MachineLibarary::AB, std::shared_ptr<FUWUQI> > addXuNiJi(const string& id, XUNIJI xuniji, unordered_set<shared_ptr<FUWUQI>> &unused_set, unordered_set<shared_ptr<FUWUQI>> &in_it); //添加虚拟机,新增排除列表
```

 3. 迁移（参考MachineLibarary里的迁移函数）

```cpp
vector<pair<string, pair<MachineLibarary::AB, std::shared_ptr<FUWUQI>> > > migaration(); //迁移
```

### 购买服务器
采用贪心算法，考虑了服务器的内存内核比和性价比为每台服务器打分，最后选取分数最高的服务器

#### 优化点
 - 虚拟机的内存内核比  用未来的所有还未分配的虚拟机取代当前虚拟机，加入全局的特征
 - 优先选取内存内核能存放下请求序列里面最大的虚拟机的服务器，在代码中体现为提高该服务器的分数，因为有些小服务器后面空了，有一天大的虚拟机来时，如果放不下且没有合适的服务器就又要去买一台，这样很亏
 - 同时考虑了内存内核比还有性价比

### 将虚拟机插入服务器
我设计的滑窗法
即服务器一个释放日期，来一台虚拟机的时候，优先插入释放日期大于等于虚拟机释放日期的服务器
如果不存在，则选取一台释放日期与虚拟机释放日期最接近的合适服务器，并把该服务器的释放日期赋值为该虚拟机的释放日期
然后还不存在，再选一台性价比最高的空的服务器插入
#### 优化点
 - 滑窗保证释放日期接近的服务器尽量放在同一台服务器上
 - 把成本量化了，只要保证相同的资源下得到的利益最大化就行了，即相同成本下能放置的虚拟机多，内存内核利用率最大
 - 保证AB节点平衡
 - 考虑了插入时服务器的内存内核比，同时在一批内存内核比合适的服务器里面找剩余空间最小的

### 迁移
找利用率最低的服务器，将该服务器里面的虚拟机迁移到合适的服务器，迁移这一块主要是优化了速度，因为正式赛的时候一直超时（气哭），经过两次优化后速度快了两倍
#### 优化点
 - 我们考虑了时间复杂度，认为算法耗时主要是在迁移选服务器的阶段，我们采用分区的方法区内迁移，如果我分8个区的话那需要遍历的服务器就是以前的1/8，考虑到线上服务器是2核的，我们750个服务器作为一个分区，设计了4个线程去处理1/4的分区
 - 做了减枝，即如果内核内存利用率大于1.9就直接跳过该服务器，不迁移该服务器下的虚拟机


