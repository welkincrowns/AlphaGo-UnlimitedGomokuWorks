## 2016-06-01 #2

- demo just for fun

## 2016-06-01

现在一共是这几个东西

- pSigma.py 主程序，里面有pSigma的训练和调用
- rule1.dat 训练集，格式是每一行一步，一局以-1 -1结束
- input_data.py 训练集
- a.in 临时文件，可以用来测试结果
- execute.py 调用pSigma.py的程序，在a.in里面输入之前的下棋的步骤，可以跑出神经网络的结果
- callp.cpp 用c++调用python的程序，现在调不了，求差错。
- tmp/pSigma.ckpt 之前训练的结果。
- data/pSigma.ckpt 同上，备份用。

### callp的编译命令

网上找的...

大概是找到你的python的目录，然后敲

	./python-config --cflags
	./python-config --ldflags

把两个结果复制一下

	-I/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/include/python2.7 -I/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/include/python2.7 -fno-strict-aliasing -fno-common -dynamic -g -O2 -DNDEBUG -g -fwrapv -O3 -Wall -Wstrict-prototypes 
	-L/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/lib/python2.7/config -lpython2.7 -ldl -framework CoreFoundation

然后加到后面编译

	g++ -o callp callp.cpp -I/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/include/python2.7 -I/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/include/python2.7 -fno-strict-aliasing -fno-common -dynamic -g -O2 -DNDEBUG -g -fwrapv -O3 -Wall -Wstrict-prototypes -L/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/lib/python2.7/config -lpython2.7 -ldl -framework CoreFoundation

### pSigma

训练

training(train_size, test_size, batch_size, training_rate)

调用

player(filename)