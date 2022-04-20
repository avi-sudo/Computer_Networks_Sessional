count=0
received_packets = 0
sent_packets = 0
dropped_packets = 0
total_delay = 0
received_bytes = 0
    
start_time = 1000000
end_time = 0
sent_time={}

# constants
header_bytes = 20
with open("trace.tr") as fp: 
    for line in fp: 
        count += 1
        line.strip()
        x=line.split()
        if x[0]=="M":
            continue
        event=x[0]
        time_sec=float(x[1])
        node=x[2].replace("_","")
        layer=x[3]
        packet_id=int(x[5])
        packet_type=x[6]
        packet_bytes=float(x[7])
        
        if start_time>time_sec:
            start_time=time_sec
        if layer=="AGT" and packet_type=="exp":
            if event=="s":
                sent_time[packet_id]=time_sec
                #print(packet_id)
                sent_packets+=1
            elif event=="r":
                received_packets+=1
                delay=time_sec-sent_time[packet_id]
                total_delay+=delay
                byte=packet_bytes-header_bytes
                received_bytes+=byte
        if packet_type=="exp" and event=="D":
            dropped_packets+=1
    
end_time=time_sec
simulation_time=end_time-start_time   
print("Sent Packets: ", sent_packets)
print("Dropped Packets: ", dropped_packets)
print("Received Packets: ", received_packets)

print("-------------------------------------------------------------")
print("Throughput: ", (received_bytes * 8) / simulation_time, "bits/sec")
print("Average Delay: ", float(total_delay / received_packets), "seconds")
print("Delivery ratio: ", float(received_packets / sent_packets))
print("Drop ratio: ", float(dropped_packets / sent_packets))
		
		
		
	
