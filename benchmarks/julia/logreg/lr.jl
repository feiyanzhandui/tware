type DataPoint
	x::Array{Float64, 1}
	y::Float64
end


function generateData!(N::Integer, D::Integer, R::Real, X::Vector)
	for i in 1:N
		y = i % 2 == 0 ? 1.0 : -1.0
		x = Array(Float64, D)
		for j in 1:D
			x[j] = rand() + y * R
		end
		X[i]=DataPoint(x,y)
	end
	return
end


function gradient!(N::Integer, D::Integer, X::Vector,
	               w::Vector, gr::Vector)
	for i in 1:N
		z = 0.0
		for j in 1:D
			z += (X[i].x)[j] * w[j]
		end
		scale = (1 / (1 + exp(-X[i].y * z)) - 1) * X[i].y
		for j in 1:D
			gr[j] += scale * (X[i].x)[j]
		end
	end
	return
end

function do_loop(N::Integer, D::Integer, X::Vector, w::Vector,
	             iterations::Integer, gr::Vector)
	for i in 1:iterations
		gradient!(N, D, X, w, gr)
		for i in 1:D
			w[i] -= gr[i]
		end
	end
	return
end

function do_lr!(N::Integer, D::Integer, iterations::Integer,
	            X::Vector,gr::Vector)
	w = rand(D)
	return @elapsed do_loop(N, D, X, w, iterations, gr)
end

function main()
	N = 1000000
	D = 10
	R = 0.7
	iterations = 30
	data = Array(DataPoint, N)
	gr = Array(Float64, D)
	generateData!(N, D, R, data)
	return do_lr!(N, D, iterations, data, gr)
end

println(main()*1000)

