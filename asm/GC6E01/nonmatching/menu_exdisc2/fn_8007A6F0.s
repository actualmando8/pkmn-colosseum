stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
stw r30, 0x8(r1)
mr r30, r4
lwz r0, lbl_8047A638@sda21(r0)
cmpwi r0, 0x4
bne @8007A71C
lwz r3, lbl_8047A628@sda21(r0)
b @8007A72C
@8007A71C
li r3, 0x0
li r4, 0xe
li r5, 0x0
bl fn_8012A5B0
@8007A72C
lwz r5, lbl_804788F0@sda21(r0)
lis r4, lbl_802E61D8@ha
addi r0, r4, lbl_802E61D8@l
subi r31, r5, 0x1
slwi r4, r31, 2
add r4, r0, r4
addi r0, r31, 0x1
mtctr r0
cmpwi r31, 0x0
blt @8007A76C
@8007A754
lwz r0, 0x0(r4)
cmplw r0, r3
ble @8007A76C
subi r4, r4, 0x4
subi r31, r31, 0x1
bdnz @8007A754
@8007A76C
cmpwi r31, 0x0
bge @8007A778
li r31, 0x0
@8007A778
mr r3, r30
li r4, 0x0
bl fn_80109220
cmpwi r31, 0x2
beq @8007A7C4
bge @8007A79C
cmpwi r31, 0x1
bge @8007A7A8
b @8007A7FC
@8007A79C
cmpwi r31, 0x4
bge @8007A7FC
b @8007A7E0
@8007A7A8
lha r0, 0x6(r30)
cmpwi r0, 0x10c3
bne @8007A814
mr r3, r30
li r4, 0x1
bl fn_80109220
b @8007A814
@8007A7C4
lha r0, 0x6(r30)
cmpwi r0, 0x10c4
bne @8007A814
mr r3, r30
li r4, 0x1
bl fn_80109220
b @8007A814
@8007A7E0
lha r0, 0x6(r30)
cmpwi r0, 0x10c5
bne @8007A814
mr r3, r30
li r4, 0x1
bl fn_80109220
b @8007A814
@8007A7FC
lha r0, 0x6(r30)
cmpwi r0, 0x10c2
bne @8007A814
mr r3, r30
li r4, 0x1
bl fn_80109220
@8007A814
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
lwz r30, 0x8(r1)
mtlr r0
addi r1, r1, 0x10
blr
