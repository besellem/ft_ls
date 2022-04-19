#!/bin/bash


_ext=".txt"

real_file="/tmp/__real_ls__${_ext}"
my_file="/tmp/__my_ls__${_ext}"
log_file="../log${_ext}"
tmp_file="__tmp__"


do_test() {
	
	# may avoid some basic problems
	touch $real_file $my_file

	/bin/ls $* > $real_file 2>/dev/null #2>&1
	./ft_ls $* > $my_file 2>/dev/null #2>&1
	
	diff -U0 $real_file $my_file >> $tmp_file
	if [ $? -eq 0 ]; then
		echo "✅ =>" $* # debug purpose
		# echo "✅"
	else
		# echo "❌  "
		echo "❌ =>" $* # debug purpose
		
		echo "---------------------------------------------------" >> $log_file
		echo "-- Test: ls" $* >> $log_file
		echo "---------------------------------------------------" >> $log_file
		cat $tmp_file >> $log_file
		echo >> $log_file
	fi

	rm -f $real_file $my_file $tmp_file
}


# make fclean
echo -n "Compiling project... "
make 1>/dev/null
if [ $? -eq 2 ]; then
	exit 1
fi
echo "done !"

rm -f $log_file 2>/dev/null


# basic tests
do_test
do_test Makefile
do_test ./Makefile
do_test srcs
do_test srcs/
do_test ./srcs
do_test ./srcs/
do_test ./Makefile srcs
do_test ./Makefile ./srcs
do_test srcs incs
do_test srcs Makefile incs
do_test srcs Makefile incs Makefile
do_test Makefile srcs incs Makefile


# error management
do_test i_dont_exist
do_test ''
do_test -z

do_test i_dont_exist srcs
do_test i_dont_exist Makefile
do_test i_dont_exist Makefile i_dont_exist


# options
do_test -a
do_test -s
do_test -i
do_test -is
do_test -l
do_test -la
do_test -laA
do_test -ls
do_test -las

do_test -l Makefile
do_test -la Makefile
do_test -l ./Makefile
do_test -la ./Makefile
do_test -l Makefile srcs
do_test -la Makefile srcs
do_test -l /var
do_test -l /var/


# recursive option
do_test -lR
do_test -laR
do_test -lR srcs
do_test -laR srcs incs
do_test -larR srcs incs
do_test -latR srcs incs
do_test -lartR srcs incs
do_test -lartR . srcs tester.sh incs Makefile


# bonus
do_test -L ~
do_test -l ~
do_test -lL ~
do_test -L /
do_test -lL /
do_test -lL /var
do_test -lL /var/
do_test -Ls /
do_test -Lls /
do_test -Lls
do_test -h
do_test -hl ~
do_test -lLsaAp ~
do_test -lLsaArp ~
do_test -lLstaAup ~
do_test -lLsAarioGTtup srcs incs Makefile
do_test -lLsAarioGTtup ~
do_test -lLsAaioGTtup /
do_test -lLsAarioGTtup /
do_test -startup

do_test -d . srcs
do_test -lGad
do_test -L@ /

# make fclean
