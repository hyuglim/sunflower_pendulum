setrandomseed		936977
setquantum		1
newbatt 		0 0.1

--
--			Node 1  
--
newnode			superH 0 0 0 "pendulum.trajectory" 1 1000000000
forceavgpwr		0.0264 0.0000495
setfreq			4
settimerdelay		4000
nodefailprob		0.0
nodefaildurmax		1000000
battnodeattach		0
battalertfrac		0.0
clockintr		1
ebattintr		1
cacheoff
ff
srecl			pendulum.sr
run

on
