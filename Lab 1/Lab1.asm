/ Kevin Siraki
/ Lab 1
/ CSIS 166

*0100
number= 0
main,	cla
		push
		tad aname
		jms i apUpCount
		dca s number
		cla
		tad amsg1
		jms i aprt_msg
		cla
		tad s number
		jms i adecout
		cla
		tad NL
		tpc
		
		cla
		tad afavorite
		jms i apUpCount
		dca s number
		tad amsg2
		jms i aprt_msg
		tad s number
		jms i adecout
		cla
		tad NL
		tpc
		pop
		hlt

/------main's strings & addresses--------
amsg2, msg2
amsg1, msg1
aname,			name
afavorite,		favorite
apUpCount,		pUpCount
adecout, decout
aprt_msg, prt_msg
name,			"Kevin Siraki\0"
favorite,		"Pizza with Olives.\0"
NL, '\n'
msg1, "Number for name converted is: \0"
msg2, "Number for favorite converted is: \0"


/------message printing routine --------
tos=		0 / top of stack
prt_msg,	push / save the message address
prt_loop,	tad is tos

			sna
			ret 1 / null character, exit
			tpc / else print the character
			isz s tos / update pointer
			cla
			jmp prt_loop

/-------pUpCount---------------------
str= 2
ch= 1
count= 0
pUpCount,	
			push
			push
			push
loop,
			cla
			tad si str
			dca s ch
			tad s ch
			tad na
			spa cla
			jmp printch
			tad s ch
			tad nz
			sma sza cla
			jmp printch
			tad s ch
			and mask
			dca s ch
			tad s count
			iac
			dca s count
printch,
			cla
			tad s ch
			tpc
			cla
			
			tad s str
			iac
			dca s str
			tad si str
			sza cla
			jmp loop
			cla
			tad newline
			tpc
			pop
			ret 2



/--------------upCount's constants-----
na,			-'a'
nz,			-'z'
mask,		0xDF
newline,	'\n'


/ decout - print decimal value , value to print in AC
decout,		mql / save the number
			push / push 0 onto stack to end print
			mqa / see if negative
			sma
			jmp deco_pos / positive number
			cma iac / make positive
			mql / update number
			tad minus / print minus
			tls / print

deco_pos,	cla
deco_lp1,	div / divide by 10 to get least digit

			10
			tad dec_asc0 / make printable
			push / push onto stack
			mqa / get quotient
			sza cla / quotient zero?
			jmp deco_lp1 / no, keep dividing
deco_lp2,	pop / else print all the digits
			sna / value 0?
			ret / yes, exit
			tls / no, print digit
			jmp deco_lp2 / next digit

minus,		'-'
dec_asc0,	'0'
