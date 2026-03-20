stwu r1, -0x60(r1)
mflr r0
stw r0, 0x64(r1)
stmw r15, 0x1c(r1)
mr r21, r3
mr r22, r4
mr r23, r5
addi r26, r21, 0x1
li r4, 0x2
mr r3, r26
bl fn_8008ABE4
lis r4, 0x8000
lis r3, 0x1062
lwz r0, 0xf8(r4)
addi r3, r3, 0x4dd3
srwi r0, r0, 2
mulhwu r0, r3, r0
srwi r0, r0, 6
mulli r25, r0, 0x64
bl OSGetTick
lis r5, lbl_803B6E18@ha
lis r4, lbl_803B6E08@ha
slwi r6, r21, 3
mr r29, r3
addi r0, r5, lbl_803B6E18@l
slwi r30, r21, 2
add r27, r0, r6
addi r31, r4, lbl_803B6E08@l
addi r28, r27, 0x4
@80072DCC
bl OSGetTick
subf r4, r29, r3
mr r3, r21
xor r0, r4, r25
cntlzw r0, r0
slw r0, r4, r0
srwi r24, r0, 31
bl fn_80073C38
cmpwi r3, 0x0
beq @80072DFC
mr r15, r3
b @80072FF0
@80072DFC
li r0, 0x66
mr r3, r21
stw r0, 0x10(r1)
addi r4, r1, 0x10
addi r5, r1, 0xa
bl fn_8025F648
cmpwi r3, 0x0
beq @80072E24
li r15, 0xb
b @80072EF8
@80072E24
lis r4, 0x8000
lis r3, 0x1062
lwz r0, 0xf8(r4)
addi r3, r3, 0x4dd3
srwi r0, r0, 2
mulhwu r0, r3, r0
srwi r0, r0, 6
mulli r16, r0, 0x64
bl OSGetTick
mr r18, r3
@80072E4C
bl OSGetTick
subf r0, r18, r3
cmplw r0, r16
ble @80072E64
li r3, 0x1
b @80072EE4
@80072E64
mr r3, r21
addi r4, r1, 0x8
bl fn_8025F3F4
cmpwi r3, 0x0
beq @80072E80
li r3, 0x2
b @80072EE4
@80072E80
lbz r0, 0x8(r1)
andi. r0, r0, 0xa
cmpwi r0, 0x8
beq @80072EC0
lwz r12, 0x0(r27)
cmplwi r12, 0x0
beq @80072EAC
mr r3, r21
lwz r4, 0x0(r28)
mtctr r12
bctrl
@80072EAC
lwzx r0, r31, r30
cmpwi r0, 0x0
beq @80072E4C
li r3, 0x3e8
b @80072EE4
@80072EC0
mr r3, r21
addi r4, r1, 0x14
addi r5, r1, 0xa
bl fn_8025F584
cmpwi r3, 0x0
beq @80072EE0
li r3, 0x3
b @80072EE4
@80072EE0
li r3, 0x0
@80072EE4
cmpwi r3, 0x0
beq @80072EF4
addi r15, r3, 0xb
b @80072EF8
@80072EF4
li r15, 0x0
@80072EF8
cmpwi r15, 0x0
beq @80072F04
b @80072FF0
@80072F04
lwz r0, 0x14(r1)
srwi r0, r0, 24
cmplwi r0, 0x66
beq @80072F1C
li r15, 0xf
b @80072FF0
@80072F1C
li r17, 0x0
lis r3, 0x1062
mr r15, r22
addi r18, r3, 0x4dd3
lis r19, 0x8000
b @80072FE4
@80072F34
lwz r0, 0x0(r15)
mr r3, r21
addi r4, r1, 0xc
addi r5, r1, 0x9
stw r0, 0xc(r1)
bl fn_8025F648
cmpwi r3, 0x0
beq @80072F5C
li r15, 0x10
b @80072FF0
@80072F5C
lwz r0, 0xf8(r19)
srwi r0, r0, 2
mulhwu r0, r18, r0
srwi r0, r0, 6
mulli r16, r0, 0x64
bl OSGetTick
mr r20, r3
@80072F78
bl OSGetTick
subf r0, r20, r3
cmplw r0, r16
ble @80072F90
li r15, 0x11
b @80072FF0
@80072F90
mr r3, r21
addi r4, r1, 0x9
bl fn_8025F3F4
cmpwi r3, 0x0
beq @80072FAC
li r15, 0x12
b @80072FF0
@80072FAC
lbz r0, 0x9(r1)
rlwinm r0, r0, 0, 30, 30
cmpwi r0, 0x0
bne @80072F78
slwi r0, r17, 26
srwi r3, r17, 31
subf r0, r3, r0
rotlwi r0, r0, 6
add r0, r0, r3
cmpwi r0, 0x0
bne @80072FDC
bl fn_800F0308
@80072FDC
addi r17, r17, 0x4
addi r15, r15, 0x4
@80072FE4
cmpw r17, r23
blt @80072F34
li r15, 0x0
@80072FF0
cmpwi r15, 0x1
bne @80073000
cmpwi r24, 0x0
beq @80072DCC
@80073000
cmpwi r15, 0x0
mr r3, r26
beq @80073014
li r4, 0x1
b @80073018
@80073014
li r4, 0x3
@80073018
bl fn_8008ABE4
mr r3, r15
lmw r15, 0x1c(r1)
lwz r0, 0x64(r1)
mtlr r0
addi r1, r1, 0x60
blr
