stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
stw r30, 0x8(r1)
mr r31, r3
mr r30, r4
cmplwi r31, 0x0
bne @80082AC0
lis r3, lbl_8026F1C8@ha
li r4, 0x17f
addi r3, r3, lbl_8026F1C8@l
li r5, lbl_8047C180@sda21
bl fn_80196E10
@80082AC0
extsb r3, r30
li r4, 0x0
cmpwi r3, 0x0
blt @80082AE4
lbz r0, 0x1b(r31)
extsb r0, r0
cmpw r3, r0
bge @80082AE4
li r4, 0x1
@80082AE4
cmpwi r4, 0x0
bne @80082B04
lis r3, lbl_8026F1C8@ha
lis r5, lbl_8026F1D8@ha
addi r3, r3, lbl_8026F1C8@l
li r4, 0x180
addi r5, r5, lbl_8026F1D8@l
bl fn_80196E10
@80082B04
lbz r3, 0x1c(r31)
extsb r4, r30
lbz r0, 0x1d(r31)
extsb r3, r3
extsb r0, r0
mullw r0, r3, r0
slwi r3, r0, 4
addi r0, r3, 0x76
mullw r0, r4, r0
add r30, r31, r0
addi r30, r30, 0x24
cmplwi r30, 0x0
bne @80082B4C
lis r3, lbl_8026F1C8@ha
li r4, 0x1f1
addi r3, r3, lbl_8026F1C8@l
li r5, lbl_8047C188@sda21
bl fn_80196E10
@80082B4C
lbz r3, 0x1c(r31)
lbz r0, 0x1d(r31)
extsb r3, r3
extsb r0, r0
mullw r0, r3, r0
mtctr r0
cmpwi r0, 0x0
ble @80082B88
@80082B6C
lbz r0, 0x82(r30)
cmplwi r0, 0x0
beq @80082B80
li r3, 0x1
b @80082B8C
@80082B80
addi r30, r30, 0x10
bdnz @80082B6C
@80082B88
li r3, 0x0
@80082B8C
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
lwz r30, 0x8(r1)
mtlr r0
addi r1, r1, 0x10
blr
