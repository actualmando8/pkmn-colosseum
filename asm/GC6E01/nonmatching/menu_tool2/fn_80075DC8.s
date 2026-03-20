stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
stw r30, 0x8(r1)
bl fn_80113F48
lis r4, 0xb56
li r5, 0x0
addi r4, r4, 0x1800
li r6, 0x0
bl fn_80176E0C
li r30, 0x1
bl fn_800D37CC
cmpwi r3, 0x32
bne @80075E1C
lfs f1, lbl_8047C0C0@sda21(r0)
bl fn_800C46B0
mr r30, r3
cmplwi r30, 0x1
bge @80075E1C
li r30, 0x1
@80075E1C
li r31, 0x0
b @80075E30
@80075E24
bl fn_800F0308
bl fn_800D3088
add r31, r31, r3
@80075E30
cmplw r31, r30
blt @80075E24
lis r3, 0xb54
li r4, 0x2
addi r3, r3, 0x1000
li r5, 0x0
li r6, 0x1
bl fn_801CB834
li r3, 0xe2
li r4, 0x0
li r5, 0x0
li r6, 0x10
li r7, 0x1
li r8, 0x0
crclr 6
bl fn_801026A4
cmpwi r3, 0x0
beq @80075E8C
bge @80075E84
cmpwi r3, -0x1
b @80075E94
@80075E84
cmpwi r3, 0x2
b @80075E94
@80075E8C
li r30, 0x321
b @80075E98
@80075E94
li r30, 0x384
@80075E98
li r3, 0x1
bl fn_801C40F0
lfs f1, lbl_8047C0C4@sda21(r0)
li r3, 0x3
bl fn_801C41C8
mr r3, r30
li r4, 0x0
bl fn_80113828
lis r4, 0x596
li r3, 0x0
addi r4, r4, 0x8
bl fn_8011288C
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
lwz r30, 0x8(r1)
mtlr r0
addi r1, r1, 0x10
blr
