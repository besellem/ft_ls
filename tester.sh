#!/bin/bash


_ext=".txt"

real_file="real${_ext}"
my_file="mine${_ext}"
log_file="log${_ext}"
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
		
		echo "###############################" >> $log_file
		echo "###### Test: ls" $* >> $log_file
		echo "###############################" >> $log_file
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

# error management
do_test i_dont_exist
do_test -z

do_test -a
do_test -s
do_test -l
do_test -la
do_test -ls
do_test -lR
# do_test -las


do_test -l /var
do_test -l /var/


# remove the log file if it's empty
if [ $(wc -c < $log_file | xargs) == "0" ]; then
	rm -f $log_file
fi


# make fclean
