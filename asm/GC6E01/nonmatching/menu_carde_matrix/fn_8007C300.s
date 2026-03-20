stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stw r31, 0x1c(r1)
stw r30, 0x18(r1)
stw r29, 0x14(r1)
stw r28, 0x10(r1)
mr r28, r3
mr r29, r4
li r3, 0xa6
bl fn_80104704
bl fn_801040A0
lwz r31, 0x0(r3)
cmplwi r31, 0x0
bne @8007C344
li r31, 0x0
b @8007C380
@8007C344
li r0, 0x0
stb r0, 0xc8(r31)
b @8007C374
@8007C350
bl fn_800F0308
li r3, 0xa6
bl fn_80104704
bl fn_801040A0
lwz r31, 0x0(r3)
cmplwi r31, 0x0
bne @8007C374
li r31, 0x0
b @8007C380
@8007C374
lbz r0, 0xb6(r31)
cmplwi r0, 0x0
bne @8007C350
@8007C380
cmplwi r31, 0x0
beq @8007C3F4
lwz r6, 0xac(r31)
li r30, 0x0
li r5, 0x0
clrlwi r4, r28, 24
mtctr r6
cmpwi r6, 0x0
ble @8007C3C4
@8007C3A4
lwz r3, 0xb0(r31)
lwzx r3, r3, r5
lbz r0, 0x1a(r3)
cmplw r4, r0
beq @8007C3C4
addi r5, r5, 0x4
addi r30, r30, 0x1
bdnz @8007C3A4
@8007C3C4
cmpw r30, r6
blt @8007C3E4
lis r3, lbl_80268D78@ha
lis r5, lbl_80268D8C@ha
addi r3, r3, lbl_80268D78@l
li r4, 0x648
addi r5, r5, lbl_80268D8C@l
bl fn_80196E10
@8007C3E4
stw r30, 0xa4(r31)
stw r30, 0xa0(r31)
stb r29, 0xb5(r31)
stb r29, 0xb4(r31)
@8007C3F4
lwz r0, 0x24(r1)
lwz r31, 0x1c(r1)
lwz r30, 0x18(r1)
lwz r29, 0x14(r1)
lwz r28, 0x10(r1)
mtlr r0
addi r1, r1, 0x20
blr
