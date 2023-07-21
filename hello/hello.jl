function plus_pi(x = 0)

	return x + pi
end


module MyModule


function double_it(x)
	# println(typeof(x))
	return 2*x
end

function double_it_ip!(y, x)
	println(typeof(x))
	for i=1:length(x)
		y[i] = 2 * x[i]
	end
end


precompile(double_it, (Float64, ))
precompile(double_it, (Vector{Float64}, ))

end
