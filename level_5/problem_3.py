
#Problem 3:
#The prime factors of 13195 are 5, 7, 13 and 29.
#What is the largest prime factor of the number 600851475143?

string = []
a=600851475143
x = 4

while x < a:
	for i in range(3,x):
		if a%i==0:
			a = a/i
			string.append(i)
			break
		else:
			x+=1
	
string.append(a)
print(string)

print(max(string))


