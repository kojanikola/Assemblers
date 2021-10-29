.data
movb a, b
mov a(%r7), b
.skip 10
b:
.text
jmp *a
mov a(%r7), 5
jmp *a(%r7)
jmp a
jmp c
.skip 10
jmp b
.equ a, b
.end

.equ k, g
.equ a, b
.equ g, 10
.equ c, g+h
.end
#koki
d: .long 10
jne *10(%r14)
push q
pop %r2
d: 
	mov 	a, b
	mov	c, d
	mov	g, e
w:
.byte 10, 11
.byte d

.equ g, 10
.equ h, g+10
.equ c, g+h
b: 
	.equ p, 10
	jeq *%r12
	jne *(%r14)
	jne *10(%r14)
	jne *(%r14)
	call	d
	jmp	10

	jne *d(%r14)
	jne *d(%r7)
.data
.skip 	8
e:
	.long	c
	.long	.bss 
a:	
.bss
c:
.end
