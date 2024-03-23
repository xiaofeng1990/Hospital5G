# Hospital5G
test

# 安装依赖
sudo apt-get install gettext cmake
sudo apt-get install  libffi-dev
# 安装glib2
wget http://ftp.gnome.org/pub/gnome/sources/glib/2.40/glib-2.40.0.tar.xz
tar -xJf glib-2.40.0.tar.xz
cd glib-2.40.0
./configure
make -j4
make install
如果遇到报错
gdate.c:2497:7: error: format not a string literal, format string not checked [-Werror=format-nonlit
第一步先找到这个文件 然后加入如下语句
#pragma GCC diagnostic ignored "-Wformat-nonliteral"

遇到报错
gdbusauth.c:1302:11: error: ‘%s’ directive argument is null [-Werror=format-overflow=]
 1302 |           debug_print ("SERVER: WaitingForBegin, read '%s'", line);
gdbusmessage.c:2702:30: error: ‘%s’ directive argument is null [-Werror=format-overflow=]
 2702 |       tupled_signature_str = g_strdup_printf ("(%s)", signature_str);

sudo find / -name gdbusauth.c
sudo find / -name gdbusmessage.c

sudo vi gdbusauth.c
 
#进入vi编辑器  根据错误找到第1302行  可在一般模式输入1302gg移动到该位置
#在错误前输入    if(line != NULL)
#：wq退出保存
 
同理
 
sudo vi gdbusmessage.c
 
#进入vi编辑器  根据错误找到第2702行  可在一般模式输入2702gg移动到该位置
#在错误前输入    if(signature_str!= NULL)
#：wq退出保存