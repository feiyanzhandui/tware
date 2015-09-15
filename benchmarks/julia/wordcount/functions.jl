function localReduceByKey(lst)
      	res = Dict{ASCIIString, Int64}()
        for elt in lst
          res[elt] = get(res, elt, 0) + 1
        end
        return res
end


#to do: actually use op, now just using +
function reduceByKey(op, lst)
	
        res = Dict{ASCIIString, Int64}()
       
	for elt in lst
 		for word in elt
			res[word[1]] = get(res, word[1], 0) + word[2]
		end
	end
	return res
end


# applies functions, in order, to lst. 
# returns an array with f[last] for each item in lst
function mapper(fns, lst)

	res = cell(length(lst))
        for i = 1:length(lst)
             ans = lst[i]
		lst[i] = {""}
             for f in fns
		ans = f(ans)
	     end
             res[i] = ans
        end 

     	 return res
end


# not currently using, may be helpful later
function flatmap(fn, lst)
	 elts = map(fn, lst)
	 for i = 2:length(elts)
                append!(elts[1], elts[i])
        end
        return elts[1]
end

#merge arrays into one array, useful for merging results
#from a remote_call operation on another processor
function combineArrays(arr)
	for i = 2:length(arr)
		append!(arr[1], arr[i])
	end
	return arr[1]
end

#f can be a list of functins, lst is a list of values to work on
#function divides lst into equal parts, and executes each
#chunk on a remote machine. Results are merged together at the end
function mapchunk(f, lst)
	elts = divideArray(lst, nprocs()-1)
	#elts has nprocs() elements to be sent to a processor
	n = length(elts)

        results = cell(n)
	retArr = cell(n)
	i = 1
	next_idx() = (idx=i; i+=1; idx)
        @sync for p=1:length(elts)
            @async begin
                while true
                    idx = next_idx()
                    if idx > n
                        break
                    end
                    results[idx] = remote_call_fetch(idx+1, mapper, f, elts[idx])
                end
            end
        end

	
	returnVals = cell(length(lst))
	k = 1
	for i = 1:length(results)
		for j = 1:length(results[i])
			returnVals[k] = results[i][j]
			k = k + 1
		end
	end
	
	return returnVals
	
end

#arr is the array to divide
#n = number of parts to divide the array into
function divideArray(arr, n)
       	
	 len = length(arr)
	chunkSize = ceil(len/n)
        results = cell(n)
	start = 1
	for i = 1:n
		if i == n
			results[i] = arr[start: len]
			break
		end
		results[i] = arr[start:(start+chunkSize-1)]
		start = start + chunkSize
	end
	return results
end
