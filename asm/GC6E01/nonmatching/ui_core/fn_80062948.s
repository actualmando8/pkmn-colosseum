stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
stw r30, 0x8(r1)
mr r30, r3
li r4, 0x1
bl fn_80061F6C
bl fn_8025D9A8
mr r31, r3
bl fn_8025DA88
cmpwi r31, 0x2
beq @80062A1C
bge @8006298C
cmpwi r31, 0x0
bge @80062998
b @80062A1C
@8006298C
cmpwi r31, 0x4
bge @80062A1C
b @800629A8
@80062998
mr r3, r30
bl fn_80063060
mr r31, r3
b @80062A28
@800629A8
li r3, 0xdf
li r4, 0x0
bl fn_8010264C
li r3, 0xba
li r4, 0x1
bl fn_8010264C
li r3, 0x106
li r4, 0x1
bl fn_8010264C
lwz r0, 0x4(r30)
cmpwi r0, 0x2
beq @800629E4
cmpwi r3, 0x0
ble @800629E4
addi r3, r3, 0x1
@800629E4
cmpwi r3, 0x0
beq @800629F8
bge @80062A04
cmpwi r3, -0x1
b @80062A04
@800629F8
bl fn_8025D788
li r31, 0xd1
b @80062A08
@80062A04
li r31, -0x1
@80062A08
li r3, 0x106
li r4, 0x0
li r5, 0x1
bl fn_80102568
b @80062A28
@80062A1C
mr r3, r30
bl fn_80062AB4
mr r31, r3
@80062A28
li r3, 0xdf
li r4, 0x1c6
bl fn_801080CC
li r3, 0xba
li r4, 0x1c6
bl fn_801080CC
b @80062A48
@80062A44
bl fn_800F0308
@80062A48
li r3, 0xdf
bl fn_801070F4
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80062A44
b @80062A64
@80062A60
bl fn_800F0308
@80062A64
li r3, 0xba
bl fn_801070F4
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80062A60
bl fn_80069944
bl fn_80062834
li r3, 0x1
bl fn_80061028
li r3, 0xdf
li r4, 0x0
li r5, 0x1
bl fn_80102568
mr r3, r31
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
lwz r30, 0x8(r1)
mtlr r0
addi r1, r1, 0x10
blr
