#!/bin/sh
#file comandi ricorsivo

cd $1

case $1 in
	*/$2)
		
for F in *
do 
	if test -f $F
	then 
		NL=`wc -l < $F`
		if test $NL -eq $3
		then 
			cont=`expr $cont + 1`
			files="$files `pwd`/$F"
		fi
	fi
done;;
	*);;
esac

if test $cont -ge 2
then 
	echo trovato direttorio `pwd`
	echo $files >> $4
fi

for i in *
do 
	if test -d $i -a -x $i
	then 
		$0 `pwd`/$i $2 $3 $3
	fi
done
	
