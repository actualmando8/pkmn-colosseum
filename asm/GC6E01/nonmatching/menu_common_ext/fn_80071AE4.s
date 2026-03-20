stwu r1, -0x70(r1)
mflr r0
stw r0, 0x74(r1)
stmw r14, 0x28(r1)
mr r15, r3
mr r16, r4
addi r0, r15, 0x1
li r4, 0x2
stw r0, 0x18(r1)
mr r3, r0
bl fn_8008ABE4
lis r4, 0x8000
lis r3, 0x1062
lwz r0, 0xf8(r4)
addi r3, r3, 0x4dd3
srwi r0, r0, 2
mulhwu r0, r3, r0
srwi r0, r0, 6
mulli r18, r0, 0x64
bl OSGetTick
lis r5, lbl_803B6E18@ha
lis r4, lbl_803B6E08@ha
slwi r6, r15, 3
mr r25, r3
addi r0, r5, lbl_803B6E18@l
slwi r26, r15, 2
add r19, r0, r6
addi r27, r4, lbl_803B6E08@l
addi r20, r19, 0x4
lwz r5, 0xc(r16)
srwi r3, r5, 24
rlwinm r0, r5, 24, 16, 23
rlwinm r4, r5, 8, 8, 15
slwi r5, r5, 24
or r0, r3, r0
or r0, r4, r0
or r0, r5, r0
srwi r14, r0, 16
clrlwi r23, r0, 16
@80071B80
bl OSGetTick
subf r4, r25, r3
mr r3, r15
xor r0, r4, r18
cntlzw r0, r0
slw r0, r4, r0
srwi r17, r0, 31
bl fn_80073C38
cmpwi r3, 0x0
beq @80071BB0
mr r21, r3
b @80071DF0
@80071BB0
li r0, 0x22
mr r3, r15
stw r0, 0x10(r1)
addi r4, r1, 0x10
addi r5, r1, 0xa
bl fn_8025F648
cmpwi r3, 0x0
beq @80071BD8
li r21, 0xb
b @80071CAC
@80071BD8
lis r4, 0x8000
lis r3, 0x1062
lwz r0, 0xf8(r4)
addi r3, r3, 0x4dd3
srwi r0, r0, 2
mulhwu r0, r3, r0
srwi r0, r0, 6
mulli r22, r0, 0x64
bl OSGetTick
mr r21, r3
@80071C00
bl OSGetTick
subf r0, r21, r3
cmplw r0, r22
ble @80071C18
li r3, 0x1
b @80071C98
@80071C18
mr r3, r15
addi r4, r1, 0x8
bl fn_8025F3F4
cmpwi r3, 0x0
beq @80071C34
li r3, 0x2
b @80071C98
@80071C34
lbz r0, 0x8(r1)
andi. r0, r0, 0xa
cmpwi r0, 0x8
beq @80071C74
lwz r12, 0x0(r19)
cmplwi r12, 0x0
beq @80071C60
mr r3, r15
lwz r4, 0x0(r20)
mtctr r12
bctrl
@80071C60
lwzx r0, r27, r26
cmpwi r0, 0x0
beq @80071C00
li r3, 0x3e8
b @80071C98
@80071C74
mr r3, r15
addi r4, r1, 0x14
addi r5, r1, 0xa
bl fn_8025F584
cmpwi r3, 0x0
beq @80071C94
li r3, 0x3
b @80071C98
@80071C94
li r3, 0x0
@80071C98
cmpwi r3, 0x0
beq @80071CA8
addi r21, r3, 0xb
b @80071CAC
@80071CA8
li r21, 0x0
@80071CAC
cmpwi r21, 0x0
beq @80071CB8
b @80071DF0
@80071CB8
lwz r0, 0x14(r1)
srwi r0, r0, 24
cmplwi r0, 0x22
beq @80071CD0
li r21, 0xf
b @80071DF0
@80071CD0
clrlwi r0, r14, 16
slwi r3, r23, 2
cmplwi r0, 0x0
addi r24, r3, 0x10
beq @80071CE8
addi r24, r24, 0x70
@80071CE8
li r22, 0x0
lis r3, 0x1062
mr r31, r16
addi r28, r3, 0x4dd3
lis r29, 0x8000
b @80071DD4
@80071D00
lwz r0, 0x0(r31)
mr r3, r15
addi r4, r1, 0xc
addi r5, r1, 0x9
stw r0, 0xc(r1)
bl fn_8025F648
cmpwi r3, 0x0
beq @80071D28
li r21, 0x10
b @80071DE0
@80071D28
lwz r0, 0xf8(r29)
srwi r0, r0, 2
mulhwu r0, r28, r0
srwi r0, r0, 6
mulli r21, r0, 0x64
bl OSGetTick
mr r30, r3
@80071D44
bl OSGetTick
subf r0, r30, r3
cmplw r0, r21
ble @80071D5C
li r21, 0x11
b @80071DE0
@80071D5C
mr r3, r15
addi r4, r1, 0x9
bl fn_8025F3F4
cmpwi r3, 0x0
beq @80071D78
li r21, 0x12
b @80071DE0
@80071D78
lbz r0, 0x9(r1)
rlwinm r0, r0, 0, 30, 30
cmpwi r0, 0x0
beq @80071DB8
lwz r12, 0x0(r19)
cmplwi r12, 0x0
beq @80071DA4
mr r3, r15
lwz r4, 0x0(r20)
mtctr r12
bctrl
@80071DA4
lwzx r0, r27, r26
cmpwi r0, 0x0
beq @80071D44
li r21, 0x3e8
b @80071DE0
@80071DB8
lwzx r0, r27, r26
cmpwi r0, 0x0
beq @80071DCC
li r21, 0x3e8
b @80071DE0
@80071DCC
addi r22, r22, 0x4
addi r31, r31, 0x4
@80071DD4
cmpw r22, r24
blt @80071D00
li r21, 0x0
@80071DE0
cmpwi r21, 0x0
beq @80071DEC
b @80071DF0
@80071DEC
li r21, 0x0
@80071DF0
cmpwi r21, 0x1
bne @80071E00
cmpwi r17, 0x0
beq @80071B80
@80071E00
cmpwi r21, 0x0
lwz r3, 0x18(r1)
beq @80071E14
li r4, 0x1
b @80071E18
@80071E14
li r4, 0x3
@80071E18
bl fn_8008ABE4
mr r3, r21
lmw r14, 0x28(r1)
lwz r0, 0x74(r1)
mtlr r0
addi r1, r1, 0x70
blr
