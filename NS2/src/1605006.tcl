# simulator
set ns [new Simulator]


# ======================================================================
# Define options

set val(chan)         Channel/WirelessChannel  ;# channel type
set val(prop)         Propagation/TwoRayGround ;# radio-propagation model
set val(ant)          Antenna/OmniAntenna      ;# Antenna type
set val(ll)           LL                       ;# Link layer type
set val(ifq)          Queue/DropTail/PriQueue  ;# Interface queue type
set val(ifqlen)       50                       ;# max packet in ifq
set val(netif)        Phy/WirelessPhy/802_15_4       ;# network interface type/802_15_4          ;
set val(mac)          Mac/802_15_4         ;# MAC type
set val(rp)           AODV                   ;# ad-hoc routing protocol 
set val(nn)           40                   ;# number of mobilenodes
set val(nf)           20                    ;# number of flows
# =======================================================================

set col 5
if {$val(nn)>=50} {
    set col [expr $col*2]
}
set row [expr $val(nn)/$col]
set x_dim 500
set y_dim $x_dim


# trace file
set trace_file [open trace.tr w]
$ns trace-all $trace_file

# nam file
set nam_file [open animation.nam w]
$ns namtrace-all-wireless $nam_file $x_dim $y_dim

# topology: to keep track of node movements
set topo [new Topography]
$topo load_flatgrid $x_dim $y_dim ;# area


# general operation director for mobilenodes
create-god $val(nn)


# node configs
# ======================================================================

# $ns node-config -addressingType flat or hierarchical or expanded
#                  -adhocRouting   DSDV or DSR or TORA
#                  -llType	   LL
#                  -macType	   Mac/802_11
#                  -propType	   "Propagation/TwoRayGround"
#                  -ifqType	   "Queue/DropTail/PriQueue"
#                  -ifqLen	   50
#                  -phyType	   "Phy/WirelessPhy"
#                  -antType	   "Antenna/OmniAntenna"
#                  -channelType    "Channel/WirelessChannel"
#                  -topoInstance   $topo
#                  -energyModel    "EnergyModel"
#                  -initialEnergy  (in Joules)
#                  -rxPower        (in W)
#                  -txPower        (in W)
#                  -agentTrace     ON or OFF
#                  -routerTrace    ON or OFF
#                  -macTrace       ON or OFF
#                  -movementTrace  ON or OFF

# ======================================================================

$ns node-config -adhocRouting $val(rp) \
                -llType $val(ll) \
                -macType $val(mac) \
                -ifqType $val(ifq) \
                -ifqLen $val(ifqlen) \
                -antType $val(ant) \
                -propType $val(prop) \
                -phyType $val(netif) \
                -topoInstance $topo \
                -channelType $val(chan) \
                -agentTrace ON \
                -routerTrace ON \
                -macTrace OFF \
                -movementTrace OFF

# create nodes

for {set i 0} {$i < $val(nn) } {incr i} {
    set node($i) [$ns node]
    $node($i) random-motion 1      ;# disable random motion

    
    #$node($i) set X_ [expr int(500 * rand())]
    #$node($i) set Y_ [expr int(500 * rand())]
   
} 

set x_start [expr $x_dim/(2*$col)]
set y_start [expr $y_dim/(2*$row)]
set x_offset [expr $x_dim/$col]
set y_offset [expr $y_dim/$row]

for {set j 0} { $j < $row } { incr j } {
    for {set k 0} { $k < $col } { incr k } {
        set w [expr ($j*$col)+$k]

        set x_pos [expr $x_start+($k*$x_offset)]
        set y_pos [expr $y_start+($j*$y_offset)]

        $node($w) set X_ $x_pos
        $node($w) set Y_ $y_pos
        $node($w) set Z_ 0

        #puts "Put $w to ($x_pos , $y_pos)"

        set speed [expr int(5*rand())+1]
        set x_new [expr int($x_dim*rand())]
        set y_new [expr int($y_dim*rand())]
        $ns at 1.00 "$node($w) setdest $x_new $y_new $speed"
        $ns initial_node_pos $node($w) 20
    }
}


# Traffic


for {set i 0} {$i < $val(nf)} {incr i} {
    set src [expr int($val(nn)*rand())] ;# src node
    set dest $src
    while {$dest==$src} {
		set dest [expr int($val(nn)*rand())] ;# dest node
	}
    #puts "RANDOM:  Src: $src Dest: $dest\n"
    

    # Traffic config
    # create agent
    #set tcp [new Agent/TCP]
    set udp [new Agent/UDP]
    set null [new Agent/Null]
    #set tcp_sink [new Agent/TCPSink]
    # attach to nodes
    $ns attach-agent $node($src) $udp
    $ns attach-agent $node($dest) $null
    # connect agents
    $ns connect $udp $null
    $udp set fid_ $i

    # Traffic generator
    set exp [new Application/Traffic/Exponential]
     
    # attach to agent
    $exp attach-agent $udp

    $exp set packet_size_ 100
    $exp set burst_time_ 500ms
    $exp set idle_time_ 500ms
    $exp set rate 100k
    
    # start traffic generation
    $ns at 1.0 "$exp start"
}



# End Simulation

# Stop nodes
for {set i 0} {$i < $val(nn)} {incr i} {
    $ns at 20.0 "$node($i) reset"
}

# call final function
proc finish {} {
    global ns trace_file nam_file
    $ns flush-trace
    close $trace_file
    close $nam_file
}

proc halt_simulation {} {
    global ns
    puts "Simulation ending"
    $ns halt
}

$ns at 20.0001 "finish"
$ns at 20.0002 "halt_simulation"




# Run simulation
puts "Simulation starting"
$ns run

