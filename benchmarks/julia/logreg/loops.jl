
function main()
	const N=1::Int32
	const D=1::Int32

	t=time_ns()::Uint64
	for i=1:N
		for j=1:D
		end
	end
	s= time_ns()::Uint64

	bench=((s-t)/1000000)::Float64
	return(bench::Float64)
end

println(main())
