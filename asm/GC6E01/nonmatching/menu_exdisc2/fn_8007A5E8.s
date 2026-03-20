stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
mr r31, r4
lwz r0, lbl_8047A638@sda21(r0)
cmpwi r0, 0x4
bne @8007A610
lwz r3, lbl_8047A62C@sda21(r0)
b @8007A620
@8007A610
li r3, 0x0
li r4, 0xd
li r5, 0x0
bl fn_8012A5B0
@8007A620
mr r4, r3
li r3, 0x50
bl fn_80132A38
li r3, 0x153
bl fn_800FA444
lha r0, 0x54(r31)
srwi r3, r3, 16
li r4, 0x0
li r5, -0x1
subf r3, r3, r0
li r6, 0x153
bl fn_800FB680
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
mtlr r0
addi r1, r1, 0x10
blr
