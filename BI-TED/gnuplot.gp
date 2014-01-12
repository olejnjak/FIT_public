set encoding utf8
set terminal pngcairo enhanced
set output "vysledek.png"

set multiplot title "" layout 1,1

set size 1,1
set origin 0,0

set yrange [-3:10.5]
set xrange [-0.5:10.5]
set style fill solid 0.2
set key on right bottom title "Legenda" font ",8"
set xlabel "x" font ",8"
set ylabel "f(x)=1/x" font ",8"
set y2label "g(x) = součet obdélníků" font ",8"
set xtics 1 font ",8"
set ytics 2 font ",8"
set y2tics ("" 0)
set x2tics ("" 0)
set grid y2tics x2tics
plot "data.txt" using 1:2 with boxes linecolor rgbcolor "green" title "f(x)", "data.txt" using 1:3 linecolor rgbcolor "red" title "g(x)", log(x) linecolor rgbcolor "blue" title "ln(x)"

set title ""
unset key
set grid xtics ytics
set size 0.65,0.4
set xlabel ""
set ylabel ""
set y2label ""
set origin 0.3,0.5
set xrange [0:1.52]
set yrange [-3:0.5]
set xtics 0.1 font "0,6"
set ytics 0.5 font "0,6"
plot "data.txt" using 1:3 linecolor rgbcolor "red" title "g(x)", log(x) linecolor rgbcolor "blue" title "ln(x)"

unset multiplot
