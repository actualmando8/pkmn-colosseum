stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
li r3, 0x0
bl fn_80083CFC
cmplwi r3, 0x0
beq @80089BB0
lbz r4, 0x4136(r3)
b @80089BB4
@80089BB0
li r4, 0x0
@80089BB4
lis r3, lbl_802EEB98@ha
li r5, 0x0
addi r3, r3, lbl_802EEB98@l
li r0, 0x10
mtctr r0
@80089BC8
lbz r0, 0x1(r3)
cmplw r4, r0
bne @80089BE8
lis r3, lbl_802EEB98@ha
slwi r0, r5, 1
addi r3, r3, lbl_802EEB98@l
lbzx r3, r3, r0
b @80089C00
@80089BE8
addi r3, r3, 0x2
addi r5, r5, 0x1
bdnz @80089BC8
lis r3, lbl_802EEB98@ha
addi r3, r3, lbl_802EEB98@l
lbz r3, 0x0(r3)
@80089C00
lwz r0, 0x14(r1)
mtlr r0
addi r1, r1, 0x10
blr
