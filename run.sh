#/bin/bash 
./compile.sh
input_filename=$1
#filename=${input_filename%.*}
#echo $filename
#extension=${input_filename##*.}
#echo $extension
compress_filename=compress.raw
decompress_filename=restore.raw
echo "input file:"$1
echo "compression output file:"$compress_filename
echo "decompression output file:"$decompress_filename
echo
./compress  $input_filename  $compress_filename #  > stdout_en.txt
echo
#./decompress  $compress_filename  $decompress_filename #  > stdout_de.txt
#echo 
#echo "result:"
#./compare $input_filename $decode_filename
#hexdump -C $encode_filename >${filename}_code_hex.txt
#hexdump -C $input_filename > ${filename}_origin_hex.txt
#hexdump -C $decode_filename > ${filename}_decode_hex.txt
