function localReduceByKey(lst)
        res = Dict{ASCIIString, Int64}()
        for elt in lst
          res[elt] = get(res, elt, 0) + 1
        end
        return res
end

function reduceByKey(op, lst)

        res = Dict{ASCIIString, Int64}()
        for elt in lst
                for word in elt
                        res[word[1]] = get(res, word[1], 0) + word[2]
                end
        end
        return res
end


function mapper(fns, lst)


        res = cell(length(lst))
        for i = 1:length(lst)
                ans = lst[i] 
             for j = 1:length(fns)
		  ans = fns[j](ans)
	     end
             res[i] = ans
        end

         return res
end



#path to file portions, use as input to mapper function...do not change, just change subarray (parameter to mapper)
files = {"txt/xaa", "txt/xab", "txt/xac", "txt/xad", "txt/xae", "txt/xaf","txt/xag","txt/xah","txt/xai","txt/xaj","txt/xal","txt/xam","txt/xae","txt/xan","txt/xao","txt/xap","txt/xaq","txt/xar","txt/xas","txt/xat","txt/xau","txt/xav","txt/xaw","txt/xax","txt/xay",}
#adjust the path
files = map(x->"/home/ubuntu/" * x, files)

#perform readall THEN split THEN localReduceByKey on file(s)
texts = mapper({readall, split, localReduceByKey},files[1:5])

#take results after applying list of functions to each element and combine similar items
#and return a hash map
ans = reduceByKey("+",texts)

print(length(ans))
