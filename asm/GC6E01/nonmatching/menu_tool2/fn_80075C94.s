stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
@80075CA4
li r3, 0x37
li r4, 0x0
bl fn_80132A38
li r3, 0xe0
li r4, 0x0
li r5, 0x0
li r6, 0x10
li r7, 0x1
li r8, 0x0
crclr 6
bl fn_801026A4
mr r31, r3
li r3, 0x1
bl fn_801C40F0
cmpwi r31, 0x1
beq @80075D10
bge @80075CF8
cmpwi r31, -0x1
beq @80075D78
bge @80075D00
b @80075D78
@80075CF8
cmpwi r31, 0x3
b @80075D78
@80075D00
li r3, 0x322
li r4, 0x0
bl fn_80113828
b @80075D84
@80075D10
li r3, 0x2
li r4, 0x2
li r5, 0x0
bl fn_801D0748
mr r31, r3
cmpwi r31, 0x3
bne @80075D40
li r3, 0x0
li r4, 0x4
bl fn_80135168
cmplwi r3, 0x0
bne @80075D68
@80075D40
cmpwi r31, -0x1
beq @80075CA4
li r3, 0x2
li r4, 0x44db
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
li r3, 0x1
bl fn_801069FC
b @80075CA4
@80075D68
li r3, 0x323
li r4, 0x0
bl fn_80113828
b @80075D84
@80075D78
li r3, 0x320
li r4, 0x0
bl fn_80113828
@80075D84
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
mtlr r0
addi r1, r1, 0x10
blr
