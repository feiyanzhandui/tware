@everywhere function generateData(idx)
	m=Array(Float64, (length(idx[1]), length(idx[2])))
	for i=1:length(idx[1])
		y = i % 2 == 0 ? 1.0 : -1.0
		for j=1:length(idx[2])
			m[i,j]=rand() +y *0.7
		end
	end
	return m
end


@everywhere function generateY(idx)
	m=Array(Integer, (length(idx[1]),1))
	for i=1:length(idx[1])
		m[i] = i % 2 == 0 ? 1.0 : -1.0
	end
	return m
end


@everywhere function gradient!(N::Integer, D::Integer, X::DArray,
	               w::Vector, y::DArray, gr::Vector)
	for i in myindexes(X)[1]
		z = @parallel (+) for j in myindexes(X)[2]
			X[i, j] * w[j]
		end
		scale = (1 / (1 + exp(-y[i] * z)) - 1) * y[i]
		for j in myindexes(X)[2]
			gr[j] = scale * X[i, j]
		end
	end
	println(gr)
	return
end


@everywhere function do_loop!(N::Integer, D::Integer, X::DArray, w::Vector,
	             iterations::Integer, y::DArray, gr::Vector)
	for i in 1:iterations
		fill!(gr,0.0)
		@async begin
			for i in procs(X)
				@spawnat i gradient!(N, D, X, w, y, gr)
			end
		end
		@sync begin
			for i in 1:D
				w[i] -= gr[i]
			end
		end
	end
	return
end

@everywhere function do_lr!(N::Integer, D::Integer, iterations::Integer,
	            X::DArray, y::DArray, gr::Vector)
	w = rand(D)
	println(w)
	time= @elapsed do_loop!(N, D, X, w, iterations, y, gr)
	test!(X, y, w, N,D)
	return time
end

function test!(X::DArray, y::DArray, w::Vector, N::Integer, D::Integer)
	t=0
	for i=1:N
		e=0
		for j=1:D
			e+=X[i,j]*w[j]		
		end
		if exp(e)/(1+exp(e)) > 1-0.05 
			if(y[i]==1)
				t+=1
			end
		else
			if(y[i]==-1)
				t+=1
			end		
		end
	end
	println(w)
	println(string(t)*" of "*string(N)*" classified correctly")
end

function main()
	N = 4
	D = 4
	R = 0.7
	iterations = 1
	gr = Array(Float64, D)
	X = DArray(generateData, (N,D))
	println(X)
	y = DArray(generateY, (N,1))
	do_lr!(N, D, iterations, X, y, gr)
end

println(main()*1000)


