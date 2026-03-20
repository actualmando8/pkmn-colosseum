stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stw r31, 0x1c(r1)
stw r30, 0x18(r1)
stw r29, 0x14(r1)
stw r28, 0x10(r1)
mr r28, r3
mr r29, r4
mr r30, r5
lbz r3, 0x1a(r28)
lis r4, lbl_8026F1C8@ha
lbz r0, 0x8(r29)
addi r31, r4, lbl_8026F1C8@l
cmplw r3, r0
beq @80082BF4
addi r3, r31, 0x0
addi r5, r31, 0x38
li r4, 0x1d1
bl fn_80196E10
@80082BF4
cmplwi r28, 0x0
bne @80082C0C
addi r3, r31, 0x0
li r4, 0x17f
li r5, lbl_8047C180@sda21
bl fn_80196E10
@80082C0C
extsb r3, r30
li r4, 0x0
cmpwi r3, 0x0
blt @80082C30
lbz r0, 0x1b(r28)
extsb r0, r0
cmpw r3, r0
bge @80082C30
li r4, 0x1
@80082C30
cmpwi r4, 0x0
bne @80082C48
addi r3, r31, 0x0
addi r5, r31, 0x10
li r4, 0x180
bl fn_80196E10
@80082C48
lbz r3, 0x1c(r28)
extsb r4, r30
lbz r0, 0x1d(r28)
extsb r3, r3
extsb r0, r0
mullw r0, r3, r0
slwi r3, r0, 4
addi r0, r3, 0x76
mullw r0, r4, r0
add r28, r28, r0
addi r28, r28, 0x24
cmplwi r28, 0x0
bne @80082C8C
addi r3, r31, 0x0
li r4, 0x1d4
li r5, lbl_8047C188@sda21
bl fn_80196E10
@80082C8C
extsb r0, r30
lbz r4, 0x24(r29)
add r3, r29, r0
lbz r0, 0x5e(r3)
extsb r3, r4
mulli r4, r3, 0xe
extsb r0, r0
mulli r3, r0, 0x28
addi r30, r4, 0x10
add r30, r28, r30
addi r4, r3, 0x3ac
mr r3, r30
add r4, r29, r4
bl fn_800CAA3C
li r0, 0x1
mr r3, r28
stb r0, 0xc(r30)
lwz r0, 0x24(r1)
lwz r31, 0x1c(r1)
lwz r30, 0x18(r1)
lwz r29, 0x14(r1)
lwz r28, 0x10(r1)
mtlr r0
addi r1, r1, 0x20
blr
