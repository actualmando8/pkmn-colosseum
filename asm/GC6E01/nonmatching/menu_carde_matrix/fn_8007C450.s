stwu r1, -0x30(r1)
mflr r0
stw r0, 0x34(r1)
stmw r25, 0x14(r1)
mr r25, r3
mr r26, r4
mr r27, r5
mr r28, r6
mr r29, r7
li r3, 0xa6
bl fn_80104704
bl fn_801040A0
lwz r31, 0x0(r3)
cmplwi r31, 0x0
bne @8007C494
li r31, 0x0
b @8007C4D0
@8007C494
li r0, 0x0
stb r0, 0xc8(r31)
b @8007C4C4
@8007C4A0
bl fn_800F0308
li r3, 0xa6
bl fn_80104704
bl fn_801040A0
lwz r31, 0x0(r3)
cmplwi r31, 0x0
bne @8007C4C4
li r31, 0x0
b @8007C4D0
@8007C4C4
lbz r0, 0xb6(r31)
cmplwi r0, 0x0
bne @8007C4A0
@8007C4D0
cmplwi r31, 0x0
beq @8007C620
lwz r0, 0xa4(r31)
li r30, 0x0
li r5, 0x0
stw r0, 0xa0(r31)
lbz r0, 0xb5(r31)
stb r0, 0xb4(r31)
lwz r6, 0xac(r31)
clrlwi r4, r25, 24
mtctr r6
cmpwi r6, 0x0
ble @8007C524
@8007C504
lwz r3, 0xb0(r31)
lwzx r3, r3, r5
lbz r0, 0x1a(r3)
cmplw r4, r0
beq @8007C524
addi r5, r5, 0x4
addi r30, r30, 0x1
bdnz @8007C504
@8007C524
cmpw r30, r6
blt @8007C544
lis r3, lbl_80268D78@ha
lis r5, lbl_80268D8C@ha
addi r3, r3, lbl_80268D78@l
li r4, 0x608
addi r5, r5, lbl_80268D8C@l
bl fn_80196E10
@8007C544
stw r30, 0xa4(r31)
extsb r3, r28
slwi r0, r30, 2
stb r26, 0xb5(r31)
cmpwi r3, 0x0
lwz r4, 0xb0(r31)
lwzx r4, r4, r0
stw r29, 0xbc(r31)
bge @8007C584
extsb r0, r27
lbz r3, 0x1d(r4)
mulli r0, r0, 0x6
extsb r3, r3
add r0, r3, r0
stw r0, 0xc0(r31)
b @8007C594
@8007C584
extsb r0, r27
mulli r0, r0, 0x6
add r0, r3, r0
stw r0, 0xc0(r31)
@8007C594
li r0, 0x0
stw r0, 0xc4(r31)
lwz r3, 0xa4(r31)
lwz r0, 0xa0(r31)
cmpw r3, r0
bne @8007C5C4
lbz r3, 0xb5(r31)
lbz r0, 0xb4(r31)
extsb r3, r3
extsb r0, r0
cmpw r3, r0
beq @8007C620
@8007C5C4
li r3, 0x1
li r0, 0x0
stb r3, 0xb6(r31)
li r3, 0xa6
stw r0, 0xb8(r31)
bl fn_80104704
bl fn_801040A0
lwz r3, 0x0(r3)
cmplwi r3, 0x0
beq @8007C620
li r0, 0x0
stb r0, 0xc8(r3)
b @8007C614
@8007C5F8
bl fn_800F0308
li r3, 0xa6
bl fn_80104704
bl fn_801040A0
lwz r3, 0x0(r3)
cmplwi r3, 0x0
beq @8007C620
@8007C614
lbz r0, 0xb6(r3)
cmplwi r0, 0x0
bne @8007C5F8
@8007C620
lmw r25, 0x14(r1)
lwz r0, 0x34(r1)
mtlr r0
addi r1, r1, 0x30
blr
