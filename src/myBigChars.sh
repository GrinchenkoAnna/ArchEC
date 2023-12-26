#!/bin/bash

echo -e "\E(0"

echo -e "\E[H\E[2J"
for i in {0..7}
do
    if [ $i -eq 0 ]; then
        echo -e "\E[$(( $i+5 ));10Hlqqqqqqk"
    elif [ $i -eq 1 ]; then
        echo -e "\E[$(( $i+5 ));10Hx aaaa x"
    elif [ $i -eq 2 ]; then
        echo -e "\E[$(( $i+5 ));10Hx    a x"
    elif [ $i -eq 3 ]; then
        echo -e "\E[$(( $i+5 ));10Hx   a  x"
    elif [ $i -eq 4 ] || [  $i -eq 5 ] || [ $i -eq 6 ]; then
        echo -e "\E[$(( $i+5 ));10Hx  a   x"
    else
        echo -e "\E[$(( $i+5 ));10Hmqqqqqqj"
    fi;
done

echo -e "\E(B"
