	.file	"test.c"
	.option nopic
	.attribute arch, "rv64i2p1_m2p0_a2p1_f2p2_d2p2_c2p0_zicsr2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	1
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-32
	sd	s0,24(sp)
	addi	s0,sp,32
	li	a5,10
	sw	a5,-20(s0)
	lw	a5,-20(s0)
	addiw	a5,a5,-11
	sext.w	a5,a5
	mv	a0,a5
	ld	s0,24(sp)
	addi	sp,sp,32
	jr	ra
	.size	main, .-main
	.align	1
	.globl	fact
	.type	fact, @function
fact:
	addi	sp,sp,-32
	sd	ra,24(sp)
	sd	s0,16(sp)
	addi	s0,sp,32
	mv	a5,a0
	sw	a5,-20(s0)
	lw	a5,-20(s0)
	sext.w	a4,a5
	li	a5,1
	bne	a4,a5,.L4
	lw	a5,-20(s0)
	j	.L5
.L4:
	lw	a5,-20(s0)
	addiw	a5,a5,-1
	sext.w	a5,a5
	mv	a0,a5
	call	fact
	mv	a5,a0
	lw	a4,-20(s0)
	mulw	a5,a4,a5
	sext.w	a5,a5
.L5:
	mv	a0,a5
	ld	ra,24(sp)
	ld	s0,16(sp)
	addi	sp,sp,32
	jr	ra
	.size	fact, .-fact
	.ident	"GCC: (g2ee5e430018) 12.2.0"
