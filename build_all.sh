#!/bin/sh
mkdir -p exec

echo '**********************************'
echo 'Building rgb565_to_bmp'
echo '**********************************'
make -C rgb565_to_bmp clean && make -C rgb565_to_bmp all
if [ "$?" == "0" ]; then
	make -C rgb565_to_bmp install || exit 1
fi


echo '**********************************'
echo 'Building bmp_to_rgb565 '
echo '**********************************'
make -C bmp_to_rgb565 clean && make -C bmp_to_rgb565 all
if [ "$?" == "0" ]; then
	make -C bmp_to_rgb565 install || exit 1
fi


echo '**********************************'
echo 'Building bmp_to_rgb888 '
echo '**********************************'
make -C bmp_to_rgb888 clean && make -C bmp_to_rgb888 all
if [ "$?" == "0" ]; then
	make -C bmp_to_rgb888 install || exit 1
fi


echo '**********************************'
echo 'Building rgb565_to_rgb888 '
echo '**********************************'
make -C rgb565_to_rgb888 clean && make -C rgb565_to_rgb888 all
if [ "$?" == "0" ]; then
	make -C rgb565_to_rgb888 install || exit 1
fi


