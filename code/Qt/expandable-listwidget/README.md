编译依赖
===========
sudo apt install qtbase5-dev qt5-qmake qtchooser qtscript5-dev qttools5-dev-tools qtbase5-dev-tools

编译和运行
===========
qmake
make
./expandable-listwidget


说明
===========
对列表(QListWidget)中的每个子项(QListWidgetItem)的高度进行扩展，点击子项后显示扩展后的高度，再次点击恢复则子项原有的高度。
