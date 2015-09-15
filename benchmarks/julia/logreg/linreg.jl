
#code to be available at all processors / setup code
require("functions.jl")

println("INITIAL w")
println(w)

tic()
for i = 1:ITERATIONS
        println("On Iteration: " * string(i))
	#apply function do_lr to each element in data, reduce by (+)
	#and change w for use in next iteration
	w-= @parallel (+) for i=1:length(data)
	   do_lr(data[i])
	end
end
toc()

println("FINAL w")
println(w)
