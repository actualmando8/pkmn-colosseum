stwu r1, -0x40(r1)
mflr r0
stw r0, 0x44(r1)
stmw r24, 0x20(r1)
mr r30, r3
mr r31, r4
lis r4, 0x8000
lis r3, 0x1062
lwz r0, 0xf8(r4)
addi r3, r3, 0x4dd3
srwi r0, r0, 2
mulhwu r0, r3, r0
srwi r0, r0, 6
mulli r24, r0, 0x5
bl OSGetTick
mr r27, r3
@80073A84
bl OSGetTick
subf r4, r27, r3
mr r3, r30
xor r0, r4, r24
cntlzw r0, r0
slw r0, r4, r0
srwi r25, r0, 31
bl fn_80073C38
cmpwi r3, 0x1
bne @80073AB4
cmpwi r25, 0x0
beq @80073A84
@80073AB4
cmpwi r3, 0x0
beq @80073AC0
b @80073C24
@80073AC0
li r0, 0xaa
mr r3, r30
stw r0, 0xc(r1)
addi r4, r1, 0xc
addi r5, r1, 0x9
bl fn_8025F648
cmpwi r3, 0x0
beq @80073AE8
li r3, 0xb
b @80073BDC
@80073AE8
lis r4, 0x8000
lis r3, 0x1062
lwz r0, 0xf8(r4)
addi r3, r3, 0x4dd3
srwi r0, r0, 2
mulhwu r0, r3, r0
srwi r0, r0, 6
mulli r26, r0, 0x64
bl OSGetTick
lis r5, lbl_803B6E18@ha
lis r4, lbl_803B6E08@ha
slwi r6, r30, 3
mr r27, r3
addi r0, r5, lbl_803B6E18@l
slwi r28, r30, 2
add r24, r0, r6
addi r29, r4, lbl_803B6E08@l
addi r25, r24, 0x4
@80073B30
bl OSGetTick
subf r0, r27, r3
cmplw r0, r26
ble @80073B48
li r3, 0x1
b @80073BC8
@80073B48
mr r3, r30
addi r4, r1, 0x8
bl fn_8025F3F4
cmpwi r3, 0x0
beq @80073B64
li r3, 0x2
b @80073BC8
@80073B64
lbz r0, 0x8(r1)
andi. r0, r0, 0xa
cmpwi r0, 0x8
beq @80073BA4
lwz r12, 0x0(r24)
cmplwi r12, 0x0
beq @80073B90
mr r3, r30
lwz r4, 0x0(r25)
mtctr r12
bctrl
@80073B90
lwzx r0, r29, r28
cmpwi r0, 0x0
beq @80073B30
li r3, 0x3e8
b @80073BC8
@80073BA4
mr r3, r30
addi r4, r1, 0x10
addi r5, r1, 0x9
bl fn_8025F584
cmpwi r3, 0x0
beq @80073BC4
li r3, 0x3
b @80073BC8
@80073BC4
li r3, 0x0
@80073BC8
cmpwi r3, 0x0
beq @80073BD8
addi r3, r3, 0xb
b @80073BDC
@80073BD8
li r3, 0x0
@80073BDC
cmpwi r3, 0x0
beq @80073BE8
b @80073C24
@80073BE8
lwz r4, 0x10(r1)
srwi r5, r4, 24
cmplwi r5, 0xaa
beq @80073C00
li r3, 0xf
b @80073C24
@80073C00
rlwinm r0, r4, 24, 16, 23
rlwinm r3, r4, 8, 8, 15
or r0, r5, r0
slwi r4, r4, 24
or r0, r3, r0
li r3, 0x0
or r0, r4, r0
srwi r0, r0, 16
sth r0, 0x0(r31)
@80073C24
lmw r24, 0x20(r1)
lwz r0, 0x44(r1)
mtlr r0
addi r1, r1, 0x40
blr
