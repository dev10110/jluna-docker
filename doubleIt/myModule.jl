module MyModule


function double_it(x)
	# println(typeof(x))
	return 2*x
end

function double_it!(y, x, N)
        # println(typeof(x))
	for i=1:N
		y[i] = 2 * x[i]
	end
end


# precompile(double_it, (Float64, ))
# precompile(double_it, (Vector{Float64}, ))
# precompile(double_it!, (Vector{Float64}, Vector{Float64}, UInt64))

end
