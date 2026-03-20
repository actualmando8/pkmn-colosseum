stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
lha r0, 0x6(r4)
cmpwi r0, 0xe26
beq @80063A40
bge @8006394C
cmpwi r0, 0xe16
beq @800639E0
bge @80063934
cmpwi r0, 0xe14
beq @800639A0
bge @800639C0
cmpwi r0, 0xe08
beq @80063988
b @80063AC4
@80063934
cmpwi r0, 0xe24
beq @80063A00
bge @80063A20
cmpwi r0, 0xe18
bge @80063AC4
b @80063990
@8006394C
cmpwi r0, 0x1264
beq @80063998
bge @80063970
cmpwi r0, 0x1123
beq @80063AC0
bge @80063AC4
cmpwi r0, 0xe28
bge @80063AC4
b @80063A60
@80063970
cmpwi r0, 0x1270
beq @80063AA0
bge @80063AC4
cmpwi r0, 0x126f
bge @80063A80
b @80063AC4
@80063988
bl fn_80063AD4
b @80063AC4
@80063990
bl fn_80063AD4
b @80063AC4
@80063998
bl fn_80063AD4
b @80063AC4
@800639A0
lbz r5, 0x8b(r3)
li r0, -0x100
li r3, 0x0
li r4, 0x0
or r5, r5, r0
li r6, 0x3c21
bl fn_800FB680
b @80063AC4
@800639C0
lbz r5, 0x8b(r3)
li r0, -0x100
li r3, 0x0
li r4, 0x0
or r5, r5, r0
li r6, 0x3db2
bl fn_800FB680
b @80063AC4
@800639E0
lbz r5, 0x8b(r3)
li r0, -0x100
li r3, 0x0
li r4, 0x0
or r5, r5, r0
li r6, 0x3db3
bl fn_800FB680
b @80063AC4
@80063A00
lbz r5, 0x8b(r3)
li r0, -0x100
li r3, 0x0
li r4, 0x0
or r5, r5, r0
li r6, 0x3c21
bl fn_800FB680
b @80063AC4
@80063A20
lbz r5, 0x8b(r3)
li r0, -0x100
li r3, 0x0
li r4, 0x0
or r5, r5, r0
li r6, 0x3dae
bl fn_800FB680
b @80063AC4
@80063A40
lbz r5, 0x8b(r3)
li r0, -0x100
li r3, 0x0
li r4, 0x0
or r5, r5, r0
li r6, 0x3db2
bl fn_800FB680
b @80063AC4
@80063A60
lbz r5, 0x8b(r3)
li r0, -0x100
li r3, 0x0
li r4, 0x0
or r5, r5, r0
li r6, 0x3db3
bl fn_800FB680
b @80063AC4
@80063A80
lbz r5, 0x8b(r3)
li r0, -0x100
li r3, 0x0
li r4, 0x0
or r5, r5, r0
li r6, 0x3c21
bl fn_800FB680
b @80063AC4
@80063AA0
lbz r5, 0x8b(r3)
li r0, -0x100
li r3, 0x0
li r4, 0x0
or r5, r5, r0
li r6, 0x3db3
bl fn_800FB680
b @80063AC4
@80063AC0
bl fn_80063AD4
@80063AC4
lwz r0, 0x14(r1)
mtlr r0
addi r1, r1, 0x10
blr
