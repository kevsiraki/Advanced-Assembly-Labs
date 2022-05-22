/Kevin Siraki
/Lab 2
/CS 166
					* 0100
clear, 				cla
					dca m
main, 				cla
					tad apromptMsg
					jms i aprt_msg
					krb 				/read the selection char
					dca select
					tad select
					tad mq
					sna cla
					jmp quit
					tad select
					tad mc
					sza cla
					jmp do_plus
					dca m
					jms i aprint_m
					cla
					tad newline
					tls
					jmp main
quit,				cla
					tad aquitm
					jms i aprt_msg
					cla
					hlt
do_plus,			tad select
					tad mplus
					sza cla
					jmp do_minus
					krb 				/clearing the keyboard buffer
					cla
					tad anumber
					jms i aprt_msg
					jms i adecin
					tad m
					dca m
					jms print_m
					jmp main
do_minus,			tad select
					tad mminus
					sza cla
					jmp main
					krb 				/clearing keyboard buffer
					cla
					tad anumber
					jms i aprt_msg
					jms i adecin
					cma iac				/twos comp (negate and add 1)
					tad m				/subtract
					dca m
					jms print_m 		/print number
					jmp main
					

					
apromptMsg,		 	promptMsg
select, 			.-. 				/save selection
m, 					.-. 				/save number in m
mq, 				-'q'
mc, 				-'c'
mplus,	 			-'+'
quitmsg,			"Done!\n\0"
aquitm,				quitmsg
adecin, 			decin
aprt_msg, 			prt_msg
number, 			"Number? \0"
anumber, 			number
aprint_m, 			print_m
mminus, 			-'-'
newline, 			'\n'
anewline, 			newline

promptMsg, 			"Enter c,+,-,q:	 \0"

/Output printing routine
print_m, 			tad amis 			/print
					jms prt_msg
					tad m
					jms decout 			/print the value
					krb					/clearing the keyboard buffer
					ret
mis, 				"M is \0"
amis,				mis

/message printing routine
tos=				0 					/top of stack
prt_msg, 			push 				/save the message address
prt_loop,			tad is tos
					sna
					ret 1 				/null character, exit
					tpc			 		/else print the character
					isz s tos 			/update pointer
					cla
					jmp prt_loop

/decout - print decimal value , value to print in AC
decout,				mql 			 	/save the number
					push 				/push 0 onto stack to end print
					mqa 				/see if negative
					sma
					jmp deco_pos 		/positive number
					cma iac 			/ make positive
					mql			 		/update number
					tad minus 			/print minus
					tls 				/print
deco_pos, 			cla
deco_lp1, 			div 				/divide by 10 to get least digit
					10
					tad dec_asc0 		/ make printable
					push 				/push onto stack
					mqa	 				/get quotient
					sza cla 			/quotient zero?
					jmp deco_lp1 		/no, keep dividing
deco_lp2, 			pop 				/else print all the digits
					sna 				/value 0?
					ret 				/yes, exit
					tls 				/no, print digit
					jmp deco_lp2 		/next digit
minus,				'-'
dec_asc0, 			'0'

dec_n0, 			-'0'
dec_n10, 			-10
iprtmsg, 			prt_msg

/ offsets for decimal input
dec_flag= 0	 / flag,
dec_digit= 1 	/ current digit

/ input decimal number
dec_minus,			-'-'
dec_negnl, 			-'\n'
decin, 				enter 2 			/make space for minus flag, and digit
					dca s dec_flag 		/clear flag
					mql 				/clear multiple quotient
					krb 				/read first digit
					dca s dec_digit 	/save it
					tad dec_minus 		/minus sign?
					tad s dec_digit
					sza cla
					jmp deci_lp 		/no, positive
					iac 				/yes, set flag for negate
					dca s dec_flag
					tad dec_asc0
					dca s dec_digit 	/replace minus with leading zero
deci_lp, 			tad dec_negnl 		/newline?
					tad s dec_digit
					sza cla
					jmp deci_cont 		/no, add digit
					tad s dec_flag 		/exit, but should negate number?
					sna cla
					jmp deci_ret 		/no, just leave
					mqa 				/get value
					cma iac 			/negate
					mql 				/update
deci_ret, 			leave 2 			/get remove other parameters
					cla
					mqa 				/get answer
					ret
deci_cont, 			tad s dec_digit 	/get ascii digit
					tad dec_n0 			/make into a value
					dca s dec_digit
					tad dec_n10 		/in range?
					tad s dec_digit
					spa cla
					jmp deci_m10 		/yes, add to number
					tad dec_errin 		/no, print error and leave
					jms i iprtmsg 		/print messag
					jmp deci_ret
deci_m10, 			muy 				/multiply by 10
					10
					mqa 				/get answer
					tad s dec_digit 	/add digit
					mql 				/update number
					krb 				/get next digit
					dca s dec_digit
					jmp deci_lp
dec_errin, 			"Invalid digit\n\0"