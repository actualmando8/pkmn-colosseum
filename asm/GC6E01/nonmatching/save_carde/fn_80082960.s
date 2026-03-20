stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stw r31, 0x1c(r1)
stw r30, 0x18(r1)
stw r29, 0x14(r1)
stw r28, 0x10(r1)
mr r30, r3
mr r31, r4
mr r28, r5
lbz r3, 0x1a(r30)
lis r4, lbl_8026F1C8@ha
lbz r0, 0x8(r31)
addi r29, r4, lbl_8026F1C8@l
cmplw r3, r0
beq @800829B0
addi r3, r29, 0x0
addi r5, r29, 0x38
li r4, 0x209
bl fn_80196E10
@800829B0
cmplwi r30, 0x0
bne @800829C8
addi r3, r29, 0x0
li r4, 0x17f
li r5, lbl_8047C180@sda21
bl fn_80196E10
@800829C8
extsb r3, r28
li r4, 0x0
cmpwi r3, 0x0
blt @800829EC
lbz r0, 0x1b(r30)
extsb r0, r0
cmpw r3, r0
bge @800829EC
li r4, 0x1
@800829EC
cmpwi r4, 0x0
bne @80082A04
addi r3, r29, 0x0
addi r5, r29, 0x10
li r4, 0x180
bl fn_80196E10
@80082A04
lbz r3, 0x1c(r30)
extsb r4, r28
lbz r0, 0x1d(r30)
extsb r3, r3
extsb r0, r0
mullw r0, r3, r0
slwi r3, r0, 4
addi r0, r3, 0x76
mullw r0, r4, r0
add r30, r30, r0
addi r30, r30, 0x24
cmplwi r30, 0x0
bne @80082A48
addi r3, r29, 0x0
li r4, 0x20c
li r5, lbl_8047C188@sda21
bl fn_80196E10
@80082A48
lbz r3, 0x24(r31)
li r0, 0x0
extsb r3, r3
mulli r3, r3, 0xe
addi r3, r3, 0x10
add r3, r30, r3
sth r0, 0x0(r3)
stb r0, 0xc(r3)
lwz r0, 0x24(r1)
lwz r31, 0x1c(r1)
lwz r30, 0x18(r1)
lwz r29, 0x14(r1)
lwz r28, 0x10(r1)
mtlr r0
addi r1, r1, 0x20
blr
