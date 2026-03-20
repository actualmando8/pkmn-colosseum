stwu r1, -0x30(r1)
mflr r0
stw r0, 0x34(r1)
stmw r25, 0x14(r1)
mr r27, r3
lis r3, lbl_803FB380@ha
li r29, 0x0
addi r30, r3, lbl_803FB380@l
lhz r0, 0x18(r30)
cmplwi r0, 0x0
beq @80096FD4
li r28, 0x5
b @80096FD8
@80096FD4
li r28, 0x4
@80096FD8
bl fn_80105624
lis r4, lbl_803FB380@ha
lhz r5, 0x6(r3)
addi r31, r4, lbl_803FB380@l
lbz r0, 0x1(r31)
cmplwi r0, 0x8
bgt @800973D8
lis r3, jumptable_802EF080@ha
slwi r0, r0, 2
addi r3, r3, jumptable_802EF080@l
lwzx r0, r3, r0
mtctr r0
bctr
clrlwi r3, r5, 16
lbz r4, 0x95(r27)
rlwinm r0, r3, 0, 28, 28
cmpwi r0, 0x0
beq @80097028
addi r4, r4, 0x1
b @80097038
@80097028
rlwinm r0, r3, 0, 29, 29
cmpwi r0, 0x0
beq @80097038
subi r4, r4, 0x1
@80097038
extsb r0, r4
cmpwi r0, 0x2
ble @80097048
li r4, 0x2
@80097048
extsb r0, r4
cmpwi r0, 0x0
bge @80097058
li r4, 0x0
@80097058
extsb r0, r4
stb r4, 0x95(r27)
cmpwi r0, 0x1
beq @80097090
bge @80097078
cmpwi r0, 0x0
bge @80097084
b @800970A4
@80097078
cmpwi r0, 0x3
bge @800970A4
b @8009709C
@80097084
li r0, 0x1
stb r0, 0x1(r31)
b @800970A4
@80097090
li r0, 0x2
stb r0, 0x1(r31)
b @800970A4
@8009709C
li r0, 0x5
stb r0, 0x1(r31)
@800970A4
clrlwi r0, r3, 31
cmpwi r0, 0x0
beq @800970B8
li r29, 0x1
b @800970C8
@800970B8
rlwinm r0, r3, 0, 30, 30
cmpwi r0, 0x0
beq @800970C8
li r29, 0x2
@800970C8
cmpwi r29, 0x0
beq @800973D8
lis r3, lbl_803FB380@ha
addi r28, r3, lbl_803FB380@l
lwz r12, 0x10(r28)
cmplwi r12, 0x0
beq @800973D8
mr r4, r29
lwz r3, 0xc(r28)
lwz r5, 0x14(r28)
mtctr r12
bctrl
lwz r0, 0xc(r28)
mr r25, r3
cmplw r0, r25
beq @800973D8
cmplwi r25, 0x0
beq @8009712C
lwz r3, 0x4(r27)
li r4, 0x1
bl fn_80103484
lis r3, lbl_803FB338@ha
mr r4, r25
addi r3, r3, lbl_803FB338@l
bl fn_80109C88
@8009712C
stw r25, 0xc(r28)
b @800973D8
clrlwi r4, r5, 16
lis r3, lbl_803FB380@ha
clrlwi r0, r4, 31
addi r31, r3, lbl_803FB380@l
cmpwi r0, 0x0
lbz r29, 0x2(r31)
beq @80097158
subi r29, r29, 0x1
b @80097168
@80097158
rlwinm r0, r4, 0, 30, 30
cmpwi r0, 0x0
beq @80097168
addi r29, r29, 0x1
@80097168
extsb r3, r29
extsb r0, r28
cmpw r3, r0
blt @80097180
subi r0, r28, 0x1
extsb r29, r0
@80097180
extsb r0, r29
cmpwi r0, 0x0
bge @80097190
li r29, 0x0
@80097190
extsb r25, r29
lis r3, lbl_803FB380@ha
clrlwi r0, r25, 16
addi r3, r3, lbl_803FB380@l
cmplwi r0, 0x4
lwz r26, 0xc(r3)
bne @800971B4
lhz r28, 0x18(r30)
b @800971E8
@800971B4
mr r3, r26
mr r6, r25
li r4, 0x0
li r5, 0x7f
bl fn_8012640C
clrlwi r28, r3, 16
mr r3, r26
mr r4, r25
bl fn_80123CD4
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @800971E8
li r28, 0x0
@800971E8
clrlwi r0, r28, 16
cmplwi r0, 0x0
beq @800973D8
lbz r0, 0x2(r31)
extsb r3, r29
extsb r0, r0
cmpw r3, r0
beq @800973D8
lwz r3, 0x4(r27)
li r4, 0x1
bl fn_80103484
stb r29, 0x2(r31)
b @800973D8
lbz r0, 0x1a(r31)
lis r3, 0x38e4
clrlwi r8, r5, 16
extsb r7, r0
subi r3, r3, 0x71c7
mulhw r6, r3, r7
clrlwi r3, r8, 31
cmpwi r3, 0x0
srawi r4, r6, 1
srwi r5, r4, 31
srawi r3, r6, 1
add r4, r4, r5
mulli r5, r4, 0x9
srwi r4, r3, 31
add r4, r3, r4
subf r3, r5, r7
beq @800972B8
mr r8, r4
b @800972A8
@80097268
add r6, r31, r8
slwi r5, r3, 2
mr r7, r3
add r6, r6, r5
@80097278
lbz r5, 0x20(r6)
extsb r5, r5
cmpwi r5, 0x0
blt @80097298
mr r4, r8
mr r3, r7
li r8, -0x1
b @800972A8
@80097298
cmpwi r7, 0x0
subi r6, r6, 0x4
subi r7, r7, 0x1
bgt @80097278
@800972A8
cmpwi r8, 0x0
subi r8, r8, 0x1
bgt @80097268
b @800973B0
@800972B8
rlwinm r5, r8, 0, 30, 30
cmpwi r5, 0x0
beq @8009731C
mr r7, r4
b @8009730C
@800972CC
add r6, r31, r7
slwi r5, r3, 2
mr r8, r3
add r6, r6, r5
@800972DC
lbz r5, 0x20(r6)
extsb r5, r5
cmpwi r5, 0x0
blt @800972FC
mr r4, r7
mr r3, r8
li r7, 0x5
b @8009730C
@800972FC
cmpwi r8, 0x0
subi r6, r6, 0x4
subi r8, r8, 0x1
bgt @800972DC
@8009730C
addi r7, r7, 0x1
cmpwi r7, 0x4
blt @800972CC
b @800973B0
@8009731C
rlwinm r5, r8, 0, 28, 28
cmpwi r5, 0x0
beq @80097368
mr r7, r3
addi r3, r3, 0x1
cmpwi r3, 0x9
blt @8009733C
li r3, 0x8
@8009733C
lis r6, lbl_803FB380@ha
slwi r5, r3, 2
addi r6, r6, lbl_803FB380@l
add r5, r6, r5
add r5, r5, r4
lbz r5, 0x20(r5)
extsb r5, r5
cmpwi r5, 0x0
bge @800973B0
mr r3, r7
b @800973B0
@80097368
rlwinm r5, r8, 0, 29, 29
cmpwi r5, 0x0
beq @800973B0
mr r7, r3
subi r3, r3, 0x1
cmpwi r3, 0x0
bge @80097388
li r3, 0x0
@80097388
lis r6, lbl_803FB380@ha
slwi r5, r3, 2
addi r6, r6, lbl_803FB380@l
add r5, r6, r5
add r5, r5, r4
lbz r5, 0x20(r5)
extsb r5, r5
cmpwi r5, 0x0
bge @800973B0
mr r3, r7
@800973B0
mulli r4, r4, 0x9
extsb r0, r0
add r3, r3, r4
extsb r25, r3
cmpw r25, r0
beq @800973D8
lwz r3, 0x4(r27)
li r4, 0x1
bl fn_80103484
stb r25, 0x1a(r31)
@800973D8
lmw r25, 0x14(r1)
lwz r0, 0x34(r1)
mtlr r0
addi r1, r1, 0x30
blr
