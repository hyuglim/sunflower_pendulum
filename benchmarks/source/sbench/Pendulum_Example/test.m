v

setquantum		1

-- sigsrc          0 "Pendulum xyz accelerations"  0.0 0.0     0.0 0.0 0.0 0.0 0.997356 -0.5 0.0 0.0 0.    0 2.71828       0.0 0.0 0.0 0.0 2.0 0.0 0.0 0.0     "dam.locdata.1" 100 0.0 0.0 0.0 1   " " 0 10    00.0 1

--  TODO: Check if i need to modify this
-- netnewseg		0 1024 300000000 38400 999 0 0 0 0 0 0 0 0
-- netsegfailprob		0 0.0
-- netsegfaildurmax	0 1000000
-- 
-- netsegpropmodel		0 3 9.0 

newbatt 		0 0.1

--
--			Node 1  
--
newnode superH 0 0 0 "dam.locdata.1" 1
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
netnodenewifc		0 0.0891 0.0330 0.0000033 0.0330 0 0 0 0 256 256
netsegnicattach		0 0
retryalg		0 "none"
sigsubscribe		0 0
srecl			pendulum.sr
run

setrandomseed		936977

v
on
