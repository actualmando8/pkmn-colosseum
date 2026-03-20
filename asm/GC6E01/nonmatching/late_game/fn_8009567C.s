stwu r1, -0x40(r1)
mflr r0
stw r0, 0x44(r1)
stmw r24, 0x20(r1)
mr r28, r3
mr r30, r4
lis r3, lbl_803FB380@ha
addi r3, r3, lbl_803FB380@l
lwz r31, 0xc(r3)
cmplwi r31, 0x0
beq @800965B4
mr r3, r31
li r4, 0x0
li r5, 0x6e
li r6, 0x0
bl fn_8012640C
clrlwi r3, r3, 16
bl fn_8011E778
mr r26, r3
cmplwi r26, 0x0
beq @800965B4
lha r0, 0x6(r30)
li r25, 0x1
cmpwi r0, 0x1b8
bge @80095708
cmpwi r0, 0x18b
bge @800956FC
cmpwi r0, 0x182
bge @80095790
cmpwi r0, 0x170
bge @80095724
b @80095790
@800956FC
cmpwi r0, 0x191
bge @80095790
b @80095724
@80095708
cmpwi r0, 0x1d3
bge @8009571C
cmpwi r0, 0x1ca
bge @80095790
b @80095724
@8009571C
cmpwi r0, 0x1d9
bge @80095790
@80095724
lis r3, lbl_803FB380@ha
addi r3, r3, lbl_803FB380@l
lbz r0, 0x1(r3)
cmpwi r0, 0x7
beq @80095750
bge @80095780
cmpwi r0, 0x5
bge @80095780
cmpwi r0, 0x3
bge @80095750
b @80095780
@80095750
lis r3, lbl_803FB380@ha
addi r3, r3, lbl_803FB380@l
lbz r0, 0x2(r3)
extsb r0, r0
cmpwi r0, 0x0
blt @80095778
cmpwi r0, 0x4
bgt @80095778
li r25, 0x1
b @80095784
@80095778
li r25, 0x0
b @80095784
@80095780
li r25, 0x0
@80095784
mr r3, r30
mr r4, r25
bl fn_80109220
@80095790
clrlwi r0, r25, 24
cmplwi r0, 0x0
beq @800965B4
lha r4, 0x6(r30)
li r0, -0x100
lbz r3, 0x8b(r28)
cmpwi r4, 0x57b
or r29, r3, r0
beq @80095910
bge @8009586C
cmpwi r4, 0x14b
beq @800960B8
bge @8009581C
cmpwi r4, 0x144
beq @80095F78
bge @800957F8
cmpwi r4, 0x13c
beq @80095940
bge @800957E8
cmpwi r4, 0x13b
bge @80095910
b @800965B4
@800957E8
cmpwi r4, 0x142
beq @80095988
bge @80095EFC
b @800965B4
@800957F8
cmpwi r4, 0x148
beq @80095FF8
bge @80095810
cmpwi r4, 0x147
bge @80095FB8
b @800965B4
@80095810
cmpwi r4, 0x14a
bge @80096078
b @80096038
@8009581C
cmpwi r4, 0x158
beq @80096330
bge @80095854
cmpwi r4, 0x154
beq @80096210
bge @80095848
cmpwi r4, 0x153
bge @80096188
cmpwi r4, 0x14d
bge @800965B4
b @800960F8
@80095848
cmpwi r4, 0x156
bge @800965B4
b @800962C0
@80095854
cmpwi r4, 0x54e
beq @80095BC8
bge @800965B4
cmpwi r4, 0x54d
bge @800961CC
b @800965B4
@8009586C
cmpwi r4, 0x58f
beq @80096188
bge @800958CC
cmpwi r4, 0x584
beq @80095FF8
bge @800958A8
cmpwi r4, 0x581
beq @80095988
bge @8009589C
cmpwi r4, 0x57d
beq @80095940
b @800965B4
@8009589C
cmpwi r4, 0x583
bge @80095FB8
b @80095E84
@800958A8
cmpwi r4, 0x587
beq @800960B8
bge @800958C0
cmpwi r4, 0x586
bge @80096078
b @80096038
@800958C0
cmpwi r4, 0x589
bge @800965B4
b @800960F8
@800958CC
cmpwi r4, 0x595
beq @800963CC
bge @800958F0
cmpwi r4, 0x592
beq @800962C0
bge @800965B4
cmpwi r4, 0x591
bge @80096210
b @800961CC
@800958F0
cmpwi r4, 0x12b8
bge @80095904
cmpwi r4, 0x599
beq @80095BC8
b @800965B4
@80095904
cmpwi r4, 0x12bc
bge @800965B4
b @800963CC
@80095910
mr r3, r26
li r4, 0x0
bl fn_8011E474
clrlwi r3, r3, 24
bl fn_8010C46C
clrlwi r6, r3, 16
mr r5, r28
li r3, 0x0
li r4, 0x0
li r7, 0x0
bl fn_801040F0
b @800965B4
@80095940
mr r3, r26
li r4, 0x0
bl fn_8011E474
clrlwi r24, r3, 24
mr r3, r26
li r4, 0x1
bl fn_8011E474
clrlwi r3, r3, 24
cmplw r24, r3
beq @800965B4
bl fn_8010C46C
clrlwi r6, r3, 16
mr r5, r28
li r3, 0x0
li r4, 0x0
li r7, 0x0
bl fn_801040F0
b @800965B4
@80095988
mr r3, r31
bl fn_8011F77C
clrlwi r0, r3, 24
cmplwi r0, 0x3
bge @800959A4
li r24, 0x934
b @800959C8
@800959A4
mr r3, r31
li r4, 0x0
li r5, 0xbf
li r6, 0x0
bl fn_8012640C
clrlwi r3, r3, 24
bl fn_8011CE18
bl fn_8011CE00
mr r24, r3
@800959C8
mr r4, r24
li r3, 0x55
bl fn_80132A38
cmpwi r24, 0xc96
beq @800959EC
bge @800959FC
cmpwi r24, 0xc86
beq @800959EC
b @800959FC
@800959EC
li r3, 0x56
li r4, 0x1
bl fn_80132A38
b @80095A08
@800959FC
li r3, 0x56
li r4, 0x2bd8
bl fn_80132A38
@80095A08
mr r3, r31
li r4, 0x0
li r5, 0x72
li r6, 0x0
bl fn_8012640C
clrlwi r4, r3, 24
cmplwi r4, 0x0
bne @80095A2C
li r4, 0x5
@80095A2C
li r3, 0x34
bl fn_80132A38
lis r3, lbl_803FB380@ha
cmplwi r31, 0x0
addi r3, r3, lbl_803FB380@l
lwz r25, 0x8(r3)
bne @80095A50
li r0, 0x0
b @80095B60
@80095A50
mr r3, r31
li r4, 0x0
li r5, 0x70
li r6, 0x0
bl fn_8012640C
li r4, 0x2
bl fn_80135938
clrlwi r0, r3, 24
cmplwi r0, 0xb
beq @80095A80
li r0, 0x0
b @80095B60
@80095A80
lis r3, lbl_803FB380@ha
addi r3, r3, lbl_803FB380@l
lbz r0, 0x0(r3)
rlwinm r0, r0, 0, 26, 26
cmpwi r0, 0x0
beq @80095ACC
cmplwi r25, 0x0
bne @80095AAC
li r3, 0x0
bl fn_801F2A7C
mr r25, r3
@80095AAC
cmplwi r25, 0x0
bne @80095ABC
li r0, 0x0
b @80095B60
@80095ABC
mr r3, r25
bl fn_801FCEAC
mr r25, r3
b @80095AF8
@80095ACC
li r3, 0x8ae
bl fn_801906A0
cmplwi r3, 0x0
bne @80095AF0
li r3, 0x0
li r4, 0x2
bl fn_80129280
mr r25, r3
b @80095AF8
@80095AF0
bl fn_8006AEEC
mr r25, r3
@80095AF8
mr r3, r25
bl fn_8012AC54
mr r26, r3
mr r3, r25
bl fn_8012AC3C
mr r25, r3
mr r3, r31
li r4, 0x0
li r5, 0x75
li r6, 0x0
bl fn_8012640C
cmplw r25, r3
bne @80095B5C
mr r3, r31
li r4, 0x0
li r5, 0x76
li r6, 0x0
bl fn_8012640C
mr r4, r3
mr r3, r26
bl fn_800F9EE4
cmpwi r3, 0x0
bne @80095B5C
li r0, 0x1
b @80095B60
@80095B5C
li r0, 0x0
@80095B60
clrlwi r0, r0, 24
cmplwi r0, 0x0
beq @80095BA8
mr r3, r31
li r4, 0x0
li r5, 0x6e
li r6, 0x0
bl fn_8012640C
clrlwi r0, r3, 16
cmpwi r0, 0xc6
bge @80095BA0
cmpwi r0, 0xc4
bge @80095B98
b @80095BA0
@80095B98
li r8, 0x2be3
b @80095BAC
@80095BA0
li r8, 0x2bcd
b @80095BAC
@80095BA8
li r8, 0x2bcd
@80095BAC
lha r5, 0x54(r30)
mr r7, r29
lha r6, 0x56(r30)
li r3, 0x0
li r4, 0x0
bl fn_800FBB34
b @800965B4
@80095BC8
mr r3, r31
li r26, 0x2be6
li r4, 0x0
li r5, 0x71
li r6, 0x0
bl fn_8012640C
mr r28, r3
bl fn_8011396C
lis r4, lbl_803FB380@ha
cmplwi r31, 0x0
addi r4, r4, lbl_803FB380@l
mr r27, r3
lwz r24, 0x8(r4)
bne @80095C08
li r0, 0x0
b @80095D18
@80095C08
mr r3, r31
li r4, 0x0
li r5, 0x70
li r6, 0x0
bl fn_8012640C
li r4, 0x2
bl fn_80135938
clrlwi r0, r3, 24
cmplwi r0, 0xb
beq @80095C38
li r0, 0x0
b @80095D18
@80095C38
lis r3, lbl_803FB380@ha
addi r3, r3, lbl_803FB380@l
lbz r0, 0x0(r3)
rlwinm r0, r0, 0, 26, 26
cmpwi r0, 0x0
beq @80095C84
cmplwi r24, 0x0
bne @80095C64
li r3, 0x0
bl fn_801F2A7C
mr r24, r3
@80095C64
cmplwi r24, 0x0
bne @80095C74
li r0, 0x0
b @80095D18
@80095C74
mr r3, r24
bl fn_801FCEAC
mr r25, r3
b @80095CB0
@80095C84
li r3, 0x8ae
bl fn_801906A0
cmplwi r3, 0x0
bne @80095CA8
li r3, 0x0
li r4, 0x2
bl fn_80129280
mr r25, r3
b @80095CB0
@80095CA8
bl fn_8006AEEC
mr r25, r3
@80095CB0
mr r3, r25
bl fn_8012AC54
mr r24, r3
mr r3, r25
bl fn_8012AC3C
mr r25, r3
mr r3, r31
li r4, 0x0
li r5, 0x75
li r6, 0x0
bl fn_8012640C
cmplw r25, r3
bne @80095D14
mr r3, r31
li r4, 0x0
li r5, 0x76
li r6, 0x0
bl fn_8012640C
mr r4, r3
mr r3, r24
bl fn_800F9EE4
cmpwi r3, 0x0
bne @80095D14
li r0, 0x1
b @80095D18
@80095D14
li r0, 0x0
@80095D18
clrlwi r0, r0, 24
cmplwi r0, 0x0
beq @80095D7C
mr r3, r31
li r4, 0x0
li r5, 0x6e
li r6, 0x0
bl fn_8012640C
clrlwi r0, r3, 16
cmpwi r0, 0xc6
bge @80095D58
cmpwi r0, 0xc4
bge @80095D50
b @80095D58
@80095D50
li r26, 0x2be4
b @80095E64
@80095D58
cmplwi r27, 0x0
beq @80095E64
mr r3, r27
bl fn_800FA280
mr r4, r3
li r3, 0x37
bl fn_80132A38
li r26, 0x2bd7
b @80095E64
@80095D7C
mr r3, r31
li r4, 0x0
li r5, 0x70
li r6, 0x0
bl fn_8012640C
li r4, 0x2
bl fn_80135938
clrlwi r0, r3, 24
cmpwi r0, 0xb
beq @80095DB4
bge @80095E64
cmpwi r0, 0x8
bge @80095E58
b @80095E64
@80095DB4
mr r3, r31
li r4, 0x0
li r5, 0x75
li r6, 0x0
bl fn_8012640C
addis r0, r3, 0x0
cmplwi r0, 0x911d
bne @80095E0C
li r3, 0x12ac
bl fn_800FA280
mr r25, r3
mr r3, r31
li r4, 0x0
li r5, 0x76
li r6, 0x0
bl fn_8012640C
mr r4, r25
bl fn_800F9EE4
cmpwi r3, 0x0
bne @80095E0C
li r0, 0x1
b @80095E10
@80095E0C
li r0, 0x0
@80095E10
clrlwi r0, r0, 24
cmplwi r0, 0x0
beq @80095E24
li r26, 0x2be7
b @80095E64
@80095E24
cmplwi r28, 0xff
bne @80095E34
li r26, 0x2be5
b @80095E64
@80095E34
cmplwi r27, 0x0
beq @80095E64
mr r3, r27
bl fn_800FA280
mr r4, r3
li r3, 0x37
bl fn_80132A38
li r26, 0x2bd7
b @80095E64
@80095E58
cmplwi r28, 0xff
bne @80095E64
li r26, 0x2be5
@80095E64
lha r5, 0x54(r30)
mr r7, r29
lha r6, 0x56(r30)
mr r8, r26
li r3, 0x0
li r4, 0x0
bl fn_800FBB34
b @800965B4
@80095E84
mr r3, r31
bl fn_8011F77C
clrlwi r0, r3, 24
cmplwi r0, 0x6
bgt @80095EE4
lis r3, jumptable_802EF01C@ha
slwi r0, r0, 2
addi r3, r3, jumptable_802EF01C@l
lwzx r0, r3, r0
mtctr r0
bctr
li r27, 0x2bd9
b @80095EE4
li r27, 0x2bda
b @80095EE4
li r27, 0x2bdb
b @80095EE4
li r27, 0x2bdc
b @80095EE4
li r27, 0x2bdd
b @80095EE4
li r27, 0x2bde
b @80095EE4
li r27, 0x2bdf
@80095EE4
mr r5, r29
mr r6, r27
li r3, 0x0
li r4, 0x0
bl fn_800FB680
b @800965B4
@80095EFC
mr r3, r31
li r4, 0x0
li r5, 0x7a
li r6, 0x0
bl fn_8012640C
clrlwi r4, r3, 24
mr r3, r31
addi r0, r4, 0x1
clrlwi r4, r0, 24
bl fn_801229F4
mr r26, r3
cmplwi r26, 0x0
bne @80095F38
li r4, 0x0
b @80095F50
@80095F38
mr r3, r31
li r4, 0x0
li r5, 0x79
li r6, 0x0
bl fn_8012640C
subf r4, r3, r26
@80095F50
li r3, 0x34
bl fn_80132A38
lha r5, 0x54(r30)
mr r7, r29
lha r6, 0x56(r30)
li r3, 0x0
li r4, 0x0
li r8, 0xde
bl fn_800FBB34
b @800965B4
@80095F78
mr r3, r31
li r4, 0x0
li r5, 0x79
li r6, 0x0
bl fn_8012640C
mr r4, r3
li r3, 0x34
bl fn_80132A38
lha r5, 0x54(r30)
mr r7, r29
lha r6, 0x56(r30)
li r3, 0x0
li r4, 0x0
li r8, 0xde
bl fn_800FBB34
b @800965B4
@80095FB8
mr r3, r31
li r4, 0x0
li r5, 0x8c
li r6, 0x0
bl fn_8012640C
extsh r4, r3
li r3, 0x34
bl fn_80132A38
lha r5, 0x54(r30)
mr r7, r29
lha r6, 0x56(r30)
li r3, 0x0
li r4, 0x0
li r8, 0xde
bl fn_800FBB34
b @800965B4
@80095FF8
mr r3, r31
li r4, 0x0
li r5, 0x8b
li r6, 0x0
bl fn_8012640C
extsh r4, r3
li r3, 0x34
bl fn_80132A38
lha r5, 0x54(r30)
mr r7, r29
lha r6, 0x56(r30)
li r3, 0x0
li r4, 0x0
li r8, 0xde
bl fn_800FBB34
b @800965B4
@80096038
mr r3, r31
li r4, 0x0
li r5, 0x8a
li r6, 0x0
bl fn_8012640C
extsh r4, r3
li r3, 0x34
bl fn_80132A38
lha r5, 0x54(r30)
mr r7, r29
lha r6, 0x56(r30)
li r3, 0x0
li r4, 0x0
li r8, 0xde
bl fn_800FBB34
b @800965B4
@80096078
mr r3, r31
li r4, 0x0
li r5, 0x89
li r6, 0x0
bl fn_8012640C
extsh r4, r3
li r3, 0x34
bl fn_80132A38
lha r5, 0x54(r30)
mr r7, r29
lha r6, 0x56(r30)
li r3, 0x0
li r4, 0x0
li r8, 0xde
bl fn_800FBB34
b @800965B4
@800960B8
mr r3, r31
li r4, 0x0
li r5, 0x88
li r6, 0x0
bl fn_8012640C
extsh r4, r3
li r3, 0x34
bl fn_80132A38
lha r5, 0x54(r30)
mr r7, r29
lha r6, 0x56(r30)
li r3, 0x0
li r4, 0x0
li r8, 0xde
bl fn_800FBB34
b @800965B4
@800960F8
mr r3, r31
li r4, 0x0
li r5, 0x83
li r6, 0x0
bl fn_8012640C
extsh r4, r3
li r3, 0x34
bl fn_80132A38
lha r6, 0x56(r30)
mr r7, r29
li r3, 0x0
li r4, 0x0
li r5, 0x37
li r8, 0xde
bl fn_800FBB34
mr r5, r29
li r3, 0x37
li r4, 0x0
li r6, 0x2bd4
bl fn_800FB680
mr r3, r31
li r4, 0x0
li r5, 0x87
li r6, 0x0
bl fn_8012640C
extsh r4, r3
li r3, 0x34
bl fn_80132A38
lha r5, 0x54(r30)
mr r7, r29
lha r6, 0x56(r30)
li r3, 0x0
li r4, 0x0
li r8, 0xde
bl fn_800FBB34
b @800965B4
@80096188
mr r3, r31
bl fn_801248C4
clrlwi r3, r3, 16
bl fn_8011CB6C
bl fn_8011CB3C
bl fn_800FA280
mr r4, r3
li r3, 0x37
bl fn_80132A38
lha r5, 0x54(r30)
mr r7, r29
lha r6, 0x56(r30)
li r3, 0x0
li r4, 0x0
li r8, 0xcf
bl fn_800FBB34
b @800965B4
@800961CC
mr r3, r31
bl fn_801248C4
clrlwi r3, r3, 16
bl fn_8011CB6C
bl fn_8011CB54
bl fn_800FA280
mr r4, r3
li r3, 0x37
bl fn_80132A38
lha r5, 0x54(r30)
mr r7, r29
lha r6, 0x56(r30)
li r3, 0x0
li r4, 0x0
li r8, 0xcf
bl fn_800FBB34
b @800965B4
@80096210
lwz r3, 0x4c(r30)
bl fn_800FA444
srwi r0, r3, 16
mr r3, r31
extsh r0, r0
mr r24, r0
bl fn_8011FC74
clrlwi r0, r3, 24
cmplwi r0, 0x1
bne @80096250
mr r3, r24
mr r5, r29
li r4, 0x0
li r6, 0x2b70
bl fn_800FB680
b @800965B4
@80096250
mr r3, r31
li r4, 0x0
li r5, 0x75
li r6, 0x0
bl fn_8012640C
mr r27, r24
clrlwi r24, r3, 16
li r28, 0x2710
li r25, 0x0
lis r3, 0xcccd
subi r26, r3, 0x3333
@8009627C
divwu r4, r24, r28
li r3, 0x34
mullw r5, r4, r28
mulhwu r0, r26, r28
subf r24, r5, r24
srwi r28, r0, 3
bl fn_80132A38
mr r3, r27
mr r5, r29
li r4, 0x0
li r6, 0xca
bl fn_800FB680
addi r27, r27, 0xd
addi r25, r25, 0x1
cmpwi r25, 0x5
blt @8009627C
b @800965B4
@800962C0
mr r3, r31
bl fn_8011FC74
clrlwi r0, r3, 24
cmplwi r0, 0x1
bne @800962EC
li r3, 0x2b70
bl fn_800FA280
mr r4, r3
li r3, 0x37
bl fn_80132A38
b @8009630C
@800962EC
mr r3, r31
li r4, 0x0
li r5, 0x76
li r6, 0x0
bl fn_8012640C
mr r4, r3
li r3, 0x37
bl fn_80132A38
@8009630C
lwz r3, 0x4c(r30)
bl fn_800FA444
srwi r0, r3, 16
mr r5, r29
extsh r3, r0
li r4, 0x0
li r6, 0xcf
bl fn_800FB680
b @800965B4
@80096330
mr r3, r31
li r4, 0x0
li r5, 0x7a
li r6, 0x0
bl fn_8012640C
clrlwi r24, r3, 24
mr r3, r31
addi r0, r24, 0x1
clrlwi r4, r0, 24
bl fn_801229F4
mr r27, r3
cmplwi r27, 0x0
beq @800965B4
mr r3, r31
mr r4, r24
bl fn_801229F4
mr r26, r3
mr r3, r31
subf r24, r26, r27
li r4, 0x0
li r5, 0x79
li r6, 0x0
bl fn_8012640C
lha r0, 0x54(r30)
subf r3, r26, r3
lha r6, 0x56(r30)
mr r7, r29
mullw r0, r3, r0
mr r8, r28
li r3, 0x0
li r4, 0x0
li r9, 0x117
li r10, 0x0
add r5, r24, r0
subi r0, r5, 0x1
divwu r0, r0, r24
extsh r5, r0
bl fn_80104160
b @800965B4
@800963CC
cmpwi r4, 0x12b9
beq @80096410
bge @800963F0
cmpwi r4, 0x595
beq @80096400
blt @80096424
cmpwi r4, 0x12b8
bge @80096408
b @80096424
@800963F0
cmpwi r4, 0x12bb
beq @80096420
bge @80096424
b @80096418
@80096400
li r27, 0x0
b @80096424
@80096408
li r27, 0x1
b @80096424
@80096410
li r27, 0x2
b @80096424
@80096418
li r27, 0x3
b @80096424
@80096420
li r27, 0x4
@80096424
mr r3, r31
li r4, 0x0
li r5, 0xc4
li r6, 0x0
bl fn_8012640C
clrlwi r24, r3, 16
cmplwi r24, 0x0
bne @8009644C
lfs f4, lbl_8047C208@sda21(r0)
b @800964A0
@8009644C
mr r3, r31
bl fn_8011FC14
lis r0, 0x4330
stw r24, 0xc(r1)
lfd f2, lbl_8047C220@sda21(r0)
stw r0, 0x8(r1)
lfd f0, 0x8(r1)
fsubs f0, f0, f2
fcmpo cr0, f1, f0
ble @80096484
stw r24, 0x14(r1)
stw r0, 0x10(r1)
lfd f0, 0x10(r1)
fsubs f1, f0, f2
@80096484
lis r0, 0x4330
stw r24, 0x1c(r1)
lfd f2, lbl_8047C220@sda21(r0)
stw r0, 0x18(r1)
lfd f0, 0x18(r1)
fsubs f0, f0, f2
fdivs f4, f1, f0
@800964A0
lfs f0, lbl_8047C20C@sda21(r0)
fcmpo cr0, f4, f0
cror eq, gt, eq
bne @800964B8
li r0, 0x4
b @80096504
@800964B8
lfs f0, lbl_8047C210@sda21(r0)
fcmpo cr0, f4, f0
cror eq, gt, eq
bne @800964D0
li r0, 0x3
b @80096504
@800964D0
lfs f0, lbl_8047C214@sda21(r0)
fcmpo cr0, f4, f0
cror eq, gt, eq
bne @800964E8
li r0, 0x2
b @80096504
@800964E8
lfs f0, lbl_8047C218@sda21(r0)
fcmpo cr0, f4, f0
cror eq, gt, eq
bne @80096500
li r0, 0x1
b @80096504
@80096500
li r0, 0x0
@80096504
clrlwi r4, r0, 16
clrlwi r0, r27, 16
cmplw r4, r0
ble @8009653C
lha r5, 0x54(r30)
mr r7, r29
lha r6, 0x56(r30)
mr r8, r28
li r3, 0x0
li r4, 0x0
li r9, 0x116
li r10, 0x0
bl fn_80104160
b @800965B4
@8009653C
bne @800965B4
lis r3, 0x4330
lha r0, 0x54(r30)
stw r4, 0x1c(r1)
xoris r0, r0, 0x8000
lfd f1, lbl_8047C220@sda21(r0)
stw r3, 0x18(r1)
lfs f2, lbl_8047C218@sda21(r0)
lfd f0, 0x18(r1)
stw r0, 0x14(r1)
fsubs f0, f0, f1
lfs f3, lbl_8047C21C@sda21(r0)
stw r3, 0x10(r1)
lfd f1, lbl_8047C228@sda21(r0)
fmuls f2, f2, f0
lfd f0, 0x10(r1)
fsubs f0, f0, f1
fsubs f1, f4, f2
fmuls f1, f3, f1
fmuls f1, f0, f1
bl fn_800C46B0
lha r6, 0x56(r30)
extsh r5, r3
mr r7, r29
mr r8, r28
li r3, 0x0
li r4, 0x0
li r9, 0x116
li r10, 0x0
bl fn_80104160
@800965B4
lmw r24, 0x20(r1)
lwz r0, 0x44(r1)
mtlr r0
addi r1, r1, 0x40
blr
