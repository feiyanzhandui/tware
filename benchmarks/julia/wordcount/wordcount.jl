#ip addresses of worker nodes, add ip address twice to list for dual core nodes
const ips = {"10.96.43.200", "10.83.54.128", "10.85.102.63", "10.32.255.45", "10.76.231.168", "10.96.43.200", "10.83.54.128", "10.85.102.63", "10.32.255.45", "10.76.231.168"}
const machines = map(x -> "ubuntu@" * x, ips)

addprocs_ssh(machines)

require("functions.jl")

#path to file portions, use as input to mapper function...do not change, just change subarray (parameter to mapchunk)
files = {"txt/xaa", "txt/xab", "txt/xac", "txt/xad", "txt/xae", "txt/xaf","txt/xag","txt/xah","txt/xai","txt/xaj","txt/xal","txt/xam","txt/xae","txt/xan","txt/xao","txt/xap","txt/xaq","txt/xar","txt/xas","txt/xat","txt/xau","txt/xav","txt/xaw","txt/xax","txt/xay",}
#fix path to files
files = map(x -> "/home/ubuntu/txt/" * x, files)

#apply chunking readall THEN split THEN localReduceByKey to files[1:n]
texts = mapchunk({readall, split, localReduceByKey}, files[1:10])

ans  = reduceByKey("+", texts)

#prints num. of times "the" appears
println(ans["the"])
