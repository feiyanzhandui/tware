function generateData!(N::Integer, D::Integer, R::Real, X::Matrix)
	for i in 1:N
		y = i % 2 == 0 ? 1.0 : -1.0
		for j in 1:D
			X[i, j] = rand() + y * R
		end
	end
	return
end

function generateY!(N::Integer, y::Vector)
	for i in 1:N
		y[i] = i % 2 == 0 ? 1.0 : -1.0
	end
	return
end

function gradient!(N::Integer, D::Integer, X::Matrix,
	               w::Vector, y::Vector, gr::Vector)
	fill!(gr,0.0)
	for i in 1:N
		z = 0.0
		for j in 1:D
			z += X[i, j] * w[j]
		end
		scale = (1 / (1 + exp(-y[i] * z)) - 1) * y[i]
		for j in 1:D
			gr[j] += scale * X[i, j]
		end
	end
	return
end

function do_loop(N::Integer, D::Integer, X::Matrix, w::Vector,
	             iterations::Integer, y::Vector, gr::Vector)
	for i in 1:iterations
		gradient!(N, D, X, w, y, gr)
		for i in 1:D
			w[i] -= gr[i]
		end
	end
	return
end

function do_lr!(N::Integer, D::Integer, iterations::Integer,
	            X::Matrix, y::Vector, gr::Vector)
	w = rand(D)
	return @elapsed do_loop(N, D, X, w, iterations, y, gr)
end

function main()
	N = 100000s
	D = 50
	R = 0.7
	iterations = 30
	X = Array(Float64, N, D)
	y = Array(Float64, N)
	gr = Array(Float64, D)
	generateData!(N, D, R, X)
	generateY!(N, y)
	return do_lr!(N, D, iterations, X, y, gr)
end

println(main()*1000)
