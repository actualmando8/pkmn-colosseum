stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stmw r27, 0xc(r1)
mr r29, r3
mr r30, r4
mr r27, r5
mr r31, r6
li r28, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @80068BFC
bge @80068BFC
cmpwi r3, 0x0
bge @80068BF0
b @80068BFC
@80068BF0
cmpwi r27, 0x2
blt @80068BFC
li r28, 0x0
@80068BFC
clrlwi r0, r28, 24
cmplwi r0, 0x0
beq @80068DA8
lha r0, 0x6(r30)
lis r3, lbl_802EDA20@ha
addi r3, r3, lbl_802EDA20@l
li r4, 0x0
clrlwi r5, r0, 16
li r0, 0x8
mtctr r0
@80068C24
lhz r0, 0x0(r3)
cmplw r0, r5
bne @80068C38
lhz r0, 0x2(r3)
b @80068D08
@80068C38
addi r3, r3, 0x4
lhz r0, 0x0(r3)
cmplw r0, r5
bne @80068C50
lhz r0, 0x2(r3)
b @80068D08
@80068C50
addi r3, r3, 0x4
lhz r0, 0x0(r3)
cmplw r0, r5
bne @80068C68
lhz r0, 0x2(r3)
b @80068D08
@80068C68
addi r3, r3, 0x4
lhz r0, 0x0(r3)
cmplw r0, r5
bne @80068C80
lhz r0, 0x2(r3)
b @80068D08
@80068C80
addi r3, r3, 0x4
lhz r0, 0x0(r3)
cmplw r0, r5
bne @80068C98
lhz r0, 0x2(r3)
b @80068D08
@80068C98
addi r3, r3, 0x4
lhz r0, 0x0(r3)
cmplw r0, r5
bne @80068CB0
lhz r0, 0x2(r3)
b @80068D08
@80068CB0
addi r3, r3, 0x4
lhz r0, 0x0(r3)
cmplw r0, r5
bne @80068CC8
lhz r0, 0x2(r3)
b @80068D08
@80068CC8
addi r3, r3, 0x4
lhz r0, 0x0(r3)
cmplw r0, r5
bne @80068CE0
lhz r0, 0x2(r3)
b @80068D08
@80068CE0
addi r3, r3, 0x4
lhz r0, 0x0(r3)
cmplw r0, r5
bne @80068CF8
lhz r0, 0x2(r3)
b @80068D08
@80068CF8
addi r3, r3, 0x4
addi r4, r4, 0x8
bdnz @80068C24
li r0, 0x0
@80068D08
clrlwi r4, r0, 16
mr r3, r27
bl fn_8025D970
mr r28, r3
cmplwi r28, 0x0
beq @80068DA8
bl fn_80123FBC
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80068D44
lbz r0, 0x4(r30)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r30)
b @80068DA8
@80068D44
mr r3, r28
li r4, 0x0
li r5, 0x7a
li r6, 0x0
bl fn_8012640C
clrlwi r4, r3, 24
li r3, 0x34
bl fn_80132A38
cmpwi r31, 0x0
bne @80068D8C
lbz r5, 0x8b(r29)
li r0, -0x100
li r3, 0x0
li r4, 0x0
or r5, r5, r0
li r6, 0x30d4
bl fn_800FB680
b @80068DA8
@80068D8C
lbz r5, 0x8b(r29)
li r0, -0x100
li r3, 0x0
li r4, 0x0
or r5, r5, r0
li r6, 0xd3
bl fn_800FB680
@80068DA8
lmw r27, 0xc(r1)
lwz r0, 0x24(r1)
mtlr r0
addi r1, r1, 0x20
blr
