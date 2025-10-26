-- test qd library

local qd=require"mapm"
qd.digits(64)
qd.pi=qd.new"3.1415926535897932384626433832795028841971693993751058209749445923078164062862090"

------------------------------------------------------------------------------
print(qd.version)

------------------------------------------------------------------------------
print""
print"Exp by series"
function test(w,x,t)
	local s=t
	local S
	for n=1,1000 do
		t=t*x/n
		S=s
		s=s+t
		if s==S then print(w,n,s) return end
	end
	print(w,'*',s)
end

function TEST(x)
	print("\nexp",x)
	test("fp loop",x,1)
	print("fp lib",'',math.exp(x))
	test("qd loop",x,qd.new(1))
	print("qd lib",'',qd.exp(x))
end
TEST(1)
TEST(-1)
TEST(-50)

------------------------------------------------------------------------------
print""
print"Pi algorithm of order 4"

math.new=tonumber

-- http://pauillac.inria.fr/algo/bsolve/constant/pi/pi.html
function test(qd)
	PI=qd.new"3.1415926535897932384626433832795028841971693993751058209749445923078164062862090"
	local x=qd.sqrt(2)
	local p=2+x
	local y=qd.sqrt(x)
	print(-1,p)
	x=(y+1/y)/2
	p=p*(x+1)/(y+1)
	print(0,p)
	for i=1,20 do
		local P=p
		local t=qd.sqrt(x)
		y=(y*t+1/t)/(y+1)
		x=(t+1/t)/2
		p=p*(x+1)/(y+1)
		print(i,p)
		if p==P then break end
	end
	print("exact",PI)
	print("-",qd.abs(PI-p))
	return p
end

print"fp"
test(math)
print"qd"
test(qd)

------------------------------------------------------------------------------
print""
print"Square root of 2"

function mysqrt(x)
	local y,z=x,x
	repeat z,y=y,(y+x/y)/2 until z==y
	return y
end

print("fp math",math.sqrt(2))
print("fp mine",mysqrt(2))
a=qd.sqrt(2) print("qd sqrt",a)
b=mysqrt(qd.new(2)) print("qd mine",b)
R=qd.new"1.414213562373095048801688724209698078569671875376948073176679737990732478462107038850387534327641573"
print("exact",R)

------------------------------------------------------------------------------
print""
print"Is exp(pi*sqrt(163)) an integer?"

a=math.exp(math.pi*math.sqrt(163))
print("fp",string.format("%.64e",a),"\nfrac",a-math.floor(a))
a=qd.exp(qd.pi*qd.sqrt(163))
print("qd",a,"\nfrac",a-qd.floor(a))
R=qd.new"2.625374126407687439999999999992500725971981856888793538563373369908627075374103782106479101186073129511813461e17"
print("exact",R)

------------------------------------------------------------------------------
print""
print"Pi arctan expressions"

function math.div(x,y) return x/y end

function A(x,y) return M.atan(M.div(x,y)) end

function test(w,l,x1,y1,x2,y2)
	M=l
	print(w,x1,y1,x2,y2)
	pi=B(x1,y1,x2,y2)
	print(pi)
	print(M.pi)
	print(pi-M.pi)
end

function B(x1,y1,x2,y2) return 4*(4*A(x1,y1)-A(x2,y2)) end
test("fp",math,1,5,1,239)
test("qd",qd,1,5,1,239)

function B(x1,y1,x2,y2) return 4*(5*A(1,7)+2*A(3,79)) end
test("fp",math,1,7,3,79)
test("qd",qd,1,7,3,79)

R=qd.new"3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798214808651328"
print("exact")
print(R)

------------------------------------------------------------------------------
print""
print(qd.version)
