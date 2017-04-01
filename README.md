zhtmltopdf
==========

zhtmltopdf， 把html页面转成pdf或image的php扩展

编译和安装
====
	phpize
	./configure
	make
	make install

	得到的zhtmltopdf.so文件加入到你的php.ini里


用法
=======
	
	//参数1： 网址
	//参数2： 输出的pdf文件名
	//会生成pdf文件
	zhtml2pdf("http://www.baidu.com", "./baidu.pdf");

	//参数1： 网址
	//只输入参数，可直接到到pdf的数据, 可直接echo或输出到浏览器
	$content = zhtml2pdf("http://www.baidu.com");
	echo $content;

	//参数1： 网址
	//参数2： 生成的img文件名 （可选）
	//参数3:  图片fmt(可选) jpeg(默认), png, bmp
	//参数4:  图片质量(可选) 默认80
	//参数5:  窗口宽度像素(可选) 默认1000
	//参数6:  窗口高度像素(可选) 默认1000
	zhtml2img("http://www.baidu.com", "./baidu.jpg");

	//参数1： 网址
	//只输入参数，可直接到到图片的数据, 可直接echo或输出到浏览器
	$content = zhtml2img("http://www.baidu.com");
	echo $content;

一些问题
===

   一、线上服务器乱码解决：

   1）系统的LANG设置为： LANG=zh_CN.UTF-8

   2) 缺少字体，可以把本地的字体copy到服务器上 /usr/share/fonts/chinese/TrueType目录下

   二、mac等系统编译问题 (自带的so是在linux amd64 下编译的)

   1) 去：https://github.com/wkhtmltopdf/wkhtmltopdf 下载，然后编译安装到xxx目录，

   2）安装扩展时 ./configure --with-wkhtmltox-dir=xxx (xxx为1里wkhtmltopdf的安装目录)

