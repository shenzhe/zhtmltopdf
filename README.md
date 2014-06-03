zhtmltopdf
==========

zhtmltopdf， 把html页面转成pdf文件的php扩展

编译
====
phpize
./configure
make
make install

得出的zhtmltopdf.so文件加入到你的php.ini里


函数原型：zhtml2pdf("pdf文件名", "url地址")
=======

	zhtml2pdf("./test.pdf", "http://www.baidu.com");

