stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
lwz r0, lbl_8047A638@sda21(r0)
cmpwi r0, 0x4
beq @8007A68C
bge @8007A6E0
cmpwi r0, 0x3
bge @8007A6B8
b @8007A6E0
@8007A68C
lha r0, 0x6(r4)
cmpwi r0, 0x10bf
bne @8007A6A8
mr r3, r4
li r4, 0x1
bl fn_80109220
b @8007A6E0
@8007A6A8
mr r3, r4
li r4, 0x0
bl fn_80109220
b @8007A6E0
@8007A6B8
lha r0, 0x6(r4)
cmpwi r0, 0x10c0
bne @8007A6D4
mr r3, r4
li r4, 0x1
bl fn_80109220
b @8007A6E0
@8007A6D4
mr r3, r4
li r4, 0x0
bl fn_80109220
@8007A6E0
lwz r0, 0x14(r1)
mtlr r0
addi r1, r1, 0x10
blr
