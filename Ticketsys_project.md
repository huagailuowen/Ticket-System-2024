本系统分为各部分：
1. 用户管理系统
2. 火车管理系统
3. 订单管理系统
## 文件和依赖图
train->ticket->user--->systhesis
            command-/

## 用户管理系统
### 采用类管理，进行处理。
用户管理系统主要包括用户注册、登录、个人信息管理等功能。用户注册时需要填写用户名、密码、邮箱等信息，用户登录时需要输入用户名和密码。用户登录后可以查看个人信息，修改个人信息等。

登陆列表使用hashmap 进行记录
## 火车管理系统
火车管理系统主要包括火车信息管理、车次查询等功能。火车信息管理包括火车信息的添加、删除、修改等功能。车次查询功能可以根据出发地、目的地、出发时间等条件查询车次信息。
该系统有两部分：
### 采用类管理，进行处理
类不需要管理每一天每一班车每一站所剩的订单信息，由订票系统处理

# release 部分

# back 部分


## 订单管理系统
订单管理系统主要包括订单的生成、退票等功能。订单生成时需要填写乘车人信息、车次信息等，订单退票时需要输入订单号等信息。
### 类需要管理每一天每一班车每一站所剩的订单信息，每一个release都会有若干个实例化对象，分别代表每一班车以及其信息,包括剩余票数，订单信息等

### 候补处理直接在一个BPT中处理 Alternate

除此之外为了方便查询，我们记下经过每一个站的所有车次，这样查询就只需要遍历（transfer就记个最优值即可,小心自己转自己不算，判断逻辑需要精心设计，这部分查询在内存中进行处理）。


