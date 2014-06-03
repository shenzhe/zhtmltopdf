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
	//参数2： 输出的img文件名
	//会生成图片文件
	zhtml2img("http://www.baidu.com", "./baidu.jpg");

	//参数1： 网址
	//只输入参数，可直接到到图片的数据, 可直接echo或输出到浏览器
	$content = zhtml2img("http://www.baidu.com");
	echo $content;
