# GameServer
![image]([https://github.com/MaiEmily/map/blob/master/public/image/20190528145810708.png](https://github.com/zhenliu123123/GameServer/blob/master/Readme.png))
**需要游戏客户端的star后联系qq2368410997**

游戏服务器
描述：该项目是实现的是网游的服务器部分，该游戏类似吃鸡是一个多人在线对战游戏。
特性：
1. 玩家上下线数据同步（基于AOI算法进行位置更新）
2. 玩家信息管理（基于redis存储和获取数据）
3. 服务器延时自动关闭（轮转定时器）
4. 游戏房间管理（redis存储房间信息，RPC实现房间操作）
5. 玩家登陆注册（FastCGI结合数据库）
