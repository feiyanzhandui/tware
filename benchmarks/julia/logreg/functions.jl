#type def for a datapoint <vector>, int
type DataPoint
        x
        y
end

#run parameters
const D = 10 #num dimensions
const N = 1000000 #num data items to generate
const ITERATIONS=30 #num iterations

#create initail random vector
w = rand(D)

function generateData()
        data = cell(N)
        for i = 1:N
                x = rand(D)
                if i%2 == 0
                        y = -1
                else
                        y = 1
                end
                data[i] = DataPoint(x,y)
        end
        return data
end

#data is an array of <vector>, int pairs of length N
const data = generateData()

#function to do computation in parallel for loop
function do_lr(p)
	return (1/(1+exp(-p.y*dot(w,p.x)))-1)*p.y*p.x
end

