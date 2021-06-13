#!/bin/sh
mkdir -p exec

echo '**********************************'
echo 'Building rgb565_to_bmp'
echo '**********************************'
make -C rgb565_to_bmp clean && make -C rgb565_to_bmp all
make -C rgb565_to_bmp install || exit 1


echo '**********************************'
echo 'Building rgb888_to_bmp'
echo '**********************************'
make -C rgb888_to_bmp clean && make -C rgb888_to_bmp all
make -C rgb888_to_bmp install || exit 1


echo '**********************************'
echo 'Building bmp_to_rgb565 '
echo '**********************************'
make -C bmp_to_rgb565 clean && make -C bmp_to_rgb565 all
make -C bmp_to_rgb565 install || exit 1


echo '**********************************'
echo 'Building bmp_to_rgb888 '
echo '**********************************'
make -C bmp_to_rgb888 clean && make -C bmp_to_rgb888 all
make -C bmp_to_rgb888 install || exit 1


echo '**********************************'
echo 'Building rgb565_to_rgb888 '
echo '**********************************'
make -C rgb565_to_rgb888 clean && make -C rgb565_to_rgb888 all
make -C rgb565_to_rgb888 install || exit 1


